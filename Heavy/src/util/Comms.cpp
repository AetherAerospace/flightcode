#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include "Comms.h"
#include "main/Control.h"
#include "main/Power.h"
#include "util/Logs.h"
#include "pins/loraPins.h"
#include "util/Serial.h"

// other vars
String LoRaData;
int comInt = 1000;
unsigned long comPrevMillis = 0;
String dataReceived; 

void handleReceivedLora(){
    dataReceived = readLora();
    if(dataReceived != ""){
        writeLog(dataReceived + "handlereceivedlora");
    }
    if (dataReceived == "ENGINE OFF"){
        return;
    }else if (dataReceived == "ENGINE HOLD"){
        toggleESCHold();
    }else if (dataReceived == "ENGINE KILL"){
        killPower();
    }else if (dataReceived == "ENGINE ON"){
        return;
    }else if (dataReceived == "NO ENGINE HOLD"){
        toggleESCHold();
    }
}

void initLora(){
    SPI.begin(SCK, MISO, MOSI, SS);
    LoRa.setPins(SS, RST, DIO0);
    if (!LoRa.begin(BAND)) {
        srlError("Unable to start LoRa!");
        while (1);
    }
}

void sendLora(String messageToSend){
    unsigned long comMillis = millis();
    
    if(comMillis - comPrevMillis > comInt) {
        comPrevMillis = comMillis;
        LoRa.beginPacket();
        LoRa.print(messageToSend);
        LoRa.endPacket();
        srlInfo(String(messageToSend));
    }
}

String readLora(){
    int packetSize = LoRa.parsePacket();
    if (packetSize){
        while(LoRa.available()){
            LoRaData = LoRa.readString();
            srlInfo(String(LoRaData));
        }
    }
    return LoRaData;
}
