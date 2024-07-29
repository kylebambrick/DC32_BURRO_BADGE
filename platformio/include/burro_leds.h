// burro_leds.h
#ifndef BURRO_LEDS_H
#define BURRO_LEDS_H

#include <Adafruit_NeoPixel.h>
#include "burro_mem.h"

// RGB variables
#define RGB_PIN 27
#define NUM_LEDS 9

extern Adafruit_NeoPixel strip;
extern int brightness;

extern unsigned long previousMillis;
extern unsigned long patternChangeMillis;
extern unsigned long speed;
extern unsigned long interval;
extern unsigned long patternInterval;

extern bool onOff;
extern int pattern;
extern int counter;
extern bool okToChange;
extern bool forceNotChange;
extern bool forceChange;
extern uint32_t currentColor;
extern int meshCounter;

extern bool unlockedOne;
extern bool unlockedTwo;
extern bool unlockedThree;
extern bool unlockedFour;

// Eye Variables
#define EYE_LEFT 18
#define EYE_RIGHT 19

extern unsigned long previousMillisLeft;
extern unsigned long previousMillisRight;
extern unsigned long intervalEyes;
extern bool leftEyeState;
extern bool rightEyeState;


//button variables
extern int buttonPin;
extern int debounceDelay;
extern bool buttonState;
extern bool lastButtonState;
extern unsigned long lastDebounceTime; 


// Function prototypes
void initLEDs();
void updateLEDs(uint32_t meshTime);

void updateEye(int pin, unsigned long &lastUpdateTime, const long interval, bool &currentState);
void wink();

void updateButton();

void updateRGB(uint32_t meshTime);
void colorStrip(uint32_t color);
void chasePlus(uint32_t color);
void chaseMinus(uint32_t color);
void blink(uint32_t color);
void sparkle(uint32_t color);
void drip(uint32_t color);
void chain(uint32_t color);
void pulse(uint32_t color);
void rainbow();
void randomLED();
void rainbowWheel();
void moreColors();

extern void setColor(int i);
extern void setRGB(int r, int g, int b);
extern void unlockLEDPattern(String flag);
extern void setRGB(int r, int g, int b);
extern void setPattern(int id);
extern void setItToChange();
extern void resetPattern();

#endif // BURRO_LEDS_H