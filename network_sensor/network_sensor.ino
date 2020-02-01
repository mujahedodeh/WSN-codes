/*
        == Node 01 (Child of Master node 00) ==
*/


#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#define echoPin 8 //Echo
#define trigPin 7 //Trigger
#define led 2

# define SAMPLES 50 // number of samples to take mean of
# define TOLERANCE 0.1 // % range of allowable deviation         
                       //between current mean and new values, expressed as decimal

RF24 radio(10, 9);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 03;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t master00 = 00;    // Address of the other node in Octal format
unsigned long array_of_readings[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
byte count=0; //counter

const unsigned long interval = 900000;  //ms  // How often to send data to the other unit
unsigned long last_sent;            // When did we last send?
unsigned long dataToSend = 0;

void setup() {
  SPI.begin();
  radio.begin();
  network.begin(90, this_node); //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  network.update();
  
  //===== Reading =====//
  delay(60000); //delay 1 min
    unsigned long temp= watch();
    Serial.print(count);
    Serial.print("- reading=");
    Serial.println(temp);
    if(temp <= 500 && temp >0) {
      array_of_readings[count]=temp;
      count++;
    }
  
    if(count==15){
        int sum=0;
        byte j;
        for(j=0;j<15;j++){
           Serial.print(array_of_readings[j]);
           Serial.print(",");
         }
        for(j=0;j<15;j++){
           sum+=array_of_readings[j];
         }
         sum=(array_of_readings[0]+array_of_readings[1]+array_of_readings[2]+array_of_readings[3]+array_of_readings[4]+array_of_readings[5]+array_of_readings[6]+array_of_readings[7]+array_of_readings[8]+array_of_readings[9]+array_of_readings[10]+array_of_readings[11]+array_of_readings[12]+array_of_readings[13]+array_of_readings[14]);
         Serial.println(sum);
         dataToSend = sum/15;
         count=0;
      }
      
  //===== Sending =====//
  unsigned long now = millis();
  if (now - last_sent >= interval) {   // If it's time to send a data, send it!
    last_sent = now;
    //unsigned long potValue = analogRead(A0);
    //unsigned long ledBrightness = map(potValue, 0, 1023, 0, 255);
    Serial.print("sent data=");
    Serial.println(dataToSend);
    RF24NetworkHeader header(master00);   // (Address where the data is going)
    bool ok = network.write(header, &dataToSend, sizeof(dataToSend)); // Send the data
 
    
  }
}
int watch(){
  long echo_distance;
  digitalWrite(trigPin,LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  echo_distance=pulseIn(echoPin,HIGH);
  echo_distance=echo_distance*0.01657; //how far away is the object in cm
  //Serial.println((int)echo_distance);
  return echo_distance;
}
