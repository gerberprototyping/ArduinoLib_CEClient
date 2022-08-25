/*
    Example2_OpenDrain.ino

    This example uses demonstrated using the software open-drain
    functionality to connect the CEC bus directly to the MCU using
    only a single wire.
    
    Warning: The CEC bus requires 3.3V logic. Connecting a 5V
            device directly to the bus may cause damage to other
            HDMI devices.

    Basic example to demonstrate the use of the CEClient library
    The client is configured in promiscuous and monitor mode 
    to receive all the messages on the CEC bus
    
    No specific callback function is defined, therefore the client
    calls the default one, which prints the packets on the Serial port

    Use http://www.cec-o-matic.com/ to decode captured messages
*/

#include "CEClient.h"

// Enable software open-drain feature
#define SOFTWARE_OPEN_DRAIN

#define CEC_PHYSICAL_ADDRESS    0x1000
#define CEC_INPUT_PIN           2
#define CEC_OUTPUT_PIN          CEC_INPUT_PIN

// create a CEC client
CEClient ceclient(CEC_PHYSICAL_ADDRESS, CEC_INPUT_PIN, CEC_OUTPUT_PIN);


void setup() {

    Serial.begin(115200);

    // initialize the client with the default device type (PLAYBACK)
    ceclient.begin();

    // enable promiscuous mode (print all the incoming messages)
    ceclient.setPromiscuous(true);

    // enable monitor mode (do not transmit)
    ceclient.setMonitorMode(true);
}

void loop() {

    // run the client
    ceclient.run();
}