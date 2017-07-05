//
//  Aardcom.cpp
//  
//
//  Created by Grant Giesbrecht on 7/5/17.
//
//

#include "Aardcom.h"

//if (Serial.available() > 0){
//    
//    if (recieve_char(mode)){
//        performOperation(mode);
//    }
//    
//}

//float getParamValue(char param){ //!! User defined
//    switch(param){
//        case(0x01):
//            return saved_parameter;
//            break;
//    }
//}

//void setParamValue(char param, float x){ //!! User defined
//    switch(param){
//        case(0x01):
//            saved_parameter = x;
//            break;
//    }
//}

//float interpretPing(char param, float x){ //!! User defined
//    switch(param){
//        case(0x01):
//            return x * 7;
//            break;
//    }
//}

void performOperation(char mode){ //!! Limited User Modification
    
    char param;
    float x, y;
    
    switch(mode){
        case(FN_VERIFY):
            write_char(VAL_ACK);
            break;
        case(FN_INQUIRY):
            if (recieve_char(param)){
                x = getParamValue(param);
                write_float(x);
            }
            break;
        case(FN_SET):
            recieve_char(param);
            delayMicroseconds(1000);
            if (!recieve_float(x)){
                delayMicroseconds(1000);
                recieve_float(x);
            }
            setParamValue(param, x);
            break;
        case(FN_PING):
            recieve_char(param);
            delayMicroseconds(1000);
            if (!recieve_float(x)){
                delayMicroseconds(1000);
                recieve_float(x);
            }
            y = interpretPing(param, x);
            write_float(y);
            break;
        case(FN_PINMODE):
            recieve_float(x);
            recieve_char(param);
            pinMode((int)x, (int)param);
            break;
        case(FN_DIGW):
            recieve_float(x);
            recieve_char(param);
            digitalWrite((int)x, (int)param);
            break;
        case(FN_DIGR):
            recieve_float(x);
            write_char((char)digitalRead((int)x));
            break;
        case(FN_ANLGR):
            recieve_float(x);
            y = (float) analogRead((int)x);
            write_float( y );
            break;
        default:
            write_char(VAL_NAK);
            break;
    }
}

/* Reads (non-blocking) four bytes from USB
 
 PARAMETERS:
 x - float reference in which to write value
 
 Returns true if successful
 */
bool recieve_float(float& x){
    
    if (sizeof(float) != 4) return false;
    
    int idx = 0;
    char bytes_in[4];
    
    for (int i = 0 ; i < 100 ; i++){
        
        delayMicroseconds(1000);
        
        if (idx == 4) break; //Only read 4 bytes
        
        if (Serial.available() > 0){ //Read a byte if available
            bytes_in[idx] = Serial.read();
            idx++;
        }
        
    }
    
    if (idx == 4){
        memcpy(&x, bytes_in, 4);
    }else{
        return false;
    }
    
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    switch((int)x){
        case(0):
            digitalWrite(2, HIGH);
            break;
        case(1):
            digitalWrite(3, HIGH);
            break;
        case(2):
            digitalWrite(4, HIGH);
            break;
        case(3):
            digitalWrite(5, HIGH);
            break;
        case(4):
            digitalWrite(6, HIGH);
            break;
    }
    
    return true;
}

bool write_float(float& x){
    
    if (sizeof(float) != 4) return false;
    
    char send_bytes[4];
    
    memcpy(send_bytes, &x, 4);
    
    for (int i = 0 ; i < 4 ; i++){
        if (Serial.write(send_bytes[i]) != 1) return false;
    }
    
    return true;
}

bool recieve_char(char& x){
    
    int idx = 0;
    
    for (int i = 0 ; i < 100 ; i++){
        
        delayMicroseconds(1000);
        
        if (Serial.available() > 0){ //Read a byte if available
            x = Serial.read();
            idx++;
            break;
        }
        
    }
    
    if (idx != 1){
        return false;
    }
    
    return true;
}

bool write_char(char x){
    
    if (Serial.write(x) != 1){
        return false;
    }
    
}



































