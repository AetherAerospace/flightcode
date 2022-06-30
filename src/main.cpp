#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include "main/Control.h"
#include "main/Power.h"
#include "util/Baro.h"
#include "util/Comms.h"
#include "util/Serial.h"

void setup() {
  // force fastest speed
  setCpuFrequencyMhz(240);

  // start serial out
  Serial.begin(115200);

  // base functions init
  initMPU();
  initPID();
  initServo();

  // comm init
  initLora();
  while (!checkComm());
  
  // if we need to recalibrate, swap
  initESC();
  //initESCCalibrate();
  
  srlInitFin();
  
  // last motion ready check
  while (!motionReady());
}

void loop() {
  // rocket stabilization functions
  loopControl();

  // communication functions
  receiveLoRa();

  // propulsion functions
  loopPower();
}
