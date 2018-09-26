#ifndef DOIP_SIMULATOR_H
#define DOIP_SIMULATOR_H

#include "DoIPServer.h"
#include "electronic_control_unit.h"
#include <functional>
#include <vector>

class DoIPSimulator
{
public:
    DoIPSimulator();
    void start();
    void receiveFromLib(unsigned char* address, unsigned char* data, int length);
    void processDiagData();
    void sendDiag(const std::vector<unsigned char> data);
    void addECU(ElectronicControlUnit* ecu);
    

private:
    DoIPServer* doipserver;
    int curr_diag_data_length;
    unsigned char* curr_diag_data;
    std::vector<ElectronicControlUnit*> ecus;
    
    bool diagMessageReceived(unsigned char* targetAddress);
    int findECU(unsigned char* address);
};

#endif /* DOIP_SIMULATOR_H */