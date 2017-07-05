//
//  ArdcomMain.cpp
//  
//
//  Created by Grant Giesbrecht on 6/29/17.
//
//

#include <stdio.h>
#include "Ardcom.hpp"
#include <iostream>
#include <cstdlib>
#include <unistd.h>

using namespace std;

int main(){
    
    Ardcom mcu(USB_RIGHT_ALT);
    if (!mcu.is_open()){
        cout << "ERROR: Failed to connect to MCU on port " << USB_RIGHT << endl;
        return 1;
    }
    
    mcu.wait_for_MCU(2); //12 for Arduino 101
    
    if (!mcu.verify_connection()){
        cout << "Connection Verification Failed" << endl;
    }else{
        cout << "MCU Connection verified" << endl;
    }
    
    float x;
    
    mcu.read_MCU_parameter(0x01, x);
    cout << "Read value: " << x << endl;
    
    mcu.set_MCU_parameter(0x01, 14.2);
    
    mcu.read_MCU_parameter(0x01, x);
    cout << "Read value: " << x << endl;
    
    float y;
    
    while (true){
        
        x = (rand() % 10000)/100; //generate random number
        
        //Ping float to MCU and print values
        cout << "Float sent to MCU: " << to_string(x) << "\t * 7 = " << to_string(x*7) << endl;
        mcu.ping_float(0x01, x, y);
        cout << "\tFloat recieved from MCU: " << to_string(y) << endl;
        
        usleep(100000); //delay
        
    }
    
    return 0;
}
