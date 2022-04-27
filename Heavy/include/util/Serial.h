#define FLIGHTCODE_VERSION "0.1.1"
#define END_SEPERATOR ">"

// interval for main logging cases
#define GYRO_LOG_INTERVAL 150
#define BARO_LOG_INTERVAL 1000
#define POWER_LOG_INTERVAL 250

void initSerial();
void srlInitFin();
void srlPower(String powerOutput);
void srlGyro(String roll, String pitch);
void srlError(String errorMsg);
void srlInfo(String infoMsg);
void srlBaro(String prs, String alt);