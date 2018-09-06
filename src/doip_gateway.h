#ifndef DOIP_GATEWAY_H
#define DOIP_GATEWAY_H

#include "DoIPServer.h"
#include "isotp_sender.h"
#include "doip_receiver.h"
#include <functional>

class DoIPGateway 
{
public:
    DoIPGateway();
    void startGateway();
    void startReceiver();
    DoIPServer* getServer() const;
    DoIPReceiver* recv_;
    void receiveFromLib(unsigned char* value, int length);
    void sendToEcu();
    void sendDiag(const uint8_t* data, int num_bytes);
private:  
    DoIPServer* doipserver;
    IsoTpSender* sender_;
    int curr_diag_data_length;
    unsigned char* curr_diag_data;
};

#endif /* DOIP_GATEWAY_H */
