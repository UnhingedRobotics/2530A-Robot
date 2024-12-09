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
motor intakeMotorA = motor(PORT1, ratio6_1, false);
motor intakeMotorB = motor(PORT6, ratio6_1, true);
motor_group intake = motor_group(intakeMotorA, intakeMotorB);
motor fishMechMotorA = motor(PORT4, ratio36_1, false);
motor fishMechMotorB = motor(PORT3, ratio36_1, true);
motor_group fishMech = motor_group(fishMechMotorA, fishMechMotorB);
motor leftdrivefront = motor(PORT16, ratio18_1, true);
motor leftdriveback = motor(PORT14, ratio18_1, true);
motor rightdrivefront = motor(PORT20, ratio18_1, false);
motor rightdriveback = motor(PORT19, ratio18_1, false);
inertial leftinertialsensor = inertial(PORT12);
inertial rightinertialsensor = inertial(PORT9);
distance distancesensor = distance(PORT2);
// optical opticalsensor = optical(PORT10);
digital_out goalclamp = digital_out(Brain.ThreeWirePort.A);
aivision::colordesc aivisionsensor__bluering(1, 27, 47, 104, 10, 0.2);
aivision::colordesc aivisionsensor__redring(2, 161, 13, 72, 31, 0.34);
aivision aivisionsensor(PORT10, aivisionsensor__bluering, aivisionsensor__redring);
// vex::aivision AIVisionFront(PORT8, aivision::ALL_AIOBJS);
// vex::aivision AIVisionBack(PORT9, aivision::ALL_AIOBJS);
rotation verticalwheelrotation = rotation(PORT15, true);
rotation horizaontalwheelrotation = rotation(PORT7, true);

void vexcodeInit( void ) {
  // nothing to initialize
}
