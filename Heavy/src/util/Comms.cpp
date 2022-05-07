#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include "Comms.h"
#include "main/Power.h"
#include "pins/loraPins.h"
#include "util/Serial.h"
#include "settings/loraSet.h"

String LoRaData;
int answerTimeout = 250;
unsigned long now = 0;
unsigned long startTime = 0;
bool checkReceived = false;

// initialize
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

// handle packets based on the code we get
void loRaDecode(String code) {
    // more codes to be added
    if (code == "404") {
        killPower();
    } else {
        return;
    }
}

// send packet
void sendLoRa(String msg) {
    LoRa.beginPacket();
    LoRa.print(msg);
    LoRa.endPacket();
}

// initial comm check
bool checkComm() {
    if (!checkReceived) {
        int packetSize = LoRa.parsePacket();
        if (packetSize) {
            while ( LoRa.available() ) {
                if (LoRa.readString() == "Ground Hello") {
                    checkReceived = true;
                    startTime = millis();
                    srlInfo("LoRa RX: Comm Check received");
                }
            }
        }
    } else {
        now = millis();
        if ( (now - startTime) >= answerTimeout) {
            sendLoRa("Onboard Hello");
            srlInfo("LoRa TX: ACK sent");
            return true;
        }
    }
    return false;
}

// operational LoRa Loop
void receiveLoRa() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        while ( LoRa.available() ) {
            LoRaData = LoRa.readString();
            srlInfo("LoRa RX: " + LoRaData);
            loRaDecode(LoRaData);
        }
    }
}
