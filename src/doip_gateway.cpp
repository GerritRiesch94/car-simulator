#include "doip_gateway.h"
#include <string>
#include <iostream>
#include <thread>

DoIPGateway::DoIPGateway() {
    curr_diag_data = nullptr;
    doipserver = new DoIPServer();

    GatewayCallback cb = std::bind(&DoIPGateway::sendDiag, this, std::placeholders::_1, std::placeholders::_2);
    
    std::string device = "vcan0";
    sender_ = new IsoTpSender(0x7E0, 0x100, device);
    recv_ = new DoIPReceiver(0x100, 0x7E8, device, cb);
}

/*
 * Setup the gateway to use a doip server
 */
void DoIPGateway::startGateway() {
    DiagnosticCallback cb = std::bind(&DoIPGateway::receiveFromLib, this, std::placeholders::_1, std::placeholders::_2);
    doipserver->setCallback(cb);
    
    //Udp
    doipserver->setupUdpSocket();
    doipserver->receiveUdpMessage();
    
    //Tcp
    doipserver->setupSocket();
    while(1)
        doipserver->receiveMessage();     
}

void DoIPGateway::startReceiver() {
    recv_->readData();
}

void DoIPGateway::receiveFromLib(unsigned char* value, int length) {
    printf("CarSimulator DoIP Gateway received: ");
    for(int i = 0; i < length; i++) {
        printf("0x%x ", value[i]);
    }
    printf(" from doip lib.\n");
    
    curr_diag_data_length = length;
    curr_diag_data = value;
}

void DoIPGateway::sendToEcu() {
    while(curr_diag_data == NULL) {
        sleep(5);
    }
    
    sender_->sendData(curr_diag_data, curr_diag_data_length);
    curr_diag_data = nullptr;
}

void DoIPGateway::sendDiag(const uint8_t* data, int num_bytes) {
    unsigned char* msg = new unsigned char[num_bytes];
    for(int i = 0; i < num_bytes; i++) {
        msg[i] = data[i];
    }
    
    doipserver->receiveDiagnosticPayload(msg, num_bytes);
}

DoIPServer* DoIPGateway::getServer() const {
    return doipserver;
}