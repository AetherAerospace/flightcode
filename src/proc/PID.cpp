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
    sampleTime = 2;
    isRelaxed = false;
    lastTime = millis() - sampleTime;
}

void PID::set(double SkP, double SkI, double SkD, double SrelaxMin, double SrelaxMax) {
    if (SkP<0 || 
       SkI<0 || 
       SkD<0)
    return;
    double sampleTimeInSec = (double)sampleTime/1000;
    kP = SkP;
    kI = SkI * sampleTimeInSec;
    kD = SkD / sampleTimeInSec;
    if ( SrelaxMin != 0 && SrelaxMin != 0 ) {
        isRelaxed = true;
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
        if ( 
            isRelaxed && 
            (output + *pidSetpoint) > relaxMax
            ) {
            *pidOutput = relaxMax;
        } else if ( 
            isRelaxed &&
            (output + *pidSetpoint) < relaxMin 
            ) {
            *pidOutput = relaxMin;
        } else {
            *pidOutput = output + *pidSetpoint;
        }
        lastInput = input;
        lastTime = now;
        return true;
    }
    else return false;
}
