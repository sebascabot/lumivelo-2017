#include <ESP8266WiFi.h>
#include <ESP8266WiFiMesh.h>
#include "FastLED.h"

#define NUM_LEDS 85
#define BRIGHTNESS 180
#define DATA_PIN 14
#define MOSFET_PIN 16

#define RED_PIN   15
#define GREEN_PIN 12
#define BLUE_PIN  13

// #define FLASH_BUTTON_PIN 0
#define PUSH_BUTTON_PIN 0

#define FRAMES_PER_SECOND  120
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

unsigned int request_i = 0;
unsigned int response_i = 0;

String requestHandler(String request);

ESP8266WiFiMesh mesh_node = ESP8266WiFiMesh(ESP.getChipId(), requestHandler);

CRGB leds[NUM_LEDS];
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void solid()
{
  // FastLED's built-in rainbow generator
  fill_solid( leds, NUM_LEDS, gHue);
}

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

typedef void (*SimplePatternList[])();
SimplePatternList patterns = {solid, sinelon, juggle, bpm, rainbow, rainbowWithGlitter, confetti };
// char* SimplePatternNames[] = {"solid", "sinelon", "juggle", "bpm", "rainbow", "rainbowWithGlitter", "confetti" };
uint8_t currentPatternIdx = 1; // Index number of which pattern is current

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  currentPatternIdx = (currentPatternIdx + 1) % ARRAY_SIZE( patterns);
}

void setPattern(int idx) {
  currentPatternIdx = idx;

}

String requestHandler(String request)
{
  Serial.print("Request(received): ");
  Serial.println(request);

  Serial.println(request[0]);

  int idx = atoi(request.c_str());

  setPattern(idx);

  /* return a string to send back */
  char response[60];
  sprintf(response, "%d Node %X --- response # %d", idx, ESP.getChipId(), response_i++);

  return response;
}

bool wasPressed = false;

void setup()
{
  Serial.begin(9600);
  delay(25);

  pinMode(MOSFET_PIN, OUTPUT);
  digitalWrite(MOSFET_PIN, HIGH);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();

  Serial.printf("Chip Id: %X\n", ESP.getChipId());

  pinMode(PUSH_BUTTON_PIN, INPUT_PULLUP);

  setPattern(0);

  Serial.println("\nSetting up mesh node...\n");

  mesh_node.begin();
}

void loop()
{
  mesh_node.acceptRequest();

  bool isPressed = (digitalRead(PUSH_BUTTON_PIN) == LOW);
  if (isPressed && !wasPressed) {
    wasPressed = true;
    Serial.println("Push button pressed!");

    char request[60];

    Serial.printf("Next pattern: ...\n");
    nextPattern();

    sprintf(request, "%d : Node %X --- Request # %d", currentPatternIdx, ESP.getChipId(), request_i++);

    Serial.print("Request(sending...): ");
    Serial.println(request);

    mesh_node.attemptScan(request);
  }

  if (!isPressed && wasPressed) {
    wasPressed = false;
    Serial.println("Push button released!");
  }

  // send the 'leds' array out to the actual LED strip
  //  FastLED.delay(1000/FRAMES_PER_SECOND);
  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
  EVERY_N_MILLISECONDS( 1000 / FRAMES_PER_SECOND ) {
    patterns[currentPatternIdx]();
    FastLED.show();  // insert a delay to keep the framerate modest
  }

  delay(100); // Push button debouncing

}

