/**
 * @file main.cpp
 *
 * This file contains the `main()`-function with a simple Lua script test. See
 * https://github.com/jeremyong/Selene for usage instructions.
 */

#include "ecu_lua_script.h"
#include "electronic_control_unit.h"
#include "ecu_timer.h"
#include "utilities.h"
#include "doip_simulator.h"
#include "doip_lua_script.h"
#include "start_arguments.h"
#include <string>
#include <unistd.h>

using namespace std;

DoIPSimulator doip;

void start_server(const string &config_file, const string &device)
{
    cout << "start_server for config file: " << config_file
         << " on device: " << device << endl;

    auto script = make_unique<EcuLuaScript>("Main", LUA_CONFIG_PATH + config_file);
    ElectronicControlUnit ecu(device, move(script));
    
    doip.addECU(&ecu);
}

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
}

/**
 * The main application only for testing purposes.
 *
 * @param argc: the number of arguments
 * @param argv: the argument list
 * @return 0 on success, otherwise a negative value
 */
int main(int argc, char** argv)
{   
    startargs::parse_arguments(argc, argv);

    vector<string> config_files = utils::getConfigFilenames(LUA_CONFIG_PATH);
    vector<thread> threads;
    
    for (const string &config_file : config_files)
    {
        if(config_file.find("doip") != string::npos || config_file.find("carsimconfig") != string::npos) {   
            doip.doipConfig = new DoipLuaScript(LUA_CONFIG_PATH + config_file);
            continue;
        }
            
        thread t(start_server, config_file, startargs::can_device);
        threads.push_back(move(t));
        usleep(50000);
    }
    
    //if there is no config file for the doip server, set it to the default configuration
    if(doip.doipConfig == NULL)   
    {
        doip.doipConfig = new DoipLuaScript();
    }
    
    thread t(&DoIPSimulator::start, &doip);  
    threads.push_back(move(t)); 
    
    for (unsigned int i = 0; i < threads.size(); ++i)
    {
        threads[i].join();
    }

    return 0;
}

