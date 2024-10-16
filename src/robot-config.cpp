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
motor intakeMotorA = motor(PORT15, ratio6_1, false);
motor intakeMotorB = motor(PORT16, ratio6_1, true);
motor_group intake = motor_group(intakeMotorA, intakeMotorB);
motor leftdrivefront = motor(PORT12, ratio18_1, true);
motor leftdriveback = motor(PORT13, ratio18_1, true);
motor rightdrivefront = motor(PORT19, ratio18_1, false);
motor rightdriveback = motor(PORT18, ratio18_1, false);
inertial inertialsensor = inertial(PORT17);
distance distancesensor = distance(PORT3);
optical opticalsensor = optical(PORT6);
digital_out goalclamp = digital_out(Brain.ThreeWirePort.A);
// vex::aivision AIVisionFront(PORT8, aivision::ALL_AIOBJS);
// vex::aivision AIVisionBack(PORT9, aivision::ALL_AIOBJS);
// rotation verticalwheelrotation = rotation(PORT4, false);
// rotation backwheelrotation = rotation(PORT12, false);

void vexcodeInit( void ) {
  // nothing to initialize
}