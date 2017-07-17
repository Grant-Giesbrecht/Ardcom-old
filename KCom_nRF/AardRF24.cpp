#include "AardRF24.h"

AardRF24::AardRF24(char mode, RF24* radio_module){

    AardRF24::mode = mode;
    radio = radio_module;
    
}

void AardRF24::begin(){
    
    //Initialize variables
    trial_time = 2000;
    slave_callback = NULL;

    const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };

    //Initialize radio
    if (mode == MODE_MASTER){
        radio->begin();
        radio->setChannel(1);
        radio->setPALevel(RF24_PA_MAX);
        radio->setDataRate(RF24_1MBPS);
        radio->setAutoAck(1);                     // Ensure autoACK is enabled
        radio->setRetries(2,15);                  // Optionally, increase the delay between retries & # of retries
        radio->setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance
        radio->openWritingPipe(pipes[1]);
        radio->openReadingPipe(1,pipes[0]);
        radio->stopListening();
    }else{
        radio->begin();                           // Setup and configure rf radio
        radio->setChannel(1);
        radio->setPALevel(RF24_PA_MAX);
        radio->setDataRate(RF24_1MBPS);
        radio->setAutoAck(1);                     // Ensure autoACK is enabled
        radio->setRetries(2,15);                  // Optionally, increase the delay between retries & # of retries
        radio->setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance
        radio->openWritingPipe(pipes[0]);
        radio->openReadingPipe(1,pipes[1]);
        radio->startListening();                 // Start listening
        Serial.println("XX");
    }    

    radio->powerUp();
}

void AardRF24::set(int property, void (*fn)(char*, char*)){
    switch(property){
        case(KC_SLAVE_CALLBACK):
            AardRF24::slave_callback = fn;
            break;
    }
}

void AardRF24::set(int property, int value){
    switch(property){
        case(KC_RESPONSE_TIME):
            trial_time = value;
            break;
    }
}

bool AardRF24::send_packet(char command, char parameter_1, char parameter_2, char data[29], char output[32], short* error_code){
    
    //Create packet to send
    char packet[32];
    packet[0] = command;
    packet[1] = parameter_1;
    packet[2] = parameter_2;
    for (int i = 0 ; i < 29 ; i++){ //Copy data from data buffer into packet
        packet[i+3] = data[i];
    }

    return AardRF24::send_packet(packet, output, error_code);
}

bool AardRF24::send_packet(char packet[32], char output[32], short* error_code){
    
    unsigned long previous;

    if (radio->write(packet, PACKET_SIZE)){ //If write...

        radio->startListening(); // (Prepare radio for reading)

        //For 'X' amount of time...
        bool read_success = false;
        previous = millis();
        while (millis() - previous < trial_time){
            
            if (radio->available()){ //If available...
                
                radio->read(output, PACKET_SIZE); //Read

                //Break
                read_success = true;
                break;
            
            }
            
        }

        radio->stopListening(); // (Prepare radio for sending)

        if (!read_success){
            *error_code = SALVE_FAIL;
            return false;
        }

        

    }else{ //Else (write fails)...

        //Print error
        *error_code = WRITE_FAIL;
        return false;

    }

    return true;
}

bool AardRF24::read_available(char data[32], char output[32], short* error_code){

    if (radio->available()){ //If available...

        radio->read(data, PACKET_SIZE); //Read

        radio->stopListening(); // (Prepare radio for sending)

        if (slave_callback != NULL){
            slave_callback(data, output);
        }

        if (!radio->write(output, PACKET_SIZE)){ //If write...
            *error_code = SALVE_SEND_FAIL;
            radio->startListening();
            return false;
        }

        radio->startListening(); // (Prepare radio for listening)

    }else{
        *error_code = N_AVAIL; //Report that no data was available
        return false;
    }

    return true;
    
}

void basic_callback(char data[32], char output[32]){

    //Get variables from data array
    char command = data[0];
    char parameter_1 = data[1];
    char parameter_2 = data[2];

    if (command == CMD_ACTION){
        // switch(parameter_1){
        //     case('7'):
        //         break;
        //     case('8'):
        //         break;
        //     case('9'):
        //         break;
        // }
    }else if (command == CMD_REPORT){
        switch(parameter_1){
            case('7'):

                for (int i = 0 ; i < 32 ; i++){
                    output[i] = data[i];
                }
                output[3] = output[3] + 7;

                break;
            case('8'):
                break;
            case('9'):
                break;
        }
    }
}








