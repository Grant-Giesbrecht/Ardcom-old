//#include <RF24.h>
//#include <printf.h>
//#include <nRF24L01.h>
//#include <RF24_config.h>

#include "AardRF24.h"

RF24 rf24_module(7, 8);
AardRF24 radio(MODE_MASTER, &rf24_module);

byte data[32];
byte return_buffer[32];

void setup() {

  Serial.begin(115200);

  radio.begin();

  Serial.println(F("TRANSMITTING"));

  //Load the buffer with random data
  randomSeed(analogRead(0));
  for (int i = 0; i < 32; i++) {
    data[i] = random(255);
  }

  data[0] = COM_REPORT;
  data[1] = '7';

}

short ec;

int index = 0;

void loop() {

  data[3] = index++;
  if (index == 248) index = 0;
  if (!radio.send_packet(data, return_buffer, &ec)) {
    if (ec == WRITE_FAIL) {
      Serial.println("ERROR: Failed to write to slave");
    }else{
      Serial.print("Packet successfully sent - ");
      Serial.println(String(data[3]));
      Serial.println("\tERROR: Failed to recieve SALVE");
    }
  } else {
    Serial.print("Packet successfully sent - ");
    Serial.println(String(data[3]));
    Serial.print("\tRecieved SALVE packet - ");
    Serial.println(String(return_buffer[3]));
  }

}
