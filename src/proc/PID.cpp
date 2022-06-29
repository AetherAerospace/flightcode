#include <Arduino.h>
#include "PID.h"

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
    sampleTime = 4;
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
   if ( SrelaxMin != 0 && SrelaxMin != 0 ) {
       isRelaxed = true;
       relaxMin = SrelaxMin;
       relaxMax = SrelaxMax;
   } else {
       isRelaxed = false;
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
        if (isRelaxed) {
            if ( (output + *pidSetpoint) > relaxMax) {
                *pidOutput = relaxMax;
            } else if ( (output + *pidSetpoint) < relaxMin) {
                *pidOutput = relaxMin;
            } else {
                *pidOutput = output + *pidSetpoint;
            }
        } else {
            *pidOutput = output + *pidSetpoint;
        }
        if (isReversed) {
            *pidOutput = *pidOutput*(-1);
        }
        lastInput = input;
        lastTime = now;
        return true;
    }
    else return false;
}
