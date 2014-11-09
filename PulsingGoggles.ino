#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

#define STATE_OFF 0
#define STATE_FADING_IN 1
#define STATE_FADING_OUT 2
#define STATE_ACTIVE 3

#define MAX_RED 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, 12, NEO_GRB + NEO_KHZ800);

static int state = STATE_FADING_IN;
static int fade = 0;
static int redLevel[24];
static int redLevelTarget[24];

void setup()
{
  randomSeed(analogRead(0));
  Serial.begin(115200);
  strip.begin();
  strip.show();
  for (int i = 0; i < 24; i++) {
    redLevel[i] = random(0,MAX_RED);
    redLevelTarget[i] = random(0,MAX_RED);
  }
  pinMode(8,INPUT);
}

void updateFade()
{
  for (int i = 0; i < 24; i++) {
    int rl = (redLevel[i] * fade) / 255;
    strip.setPixelColor(i, rl, 0, 0);
  }
  strip.show();
}

void updateFadeIn()
{
  fade++;
  if (fade == 255) {
    state = STATE_ACTIVE;
  }
  updateFade();
}

void updateFadeOut()
{
  fade--;
  if (fade == 0) {
    state = STATE_OFF;
  }
  updateFade();
}

void updateActive()
{
  for (int i = 0; i < 24; i++) {
    if (redLevel[i] == redLevelTarget[i]) {
      if (redLevelTarget[i] > 20) {
        redLevelTarget[i] = random(0,30);
      } else {
        redLevelTarget[i] = random(30,MAX_RED);
      }
    } else if (redLevel[i] > redLevelTarget[i]) {
      redLevel[i]--;
    } else if (redLevel[i] < redLevelTarget[i]) {
      redLevel[i]++;
    }
  }
  updateFade();
}

void loop() {
  delay(15);
  switch (state) {
    case STATE_OFF:
      if (digitalRead(8) == HIGH) {
        state = STATE_FADING_IN;
      }
      break;
    case STATE_FADING_IN:
      updateFadeIn();
      break;
    case STATE_FADING_OUT:
      updateFadeOut();
      break;
    case STATE_ACTIVE:
      updateActive();
      if (digitalRead(8) == HIGH) {
        state = STATE_FADING_OUT;
      }
      break;
  }
}
