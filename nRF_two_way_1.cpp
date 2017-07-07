//
//  nRF_two_way_1.cpp
//  
//
//  Created by Grant Giesbrecht on 7/5/17.
//
//

#include <stdio.h>
#include <string>
#include <iostream>
#include "Ardcom.hpp"
#include "stdutil.hpp"
#include "string_manip.hpp"

//Define application specific command parameters
#define UPLOAD_GATE 0x01
#define RESET_GATE 0x02
#define MANUAL_CONTROL 0x03
#define BATTERY_STATUS 0x04
#define GET_POSITION 0x05
#define RUN_STATUS 0x06
#define START_RUN 0x07
#define ALL_STOP 0x08

using namespace std;

int main(){
    
    Ardcom mcu(USB_RIGHT_ALT);
    if (!mcu.is_open()){
        cout << "ERROR: Failed to connect to MCU on port " << USB_RIGHT << endl;
        return 1;
    }
    
    mcu.wait_for_MCU(2);
    
    if (!mcu.verify_connection()){
        cout << "Connection Verification Failed" << endl;
    }else{
        cout << "MCU Connection verified" << endl;
    }
    
    string input, Input;
    float x;
    while (true){
        
        cout << ": " << std::flush;
        getline(cin, input);
        Input = to_uppercase(input);
        
        if (Input == "EXIT"){
            cout << "Exiting..." << endl;
            break;
        }else if (Input == "BATTERY_STATUS" || Input == "BATT"){
            cout << "\tContacting MCUs" << endl;
            mcu.read_MCU_parameter(BATTERY_STATUS, x);
            cout << "Battery voltage: " << to_string(x) << endl;
        }else{
            cout << '\t' << "ERROR: Unrecognized input" << endl;
        }
        
        
    }
    
    return 0;
}
