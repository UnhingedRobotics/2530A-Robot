#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen.
brain  Brain;

//The motor constructor takes motors as (port, ratio, reversed), so for example
//motor LeftFront = motor(PORT1, ratio6_1, false);

//Add your devices below, and don't forget to do the same in robot-config.h:
controller Controller1 = controller(primary);
motor intakeMotorA = motor(PORT9, ratio18_1, false);
motor intakeMotorB = motor(PORT19, ratio18_1, true);
motor_group intake = motor_group(intakeMotorA, intakeMotorB);
motor leftdrivefront = motor(PORT1, ratio6_1, false);
motor leftdrivemid = motor(PORT5, ratio6_1, false);
motor leftdriveback = motor(PORT6, ratio6_1, false);
motor rightdrivefront = motor(PORT11, ratio6_1, true);
motor rightdrivemid = motor(PORT12, ratio6_1, true);
motor rightdriveback = motor(PORT14, ratio6_1, true);
digital_out goalclamp = digital_out(Brain.ThreeWirePort.G);
digital_out swingarm = digital_out(Brain.ThreeWirePort.H);
inertial leftinertialsensor = inertial(PORT7);
inertial rightinertialsensor = inertial(PORT13);
aivision::colordesc aivisionsensor__bluering(1, 27, 47, 104, 10, 0.2);
aivision::colordesc aivisionsensor__redring(2, 161, 13, 72, 31, 0.34);
aivision aivisionsensor(PORT10, aivisionsensor__bluering, aivisionsensor__redring);

void vexcodeInit( void ) {
  // nothing to initialize
}
