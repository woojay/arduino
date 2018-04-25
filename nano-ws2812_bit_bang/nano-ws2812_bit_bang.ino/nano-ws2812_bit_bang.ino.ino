#include "FastLED.h"

#define NUM_LEDS 4
#define DATA_PIN 2
#define CLOCK_PIN 13

#define RMAX 230
#define GMAX RMAX
#define BMAX RMAX

CRGB leds[NUM_LEDS];

// Reference:
// https://electronics.stackexchange.com/questions/240185/controlling-color-and-brightness-of-rgb-led-strip-using-one-potentiometer

//int red_map[] = {255-RMAX, 255-RMAX, 0, 0, 0, 255-RMAX, 255-RMAX};
//int grn_map[] = {0, 255-GMAX, 255-GMAX, 255-GMAX, 0, 0, 0};

int grn_map[] = {255-RMAX, 255-RMAX, 0, 0, 0, 255-RMAX, 255-RMAX};  // r and g swapped on module
int red_map[] = {0, 255-GMAX, 255-GMAX, 255-GMAX, 0, 0, 0};
int blu_map[] = {0, 0, 0, 255-BMAX, 255-BMAX, 255-BMAX, 0};

void setup() 
{ 
       FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop() { 

  for (int i = 0; i < 600; i++) 
  {
    for (int led=0; led<4; led++)
    {
      int i2 = i + (led*150);
      int i3 = ( i2 >=600 ? i2 - 600 : i2 );
      int base = floor((float)i3 / 100);
      
      int red = red_map[base] + (red_map[base+1] - red_map[base]) * (i3 - (base*100)) / 100;
      int grn = grn_map[base] + (grn_map[base+1] - grn_map[base]) * (i3 - (base*100)) / 100;
      int blu = leds[0].b = blu_map[base] + (blu_map[base+1] - blu_map[base]) * (i3 - (base*100)) / 100;
  
      leds[led].r = red;
      leds[led].g = grn;
      leds[led].b = blu;
      
    }
    FastLED.show();
  }

//  leds[2] = CRGB::Red;    r -> g  g -> r
}
