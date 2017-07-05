//
//  Aardcom.hpp
//  
//
//  Created by Grant Giesbrecht on 7/5/17.
//
//

#ifndef Aardcom_hpp
#define Aardcom_hpp

#include <stdio.h>
#include "Arduino.h"

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

float getParamValue(char param);

void setParamValue(char param, float x);

float interpretPing(char param, float x);

void performOperation(char mode);

bool recieve_float(float& x);

bool write_float(float& x);

bool recieve_char(char& x);

bool write_char(char x);

#endif /* Aardcom_hpp */






















