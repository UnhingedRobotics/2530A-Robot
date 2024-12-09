using namespace vex;

extern brain Brain;

//To set up a motor called LeftFront here, you'd use
//extern motor LeftFront;

//Add your devices below, and don't forget to do the same in robot-config.cpp:
extern controller Controller1;
extern motor leftdrivefront;
extern motor leftdriveback;
extern motor rightdrivefront;
extern motor rightdriveback;
extern motor_group intake;
extern motor_group fishMech;
extern inertial leftinertialsensor;
extern inertial rightinertialsensor;
extern distance distancesensor;
// extern optical opticalsensor;
extern digital_out goalclamp;
extern rotation verticalwheelrotation;
extern rotation horizaontalwheelrotation;
extern aivision aivisionsensor;
extern aivision::colordesc aivisionsensor__bluering;
extern aivision::colordesc aivisionsensor__redring;

void  vexcodeInit( void );