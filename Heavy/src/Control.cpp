#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include "util/Comms.h"
#include "proc/PID.h"
#include "util/Logs.h"
#include "pins/servoPins.h"
#include "util/Serial.h"
#include "Control.h"

// Servos
Servo servoRoll1;
Servo servoRoll2;
Servo servoPitch1;
Servo servoPitch2;

// PID
double Setpoint = 90;
double InputPitch, OutputPitch;
double InputRoll, OutputRoll;
PID rollPID(&InputRoll, &OutputRoll, &Setpoint);
PID pitchPID(&InputPitch, &OutputPitch, &Setpoint);

// MPU
MPU6050 mpu;
#define OUTPUT_READABLE_YAWPITCHROLL
#define INTERRUPT_PIN 3
int sampleCount;
bool controlReady = false;
bool initialInit = true;
float correct;
bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint8_t fifoBuffer[64];
Quaternion q;
VectorFloat gravity;
float euler[3];
float ypr[3];
volatile bool mpuInterrupt = false;

// PID init
void initPID() {
  /*
    ( kP, kI, kD, relaxMin, relaxMax, changeDir )

    relax is used for limiting PID-Output, we use it as
    general axis limiters, moved here because we do the
    processing anyway...

    ! DIRECTION CHANGES/LIMITS RELATED TO THE GENERAL 
    PID-LOOP CAN AND SHOULD ONLY HAPPEN HERE !
  
  */
  rollPID.set(0.9, 0, 0, 70.00, 110.00, false);
  pitchPID.set(0.9, 0, 0, 70.00, 110.00, false);
}

// Servo Init
void initServo() {
  servoPitch1.attach(SERVO_PIN_PITCH_1, 0, 2500);
  servoPitch2.attach(SERVO_PIN_PITCH_2, 0, 2500);
  servoRoll1.attach(SERVO_PIN_ROLL_1, 0, 2500);
  servoRoll2.attach(SERVO_PIN_ROLL_2, 0, 2500);
}

// MPU init
void dmpDataReady() {
  mpuInterrupt = true;
}
void initMPU() {
  Wire.begin();
  Wire.setClock(400000);
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);
  delay(1000);
  devStatus = mpu.dmpInitialize();
  // still offsets we took from the base lib, can be tuned
  // but they seem just fine for now
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788);
  if (devStatus == 0) {
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    mpu.setDMPEnabled(true);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  }
}

// status check
bool checkReadyStatus() {
  if (controlReady == true) {
    // put more checks here if necessary
    // this arms the ESC power loop, so things get dangerous!
    return true;
  } else {
    return false;
  }
}

//main control loop
void loopControl(){
  if (!dmpReady) {
    controlReady = false;
    return;
  }
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    if (initialInit) srlInfo("Collecting Yaw Samples...");
    initialInit = false;
    if (sampleCount <= 300) {
      correct = ypr[0];
      sampleCount++;
    } else {
      // assume control loop is ready to operate
      controlReady = true;
      // raw  degree data
      double rawPitch = (ypr[1] * 180/M_PI);
      double rawRoll = (ypr[2] * 180/M_PI);
      // convert into servo readable value
      // these are the vals which are passed to the PID-Loops
      InputPitch = map(rawPitch, -180.00, 180.00, 0.00, 180.00);
      InputRoll = map(rawRoll, -180.00, 180.00, 0.00, 180.00);
      // compute PID values based on servo vals
      rollPID.computePID();
      pitchPID.computePID();
      srlGyro(String(OutputRoll), String(OutputPitch));
      // we invert values for the opposing servo because thats how our
      // TVC System works, we don't recalc PID values for each servo
      // because that saves some processing power
      servoRoll1.write(OutputRoll);
      servoRoll2.write(map(OutputRoll, 180, 0, 0, 180));
      servoPitch1.write(OutputPitch);
      servoPitch2.write(map(OutputPitch, 180, 0, 0, 180));
    }
  }
}
