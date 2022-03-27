class PID {
    public:
        PID(double*, double*, double*);
        bool computePID();
        void set(double SkP, double SkI, double SkD, double SrelaxMin, double SrelaxMax, bool dirChange);
    private:
        double kP, kI, kD;
        double *pidOutput, *pidInput, *pidSetpoint;
        double outputSum, lastInput, relaxMin, relaxMax;
        bool isReversed;
        unsigned long lastTime, sampleTime;
};
