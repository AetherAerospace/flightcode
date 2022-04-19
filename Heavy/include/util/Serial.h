#define FLIGHTCODE_VERSION "0.1.0"
#define END_SEPERATOR ">"

void initSerial();
void srlInitFin();
void srlPower(String powerOutput, int logInterval);
void srlGyro(String roll, String pitch, int logInterval);