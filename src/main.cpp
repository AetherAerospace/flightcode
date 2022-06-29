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
  Serial.begin(115200);
  initMPU();
  initPID();
  initServo();
  initLora();
  while (!checkComm());
  // if we need to recalibrate, swap
  initESC();
  //initESCCalibrate();
  srlInitFin();
}

void loop() {
  loopControl();
  receiveLoRa();
  if (motionReady()) {
    loopPower();
  } else {
    return;
  }
}
