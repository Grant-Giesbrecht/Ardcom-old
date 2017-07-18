#include "AardRF24.h"
#include "Aardcom.h"

#define REP_BATTERY 0
#define ACT_SERVO_Q 0
#define ACT_SERVO_W 1
#define ACT_SERVO_E 2

RF24 rf24_module(7, 8);
AardRF24 radio(MODE_MASTER, &rf24_module);

byte data[32];
byte return_buffer[32];

void setup() {
  
  Serial.begin(115200);
  radio.begin();
  Serial.println("Transmitter ready");

}

char byte_in;
short ec;
char mode;

void loop() {
  if (Serial.available() > 0){    
    if (recieve_char(mode)){
      performOperation(mode);
    }
  }
}

float getParamValue(char param){ //!! User defined
    switch(param){
        case(REP_BATTERY):
          radio.send_packet(CMD_REPORT, REP_BATTERY, 0x0, data, return_buffer, &ec);
          float ret;
//          ret = (float)(((byte)return_buffer[0])*5.0/255.0);
//          ret = 7.3;
          memcpy(&ret, return_buffer, 1);  
          return ret;
          break;
    }
}

void setParamValue(char param, float x){ //!! User defined
    switch(param){
        case(ACT_SERVO_Q):
          radio.send_packet(CMD_ACTION, ACT_SERVO_Q, 0x0, data, return_buffer, &ec);
          break;
        case(ACT_SERVO_W):
          radio.send_packet(CMD_ACTION, ACT_SERVO_W, 0x0, data, return_buffer, &ec);
          break;
        case(ACT_SERVO_E):
          radio.send_packet(CMD_ACTION, ACT_SERVO_E, 0x0, data, return_buffer, &ec);
          break;
    }
}

float interpretPing(char param, float x){ //!! User defined
}

//void printing_write(char a, char b, char c, char data[29], char output[32], short* ec){
//  if (!radio.send_packet(a, b, c, data, return_buffer, ec)){
//    if (ec == WRITE_FAIL){
//      Serial.println("ERROR: Failed to write to swimmer");
//    }else if(ec == SALVE_FAIL){
//      Serial.println("ERROR: No response from swimmer (SALVE_FAIL)");
//    }else{
//      Serial.println("ERROR: Failed to write - unknown error");
//    }
//  }else{
//    Serial.println("Command sent to swimmer"); 
//  }
//}
//





