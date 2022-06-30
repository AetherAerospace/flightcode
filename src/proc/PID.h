/* 
    Basic PID-Controller
    Inspired by: https://github.com/br3ttb/Arduino-PID-Library
*/

class PID {
    public:
        PID(double*, double*, double*);
        void set(double SkP, double SkI, double SkD, double SrelaxMin, double SrelaxMax);
        void computePID();
    private:
        double kP, kI, kD;
        double *pidOutput, *pidInput, *pidSetpoint;
        double outputSum, lastInput, relaxMin, relaxMax;
        bool isRelaxed;
        unsigned long lastTime, sampleTime;
};
