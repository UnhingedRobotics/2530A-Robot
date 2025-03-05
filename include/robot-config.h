using namespace vex;

extern brain Brain;

//To set up a motor called LeftFront here, you'd use
//extern motor LeftFront;

//Add your devices below, and don't forget to do the same in robot-config.cpp:
extern controller Controller1;
extern motor leftdrivefront;
extern motor leftdrivemid;
extern motor leftdriveback;
extern motor rightdrivefront;
extern motor rightdrivemid;
extern motor rightdriveback;
extern motor intake;
extern motor roller;
extern inertial leftinertialsensor;
extern inertial rightinertialsensor;
extern digital_out goalclamp;
extern digital_out swingarm; 
extern aivision aivisionsensor;
extern aivision::colordesc aivisionsensor__bluering;
extern aivision::colordesc aivisionsensor__redring;

void  vexcodeInit( void );
