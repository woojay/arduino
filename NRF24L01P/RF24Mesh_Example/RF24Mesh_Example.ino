
/** RF24Mesh_Example.ino by TMRh20

   This example sketch shows how to manually configure a node via RF24Mesh, and send data to the
   master node.
   The nodes will refresh their network address as soon as a single write fails. This allows the
   nodes to change position in relation to each other and the master node.
*/


#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <SPI.h>
//#include <printf.h>
#include <stdio.h>

//=============== JOYSTICK
#define JOY_X_PIN 15
#define JOY_BUTTON_PIN 3

unsigned long joy_x_final = 0;
int joy_x_raw = 0;
int joy_button_final = 0;


/**** Configure the nrf24l01 CE and CS pins ****/
RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

/**
   User Configuration: nodeID - A unique identifier for each radio. Allows addressing
   to change dynamically with physical changes to the mesh.

   In this example, configuration takes place below, prior to uploading the sketch to the device
   A unique value from 1-255 must be configured for each node.
   This will be stored in EEPROM on AVR devices, so remains persistent between further uploads, loss of power, etc.

 **/
#define nodeID 1

uint8_t msgBuff[64] = {0};

unsigned long displayTimer = 0;
unsigned long joystickVal = 0;

struct payload_t {
  unsigned long ms;
  unsigned long counter;
};

void setup() {

  //========== JOYSTICK
  pinMode(JOY_BUTTON_PIN, INPUT_PULLUP);
  pinMode(JOY_X_PIN, INPUT);
   
  Serial.begin(115200);
  //printf_begin();
  // Set the nodeID manually
  mesh.setNodeID(nodeID);
  // Connect to the mesh
  Serial.println(F("Connecting to the mesh..."));
  mesh.begin();
}



void loop() {

  int str_len = 0;
  mesh.update();

  // Send to the master node every 500ms
  if (millis() - displayTimer >= 299) {
    displayTimer = millis();

    //========== JOYSTICK
    joy_button_final = digitalRead(JOY_BUTTON_PIN) == HIGH ? 1 : 0;
    joy_x_final = map(analogRead(JOY_X_PIN), 0, 1023, 0, 98);

//    sprintf(msgBuff, "%2d:                                    %lu", nodeID, displayTimer);
    str_len = sprintf(msgBuff, "%2d,%lu,%lu,%d\r\n", nodeID, displayTimer, joy_x_final, joy_button_final);
    
    // Send an 'M' type message containing the current millis()
    if (!mesh.write(msgBuff, 'M', str_len)) {

      // If a write fails, check connectivity to the mesh network
      if ( ! mesh.checkConnection() ) {
        //refresh the network address
        Serial.println("Renewing Address");
        mesh.renewAddress();
      } else {
        Serial.println("Send fail, Test OK");
      }
    } else {
      Serial.print("Send OK: "); Serial.println(displayTimer);
    }
  }

  while (network.available()) {
    RF24NetworkHeader header;
    payload_t payload;
    network.read(header, &payload, sizeof(payload));
    Serial.print("Received packet #");
    Serial.print(payload.counter);
    Serial.print(" at ");
    Serial.println(payload.ms);
  }
}






