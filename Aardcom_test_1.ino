#include <Aardcom.h>

//Transmission Mode Codes
#define FN_VERIFY 0x0A
#define FN_INQUIRY 0x05
#define FN_SET 0x02
#define FN_PING 0x03
#define FN_PINMODE 0x04
#define FN_DIGW 0x06
#define FN_DIGR 0x07
#define FN_ANLGR 0x08

//Transmission Value Codes
#define VAL_ACK 0x0A
#define VAL_NAK 0x15

//Pin Booleans
#define VAL_HIGH 1
#define VAL_LOW 0
#define VAL_OUTPUT 1
#define VAL_INPUT 0
#define VAL_INPUT_PULLUP 2
 
void setup(){
    Serial.begin(9600);
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
}

void loop(){
  
  char mode;
  if (Serial.available() > 0){

    if (recieve_char(mode)){
      performOperation(mode);
    }
    
  }
  
}

float saved_parameter = 7;

float getParamValue(char param){
  switch(param){
    case(0x01):
      return saved_parameter;
      break;
  }
}

void setParamValue(char param, float x){
  switch(param){
    case(0x01):
      saved_parameter = x;
      break;
  }
}

float interpretPing(char param, float x){
  switch(param){
    case(0x01):
      return x * 7;
      break;
  }
}
