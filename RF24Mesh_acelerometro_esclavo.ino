

///variables accel
//Analog read pins
const int xPin = 0;


//The minimum and maximum values that came from
//the accelerometer while standing still
//You very well may need to change these
//int minVal = 265;
//int maxVal = 402;


//to hold the caculated values
double x;


/** RF24Mesh_Example.ino by TMRh20
 *
 * This example sketch shows how to manually configure a node via RF24Mesh, and send data to the
 * master node.
 * The nodes will refresh their network address as soon as a single write fails. This allows the
 * nodes to change position in relation to each other and the master node.
 */


#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <SPI.h>
#include <EEPROM.h>


/**** Configure the nrf24l01 CE and CS pins ****/
RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

/**
 * User Configuration: nodeID - A unique identifier for each radio. Allows addressing
 * to change dynamically with physical changes to the mesh.
 *
 * In this example, configuration takes place below, prior to uploading the sketch to the device
 * A unique value from 1-255 must be configured for each node.
 * This will be stored in EEPROM on AVR devices, so remains persistent between further uploads, loss of power, etc.
 *
 **/
#define nodeID 2


uint32_t displayTimer = 0;
int acc_x;
struct payload_t {
  unsigned long ms;
  unsigned long counter;
};

void setup() {

  Serial.begin(38400);
  //printf_begin();
  // Set the nodeID manually
  mesh.setNodeID(nodeID);
  // Connect to the mesh
  mesh.begin();
}



void loop() {

  mesh.update();

  //read the analog values from the accelerometer
  int xRead = analogRead(xPin);
 
  Serial.println(xRead);
  //convert read values to degrees -90 to 90 - Needed for atan2
  //int xAng = map(xRead, minVal, maxVal, -90, 90);

  


  // Send to the master node every second
  if (millis() - displayTimer >= 30) {
    displayTimer = millis();
    acc_x=xRead;
    // Send an 'E' type message containing the current millis()
    if (!mesh.write(&acc_x, 'B', sizeof(acc_x))) {

      // If a write fails, check connectivity to the mesh network
      if ( ! mesh.checkConnection() ) {
        //refresh the network address
        Serial.println("Renewing Address");
        mesh.renewAddress();
      } else {
        Serial.println("Send fail, Test OK");
      }
    } else {
      Serial.print("Send OK: "); Serial.println(acc_x);
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






