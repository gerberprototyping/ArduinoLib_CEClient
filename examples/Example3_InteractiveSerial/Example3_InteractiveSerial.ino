/*
    Example3_InteractiveSerial

    This example allows CEC codes to be sent via the serial console.
    
    No specific callback function is defined, therefore the client
    calls the default one, which prints the packets on the Serial port

    Use http://www.cec-o-matic.com/ to decode captured messages
*/
#include "CEClient.h"

#define BUFF_SIZE               512
#define ECHO                    true


#define CEC_PHYSICAL_ADDRESS    0x1000
#define CEC_INPUT_PIN           3
#define CEC_OUTPUT_PIN          CEC_INPUT_PIN


// create a CEC client
CEClient ceclient(CEC_PHYSICAL_ADDRESS, CEC_INPUT_PIN, CEC_OUTPUT_PIN);

// Serial character buffer
uint8_t buff[BUFF_SIZE];
uint16_t buffIdx = 0;

// Forward declare functions
void parseCec();
void cecReceiveCallback(int, int, unsigned char*, int);


void setup() {

    Serial.begin(115200);

    // initialize the client with the default device type (PLAYBACK)
    ceclient.begin();

    // set cec recieve callback
    ceclient.onReceiveCallback(cecReceiveCallback);

    // enable promiscuous mode (print all the incoming messages)
    ceclient.setPromiscuous(true);

    delay(2000); // Not required

    while (!ceclient.isReady()) {
      ceclient.run();
    }
    Serial.println("Init Complete");

    Serial.print("Logical address: 0x");
    Serial.println((uint8_t)ceclient.getLogicalAddress(), HEX);
    
}

void loop() {

    // run the client
    ceclient.run();

    while (Serial.available()) {
      buff[buffIdx] = Serial.read();
      if ('\n' == buff[buffIdx]) {
        buffIdx--;
        parseCec();
        buffIdx = 0;
      } else {
        buffIdx++;
      }
    }

}


void cecReceiveCallback(int source, int dest, unsigned char* buffer, int count) {
  Serial.print("cec ");
  Serial.print(source,HEX);
  Serial.print(dest,HEX);
  for (uint8_t i; i<count; i++) {
    Serial.print(':');
    if (buffer[i] < 0x10) {
      Serial.print('0');
    }
    Serial.print(buffer[i],HEX);
  }
  Serial.println();
}


void parseCec() {
    uint16_t readIndex = 0;
    uint16_t writeIndex = 0;

    bool highNyble = true;
    while (readIndex <= buffIdx) {
      uint8_t c = buff[readIndex++];
      uint8_t hexVal;
      if (c >= '0'&& c <= '9') {
        hexVal = c-'0';
      }
      else if (c >= 'A' && c <= 'F') {
        hexVal = c-'A'+10;
      }
      else if (c >= 'a' && c <= 'f') {
        hexVal = c-'a'+10;
      }
      else if (c != ':') {
        if (ECHO) {
          Serial.print("Error: Unknown character '");
          Serial.print((char)c);
          Serial.print("' (0x");
          Serial.print(c,HEX);
          Serial.print(") at char ");
          Serial.println(readIndex,DEC);
        }
        return;
      }
      if (c != ':') {
        if (highNyble) {
          buff[writeIndex] = hexVal << 4;
        } else {
          buff[writeIndex] |= hexVal & 0x0F;
          writeIndex++;
        }
        highNyble = !highNyble;
      }
    }

    if (ECHO) {
      Serial.print("Sending message '");
      for (uint8_t i=0; i<writeIndex-1; i++) {
        Serial.print(buff[i],HEX);
        Serial.print(':');
      }
      Serial.print(buff[writeIndex-1],HEX);
      Serial.print("' ");
      if (ceclient.write(buff[0], (buff)+1, writeIndex)) {
        Serial.println("success");
      } else {
        Serial.println("FAIL!");
      }
    }
    
}
