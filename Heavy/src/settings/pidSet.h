// base pid settings
#define PID_SETPOINT 90
#define PID_INVERT false

// main pid vals
#define PID_P 0.7
#define PID_I 0.1
#define PID_D 0.05

/*
    relax is used for limiting PID-Output, 
    we use it as general axis limiter
*/

// relax vals
#define PID_RELAX_ROLL_MIN 70
#define PID_RELAX_PITCH_MIN 70
#define PID_RELAX_ROLL_MAX 110
#define PID_RELAX_PITCH_MAX 110
