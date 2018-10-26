#ifndef DOIP_SIMULATOR_H
#define DOIP_SIMULATOR_H

#include "DoIPServer.h"
#include "electronic_control_unit.h"
#include "doip_lua_script.h"
#include <functional>
#include <thread>
#include <vector>

class DoIPSimulator
{
public:
    DoIPSimulator();
    DoipLuaScript* doipConfig;
    void start();
    void receiveFromLib(unsigned char* address, unsigned char* data, int length);
    void sendDiag(const std::vector<unsigned char> data, unsigned char* logicalAddress);
    void addECU(ElectronicControlUnit* ecu);

private:
    DoIPServer* doipserver;
    std::vector<ElectronicControlUnit*> ecus;
    std::vector<std::thread> doipReceiver;
    
    bool diagMessageReceived(unsigned char* targetAddress);
    int findECU(unsigned char* address);
    
    void configureDoipServer();
    void listenUdp();
    void listenTcp();
   
};

#endif /* DOIP_SIMULATOR_H */