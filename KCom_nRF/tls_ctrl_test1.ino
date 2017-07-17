#include "AardRF24.h"

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

void loop() {
  
  if (Serial.available()){
    
    byte_in = Serial.read();
    while (Serial.available()) Serial.read();

    switch(byte_in){
      case('B'): //Battery level
        printing_write(CMD_REPORT, REP_BATTERY, 0x0, data, return_buffer, &ec);
        Serial.print("\tBATTERY VOLTAGE (V): ");
        float v;
        v = return_buffer[0]*5./255;
        Serial.println(String(v));
        break;
      case('Q'): //Servo to beginning
        printing_write(CMD_ACTION, ACT_SERVO_Q, 0x0, data, return_buffer, &ec);
        Serial.println("\tServo moving to 0");
        break;
      case('W'): //Servo to middle
        printing_write(CMD_ACTION, ACT_SERVO_W, 0x0, data, return_buffer, &ec);
        Serial.println("\tServo moving to 90");
        break;
      case('E'): //Servo to end
        printing_write(CMD_ACTION, ACT_SERVO_E, 0x0, data, return_buffer, &ec);
        Serial.println("\tServo moving to 180");
        break;
      default:
        Serial.println("ERROR: Unrecognized serial command");
        Serial.println("\tB: Report battery level, Q: Move servo 0, W: Move servo 90, E: Move servo 180");
        break;
    }
  }

}

void printing_write(char a, char b, char c, char data[29], char output[32], short* ec){
  if (!radio.send_packet(a, b, c, data, return_buffer, ec)){
    if (ec == WRITE_FAIL){
      Serial.println("ERROR: Failed to write to swimmer");
    }else if(ec == SALVE_FAIL){
      Serial.println("ERROR: No response from swimmer (SALVE_FAIL)");
    }else{
      Serial.println("ERROR: Failed to write - unknown error");
    }
  }else{
    Serial.println("Command sent to swimmer"); 
  }
}






