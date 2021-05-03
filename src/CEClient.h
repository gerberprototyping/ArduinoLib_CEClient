#ifndef CEClient_H__
#define CEClient_H__

#include "CEC/CEC_Device.h"

class CEClient : public CEC_Device {

typedef void (*OnReceiveCallbackFunction)(int, int, unsigned char*, int);
typedef void (*OnTransmitCompleteCallbackFunction)(bool);

public:
    CEClient(int physicalAddress, int inputPin, int outputPin=-1);
    void begin(CEC_DEVICE_TYPE type = CEC_LogicalDevice::CDT_PLAYBACK_DEVICE);
    bool isReady();
    bool write(int targetAddress, unsigned char* buffer, int count);
    int getLogicalAddress();
    void setPromiscuous(bool promiscuous);
    void setMonitorMode(bool monitorMode);
    void onTransmitCompleteCallback(OnTransmitCompleteCallbackFunction);
    void onReceiveCallback(OnReceiveCallbackFunction);
    void run();

private:
    void OnTransmitComplete(bool);
    void OnReceive(int source, int dest, unsigned char* buffer, int count);
    void OnReady();
    OnTransmitCompleteCallbackFunction _onTransmitCompleteCallback;
    OnReceiveCallbackFunction _onReceiveCallback;
    bool _ready;
};

#endif // CEClient_H__