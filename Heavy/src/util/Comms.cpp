#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include "Comms.h"
#include "pins/loraPins.h"
#include "util/Serial.h"
#include "settings/loraSet.h"

// other vars
String LoRaData;
int comInt = 100;
unsigned long comPrevMillis = 0;

void initLora() {
    SPI.begin(SCK, MISO, MOSI, SS);
    LoRa.setPins(SS, RST, DIO0);
    if (!LoRa.begin(BAND)) {
        srlError("Unable to start LoRa!");
        while (1);
    }
    LoRa.setSyncWord(0xF3);
    srlInfo("LoRa initialized!");
}

void sendLora(String messageToSend) {
    unsigned long comMillis = millis();
    if(comMillis - comPrevMillis > comInt) {
        comPrevMillis = comMillis;
        LoRa.beginPacket();
        LoRa.print(messageToSend);
        LoRa.endPacket();
        srlInfo("Lora TX: " + String(messageToSend));
    }
}

void handleLora() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        while ( LoRa.available() ) {
            LoRaData = LoRa.readString();
            srlInfo(LoRaData);
            if (LoRaData == "Ground Hello") {
                sendLora("Onboard Hello");
            }
        }
    }
}
