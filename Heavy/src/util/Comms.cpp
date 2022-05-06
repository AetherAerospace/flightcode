#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include "Comms.h"
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
                LoRaData = LoRa.readString();
                if (LoRaData == "Ground Hello") {
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
        }
    }
}
