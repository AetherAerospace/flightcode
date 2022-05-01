#define FLIGHTCODE_VERSION "0.1.1"
#define END_SEPERATOR ">"

void initSerial();
void srlInitFin();
void srlPower(String powerOutput);
void srlGyro(String roll, String pitch);
void srlError(String errorMsg);
void srlInfo(String infoMsg);
void srlBaro(String prs, String alt);