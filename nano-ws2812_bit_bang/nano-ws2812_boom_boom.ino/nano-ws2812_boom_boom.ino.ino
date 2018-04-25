#include "FastLED.h"

#define NUM_LEDS 4
#define DATA_PIN 6
#define CLOCK_PIN 13

CRGB leds[NUM_LEDS];



void setup() 
{ 
       FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop() { 

    for (int led=0; led<4; led++)
      leds[led] = 0x000000;     
    FastLED.show();
    delay(100);
    
    
    for (int led=0; led<4; led++)
      leds[led] = 0x16AE3E;       // R & G BACKWARDS
    FastLED.show();
    delay(150);

    for (int led=0; led<4; led++)
      leds[led] = 0x000000;      
    FastLED.show();
    delay(200);

    for (int led=0; led<4; led++)
      leds[led] =  0x16AE3E;     
    FastLED.show();
    delay(500);

    
    
    
    
    
    FastLED.show();
}
