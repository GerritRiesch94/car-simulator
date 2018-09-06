#include "doip_receiver.h"
#include <iostream>

/*
 * Constructor. Opens a receiver for the doip gateway, to receive can messages
 * @param source:   the source CAN address
 * @param dest:     the destination CAN address
 * @param device:   the device used for the transmission
 * @param cb:       callback function to the doip gateway
 */
DoIPReceiver::DoIPReceiver(canid_t source, 
                           canid_t dest,
                           const std::string& device,
                           GatewayCallback cb)
: IsoTpReceiver(source, dest, device), gateway_cb(cb)
{
}

/*
 * When a message is received, the message and length get passed to
 * the doip gateway with the callback function
 * @param buffer:       the buffer containing the received data
 * @param num_bytes:    the number of received bytes
 */
void DoIPReceiver::proceedReceivedData(const uint8_t* buffer, const size_t num_bytes) noexcept
{
    gateway_cb(buffer, num_bytes);
}