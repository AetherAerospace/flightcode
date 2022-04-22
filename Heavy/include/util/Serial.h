#define FLIGHTCODE_VERSION "0.1.0"
#define END_SEPERATOR ">"

void initSerial();
void srlInitFin();
void srlPower(String powerOutput);
void srlGyro(String roll, String pitch);
void srlError(String errorMsg);
void srlInfo(String infoMsg);