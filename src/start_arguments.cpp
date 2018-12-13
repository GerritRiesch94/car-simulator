#include "start_arguments.h"
#include <unistd.h>
#include <iostream>

bool startargs::doip_flag = false;
std::string startargs::can_device;

/**
 * Parses the arguments when starting the car simulator.
 * Usage:   ./car-simulator -d vcan0    for doip and can simulation
 *          ./car-simulator             for doip and can simulation
 *          ./car-simualtor -d          for doip simulation
 *          ./car-simulator vcan0       for can simulation
 */
void startargs::parse_arguments(int argc, char** argv) {
    
    //when no arguments passed, doip and can simulation will be used
    if(argc <= 1) {
        startargs::doip_flag = true;
        startargs::can_device = startargs::DEFAULT_CAN_DEVICE;
        return;
    }
        
    int arg_index, arg = 0;
    
    //checks if the doip flag was passed in the arguments
    while((arg = getopt(argc, argv, "d")) != -1) {
        switch(arg) {
            case 'd':
                startargs::doip_flag = true;
                break;
        }
    }
    
    //checks if a can simulation device was passed in the arguments
    for(arg_index = optind; arg_index < argc; arg_index++) {
        startargs::can_device = argv[arg_index];
    }
    
    std::cout << "DoIPFlag: " << startargs::doip_flag << std::endl;
    std::cout << "Can-Device: " << startargs::can_device << std::endl;
    std::cout << "Can-Device Empty: " << startargs::can_device.empty() << std::endl;
}
