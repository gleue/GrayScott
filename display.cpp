#include "display.h"
#include <EEvar.h>

const EEstore<uint8_t> eeBrightness = 96;

CRGB leds[NUM_LEDS];

void initDisplay() {
    FastLED.addLeds<WS2812B,LED_PIN,GRB>(leds,NUM_LEDS).setCorrection(TypicalLEDStrip);

    uint8_t brightness;

    eeBrightness >> brightness;
    FastLED.setBrightness(brightness);
}

void clearDisplay() {
    memset(leds, CRGB::Black, NUM_LEDS * sizeof(CRGB));
    FastLED.show();
}

void setBrightness(uint8_t brightness) {
    eeBrightness << brightness;
    FastLED.setBrightness(brightness);
}

void drawPixels(fixed cellsA[SIM_DIM][SIM_DIM], fixed cellsB[SIM_DIM][SIM_DIM]) {
    for (uint8_t i = 0, idx = 0; i < SIM_DIM; i++) {
	    for (uint8_t j = 0; j < SIM_DIM; j++, idx++) {
            float a = float(cellsA[i][j]);
            float b = float(cellsB[i][j]);
            float f = a - b;
            uint8_t hue = 80.0 + f * 80.0;
            leds[idx] = CHSV(hue, uint8_t(constrain((a+b) * 255.0, 0.0, 255.0)), 64);
	    }
    }
    FastLED.show();
}

void drawValue(fixed value, fixed min, fixed max, CRGB color, const bool *name, CRGB name_color) {
    fixed led_count = (value - min) / (max - min) * fixed(NUM_LEDS);
    
    for (uint8_t idx = 0; idx < led_count.getInteger(); idx++) {
        leds[idx] = color;
    }

    fixed fraction = led_count - led_count.getInteger();
    CHSV hsv = rgb2hsv_approximate(color);

    hsv.value = uint8_t(fixed(hsv.value) * fraction);
    leds[led_count.getInteger()] = hsv;

    for (uint8_t idx = led_count.getInteger() + 1; idx < NUM_LEDS; idx++) {
        leds[idx] = CRGB::Black;
    }

    if (name) {
        for (uint8_t idx = 0; idx < NUM_LEDS; idx++) {
            bool on = pgm_read_byte_near(name + idx);
            if (on) leds[idx] = name_color;
        }
    }

    FastLED.show();
}

void drawName(const bool *name, CRGB color, CRGB bg_color) {
    for (uint8_t idx = 0; idx < NUM_LEDS; idx++) {
        bool on = pgm_read_byte_near(name + idx);
        leds[idx] = on ? color : bg_color;
    }

    FastLED.show();
}
