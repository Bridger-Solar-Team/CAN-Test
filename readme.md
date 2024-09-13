## Wiring
Using ESP-32 Devkit C and a TJA1050 transceiver.
This uses a simple voltage divider to bring the 5v signals
from the TJA1050 down to ESP-compatible 3.3v signals
```agsl
ESP32      TJA1050
5v ----------- VCC
Pin 5 --------> TX
GND--2k--+--1k--RX
Pin 4 <--+
GND ---------- GND
```
Two ESP32-TJA1050 combos are wired up, and then connected purely by the CANH and CANL pins. 
Each TJA1050 has a built-in 120 ohm termination resistor, so it works straight away

## CAN Protocol
Each message has a 7-bit ID that you assign, and can transmit 8 bytes of data.

## Coding
An explanation to future team members

### Setup
```C
#include <CAN.h>

int canData[8];
int messageID;

void setup() {
    CAN.setPins(RX_PIN, TX_PIN);
    CAN.begin(CAN_DATA_RATE);
    CAN.onReceive(readCAN);
}
``` 

### Sending a message
```C
CAN.beginPacket(HEXADECIMAL_CAN_ID);
CAN.write(Byte1);
...
CAN.write(Byte8);
CAN.endPacket();
```

### Reading a message
```C
void readCAN(int packetSize)
    messageID = CAN.packetId();
    int position = 0;
    while(CAN.available()) {
        canData[position] = CAN.read();
        position += 1;
    }
}
```
