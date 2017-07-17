//#include <SPI.h>
//#include "RF24.h"
#include "AardRF24.h"

RF24 rf24_module(7,8);                        // Set up nRF24L01 radio on SPI bus plus pins 7 & 8
AardRF24 radio(MODE_SLAVE, &rf24_module);

byte data[32];
byte return_buffer[32];

void setup(void) {

  Serial.begin(115200);

  radio.begin();
  radio.set(KC_SLAVE_CALLBACK, basic_callback);

  Serial.println(F("RECIEVING"));
}

//int count = 0;

short ec;

int index = 0;

void loop() {

  if (radio.read_available(data, return_buffer, &ec)){
    Serial.print("Success - recieved ");
    Serial.print(String(data[3]));
    Serial.print(" Sent: ");
    Serial.println(String(return_buffer[3]));
  }else{
    if (ec == N_AVAIL){
      Serial.println("No data available");
    }else{
      Serial.println("ERROR");
    }
  }
  
}
