#include "burro_leds.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);
int brightness = 30;

unsigned long previousMillis = 0;
unsigned long patternChangeMillis = 0;
unsigned long speed = 1;
unsigned long interval = 100;
unsigned long patternInterval = 10000;

int counter = 0;
bool onOff = true;
int pattern = 1;
bool okToChange = true;
bool forceNotChange = false;
bool forceChange = false;
int patternMax = 12;
int meshCounter = 0;

bool unlockedOne = false;
bool unlockedTwo = false;
bool unlockedThree = false;
bool unlockedFour = false;

unsigned long previousMillisLeft = 0;
unsigned long previousMillisRight = 0;
unsigned long intervalEyes = 500;
bool leftEyeState = HIGH;
bool rightEyeState = HIGH;

uint32_t yellow = strip.Color(255, 130, 0);
uint32_t white =  strip.Color(255, 255, 255);
uint32_t red =    strip.Color(255, 0, 0);
uint32_t orange = strip.Color(255, 127, 0);
uint32_t green =  strip.Color(0, 255, 0);
uint32_t blue =   strip.Color(0, 0, 255);
uint32_t indigo = strip.Color(75, 0, 130);
uint32_t violet = strip.Color(148, 0, 211);
uint32_t pink =   strip.Color(255, 20, 147);
uint32_t off =    strip.Color(0, 0, 0);

uint32_t currentColor = white;

int buttonPin = 12;
int debounceDelay = 50;
bool buttonState = true;
bool lastButtonState = false;
unsigned long lastDebounceTime = 0; 

void initLEDs() {

  pinMode(EYE_LEFT, OUTPUT);
  pinMode(EYE_RIGHT, OUTPUT);

  digitalWrite(EYE_LEFT, HIGH);
  digitalWrite(EYE_RIGHT, HIGH);

  pinMode(buttonPin, INPUT_PULLUP);

  strip.begin();
  strip.setBrightness(brightness);
  strip.show();

  setColor(flags.colorID);

  if (flags.flagSGS)
    unlockedOne = true;
  if (flags.flagUser)
    unlockedTwo = true;
  if (flags.flagPass)
    unlockedThree = true;
  if (flags.flagKyle || flags.flagUnlockIt) {
    unlockedOne = true;
    unlockedFour = true;
    unlockedThree = true;
    unlockedTwo = true;
  }

}

void updateLEDs(uint32_t meshTime) {
  updateRGB(meshTime);

  updateButton();

  if (leftEyeState == LOW)
    updateEye(EYE_LEFT, previousMillisLeft, intervalEyes, leftEyeState);

  if (rightEyeState == LOW)
    updateEye(EYE_RIGHT, previousMillisRight, intervalEyes, rightEyeState);

  if (buttonState == LOW) {
    digitalWrite(EYE_LEFT, LOW);
    digitalWrite(EYE_RIGHT, LOW);
  }

}

void updateButton() {
  int reading = digitalRead(buttonPin);
  
  if (reading != lastButtonState)
    lastDebounceTime = millis();

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      digitalWrite(EYE_LEFT, buttonState);
      digitalWrite(EYE_RIGHT,buttonState);
    }
  }
  lastButtonState = reading;
}

void updateEye(int pin, unsigned long &lastUpdateTime, const long interval, bool &currentState) {
  unsigned long currentMillis = millis();

  if (currentMillis - lastUpdateTime >= interval && buttonState == HIGH) {
    lastUpdateTime = currentMillis;
    currentState = HIGH;
    digitalWrite(pin, HIGH);
  }

}

void wink() {
  if (random(2) == 0) {
    leftEyeState = LOW;
    digitalWrite(EYE_LEFT, LOW);
    previousMillisLeft = millis();
  } else {
    rightEyeState = LOW;
    digitalWrite(EYE_RIGHT, LOW);
    previousMillisRight = millis();
  }
}

void updateRGB(uint32_t meshTime) {

  unsigned long currentMillis = meshTime;
  if (currentMillis - previousMillis >= (interval * speed)) {
    previousMillis = currentMillis;

    switch (pattern) {
      case 0:
        colorStrip(off);
        speed = 100;
        break;
      case 1:
        chasePlus(currentColor);
        speed = 1;
        break;
      case 2:
        chaseMinus(currentColor);
        speed = 1;
        break;
      case 3:
        blink(currentColor);
        speed = 3;
        break;
      case 4:
        colorStrip(currentColor);
        speed = 20;
        break;
      case 5:
        sparkle(currentColor);
        speed = 1;
        break;
      case 6:
        drip(currentColor);
        speed = 2;
        break;
      case 7:
        chain(currentColor);
        speed = 2;
        break;
      case 8:
        pulse(currentColor);
        speed = .1;
        break;
      case 9: //unlockedOne
        rainbow();
        speed = 2;
        break;
      case 10: //unlockedTwo
        randomLED();
        speed = 1;
        break;
      case 11: //unlockedThree
        rainbowWheel();
        speed = 1;
        break;
      case 12: //unlockedFour
        moreColors();
        speed = 2;
        break;
    }
  }

  if (currentMillis - patternChangeMillis >= patternInterval && okToChange) {
    patternChangeMillis = currentMillis;
    okToChange = true;
    strip.setBrightness(brightness);
    if (!forceNotChange) {
      pattern++;
      while ((pattern == 9 && !unlockedOne) || 
            (pattern == 10 && !unlockedTwo) || 
            (pattern == 11 && !unlockedThree) || 
            (pattern == 12 && !unlockedFour)) {
          pattern++;
      }
      if (pattern > patternMax)
        pattern = 0;

      counter = 0;
    } else if(forceChange) {
      meshCounter++;
      if (meshCounter > 10) {
        forceChange = false;
        forceNotChange = false;
        meshCounter = 0;
      }
    }
  }
}

