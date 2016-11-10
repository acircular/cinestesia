///variables accel
const int xPin = 4;
const int yPin = 5;
///Analog read pin flexo
const int flexoPin = 7;

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

//cada nodo debe llevar un id único
#define nodeID 4


uint32_t displayTimer = 0;
int acc_x;
int acc_y;
struct payload_t {
  unsigned long ms;
  unsigned long counter;
};

void setup() {
//definir baud del puerto serial (arduino nano max 38400)
  Serial.begin(38400);
  
  // Set the nodeID manually
 mesh.setNodeID(nodeID);
  // Connect to the mesh
 mesh.begin();
}



void loop() {

  mesh.update();

  //read the analog values from the accelerometer
  int xRead = analogRead(xPin);
  int yRead = analogRead(yPin);
  int flexoRead = analogRead(flexoPin);

  // Send to the master node every second
if (millis() - displayTimer >= 10) {
    displayTimer = millis();
    acc_x=xRead;
    acc_y=yRead;
     // Envia un mensaje con un 'CARACTER' como header y el valor X del acelerometro
    if (!mesh.write(&acc_x, 'H', sizeof(acc_x))) {

      // If a write fails, check connectivity to the mesh network
      if ( ! mesh.checkConnection() ) {
        //refresh the network address
        Serial.println("Renewing Address");
        mesh.renewAddress();
      }
    }
     // Envia un mensaje con un 'CARACTER' como header y el valor Y del acelerometro
     if (!mesh.write(&acc_y, 'I', sizeof(acc_y))) {

      // If a write fails, check connectivity to the mesh network
      if ( ! mesh.checkConnection() ) {
        //refresh the network address
        Serial.println("Renewing Address");
        mesh.renewAddress();
      }
    } 

 // Envia un mensaje con un 'CARACTER' como header y el valor del flexo
    if (!mesh.write(&flexoRead, 'J', sizeof(flexoRead))) {

      // If a write fails, check connectivity to the mesh network
      if ( ! mesh.checkConnection() ) {
        //refresh the network address
        Serial.println("Renewing Address");
        mesh.renewAddress();
      }
    }
  }
/*
  while (network.available()) {
    RF24NetworkHeader header;
    payload_t payload;
    network.read(header, &payload, sizeof(payload));
    Serial.print("Received packet #");
    Serial.print(payload.counter);
    Serial.print(" at ");
    Serial.println(payload.ms);
  }*/
}









