// demo: CAN-BUS Shield, send data
// loovee@seeed.cc

#include <mcp_can.h>
#include <SPI.h>

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_100KBPS))              
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(1000);
    }
    Serial.println("CAN BUS Shield init ok!");
}

unsigned char stmp[8] = {'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a'};
void loop()
{
    
    CAN.sendMsgBuf(0x11, 0, 8, stmp);
    Serial.println("sent");
    delay(2000);                       // send data per 100ms
}

// END FILE
