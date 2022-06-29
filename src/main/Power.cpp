#include <Arduino.h>
#include <ESP32Servo.h>
#include "Power.h"
#include "pins/escPins.h"
#include "util/Serial.h"

// ESCs
Servo ESC1;
Servo ESC2;

// bools
bool runLoop = true;
bool once = true;
bool holdEngine = false;
bool killed = false;

// other vars
long previousMillis = 0;
long interval = 50;   
float modifier = 0.5;
float value = 0;

// ESC Init
void initESC() {
    ESC1.attach(ESC_PIN_1, 1000, 2000);
    ESC2.attach(ESC_PIN_2, 1000, 2000);
    // give 2 seconds for initial powerup before we send any signals
    delay(2000);
    // make sure we set both ESC's to zero
    ESC1.write(0);
    ESC2.write(0);
    // again wait 2 seconds after first signal send just to be sure
    delay(2000);
    srlInfo("ESC's initialized!");
}

/* 
    Main Power Loop
    For now this is just a normal loop up/down of power.
    This can and will get more complex as we implement checks for
    height and postitional values and of course normal startup procedure.
*/
void loopPower() {
    unsigned long currentMillis = millis();
    if(runLoop == true){
        if(currentMillis - previousMillis > interval) {
            previousMillis = currentMillis;
            if(holdEngine == false){
                if (value > 70 && once == true) {
                    once = false;
                    modifier = 0;
                }
                if (value < 0 && once == false) {
                    modifier = 0;
                }
                value = value + modifier;
            }
            srlPower(String(value));
            ESC1.write(value);
            ESC2.write(value);
        }
    } 
}

// hold ESC value
void toggleESCHold(){
  if ( holdEngine == false ) {
      holdEngine = true;
  } else {
      holdEngine = false;
  }
}

// terminate ESCs
void killPower() {
    srlInfo("ESC-Kill received");
    if(killed == false){
        srlInfo("Zeroing both ESC's");
        runLoop = false;
        killed = true;
        ESC1.write(0);
        ESC2.write(0);
        srlInfo("ESC1: " + String(ESC1.read()));
        srlInfo("ESC2: " + String(ESC2.read()));
    } else {
        srlInfo("ESC's already killed");
    }
}

// calibrate ESCs
void initESCCalibrate() {
    // attach and write min max with delay
    // standard calibrating procedure
    ESC1.attach(ESC_PIN_1, 1000, 2000);
    ESC2.attach(ESC_PIN_2, 1000, 2000);
    srlInfo("ESC CALIBRATION");
    srlInfo("Waiting 4 Seconds...");
    delay(4000);
    ESC1.write(180);
    ESC2.write(180);
    srlInfo("Throttle MAX");
    delay(8000);
    ESC1.write(0);
    ESC2.write(0);
    srlInfo("Throttle MIN");
    delay(4000);
    srlInfo("Calibration done, please reboot!");
    while (1) delay(10);
}
