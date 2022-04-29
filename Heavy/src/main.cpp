#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include "main/Baro.h"
#include "main/Control.h"
#include "main/Power.h"
#include "util/Comms.h"
#include "util/Logs.h"
#include "util/Serial.h"

void setup() {
  initSerial();
  initMPU();
  initPID();
  initServo();
  initESC();
  srlInitFin();
}

void loop() {
  loopControl();
  // we check if control loop is ready to handle things
  if (checkReadyStatus() == true) {
    loopPower();
  } else {
    return;
  }
}
