// #include "Arduino.h"

#ifndef _AARDRF24_H_
#define _AARDRF24_H_

// #include <SPI.h>
#include "RF24.h"
// #include <printf.h>
// #include <nRF24L01.h>
// #include <RF24_config.h>

#define PACKET_SIZE 32
#define MODE_MASTER 'M'
#define MODE_SLAVE 'S'

//Error codes
#define WRITE_FAIL 0 /*Write to slave failed*/
#define SALVE_FAIL 1 /*Master fails to read salve (response from slave)*/
#define N_AVAIL 2 /*No packets avialable to be read*/
#define SALVE_SEND_FAIL 3 /*Slave fails to send salve to master*/

//Settings
#define KC_SLAVE_CALLBACK 0 //Set the function called when a packet arrives from the master node
#define KC_RESPONSE_TIME 1 //Set the maximum time for which the master will wait for a reply from the slave (in ms) (default = 2000)

//#define commands for slave
#define COM_EXECUTE 0
#define COM_REPORT 1

class AardRF24{

public:

    //Initialization functions
    AardRF24(char mode, RF24* rf24_module);
    void begin();

    //General functions
    // void set(int property, bool value);
    void set(int property, void (*fn)(char*, char*));
    void set(int property, int value);

    //Master functions
    bool send_packet(char command, char parameter_1, char parameter_2, char data[29], char output[32], short* error_code);
    bool send_packet(char data[32], char output[32], short* error_code);
    // bool read_available();

    //Slave functions
    bool read_available(char data[32], char output[32], short* error_code);
    // bool read_available(short* error_code);

private:

    //Radio & radio configuration variables
    RF24* radio;
    char mode;

    void (*slave_callback)(char data[32], char output[32]);

    //Variables
    unsigned int trial_time; // time in ms to wait for Salve

};

void basic_callback(char data[32], char output[32]);

#endif