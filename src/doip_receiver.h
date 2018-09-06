#ifndef DOIP_RECEIVER_H
#define DOIP_RECEIVER_H

#include "isotp_receiver.h"
#include <functional>

using GatewayCallback = std::function<void(const uint8_t*,int)>;

class DoIPReceiver : public IsoTpReceiver
{
    
public:
    DoIPReceiver() = delete;
    DoIPReceiver(canid_t source,
                 canid_t dest,
                 const std::string& device,
                 GatewayCallback cb);
    virtual void proceedReceivedData(const uint8_t* buffer, const size_t num_bytes) noexcept override;
    
private:
    GatewayCallback gateway_cb; 
};

#endif /* DOIP_RECEIVER_H */
