/*
 * How to load the program:
 * 
 * find the .hex file from the info window below and use the micronucleus
 * 
 *   - or -
 *   
 * Make sure the tiny is UNPLUGGED
 * Compile the program for digispark
 * Set Micronucleus as the bootloader
 * Press the load button, AND THEN
 * Plug in the tiny
 * It should load just fine
 * 
 * ADDRESS 1 is the LED location for my digispark
 * 
 */

 
void pwmWrite(int pin, int pwm, int dir)
{
  if (dir==1)
  {
    for (int i = 0; i < pwm; i++)
    {
      analogWrite(pin, i);
      delay(5);
    }
  }
  else if (dir == -1)
  {
    for (int i = pwm; i >= 0; i--)
    {
      analogWrite(pin, i);
      delay(5);
    }
  }
}

void pwm_blink(void)
{
  for (int i = 0; i < 15; i++)
  {
    pwmWrite(1, 255, 1);
    pwmWrite(1, 255, -1);
    delay(10);               // wait for a second
  }
}

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(1, OUTPUT); //LED on Model A   
  pinMode(5, INPUT);  //p5 = analog input 0;

  pwmWrite(1,100,1);
  pwmWrite(1,100, -1);
}

int val;

void loop() {
 
  val = digitalRead(5);
  if (val)
    pwm_blink();
   delay(100);


}
