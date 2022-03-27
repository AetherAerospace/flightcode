#include <Arduino.h>
#include <ESP32Servo.h>

// ESCs
Servo ESC1;
Servo ESC2;

// vars
bool holdEngine = false;
bool killed = false;
bool runLoop = false;
bool once = true;
long previousMillis = 0;
long interval = 50;   
float modifier = 0.5;
float value = 0;

// pins
const int escPin1 = 12;
const int escPin2 = 23;

// ESC Init
void initESC() {
    ESC1.attach(escPin1,1000,2000);
    ESC2.attach(escPin2,1000,2000);
    // give 2 seconds for initial powerup before we send any signals
    delay(2000);
    // make sure we set both ESC's to zero
    ESC1.write(0);
    ESC2.write(0);
    // again wait 2 seconds after first signal send just to be sure
    delay(2000);
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
                if (value > 180 && once == true) {
                    once = false;
                    modifier = -0.5;
                }
                if (value < 0 && once == false) {
                    modifier = 0;
                }
                value = value + modifier;
            }
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
    if(killed == false){
        Serial.println("recv kill");
        runLoop = false;
        ESC1.write(0);
        ESC2.write(0);
        Serial.println(ESC1.read());
        Serial.println(ESC2.read());
        killed = true;
        Serial.println("kill exec success");
    }
}