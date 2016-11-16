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
#define nodeID 1


uint32_t displayTimer = 0;
//int acc_x;
//int acc_y;
int data[2];
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
    data[0]=xRead;
    data[1]=yRead;
     // Envia un mensaje con un 'CARACTER' como header y el valor X del acelerometro
    if (!mesh.write(&data, '1', sizeof(data))) {

      // If a write fails, check connectivity to the mesh network
      if ( ! mesh.checkConnection() ) {
        //refresh the network address
        Serial.println("Renewing Address");
        mesh.renewAddress();
      }
    }   
  }
}









