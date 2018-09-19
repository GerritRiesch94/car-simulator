#include "doip_simulator.h"

/**
 * Constructor. Creates a DoIPServer for this simulator
 */
DoIPSimulator::DoIPSimulator() {
    curr_diag_data = nullptr;
    doipserver = new DoIPServer();
}

/**
 * Initialize server instance with required callbacks, start the doip server
 */
void DoIPSimulator::start() {
    DiagnosticCallback cb = std::bind(&DoIPSimulator::receiveFromLib, this, std::placeholders::_1, std::placeholders::_2);
    DiagnosticMessageNotification dmn = std::bind(&DoIPSimulator::diagMessageReceived, this);
    doipserver->setCallback(cb, dmn);
    
    //Udp
    doipserver->setupUdpSocket();
    doipserver->receiveUdpMessage();
    
    //Tcp
    doipserver->setupSocket();
    while(1)
        doipserver->receiveMessage();
}

/**
 * Is called when the doip library receives a diagnostic message. 
 * @param data      message which was received
 * @param length    length of the message
 */
void DoIPSimulator::receiveFromLib(unsigned char* data, int length) {
    printf("CarSimulator DoIP Simulator received: ");
    for(int i = 0; i < length; i++) {
        printf("0x%x ", data[i]);
    }
    printf(" from doip lib.\n");
    
    curr_diag_data_length = length;
    curr_diag_data = data;
}

/**
 * Constantly checks if the simulator received data from the lib, if thats the case
 * it passes the received data to the ecu
 */
void DoIPSimulator::processDiagData() {
    while(curr_diag_data == nullptr) {
        sleep(2);
    }
    
    std::vector<unsigned char> response = ecus.at(0)->getUdsReceiver()->proceedDoIPData(curr_diag_data, curr_diag_data_length);
    
    curr_diag_data_length = 0;
    curr_diag_data = nullptr;
    
    if(response.size() > 0) {
        sendDiag(response);
    }
}

/**
 * Passes the received response from a ecu back to the doip library
 */
void DoIPSimulator::sendDiag(const std::vector<unsigned char> data) {
    
    unsigned char* msg = new unsigned char[data.size()];
    for(unsigned int i = 0; i < data.size(); i++) {
        msg[i] = data[i];
    }
    
    doipserver->receiveDiagnosticPayload(msg, data.size());
    delete[] msg;
}

/**
 * Adds a ECU to the list
 * @param ecu   Pointer to the ecu
 */
void DoIPSimulator::addECU(ElectronicControlUnit* ecu) {
    ecus.push_back(ecu);
}

/**
 * Will be called when the doip library receives a diagnostic message.
 * The library notifies the application about the message.
 * 
 * @return  If a positive or negative ACK should be send to the client
 */
bool DoIPSimulator::diagMessageReceived() {
    
    //both cases just to switch fast when testing
    //positiv ack
    //PayloadType ackType = PayloadType::DIAGNOSTICPOSITIVEACK;
    //unsigned char ackCode = 0x00;
    
    //negative ack
    PayloadType ackType = PayloadType::DIAGNOSTICNEGATIVEACK;
    unsigned char ackCode = 0x02;
    
    doipserver->sendDiagnosticAck(ackType, ackCode);
    
    if(ackType == PayloadType::DIAGNOSTICPOSITIVEACK)
        return true;
    else
        return false;
}
