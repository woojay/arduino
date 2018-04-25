 
 
 /** RF24Mesh_Example_Master.ino by TMRh20
  * 
  *
  * This example sketch shows how to manually configure a node via RF24Mesh as a master node, which
  * will receive all data from sensor nodes.
  *
  * The nodes can change physical or logical position in the network, and reconnect through different
  * routing nodes as required. The master node manages the address assignments for the individual nodes
  * in a manner similar to DHCP.
  *
  */
  
  
#include "RF24Network.h"
#include "RF24.h"
#include "RF24Mesh.h"
#include <SPI.h>
//Include eeprom.h for AVR (Uno, Nano) etc. except ATTiny
#include <EEPROM.h>

//==========LED
#include "FastLED.h"
#define NUM_LEDS 4
#define DATA_PIN 6
#define CLOCK_PIN 13

//=============== JOYSTICK
#include <stdio.h>
#define JOY_X_PIN 15
#define JOY_BUTTON_PIN 3

unsigned long joy_x_final = 0;
int joy_x_raw = 0;
int joy_button_final = 0;
int nodeRX = 0;

CRGB leds[NUM_LEDS];

/***** Configure the chosen CE,CS pins *****/
RF24 radio(7,8);
RF24Network network(radio);
RF24Mesh mesh(radio,network);

uint32_t displayTimer = 0;

int led_count = 0;

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  // Set the nodeID to 0 for the master node
  mesh.setNodeID(0);
  Serial.println(mesh.getNodeID());
  // Connect to the mesh
  mesh.begin();

}


void loop() {    


  // Call mesh.update to keep the network updated
  mesh.update();
  
  // In addition, keep the 'DHCP service' running on the master node so addresses will
  // be assigned to the sensor nodes
  mesh.DHCP();
  
  
  // Check for incoming data from the sensors
  if(network.available()){
    RF24NetworkHeader header;
    network.peek(header);
    
    uint32_t dat=0;
    uint8_t msgBuff[64] = {0};
    
    switch(header.type){
      // Display the incoming millis() values from the sensor nodes
      case 'M': network.read(header,msgBuff,sizeof(msgBuff)); 
        Serial.write(msgBuff, sizeof(msgBuff));
        sscanf(msgBuff, "%2d,%lu,%08u,%1d", &nodeRX, &displayTimer, &joy_x_final, &joy_button_final);
        Serial.println();
        break;
      default: network.read(header,0,0); Serial.println(header.type);break;
    }
  }

  
  if(millis() - displayTimer > 5000){
    displayTimer = millis();
    Serial.println(" ");
    Serial.println(F("********Assigned Addresses********"));
     for(int i=0; i<mesh.addrListTop; i++){
       Serial.print("NodeID: ");
       Serial.print(mesh.addrList[i].nodeID);
       Serial.print(" RF24Network Address: 0");
       Serial.println(mesh.addrList[i].address,OCT);
     }
    Serial.println(F("**********************************"));
  }

  if (led_count < 200)
  {
    if (led_count == 0)
    {
        for (int led=0; led<4; led++)
          leds[led] = joy_x_final;       
        
        FastLED.show();
    }
    led_count++;
  }

//  if ((led_count >= 25000) && (led_count < 26000))
//  {
//    if (led_count == 25000)
//    {
//        for (int led=0; led<4; led++)
//          leds[led] = 0x16AE3E + joy_x_final;       // R & G BACKWARDS
//        FastLED.show();
//    }
//    led_count++;   
//  }
//
  if (led_count >=200)
  {
    led_count = 0;
  }

  
}
