// base pid settings
#define PID_SETPOINT 90

/*
    Main PID-Vals
    The P-Value:
        Accounts for PRESENT error and is basically 
        a multiplier of the raw MPU value in our case.
        Too high of a value leads to overcorrection
        Too low of a value leads to undercorrection
        Probable Range: 0.2 - 2.0
    The I-Value
        Accoutns for PAST error and accumulates overtime.
        If the error persists and the raw error value gets to
        small, I will counteract over time until setpoint is reached.
        Too high of a value leads to I-Term overflow
        Probable Range: 0.1 - 1.0
    The D-Value
        Accoutns for POSSBILE FUTURE error and dampens oscillations produced
        mostly by the P-Term.
        Too high of a value leads to extensive jittering/overdampening
        Tests have shown that anything over 0.15 leads to too much jittering
        in our setup.
        Probable Range: 0.1 - 0.15
*/
#define PID_P 1.0
#define PID_I 0.5
#define PID_D 0.05

/*
    relax is used for limiting PID-Output, 
    we use it as general axis limiter
*/
#define PID_RELAX_ROLL_MIN 70
#define PID_RELAX_PITCH_MIN 70
#define PID_RELAX_ROLL_MAX 110
#define PID_RELAX_PITCH_MAX 110
