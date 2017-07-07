//Include libraries
#include <Aardcom.h> //Aardcom
#include <nRF24L01.h> //nRF24L01
#include <printf.h> //nRF24L01
#include <RF24.h> //nRF24L01
#include <RF24_config.h> //nRF24L01

//Set program for MASTER (1) or SLAVE (0)
#define MASTER 1

//Define command parameters
#define UPLOAD_GATE 0x01
#define RESET_GATE 0x02
#define MANUAL_CONTROL 0x03
#define BATTERY_STATUS 0x04
#define GET_POSITION 0x05
#define RUN_STATUS 0x06
#define START_RUN 0x07
#define ALL_STOP 0x08

RF24 radio(7, 8);
byte addresses[][6] = {"MOSIN", "MISON"}; //MOSI or MISO Node distinction

#define BUFFER_SIZE 100
byte data[BUFFER_SIZE];

void setup() {
  
  //Open communication ports
  if (MASTER){
    Serial.begin(9600); //Open channel to Ardcom
  }
  radio.begin(); //Initialize nRF24L01

  radio.setPALevel(RF24_PA_LOW);

//  radio.setPALevel(RF24_PA_LOW);

  //Initialize debug LEDs
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  
  //Initialize nRF24L01 Pipes
  if (MASTER){
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);
  }else{
    radio.openReadingPipe(1, addresses[0]);
    radio.openWritingPipe(addresses[1]);
  }

  //Select and begin main loop
  if (MASTER){
    char mode;
    while (true){ //Main Loop - MASTER
      if (Serial.available() > 0){ //Continuously scan for commands from the connected GPC
        if (recieve_char(mode)){
          performOperation(mode); //When a command is recieved, perform the requested operation
        }
      }
    }
  }else{
    char mode;
    while (true){ //Main Loop - SLAVE
      
      radio.startListening();
      if (radio.available()){
        mode = nRF_recieve_char();
        digitalWrite(2, HIGH);
        performSlaveOperation(mode);
      }
      
    }
  }
  
}

// -----------------------------------------------------------------
// ------------------- nRF24L01 HANDLER FUNCTIONS

void performSlaveOperation(char mode){

  char param;
  float x, y;

  digitalWrite(3, HIGH);

  switch(mode){
//    case(UPLOAD_GATE):
//      break;
//    case(RESET_GATE):
//      break;
//    case(MANUAL_CONTROL):
//      break;
    case(BATTERY_STATUS):
      x = analogRead(0);
      x *= 0.0048828125; // 5/1024
      digitalWrite(4, HIGH);
      if (nRF_send_float(x)) x++; //digitalWrite(5, HIGH);
      break;
//    case(GET_POSITION):
//      break;
//    case(RUN_STATUS):
//      break;
//    case(START_RUN):
//      break;
//    case(ALL_STOP):
//      break;
    default:
      digitalWrite(5, HIGH);
      x = 3.333;
      nRF_send_float(x);
      break;
  }
  
}

// -----------------------------------------------------------------
// ------------------- nRF24L01 HELPER FUNCTIONS

/* Blocking read char from nRF24L01


 Returns the next read byte as a char
 */
char nRF_recieve_char(){

  char c;

  radio.startListening();
  while (!radio.available()){
    delayMicroseconds(1000);
  }
  radio.read(&c, 1);
  radio.stopListening();

  return c;
}

/*
 Non-blocking send char
 
 */
bool nRF_send_char(char c){

  radio.stopListening();
  return radio.write(&c, 1);
  
}

float nRF_recieve_float(){

  float x;

  radio.startListening();
  while (!radio.available()){
    delayMicroseconds(1000);
  }
  digitalWrite(3, HIGH);
  radio.read(&x, sizeof(x));
  radio.stopListening();

  return x;
}

bool nRF_send_float(float x){

  radio.stopListening();
  return radio.write(&x, sizeof(x));
  
}

// -----------------------------------------------------------------
// ------------------- AARDCOM HANDLER FUNCTIONS

float getParamValue(char param){

  float x;
  
  switch(param){
    case(BATTERY_STATUS):
      if (!nRF_send_char(BATTERY_STATUS)){
        return -1;
      }
      digitalWrite(2, HIGH);
      x = nRF_recieve_float();
      digitalWrite(4, HIGH);
      return x;
      break;
//    case(GET_POSITION):
//      return
//      break;
//    case(GET_STATUS):
//      return
//      break;
  }
}

void setParamValue(char param, float x){
  switch(param){
//    case(UPLOAD_GATE):
//      break;
//    case(RESET_GATE):
//      break;
//    case(MANUAL_CONTROL):
//      break;
//    case(START_RUN):
//      break;
//    case(ALL_STOP):
//      break;
  }
}

float interpretPing(char param, float x){
  switch(param){
  }
}

// =============================================================
// -------------------------------------------------------------

// 'loop' is never to execute
void loop() {
}
