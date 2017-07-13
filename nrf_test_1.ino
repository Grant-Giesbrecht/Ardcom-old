#include <SPI.h>
#include "RF24.h"

RF24 radio(7,8);                        // Set up nRF24L01 radio on SPI bus plus pins 7 & 8

/***************************************************************/

const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // Radio pipe addresses for the 2 nodes to communicate.

byte data[32];                           //Data buffer for testing data transfer speeds

unsigned long counter, rxTimer;          //Counter and timer for keeping track transfer info
unsigned long startTime, stopTime;  
bool TX=1,RX=0,role=1;

void setup(void) {

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  Serial.begin(115200);

  radio.begin();                           // Setup and configure rf radio
  radio.setChannel(1);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_1MBPS);
  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  radio.setRetries(2,15);                  // Optionally, increase the delay between retries & # of retries
  
  radio.setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  
  radio.startListening();                 // Start listening
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging

  if (role == TX){
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
    radio.stopListening();
    role = TX;            
  }else{
    Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  }
  
  
  randomSeed(analogRead(0));              //Seed for random number generation
  
  for(int i=0; i<32; i++){
     data[i] = random(255);               //Load the buffer with random data
  }
  radio.powerUp();                        //Power up the radio
}

int count = 0;

int index = 0;

void loop() {

  if (role == TX){
    data[0] = index++;
    if (index == 256) index = 0;
    if (radio.write(data, 1)){
      Serial.print("Wrote successfully ");
      Serial.print(String(data[0]));
      Serial.print(" \t");
      Serial.println(String(index));
      digitalWrite(2, HIGH);
      digitalWrite(4, LOW);
    }else{
      Serial.print("Write failed ");
      Serial.print(String(data[0]));
      Serial.print(" \t");
      Serial.println(String(index));
      digitalWrite(4, HIGH);
      digitalWrite(2, LOW);
//      data[0] == random(0, 255);
    }
  }

  if (role == RX){      

//    if (radio.available()){
//      radio.read(data, 32);
//      Serial.println("Read data from transciever");
//      Serial.print("\t");
//      Serial.println(String(data[0]));
//    }else{
//      if (++count >= 500){
//        Serial.println("All quiet...");
//        count = 0;
//      }
//    }

    while (radio.available()){
      radio.read(data, 1);
      Serial.println("Read data from transciever");
      Serial.print("\t");
      Serial.println(String(data[0]));
    }
  }

  if ( Serial.available() ){
    char c = toupper(Serial.read());
    if ( c == 'T' && role == RX )
    {
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      radio.openWritingPipe(pipes[1]);
      radio.openReadingPipe(1,pipes[0]);
      radio.stopListening();
      role = TX;                  // Become the primary transmitter (ping out)
    }
    else if ( c == 'R' && role == TX )
    {
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      radio.openWritingPipe(pipes[0]);
      radio.openReadingPipe(1,pipes[1]); 
      radio.startListening();
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
      role = RX;                // Become the primary receiver (pong back)
    }
  }

}
