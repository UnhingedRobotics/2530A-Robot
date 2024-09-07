/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Admin                                                     */
/*    Created:      9/6/2024, 1:49:45 PM                                      */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
brain Brain;
// Robot configuration code.
// AI Classification Competition Element IDs
enum gameElements {
  mobileGoal,
  redRing,
  blueRing,
};

controller Controller1 = controller(primary);
motor intakeMotorA = motor(PORT17, ratio18_1, false);
motor intakeMotorB = motor(PORT14, ratio18_1, true);
motor_group intake = motor_group(intakeMotorA, intakeMotorB);

motor leftdriveMotorA = motor(PORT18, ratio6_1, true);
motor leftdriveMotorB = motor(PORT19, ratio6_1, true);
motor_group leftdrive = motor_group(leftdriveMotorA, leftdriveMotorB);

motor rightdriveMotorA = motor(PORT13, ratio6_1, false);
motor rightdriveMotorB = motor(PORT12, ratio6_1, false);
motor_group rightdrive = motor_group(rightdriveMotorA, rightdriveMotorB);

// AI Vision Color Descriptions
// AI Vision Code Descriptions
vex::aivision AIVision8(PORT8, aivision::ALL_AIOBJS);

inertial Inertial13 = inertial(PORT13);

rotation leftwheelrotation = rotation(PORT10, false);

rotation backwheelrotation = rotation(PORT18, false);

digital_out goalclamp = digital_out(Brain.ThreeWirePort.H);

// generating and setting random seed
void initializeRandomSeed(){
  int systemTime = Brain.Timer.systemHighResolution();
  double batteryCurrent = Brain.Battery.current();
  double batteryVoltage = Brain.Battery.voltage(voltageUnits::mV);

  // Combine these values into a single integer
  int seed = int(batteryVoltage + batteryCurrent * 100) + systemTime;

  // Set the seed
  srand(seed);
}



void vexcodeInit() {

  //Initializing random seed.
  initializeRandomSeed(); 
}


// Helper to make playing sounds from the V5 in VEXcode easier and
// keeps the code cleaner by making it clear what is happening.
void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}



// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

int Brain_precision = 0, Console_precision = 0, Controller1_precision = 0, AIVision8_objectIndex = 0;

const float pi = 3.14159265359;
const float wheeldiameter = 3.25;
const float odomwheeldiameter = 2;
const float leftwheeldisplacement = 4;
const float backwheeldisplacement = 4;
float theta;
float dist;
float x;
float y;
bool tracking;


void PID_dist(float dist) {
  const float kpdist = 5;
  const float kddist = 5;
  float errordist = 100;
  float proportionaldist; 
  float derivativedist;
  float traveleddist;
  float preverrordist = 0;
  rightdrive.spin(forward);
  leftdrive.spin(forward);

  while (fabs(errordist) < 1) {
    traveleddist = (leftdrive.position(degrees) + rightdrive.position(degrees)) / 2 * pi * wheeldiameter / 360;
    errordist = dist - traveleddist;
    proportionaldist = (errordist * kpdist);
    derivativedist = (preverrordist - errordist) * kddist;
    leftdrive.spin(forward, proportionaldist + derivativedist, volt);
    rightdrive.spin(forward, proportionaldist + derivativedist, volt);
    preverrordist = errordist;
    wait(5, msec);
  }
}

void PID_theta(float theta) {
  const float kptheta = 0.1;
  const float kdtheta = 0.1;
  float errortheta = 100;
  float proportionaltheta; 
  float derivativetheta;
  float traveledtheta;
  float preverrortheta = 0;
  rightdrive.spin(forward);
  leftdrive.spin(forward);

  while (fabs(errortheta) < 1) {
    traveledtheta = Inertial13.rotation(degrees);
    errortheta = theta - traveledtheta;
    proportionaltheta = (errortheta * kptheta);
    derivativetheta = (preverrortheta - errortheta) * kdtheta;
    leftdrive.spin(forward, -1 * (proportionaltheta + derivativetheta), volt);
    rightdrive.spin(forward, proportionaltheta + derivativetheta, volt);
    preverrortheta = errortheta;
    wait(5, msec);
  }
}

void odometry() {
  float leftwheeldist;
  float backwheeldist;
  float traveledodomtheta;
  float traveledreverseodomtheta;
  float lefthypo;
  float backhypo;
  float thetanot = 90;
  float prevx = 0;
  float prevy = 0;
  while (tracking) {
    leftwheeldist = leftwheelrotation.position(degrees) * pi * odomwheeldiameter / 360;
    backwheeldist = backwheelrotation.position(degrees) * pi * odomwheeldiameter / 360;
    traveledreverseodomtheta = fmod((Inertial13.rotation(degrees) + thetanot) * -1, 360) * pi / 180;
    traveledodomtheta = fmod((Inertial13.rotation(degrees) + thetanot), 360) * pi / 180;
    lefthypo = 2 * (leftwheeldist / traveledreverseodomtheta + leftwheeldisplacement) * sin(traveledreverseodomtheta / 2); 
    backhypo = 2 * (backwheeldist / traveledreverseodomtheta - backwheeldisplacement) * sin(traveledreverseodomtheta / 2); 
    x = prevx + lefthypo * cos(traveledodomtheta) - backhypo * sin(traveledodomtheta); 
    y = prevy + lefthypo * sin(traveledodomtheta) + backhypo * cos(traveledodomtheta); 
    prevx = x;
    prevy = y;
    thetanot = 0;
  }
}


/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  tracking = false;
  leftwheelrotation.setPosition(0, degrees);
  backwheelrotation.setPosition(0, degrees);
  Inertial13.setRotation(0, degrees);
  Inertial13.setHeading(0, degrees);
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  rightdrive.setVelocity(0, percent);
  leftdrive.setVelocity(0, percent);
  PID_dist(3);
  // PID_theta(90);
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  rightdrive.spin(forward);
  leftdrive.spin(forward);
  rightdrive.setVelocity(0, percent);
  leftdrive.setVelocity(0, percent);
  constexpr int32_t DEADBAND = 20;
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.
    int32_t rightvelocity = Controller1.Axis2.position();
    int32_t leftvelocity = Controller1.Axis3.position();
    if (abs(leftvelocity) < DEADBAND) leftvelocity = 0;
    if (abs(rightvelocity) < DEADBAND) rightvelocity = 0;

    rightdrive.setVelocity(rightvelocity, percent);
    leftdrive.setVelocity(leftvelocity, percent);
    if (Controller1.ButtonR1.pressing()) {
      intake.spin(forward, 100, percent);
    }
    if (Controller1.ButtonR2.pressing()) {
      intake.stop();
    }
    if (Controller1.ButtonR1.pressing()) {
      intake.spin(forward, 100, percent);
    }
    if (Controller1.ButtonL1.pressing()) {
      goalclamp.set(true);
    }
    if (Controller1.ButtonL2.pressing()) {
      goalclamp.set(false);
    }
    AIVision8.takeSnapshot(aivision::ALL_AIOBJS);
    if (AIVision8.objectCount > 0 && AIVision8.objects[AIVision8_objectIndex].width > 70.0) {
      if (AIVision8.objects[AIVision8_objectIndex].id == mobileGoal) {
        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("Mobile Goal");
        goalclamp.set(true);

      }
      if (AIVision8.objects[AIVision8_objectIndex].id == redRing) {
        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("Redring");
      }
      if (AIVision8.objects[AIVision8_objectIndex].id == blueRing) {
        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("Bluering");
      }
    }
    // else {
      // goalclamp.set(false);
    // }
    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