void colorStrip(uint32_t color) {
  for(int i=0; i<NUM_LEDS; i++)
    strip.setPixelColor(i, color);
  strip.show();
}

void chasePlus(uint32_t color) {
  okToChange = false;
  colorStrip(off);
  strip.setPixelColor(counter, color);
  strip.show();
  counter++;
  if (counter >= NUM_LEDS + 1) {
    counter = 0;
    okToChange = true;
  }
}

void chaseMinus(uint32_t color) {
  okToChange = false;
  colorStrip(off);
  strip.setPixelColor(counter, color);
  strip.show();
  counter--;
  if (counter < 0) {
    okToChange = true;
    counter = NUM_LEDS + 1;
  }
}

void blink(uint32_t color) {
  if (counter > 0) {
    counter = 0;
    colorStrip(off);
  } else {
    counter = 1;
    colorStrip(color);
  }
}

void sparkle(uint32_t color) {
  colorStrip(off);
  strip.setPixelColor(random(0, 10), color);
  strip.setPixelColor(random(0, 10), color);
  strip.show();
}

void chain(uint32_t color) {
  colorStrip(off);
  if (counter) {
    strip.setPixelColor(0, color);
    strip.setPixelColor(2, color);
    strip.setPixelColor(4, color);
    strip.setPixelColor(6, color);
    strip.setPixelColor(8, color);
    counter = 0;
  } else {
    strip.setPixelColor(1, color);
    strip.setPixelColor(3, color);
    strip.setPixelColor(5, color);
    strip.setPixelColor(7, color);
    counter = 1;
  }
  strip.show();
}

void drip(uint32_t color) {
  colorStrip(off);
  switch (counter) {
    case 0:
      strip.setPixelColor(0, color);
      strip.setPixelColor(8, color);
      break;
    case 1:
      strip.setPixelColor(1, color);
      strip.setPixelColor(7, color);
      break;
    case 2:
      strip.setPixelColor(2, color);
      strip.setPixelColor(6, color);
      break;
    case 3:
      strip.setPixelColor(3, color);
      strip.setPixelColor(5, color);
      break;
  }
  strip.show();
  counter++;
  if (counter > 4) {
    counter = 0;
    okToChange = true;
  }
}

void pulse(uint32_t color) {
  okToChange = false;
  colorStrip(color);
  int temp = counter;
  if (counter > 100)
    temp = 200 - counter;
  float brightnessPercent = (float)temp / 100.0;
  strip.setBrightness(brightness * brightnessPercent);
  strip.show();
  counter++;
  if (counter >= 200) {
    okToChange = true;
    counter = 0;
  }
}

void rainbow() {
  uint32_t colors[9] = {
    red, orange, yellow, green, blue, indigo, violet, white, pink
  };
  for (int i = 0; i < 9; i++)
    strip.setPixelColor(i, colors[(i + counter) % 9]);
  counter++;
  if (counter > 9)
    counter = 0;
  strip.show();
}

void randomLED() {
  for(int i=0; i<NUM_LEDS; i++)
    strip.setPixelColor(i, strip.Color(random(0, 256), random(0, 256), random(0, 256)));
  strip.show();
}

void rainbowWheel() {
  okToChange = false;
  uint16_t WheelPos = counter % 255;
  uint32_t color;

  if(WheelPos < 85) {
      color = strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
      WheelPos -= 85;
      color = strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
      WheelPos -= 170;
      color = strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  colorStrip(color);

  counter++;
  if(counter > 255) {
    counter = 0;
    okToChange = true;
  }
}

void moreColors() {
  uint32_t colors[10] = {
    red, orange, yellow, green, blue, indigo, violet, white, pink, off
  };
  counter = random(0,10);
  
  colorStrip(colors[counter]);
}


void setColor(int i) {
  currentColor = white;
  switch(i) {
    case 0:
      currentColor = yellow;
      break;
    case 1:
      currentColor = white;
      break;
    case 2:
      currentColor = red;
      break;
    case 3:
      currentColor = orange;
      break;
    case 4:
      currentColor = green;
      break;
    case 5:
      currentColor = blue;
      break;
    case 6:
      currentColor = indigo;
      break;
    case 7:
      currentColor = violet;
      break;
    case 8:
      currentColor = pink;
      break;
  }
}

void setRGB(int r, int g, int b) {
  if (r > 255) 
    r = 255;
  if (g > 255) 
    g = 255;
  if (b > 255) 
    b = 255;

  currentColor = strip.Color(r, g, b);

}

void setPattern(int id) {
  forceNotChange = true;
  forceChange = true;
  pattern = id;
  if (id > patternMax)
    pattern = patternMax;
}

void resetPattern() {
  forceNotChange = false;
  forceChange = true;
  pattern = random(0, patternMax + 1);
}

void setItToChange() {
  forceNotChange = false;
  forceChange = true;
}

void unlockLEDPattern(String flag) {
  if (flag == "SGS") {
    unlockedOne = true;
    return;
  }
  if (flag == "the_username_is_burroadmin") {
    unlockedTwo = true;
    return;
  }
  if (flag == "sUPErZ3cR3t1337PA5SW0Rd!") {
    unlockedThree = true;
    return;
  }
  if (flag == "Ic@nL0g1n!") {
    unlockedFour = true;
    return;
  }
  if (flag == "blametheotherkyle" || flag == "unlock_it") {
    unlockedOne = true;
    unlockedTwo = true;
    unlockedThree = true;
    unlockedFour = true;
    return;
  }
}