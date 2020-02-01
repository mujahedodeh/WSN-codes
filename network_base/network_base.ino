/*
          == Base/ Master Node 00==

*/

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#define button 2
#define led 3

RF24 radio(10, 9);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;      // Address of the other node in Octal format
const uint16_t node021 = 021;
const uint16_t node02 = 02;
const uint16_t node022 = 022;
const uint16_t node03 = 03;
const uint16_t node023 = 023;

void setup() {
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  pinMode(button, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  network.update();
  //===== Receiving =====//
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    unsigned long incomingData;
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
    String str = String(header.from_node,OCT);  
    Serial.println(str+"#"+incomingData);  // 
  
    
  }

}
