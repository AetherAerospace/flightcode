#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include "Baro.h"
#include "util/Logs.h"
#include "util/Serial.h"

Adafruit_BMP280 bmp;

double atmosphericpressure = 1013.25;
float currentHeight;
float maxHeight;
int bmpInt = 200;
unsigned long bmpPrevMillis = 0;

void initBaro(){
    if(!bmp.begin(0x76)){
        srlError("BMP280 not found!");
        while (1) delay(10);
    }
    srlInfo("BMP found!");
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loopBaro(){
    srlBaro(
        String(bmp.readPressure())+"Pa", 
        String(bmp.readAltitude(1013.25))+"m"
    );
}

float getHeight(){
    return bmp.readAltitude(atmosphericpressure);
}

void loopApogee(){
    unsigned long bmpMillis = millis();
    if(bmpMillis - bmpPrevMillis > bmpInt){
        currentHeight = getHeight();
        writeLog("Current Height: " + String(currentHeight));
        writeLog("Current Apogee: " + String(maxHeight));
        if(currentHeight > maxHeight){
            maxHeight = currentHeight;
            writeLog("New Apogee: " + String(maxHeight));
        }
    }
}
