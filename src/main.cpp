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
#include <string>

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
 * The main application only for testing purposes.
 *
 * @param argc: the number of arguments
 * @param argv: the argument list
 * @return 0 on success, otherwise a negative value
 */
int main(int argc, char** argv)
{
    string device = "vcan0";
    
    if (argc > 1)
    {
        device = argv[1];
    }
    
    // listen to this communication with `isotpsniffer -s 100 -d 200 -c -td vcan0`

    vector<string> config_files = utils::getConfigFilenames(LUA_CONFIG_PATH);
    vector<thread> threads;

    for (const string &config_file : config_files)
    {
        if(config_file.find("doip") != string::npos) {
            doip.doipConfig = new DoipLuaScript(LUA_CONFIG_PATH + config_file);
            continue;
        }
        
        thread t(start_server, config_file, device);
        threads.push_back(move(t));
        usleep(50000);
    }

    thread t(&DoIPSimulator::start, &doip);  
    threads.push_back(move(t));    

    for (unsigned int i = 0; i < threads.size(); ++i)
    {
        threads[i].join();
    }

    return 0;
}

