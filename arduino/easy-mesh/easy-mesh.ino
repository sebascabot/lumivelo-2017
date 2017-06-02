
//************************************************************
// this is a simple example that uses the easyMesh library
//
// 1. blinks led once for every node on the mesh
// 2. blink cycle repeats every BLINK_PERIOD
// 3. sends a silly message to every node on the mesh at a random time betweew 1 and 5 seconds
// 4. prints anything it recieves to Serial.print
//
//
//************************************************************
#include <easyMesh.h>
#include "FastLED.h"

#define NUM_LEDS 150
#define BRIGHTNESS 75
#define DATA_PIN 14
#define MOSFET_PIN 16

#define RED_PIN   15
#define GREEN_PIN 12
#define BLUE_PIN  13

#define PUSH_BUTTON_PIN 0

#define FRAMES_PER_SECOND  120
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#define   MESH_PREFIX     "lumivelo"
#define   MESH_PASSWORD   "PapasInventeurs"
#define   MESH_PORT       5555

#define SENDING_PERIOD       60000000 // Every 60s, send Message --- 1,000,000 us = 1s (period = 1 / frequency)
#define LAST_SENT_DELAY     45000000 // Wait 45 sec, before sending message again
#define LAST_RECEIVED_DELAY  15000000 // Wait 15 sec for another received message, before going back to sending state.
uint32_t last_offset_delay = 0;

easyMesh  mesh;

bool sending = false;

uint32_t latestSentMsgTime = 0;
uint32_t latestReceivedMsgTime = 0;

int n = 0;

CRGB leds[NUM_LEDS];
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void solid()
{
  // FastLED's built-in rainbow generator
  fill_solid( leds, NUM_LEDS, CHSV(gHue, 255, 255));
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
uint8_t currentPatternIdx = 1; // Index number of which pattern is current

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  currentPatternIdx = (currentPatternIdx + 1) % ARRAY_SIZE( patterns);
}

void setPattern(int idx) {
  currentPatternIdx = idx;

}

bool wasPressed = false;

void setup() {
  Serial.begin(9600);

  pinMode(MOSFET_PIN, OUTPUT);
  digitalWrite(MOSFET_PIN, HIGH);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();

  pinMode(PUSH_BUTTON_PIN, INPUT_PULLUP);

  setPattern(0);

  // mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT );

  Serial.printf("ChipId: %x\n", mesh.getChipId());

  mesh.setReceiveCallback( &receivedCallback );
  mesh.setNewConnectionCallback( &newConnectionCallback );

  randomSeed( analogRead( A0 ) );
}

void loop() {
  mesh.update();

  bool isPressed = (digitalRead(PUSH_BUTTON_PIN) == LOW);

  if (isPressed && !wasPressed) { // Press transition
    wasPressed = true;
    Serial.println("Push button pressed transition !!!");

    n = 0;
    // sending = true;

    sendMessage();

  } else if (!isPressed && wasPressed) { // Unpress transition
    wasPressed = false;
    Serial.println("Push button released transition !!!");
  } else if (isPressed) {
    // Button is press (PRESS-STATE)
    Serial.println("Push button is pressed.");
  } else {
    // Serial.println("Push button is released.");
  }

  // Send/Receive Logic
  if (sending) {
    if (n > 30) { // After (talk 30 time in a row, stay quiet after that; }
      sending = false;
    } else if (mesh.getNodeTime() > latestSentMsgTime + SENDING_PERIOD) {
      sendMessage();
    }
  } else {
    if (mesh.getNodeTime() > latestSentMsgTime + LAST_SENT_DELAY + last_offset_delay
        && mesh.getNodeTime() > latestReceivedMsgTime + LAST_RECEIVED_DELAY + last_offset_delay) {
      n = 0;
      sendMessage(); // Start sending again.
    }
  }

  // send the 'leds' array out to the actual LED strip
  //  FastLED.delay(1000/FRAMES_PER_SECOND);
  // do some periodic updates
  EVERY_N_MILLISECONDS( 30 ) {
    gHue += 2;  // slowly cycle the "base color" through the rainbow
  }
  EVERY_N_MILLISECONDS( 1000 / FRAMES_PER_SECOND ) {
    patterns[currentPatternIdx]();
    FastLED.show();  // insert a delay to keep the framerate modest
  }
}

void sendMessage() {
  // Update state
  sending = true;
  latestSentMsgTime = mesh.getNodeTime();
  last_offset_delay = random( 3000000 );
  n = n + 1;

  nextPattern();
  String msg = String(currentPatternIdx);
  msg += " --- Message from node ";
  msg += String(mesh.getChipId(), HEX);
  mesh.sendBroadcast( msg );

  Serial.printf("Sent: %s\n", msg.c_str());
}

void receivedCallback( uint32_t from, String &msg ) {
  // Update state
  sending = false;
  latestReceivedMsgTime = mesh.getNodeTime();
  last_offset_delay = random( 3000000 );

  int idx = atoi(msg.c_str());
  setPattern(idx);

  Serial.printf("Callback-Received: From node %x msg=%s\n", from, msg.c_str());
}

void newConnectionCallback( bool adopt ) {
  setPattern(0); // Connected for 1st time, or connected again
  Serial.printf("Callback-New-Connection: adopt=%d\n", adopt);
}

