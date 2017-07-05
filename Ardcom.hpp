//
//  Ardcom.hpp
//  
//
//  Created by Grant Giesbrecht on 6/29/17.
//
//

#ifndef Ardcom_hpp
#define Ardcom_hpp

#include <stdio.h>
#include <string>

//MacBook Pro USB Port Macros
#define USB_LEFT "/dev/cu.usbmodem1411"
#define USB_RIGHT "/dev/cu.usbmodem1421"
#define USB_LEFT_ALT "/dev/cu.wchusbserial1410"
#define USB_RIGHT_ALT "/dev/cu.wchusbserial1420"

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

class Ardcom{
public:
    
    //Opening commands
    Ardcom(std::string port);
    bool is_open();

    //Auxilliary opening commands
    void wait_for_MCU(double seconds);
    bool verify_connection();
    
    //Setup commands
    void enablePinControl(bool b);
    bool getPinControlEnabled();
    
    //Parametric Commands
    void read_MCU_parameter(char parameter, float& value);
    void set_MCU_parameter(char parameter, float value);
    void ping_float(char parameter, float send, float& recieve);
    
    //Pin Commands
    void pinMode(int pin, int mode);
    void digitalWrite(int pin, int value);
    int digitalRead(int pin);
    int analogRead(int pin);
    
    bool send_float(float x); //Write a float (as 4 chars) (non-blocking)
    bool recieve_float(float& x); //Read a float (as 4 chars) (blocking)
    
    void send_char(char x); //Write a char (non-blocking)
    void recieve_char(char& x); //Read a char (blocking)
    
private:
    
    FILE* file; //Connects to MCU
    bool is_open_bool;
    
};

#endif /* Ardcom_hpp */
