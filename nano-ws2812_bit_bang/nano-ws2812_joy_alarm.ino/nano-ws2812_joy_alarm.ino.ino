#include "FastLED.h"
#include <MD_DS3231.h>
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

// RTC =====================
#define PRINTS(s)   Serial.print(F(s))
#define PRINT(s, v) { Serial.print(F(s)); Serial.print(v); }

// STICK ===================
#define X_PIN A3  // Marked backwards on the board
#define Y_PIN A2

int x;
int y;

// ALARM ===================
uint8_t al_hour = 5;
uint8_t al_min = 30;
int al_start = 0;

void setup() 
{ 
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
   
  RTC.control(DS3231_CLOCK_HALT, DS3231_OFF);
  RTC.control(DS3231_SQW_ENABLE, DS3231_OFF);
  RTC.control(DS3231_12H, DS3231_OFF);
  RTC.setAlarm1Callback(secCallback);
  RTC.setAlarm1Type(DS3231_ALM_SEC);
  RTC.setAlarm2Callback(alm2Callback_1);
  RTC.setAlarm2Type(DS3231_ALM_HM);  // @ Specific al_hour and al_min
  // Set alarm2
  setAlarm2();
  
  Serial.begin(57600);
}

void loop() 
{ 
//  do_joystick();
  FastLED.show();

  //Check clock;
  RTC.checkAlarm1();
  RTC.checkAlarm2();
  
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

char *p2dig(uint8_t v, uint8_t mode)
// 2 digits leading zero
{
  static char s[3];
  uint8_t i = 0;
  uint8_t n = 0;
 
  switch(mode)
  {
    case HEX: n = 16;  break;
    case DEC: n = 10;  break;
  }

  if (v < n) s[i++] = '0';
  itoa(v, &s[i], n);
  
  return(s);
}
const char *dow2String(uint8_t code)
{
  static const char *str[] = {" ---", " Sun", " Mon", " Tue", " Wed", " Thu", " Fri", " Sat"};

  return(str[code]);
}

void printTime()
{
  PRINT("", RTC.yyyy);
  PRINT("-", p2dig(RTC.mm, DEC));
  PRINT("-", p2dig(RTC.dd, DEC));
  PRINT(" ", p2dig(RTC.h, DEC));
  PRINT(":", p2dig(RTC.m, DEC));
  PRINT(":", p2dig(RTC.s, DEC));
  if (RTC.status(DS3231_12H) == DS3231_ON)
    PRINT("", RTC.pm ? " pm" : " am");
  PRINT("", dow2String(RTC.dow));
}

void setAlarm2(void)
// Set Alarm 2 values in the RTC 
{
  RTC.yyyy = RTC.mm = RTC.dd = 0;
  RTC.h = al_hour;
  RTC.m = al_min;
  RTC.s = RTC.dow = 0;
  RTC.writeAlarm2(DS3231_ALM_HM);
}  

void secCallback()
{
  RTC.readTime();
  PRINTS("\nTime - ");
  printTime(); 

  int sec = RTC.s;
  int minute = RTC.m;
  int hour = RTC.h;

//  float i_f = map_f(sec, 0.0, 60.0, 0.0, 600.0);
//  int base = floor((float)i_f / 100);
//  leds[1].r = red_map[base] + (red_map[base+1] - red_map[base]) * (i_f - (base*100)) / 100;
//  leds[1].g = grn_map[base] + (grn_map[base+1] - grn_map[base]) * (i_f - (base*100)) / 100;
//  leds[1].b = blu_map[base] + (blu_map[base+1] - blu_map[base]) * (i_f - (base*100)) / 100;

//  i_f = map_f(minute, 0.0, 60.0, 0.0, 600.0);
//  base = floor((float)i_f / 100);
//  leds[2].r = red_map[base] + (red_map[base+1] - red_map[base]) * (i_f - (base*100)) / 100;
//  leds[2].g = grn_map[base] + (grn_map[base+1] - grn_map[base]) * (i_f - (base*100)) / 100;
//  leds[2].b = blu_map[base] + (blu_map[base+1] - blu_map[base]) * (i_f - (base*100)) / 100;

//  i_f = map_f(hour, 0.0, 24.0, 0.0, 600.0);
//  base = floor((float)i_f / 100);
//  leds[3].r = red_map[base] + (red_map[base+1] - red_map[base]) * (i_f - (base*100)) / 100;
//  leds[3].g = grn_map[base] + (grn_map[base+1] - grn_map[base]) * (i_f - (base*100)) / 100;
//  leds[3].b = blu_map[base] + (blu_map[base+1] - blu_map[base]) * (i_f - (base*100)) / 100;
}

// Once alarm is started, switch to minute trigger for the lighting program
void alm2Callback_1()
{ 
  RTC.setAlarm2Callback(alm2Callback_2);    
  RTC.setAlarm2Type(DS3231_ALM_MIN);      // Every minute
  for (int i = 0; i <= 3; i++)
  {
    leds[i].r = 2;  // 1 is not noticeable
    leds[i].g = 2;
    leds[i].b = 2;
  }
}

void alm2Callback_2()
{
  if (leds[3].r <= 60)
  {
    for (int i = 0; i <= 3; i++)
    {  
      if (i > 7)
      {
        leds[i].r++;  
        leds[i].g++;
        leds[i].b++;
      }
      else
      {
        leds[i].r += 3;  
        leds[i].g += 3;
        leds[i].b += 3;        
      }
    }
  } 
  else
  {
    for (int i = 0; i <= 3; i++)
    { 
      leds[i].r = 0;  
      leds[i].g = 0;
      leds[i].b = 0;
    }
    // Reset alarm to previous setting
    setAlarm2();
    RTC.setAlarm2Callback(alm2Callback_1);
    RTC.setAlarm2Type(DS3231_ALM_HM); 
  }
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

  // decode rgb values from index (i_f)
  int base = floor((float)i_f / 100);
  int red = red_map[base] + (red_map[base+1] - red_map[base]) * (i_f - (base*100)) / 100;
  int grn = grn_map[base] + (grn_map[base+1] - grn_map[base]) * (i_f - (base*100)) / 100;
  int blu = blu_map[base] + (blu_map[base+1] - blu_map[base]) * (i_f - (base*100)) / 100;

  leds[0].r = red;  
  leds[0].g = grn;
  leds[0].b = blu;
}

