#include <Arduino.h>
#include "util/Serial.h"

void initSerial() {
    Serial.begin(115200);
    // chill for two seconds so user can open serial console
    delay(2000);
    Serial.println("> Aether Heavy Flightcode");
    Serial.print("> Version: ");
    Serial.println(FLIGHTCODE_VERSION);
    Serial.println(END_SEPERATOR);
}

void srlInitFin() {
    Serial.println("> Starting Main");
    Serial.println(END_SEPERATOR);
}

void srlGyro(String roll, String pitch) {
    Serial.println("> Gyro Log:");
    Serial.println("> Roll: " + roll + "Pitch: " + pitch);
    Serial.println(END_SEPERATOR);
}

void srlPower(String powerOutput) {
    Serial.println("> Power Log:");
    Serial.println("> Output Power Level: " + powerOutput);
    Serial.println(END_SEPERATOR);
}

void srlError(String errorMsg) {
    Serial.println("> Error Report:");
    Serial.println(">" + errorMsg);
    Serial.println(END_SEPERATOR);
}

void srlInfo(String infoMsg) {
    Serial.println("> Info:");
    Serial.println(">" + infoMsg);
    Serial.println(END_SEPERATOR);
}
