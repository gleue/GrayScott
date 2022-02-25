#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <FastLED.h>

#include "global.h"

#define LED_PIN 3
#define NUM_LEDS (SIM_DIM*SIM_DIM)

#define delay FastLED.delay

extern CRGB leds[NUM_LEDS];

void initDisplay();
void clearDisplay();

void drawPixels(fixed cellsA[SIM_DIM][SIM_DIM], fixed cellsB[SIM_DIM][SIM_DIM]);
void drawValue(fixed value, fixed min, fixed max, CRGB color, const bool *name = NULL, CRGB name_color = CRGB::Black);
void drawName(const bool *name, CRGB color = CRGB::Grey, CRGB bg_color = CRGB::Black);

#endif // __DISPLAY_H__
