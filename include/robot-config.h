using namespace vex;

extern brain Brain;

//To set up a motor called LeftFront here, you'd use
//extern motor LeftFront;

//Add your devices below, and don't forget to do the same in robot-config.cpp:
extern controller Controller1;
extern motor_group intake;
extern motor leftdrivefront;
extern motor leftdriveback;
extern motor rightdrivefront;
extern motor rightdriveback;
extern inertial inertialsensor;
extern distance distancesensor;
extern optical opticalsensor;
extern digital_out goalclamp;

void  vexcodeInit( void );