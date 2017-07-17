#include <Servo.h>
#include "AardRF24.h"

//Pin Configuration
#define FORE_SERVO_PIN 9
#define AFT_SERVO_PIN 10
#define BATTERY_TELEMETERY_PIN 0

#define REP_BATTERY 0
#define ACT_SERVO_Q 0
#define ACT_SERVO_W 1
#define ACT_SERVO_E 2

RF24 rf24_module(7,8);              
AardRF24 radio(MODE_SLAVE, &rf24_module);

byte data[32];
byte return_buffer[32];

Servo foreServo;
Servo aftServo;

void setup(void) {

  Serial.begin(115200);

  foreServo.attach(FORE_SERVO_PIN);
  aftServo.attach(AFT_SERVO_PIN);

  radio.begin();
  radio.set(KC_SLAVE_CALLBACK, three_link_swimmer_handler);

  Serial.println("RECIEVING");
}

short ec;

void loop() {

  if (radio.read_available(data, return_buffer, &ec)){
    Serial.print("Success - recieved ");
    Serial.print(String(data[3]));
    Serial.print(" Sent: ");
    Serial.println(String(return_buffer[3]));
  }else{
    if (ec == N_AVAIL){
//      Serial.println("No data available");
    }else{
      Serial.println("ERROR");
    }
  }
  
}

void three_link_swimmer_handler(char data[32], char output[32]){

    //Get variables from data array
    char command = data[0];
    char parameter_1 = data[1];
    char parameter_2 = data[2];

    if (command == CMD_ACTION){
      Serial.println("\tPerform Action");
      switch(parameter_1){
        case(ACT_SERVO_Q):
          Serial.println("\t\tServo 0");
          foreServo.write(10);
          aftServo.write(10);
          break;
        case(ACT_SERVO_W):
          Serial.println("\t\tServo 90");
          foreServo.write(90);
          aftServo.write(90);
          break;
        case(ACT_SERVO_E):
          Serial.println("\t\tServo 180");
          foreServo.write(170);
          aftServo.write(170);
          break;
        default:
          Serial.print("\t\t");
          Serial.println(String((int)parameter_1));
          break;
      }
    }else{
      Serial.println("\tPerform Report");
      switch(parameter_1){
        case(REP_BATTERY):
          Serial.println("\t\tBattery level");
          int v;
          v = analogRead(BATTERY_TELEMETERY_PIN);
          Serial.print("\t\t1023: ");
          Serial.print(String(v));
          Serial.print(" 255: ");
          output[0] = (char)map(v, 0, 1023, 0, 255);
          Serial.println(String((short)output[0]));
          break;
        default:
          Serial.print("\t\t");
          Serial.println(String((int)parameter_1));
          break;
      }
    }
}































