#include "menu.h"
#include "names.h"
#include "display.h"
#include "simulation.h"

typedef struct {
    float step;
    float min;
    float max;
    CRGB::HTMLColorCode color;
    const bool *name;
    CRGB::HTMLColorCode name_color;
} ParamSetting;

const ParamSetting parameters[] PROGMEM = {
    {0.01, 0.0, 1.0, CRGB::DarkBlue, name_diffA, CRGB::DimGray},
    {0.001, 0.0, 0.1, CRGB::DarkBlue, name_feedA, CRGB::DimGray},
    {0.01, 0.0, 0.25, CRGB::DarkBlue, name_dripA, CRGB::DimGray},
    {0.01, 0.0, 1.0, CRGB::OrangeRed, name_diffB, CRGB::DimGray},
    {0.001, 0.0, 0.1, CRGB::OrangeRed, name_killB, CRGB::DimGray},
    {0.01, 0.0, 0.25, CRGB::OrangeRed, name_dripB, CRGB::DimGray}
};

#define PARAM_COUNT (sizeof parameters / sizeof(ParamSetting))

fixed *values[] = { &diffA, &feedA, &dripA, &diffB, &killB, &dripB };

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
        drawName(name_close, CRGB::DarkOrchid);
    }
}

void hideMenu(unsigned long now) {
    isVisible = false;
    saveParameters();
}

void showValue(unsigned long now) {
    showsValue = true;
    needsUpdate = true;
}

void hideValue(unsigned long now) {
    showsValue = false;
    needsUpdate = true;
}

void updateBrightness(int delta) {
    auto brightness = getBrightness() + delta;
    Serial.print(F("BRIGHTNESS: ")); Serial.println(brightness);
    setBrightness(brightness);
}

void handleEnter(unsigned long now, bool longPress = false) {
    if (longPress) return;

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

void handlePrev(unsigned long now, bool longPress = false) {
    if (!isVisible) {
        updateBrightness(-8);
    } else if (showsValue) {
        fixed value = *values[itemNumber] - itemSettings.step;
        *values[itemNumber] = constrain(value, itemSettings.min, itemSettings.max);
    } else if (itemNumber == 0) {
        activateItem(PARAM_COUNT);
    } else {
        activateItem(itemNumber - 1);
    }
    needsUpdate = true;
}

void handleNext(unsigned long now, bool longPress = false) {
    if (!isVisible) {
        updateBrightness(+8);
    } else if (showsValue) {
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
#define REPEAT_DELAY 500
#define REPEAT_PAUSE 250

bool loopMenu(unsigned long now) {
    static const uint8_t pins[] = { ENTER_PIN, PREV_PIN, NEXT_PIN };
    static bool down[] = { false, false, false };
    static unsigned long time[] = { 0, 0, 0 };
    static unsigned long repeat[] = { 0, 0, 0 };
    static bool repeating[] = { false, false, false };

    bool done = false;

    for (uint8_t idx = 0; idx < 3; idx++) {
        bool button = (digitalRead(pins[idx]) == LOW) ? true : false;

        if (button == down[idx]) {
            time[idx] = now;
            if (button) {
                if (repeat[idx] == 0) {
                    // Start repeating
                    repeat[idx] = now;
                } else if (now - repeat[idx] > (repeating[idx] ? REPEAT_PAUSE : REPEAT_DELAY)) {
                    // Perform repeat after delay
                    switch (idx) {
                        case 0:
                            if (!repeating[idx]) {
                                resetSimulation();
                            } else {
                                // Serial.println(F("REPEAT ENTER"));
                                handleEnter(now, true);
                            }
                            break;
                        case 1:
                            // Serial.println(F("REPEAT PREV"));
                            handlePrev(now, true);
                            break;
                        case 2:
                            // Serial.println(F("REPEAT NEXT"));
                            handleNext(now, true);
                            break;
                    }
                    repeat[idx] = 0;
                    repeating[idx] = true;
                }
            }
        } else if (now - time[idx] > DEBOUNCE_DELAY) {
            if (button && !down[idx]) {
                // Button down
                down[idx] = true;
            } else if (!button && down[idx]) {
                down[idx] = false;
                if (!repeating[idx]) {
                    // Handle button on up
                    switch (idx) {
                        case 0:
                            // Serial.println(F("ENTER"));
                            handleEnter(now);
                            done = true;
                            break;
                        case 1:
                            // Serial.println(F("PREV"));
                            handlePrev(now);
                            done = true;
                            break;
                        case 2:
                            // Serial.println(F("NEXT"));
                            handleNext(now);
                            done = true;
                            break;
                    }
                }
                repeat[idx] = 0;
                repeating[idx] = false;
            }
        }
    }

    if (isVisible && needsUpdate) {
        needsUpdate = false;
        drawMenu(now);
    }

    return isVisible || done;
}
