#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include "util/Comms.h"
#include "util/Logs.h"
#include "util/Serial.h"
#include "Baro.h"
#include "Control.h"
#include "Power.h"

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
