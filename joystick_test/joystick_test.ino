unsigned long joy_x_final = 0;
int joy_x_raw = 0;
int joy_button_final = 0;

#define JOY_X_PIN 15
#define JOY_BUTTON_PIN 3

char buf[40] = {0};

void setup() {
  // put your setup code here, to run once:
   pinMode(JOY_BUTTON_PIN, INPUT_PULLUP);
   pinMode(JOY_X_PIN, INPUT);

    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:


    joy_button_final = digitalRead(JOY_BUTTON_PIN) == HIGH ? 1 : 0;
    joy_x_final = map(analogRead(JOY_X_PIN), 0, 1023, 0, 98);

    sprintf(buf, "%08u %1d\n", joy_x_final, joy_button_final);
    Serial.println(buf);
    delay(200);
    
}
