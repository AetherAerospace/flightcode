#include <Arduino.h>
#include "Serial.h"
#include "settings/serialSet.h"

// for delays
unsigned long gyroPrevMillis = 0;
unsigned long baroPrevMillis = 0;
unsigned long powerPrevMillis = 0;

void srlError(String errorMsg) {
    Serial.println("> Error Report:");
    Serial.println("> " + errorMsg);
    Serial.println(END_SEPERATOR);
}

void srlInfo(String infoMsg) {
    Serial.println("> Info:");
    Serial.println("> " + infoMsg);
    Serial.println(END_SEPERATOR);
}

void srlInitFin() {
    Serial.println("> Setup:");
    Serial.println("> Initialization Sequence complete!");
    Serial.println(END_SEPERATOR);
}

void srlGyro(String roll, String pitch) {
    #ifdef GYRO_LOG_INTERVAL
    unsigned long gyroMillis = millis();
    if (gyroMillis - gyroPrevMillis > GYRO_LOG_INTERVAL) {
        gyroPrevMillis = gyroMillis;
        Serial.println("> Gyro Log:");
        Serial.println("> Roll: " + roll + " Pitch: " + pitch);
        Serial.println(END_SEPERATOR);
    }
    #endif
}

void srlBaro(String prs, String alt) {
    #ifdef BARO_LOG_INTERVAL
    unsigned long baroMillis = millis();
    if (baroMillis - baroPrevMillis > BARO_LOG_INTERVAL) {
        baroPrevMillis = baroMillis;
        Serial.println("> Baro Log:");
        Serial.println("> Pressure: " + prs + " Altitude: " + alt);
        Serial.println(END_SEPERATOR);
    }
    #endif
}

void srlPower(String powerOutput) {
    #ifdef POWER_LOG_INTERVAL
    unsigned long powerMillis = millis();
    if (powerMillis - powerPrevMillis > POWER_LOG_INTERVAL) {
        powerPrevMillis = powerMillis;
        Serial.println("> Power Log:");
        Serial.println("> Output Power Level: " + powerOutput);
        Serial.println(END_SEPERATOR);
    }
    #endif
}
