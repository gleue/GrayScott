#include "menu.h"
#include "names.h"
#include "display.h"

typedef struct {
    float step;
    float min;
    float max;
    CRGB::HTMLColorCode color;
    const bool *name;
    CRGB::HTMLColorCode name_color;
} ParamSetting;

const ParamSetting parameters[] PROGMEM = {
    {0.01, 0.0, 1.0, CRGB::DarkBlue, name_diffA, CRGB::DarkGray},
    {0.001, 0.0, 0.1, CRGB::DarkBlue, name_feedA, CRGB::DarkGray},
    {0.01, 0.0, 1.0, CRGB::OrangeRed, name_diffB, CRGB::DarkGray},
    {0.001, 0.0, 0.1, CRGB::OrangeRed, name_killB, CRGB::DarkGray}
};

#define PARAM_COUNT (sizeof parameters / sizeof(ParamSetting))

fixed *values[] = { &diffA, &feedA, &diffB, &killB };

fixed paramValue(int16_t analog, float divisor = 1023.0, float min = 0.0, float max = 1.0) {
    return constrain(float(analog) / divisor, min, max);
}

void printParam(fixed value, uint8_t width = 7, uint8_t prec = 4) {
    char output[8];
    dtostrf(float(value), min(width, 7), min(prec, width), output);
    Serial.print(output);
}

void initMenu() {
    pinMode(ENTER_PIN, INPUT_PULLUP);
    pinMode(PREV_PIN, INPUT_PULLUP);
    pinMode(NEXT_PIN, INPUT_PULLUP);

    Serial.print(F("Start:")); printParam(feedA); printParam(killB); Serial.print(F("\n"));
}

bool isVisible = false;
bool showsValue = false;
bool needsUpdate = false;

// Current menu item, where PARAM_COUNT means (virtual) exist item
uint8_t itemNumber = 0;
ParamSetting itemSettings;

void activateItem(uint8_t index) {
    itemNumber = index;
    if (itemNumber < PARAM_COUNT) memcpy_P(&itemSettings, &parameters[itemNumber], sizeof(ParamSetting));
}

void showMenu(unsigned long now) {
    isVisible = true;
    needsUpdate = true;
    activateItem(0);
}

void drawMenu(unsigned long now) {
    if (!isVisible) return;

    if (showsValue && itemNumber < PARAM_COUNT) {
        // Parameter value
        drawValue(*values[itemNumber], itemSettings.min, itemSettings.max, itemSettings.color);
    } else if (!showsValue && itemNumber < PARAM_COUNT) {
        // Parameter item
        drawName(itemSettings.name, itemSettings.name_color);
    } else if (!showsValue) {
        // Virtual exit item
        drawName(name_close, CRGB::Red);
    }
}

void hideMenu(unsigned long now) {
    isVisible = false;
}

void showValue(unsigned long now) {
    showsValue = true;
    needsUpdate = true;
}

void hideValue(unsigned long now) {
    showsValue = false;
    needsUpdate = true;
}

void handleEnter(unsigned long now) {
    if (!isVisible) {
        showMenu(now);
        return;
    }

    if (showsValue) {
        hideValue(now);
    } else {
        if (itemNumber < PARAM_COUNT) {
            showValue(now);
        } else {
            hideMenu(now);
        }
    }
}

void handlePrev(unsigned long now) {
    if (!isVisible) return;

    if (showsValue) {
        fixed value = *values[itemNumber] - itemSettings.step;
        *values[itemNumber] = constrain(value, itemSettings.min, itemSettings.max);
    } else if (itemNumber == 0) {
        activateItem(PARAM_COUNT);
    } else {
        activateItem(itemNumber - 1);
    }
    needsUpdate = true;
}

void handleNext(unsigned long now) {
    if (!isVisible) return;

    if (showsValue) {
        fixed value = *values[itemNumber] + itemSettings.step;
        *values[itemNumber] = constrain(value, itemSettings.min, itemSettings.max);
    } else if (itemNumber < PARAM_COUNT) {
        activateItem(itemNumber + 1);
    } else {
        activateItem(0);
    }
    needsUpdate = true;
}

#define ENTER_BUTTON 0
#define PREV_BUTTON 1
#define NEXT_BUTTON 2
#define DEBOUNCE_DELAY 50

bool loopMenu(unsigned long now) {
    static const uint8_t pins[] = { ENTER_PIN, PREV_PIN, NEXT_PIN };
    static bool down[] = { false, false, false };
    static unsigned long time[] = { 0, 0, 0 };

    bool done = false;

    for (uint8_t idx = 0; idx < 3; idx++) {
        uint8_t button = digitalRead(pins[idx]);

        if (button != down[idx]) {
            time[idx] = now;
        } else if (now - time[idx] > DEBOUNCE_DELAY) {
            if (button == LOW && !down[idx]) {
                down[idx] = true;
            } else if (button == HIGH && down[idx]) {
                down[idx] = false;
                switch (idx) {
                    case 0:
                        Serial.println(F("ENTER"));
                        handleEnter(now);
                        done = true;
                        break;
                    case 1:
                        Serial.println(F("PREV"));
                        handlePrev(now);
                        done = true;
                        break;
                    case 2:
                        Serial.println(F("NEXT"));
                        handleNext(now);
                        done = true;
                        break;
                }
            }
        }
    }

    if (isVisible && needsUpdate) {
        needsUpdate = false;
        drawMenu(now);
    }

    return isVisible || done;
}
