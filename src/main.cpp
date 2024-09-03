#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.
// AI Classification Competition Element IDs
enum gameElements {
  mobileGoal,
  redRing,
  blueRing,
};

controller Controller1 = controller(primary);
motor intakeMotorA = motor(PORT2, ratio18_1, true);
motor intakeMotorB = motor(PORT3, ratio18_1, false);
motor_group intake = motor_group(intakeMotorA, intakeMotorB);

motor leftdriveMotorA = motor(PORT4, ratio6_1, false);
motor leftdriveMotorB = motor(PORT5, ratio6_1, false);
motor_group leftdrive = motor_group(leftdriveMotorA, leftdriveMotorB);

motor rightdriveMotorA = motor(PORT6, ratio6_1, false);
motor rightdriveMotorB = motor(PORT7, ratio6_1, false);
motor_group rightdrive = motor_group(rightdriveMotorA, rightdriveMotorB);

// AI Vision Color Descriptions
// AI Vision Code Descriptions
vex::aivision AIVision8(PORT8, aivision::ALL_AIOBJS);

inertial Inertial9 = inertial(PORT9);

rotation Rotation10 = rotation(PORT10, false);

rotation Rotation11 = rotation(PORT11, false);



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

#pragma endregion VEXcode Generated Robot Configuration

// Include the V5 Library
#include "vex.h"
  
// Allows for easier use of the VEX Library
using namespace vex;

competition Competition;

int Brain_precision = 0, Console_precision = 0, Controller1_precision = 0, AIVision8_objectIndex = 0;

const float pi = 3.14159265359;
float theta;
float dist;

// "when started" hat block
int whenStarted1() {
  Rotation10.setPosition(0.0, degrees);
  while (true) {
    AIVision8.takeSnapshot(aivision::ALL_AIOBJS);
    if (AIVision8.objectCount > 0 && AIVision8.objects[AIVision8_objectIndex].width > 70.0) {
      if (AIVision8.objects[AIVision8_objectIndex].id == mobileGoal) {
        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("Mobile Goal");
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
    wait(5, msec);
  }
  return 0;
}

// "when autonomous" hat block
int onauton_autonomous_0() {
  PID_dist(10);
  PID_theta(10);
  return 0;
}

void PID_dist(float dist) {
  const float kpdist = 0.1;
  const float kddist = 0.1;
  float errordist = 100;
  float proportionaldist; 
  float derivativedist;
  float traveleddist;
  float preverrordist = 0;

  while (abs(errordist) < 1) {
    traveleddist = (leftdrive.position(degrees) + rightdrive.position(degrees)) / 2 * pi / 180;
    errordist = dist - traveleddist;
    proportionaldist = (errordist * kpdist);
    derivativedist = (preverrordist - errordist) * kddist;
    leftdrive.spin(forward, proportionaldist + derivativedist, volt);
    rightdrive.spin(forward, proportionaldist + derivativedist, volt);
    preverrordist = errordist;
  }
  wait(5, msec);
}

void PID_theta(float theta) {
  const float kptheta = 0.1;
  const float kdtheta = 0.1;
  float errortheta = 100;
  float proportionaltheta; 
  float derivativetheta;
  float traveledtheta;
  float preverrortheta = 0;

  while (abs(errortheta) < 1) {
    traveledtheta = Inertial9.rotation(degrees);
    errortheta = theta - traveledtheta;
    proportionaltheta = (errortheta * kptheta);
    derivativetheta = (preverrortheta - errortheta) * kdtheta;
    leftdrive.spin(forward, -1 * (proportionaltheta + derivativetheta), volt);
    rightdrive.spin(forward, proportionaltheta + derivativetheta, volt);
    preverrortheta = errortheta;
  }
  wait(5, msec);
}

// "when driver control" hat block
int ondriver_drivercontrol_0() {
  return 0;
}

void VEXcode_driver_task() {
  // Start the driver control tasks....
  vex::task drive0(ondriver_drivercontrol_0);
  while(Competition.isDriverControl() && Competition.isEnabled()) {this_thread::sleep_for(10);}
  drive0.stop();
  return;
}

void VEXcode_auton_task() {
  // Start the auton control tasks....
  vex::task auto0(onauton_autonomous_0);
  while(Competition.isAutonomous() && Competition.isEnabled()) {this_thread::sleep_for(10);}
  auto0.stop();
  return;
}

int main() {
  vex::competition::bStopTasksBetweenModes = false;
  Competition.autonomous(VEXcode_auton_task);
  Competition.drivercontrol(VEXcode_driver_task);

  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // post event registration

  // set default print color to black
  printf("\033[30m");

  // wait for rotation sensor to fully initialize
  wait(30, msec);

  whenStarted1();
}

