// Pin definitions for switches and magnetic sensors
const char swA=16;
const char swB=13;
const char mag1=32;
const char mag2=35;
const char d1=5;
const char d2=14;
const char d3=23;
const char d4=19;
const char d5=18;
const char d6=26;
const char d7=15;
const char d8=34;
const char d9=33;

void FPVSetup();
void calibrateThrottle();
int isCalibrated();
int getThrottle(int throttleMax);
int getSteering(int steeringMax);
void doTest();
void bindIfNeeded();
