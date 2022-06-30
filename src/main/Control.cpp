#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include "Control.h"
#include "proc/PID.h"
#include "util/Comms.h"
#include "util/Serial.h"
#include "settings/pidSet.h"
#include "settings/mpuSet.h"
#include "settings/servoSet.h"
#include "pins/servoPins.h"

// Servos
Servo servoRoll1;
Servo servoRoll2;
Servo servoPitch1;
Servo servoPitch2;

// PID
double Setpoint = PID_SETPOINT;
double InputPitch, OutputPitch;
double InputRoll, OutputRoll;
PID rollPID(&InputRoll, &OutputRoll, &Setpoint);
PID pitchPID(&InputPitch, &OutputPitch, &Setpoint);

// MPU
MPU6050 mpu;
int sampleCount;
bool controlReady = false;
bool initialInit = true;
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
  // ( kP, kI, kD, relaxMin, relaxMax )
  // values to be changed in the "pidSet.h" file!
  rollPID.set(PID_P, PID_I, PID_D, 
              PID_RELAX_ROLL_MIN, 
              PID_RELAX_ROLL_MAX);
  pitchPID.set( PID_P, PID_I, PID_D, 
                PID_RELAX_PITCH_MIN, 
                PID_RELAX_PITCH_MAX);
  srlInfo("PID initialized!");
}

// Servo Init
void initServo() {
  servoPitch1.attach(SERVO_PIN_PITCH_1, 0, 2500);
  servoPitch2.attach(SERVO_PIN_PITCH_2, 0, 2500);
  servoRoll1.attach(SERVO_PIN_ROLL_1, 0, 2500);
  servoRoll2.attach(SERVO_PIN_ROLL_2, 0, 2500);
  srlInfo("Servo's initialized!");
}

// MPU init
void dmpDataReady() {
  mpuInterrupt = true;
}
void initMPU() {
  Wire.begin();
  Wire.setClock(I2C_CLOCKRATE);
  mpu.initialize();
  if(mpu.getDeviceID() == 0) {
    srlError("MPU not found!");
    while (1) delay(10);
  };
  srlInfo("MPU found!");
  pinMode(INTERRUPT_PIN, INPUT);
  delay(1000);
  srlInfo("MPU Initialization started!");
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
    mpu.setDMPEnabled(true);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  }
  srlInfo("MPU initialized!");
}

// status check
bool motionReady() {
  // once controlReady is true,
  // main can start/initiate power
  return controlReady;
}

// main control loop
void loopControl(){
  if (!dmpReady) {
    controlReady = false;
    return;
  }
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    // assume control loop is ready to operate
    controlReady = true;
    // do mpu magic
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
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
    // logging
    // disabled for more performance
    #ifdef GYRO_LOG_INTERVAL
      srlGyro(String(OutputRoll), String(OutputPitch));
    #endif
    // we invert values for the opposing servo because thats how our
    // TVC System works, we don't recalc PID values for each servo
    // because that saves some processing power
    servoRoll1.write( OutputRoll - SERVO_ROLL_OFFSET );
    servoRoll2.write( map(OutputRoll, 180, 0, 0, 180) - SERVO_ROLL_OFFSET );
    servoPitch1.write( OutputPitch - SERVO_PITCH_OFFSET );
    servoPitch2.write( map(OutputPitch, 180, 0, 0, 180) - SERVO_PITCH_OFFSET);
  }
}
