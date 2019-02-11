#include "start_arguments.h"

//Initialize extern variables
bool startargs::doip_flag = false;
bool startargs::can_flag = false;
std::string startargs::can_device;

/**
 * Parses the arguments when starting the car simulator.
 * Usage:   ./car-simulator -d vcan0    for doip and can simulation
 *          ./car-simulator             for doip and can simulation
 *          ./car-simulator -d          for doip simulation
 *          ./car-simulator vcan0       for can simulation
 */
void startargs::parse_arguments(int argc, char** argv) {
    
    //when no arguments passed, doip and can simulation will be used
    if(argc <= 1) {
        startargs::doip_flag = true;
        startargs::can_flag = true;
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
    
    //sets the can flag if device name was given
    if(startargs::can_device.empty())
        startargs::can_flag = false;
    else 
        startargs::can_flag = true;
}
