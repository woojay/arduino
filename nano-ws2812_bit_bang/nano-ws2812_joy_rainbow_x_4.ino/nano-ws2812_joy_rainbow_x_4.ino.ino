#include "FastLED.h"
#include <Wire.h>
#include <math.h>

// LED =====================
#define NUM_LEDS 4
#define DATA_PIN 2
#define CLOCK_PIN 13

#define RMAX 230    // 255 - val is the true max brightness
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

// STICK ===================
#define X_PIN A3  // Marked backwards on the board
#define Y_PIN A2

int x;
int y;

void setup() 
{ 
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);  
  Serial.begin(57600);
}

void loop() 
{ 
  do_joystick();
  FastLED.show();
  
//  Serial.print("  x: "); Serial.print(x);
//  Serial.print("  x_f: "); Serial.print(x_f);
//  Serial.print("  y: "); Serial.print(y);
//  Serial.print("  y_f: "); Serial.print(y_f);
//  Serial.print("  ang: "); Serial.print(angle);
//  Serial.print("  i_f: "); Serial.print(i_f);
//  
//  Serial.print("  b: "); Serial.print(base);
//  Serial.print("  r: "); Serial.print(red);
//  Serial.print("  g: "); Serial.print(grn);
//  Serial.print("  b: "); Serial.println(blu);

  delay(10);
}


float map_f(float val, float low_st, float hi_st, float low_f, float hi_f)
{
  return (val-low_st) / (hi_st - low_st) * (hi_f - low_f) + low_f;
}

void do_joystick()
{
  float x_f;
  float y_f;

  // Read joystick values
  x = analogRead(X_PIN);
  x_f = map_f((float)x, 0.0, 1023.0, -1.0, 1.0);
  y = analogRead(Y_PIN);
  y_f = map_f((float)y, 0.0, 1023.0, -1.0, 1.0);

  // find angle
  float angle = atan(y_f/x_f);

  // map angle from o to 600
  float i_f = map_f(angle, -M_PI_2, M_PI_2, 0.0, 600.0);

    for (int led=0; led<4; led++)
    {
      int i2 = (int)i_f + (led*150);
      int i3 = ( i2 >=600 ? i2 - 600 : i2 );
      int base = floor((float)i3 / 100);
      
      int red = red_map[base] + (red_map[base+1] - red_map[base]) * (i3 - (base*100)) / 100;
      int grn = grn_map[base] + (grn_map[base+1] - grn_map[base]) * (i3 - (base*100)) / 100;
      int blu = leds[0].b = blu_map[base] + (blu_map[base+1] - blu_map[base]) * (i3 - (base*100)) / 100;

      leds[led].r = red;  
      leds[led].g = grn;
      leds[led].b = blu;
    }
}

