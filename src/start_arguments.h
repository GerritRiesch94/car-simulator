#ifndef START_ARGUMENTS_H
#define START_ARGUMENTS_H

#include <string>

namespace startargs {
    const std::string DEFAULT_CAN_DEVICE = "vcan0";
    
    void parse_arguments(int argc, char** argv);
    bool doip_flag = false;
    std::string can_device;
}

#endif /* START_ARGUMENTS_H */

