#include <Arduino.h>
#include "proc/PID.h"

/* 
    Basic PID-Controller
    Inspired by: https://github.com/br3ttb/Arduino-PID-Library
*/

PID::PID(double* Input, double* Output, double* Setpoint) {
    pidOutput = Output;
    pidInput = Input;
    pidSetpoint = Setpoint;
    outputSum = *pidOutput;
    lastInput = *pidInput;
    sampleTime = 100;
    lastTime = millis() - sampleTime;
}

void PID::set(double SkP, double SkI, double SkD, double SrelaxMin, double SrelaxMax, bool dirChange)
{
   if (SkP<0 || SkI<0 || SkD<0) return;
   double sampleTimeInSec = (double)sampleTime/1000;
   isReversed = dirChange;
   kP = SkP;
   kI = SkI * sampleTimeInSec;
   kD = SkD / sampleTimeInSec;
   relaxMin = 0; 
   relaxMax = 0;
   if ( SrelaxMin != 0 && SrelaxMin != 0 ) {
       relaxMin = SrelaxMin;
       relaxMax = SrelaxMax;
   }
}

bool PID::computePID() {
    unsigned long now = millis();
    unsigned long timeChange = (now - lastTime);
    if (timeChange >= sampleTime) {
        double output;
        double input = *pidInput;
        double error = *pidSetpoint - input;
        double diffInput = (input - lastInput);
        outputSum += (kI * error);
        output = (kP * error);
        output += outputSum - (kD * diffInput);
        if ( relaxMin != 0 && relaxMax != 0 ) {
            if (output > relaxMax) output = relaxMax;
            if (output < relaxMin) output = relaxMin;
        }
        if (isReversed == true) {
            *pidOutput = output*(-1) + *pidSetpoint;
        } else {
            *pidOutput = output + *pidSetpoint;
        }
        lastInput = input;
        lastTime = now;
        return true;
    }
    else return false;
}
