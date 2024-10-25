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
motor intakeMotorA = motor(PORT8, ratio6_1, true);
motor intakeMotorB = motor(PORT16, ratio6_1, false);
motor_group intake = motor_group(intakeMotorA, intakeMotorB);
motor fourBarMotorA = motor(PORT17, ratio6_1, false);
motor fourBarMotorB = motor(PORT13, ratio6_1, true);
motor_group fourBar = motor_group(fourBarMotorA, fourBarMotorB);
motor leftdrivefront = motor(PORT12, ratio18_1, true);
motor leftdriveback = motor(PORT5, ratio18_1, true);
motor rightdrivefront = motor(PORT19, ratio18_1, false);
motor rightdriveback = motor(PORT18, ratio18_1, false);
inertial inertialsensor = inertial(PORT9);
distance distancesensor = distance(PORT3);
optical opticalsensor = optical(PORT7);
digital_out goalclamp = digital_out(Brain.ThreeWirePort.A);
// vex::aivision AIVisionFront(PORT8, aivision::ALL_AIOBJS);
// vex::aivision AIVisionBack(PORT9, aivision::ALL_AIOBJS);
rotation verticalwheelrotation input = pow(input / 100.0, 2) * 100;= rotation(PORT6, false);
rotation horizaontalwheelrotation = rotation(PORT5, false);

void vexcodeInit( void ) {
  // nothing to initialize
}