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
motor intake = motor(PORT13, ratio18_1, false);

motor leftdriveMotorA = motor(PORT15, ratio6_1, false);
motor leftdriveMotorB = motor(PORT12, ratio6_1, false);
motor_group leftdrive = motor_group(leftdriveMotorA, leftdriveMotorB);

motor rightdriveMotorA = motor(PORT14, ratio6_1, true);
motor rightdriveMotorB = motor(PORT16, ratio6_1, true);
motor_group rightdrive = motor_group(rightdriveMotorA, rightdriveMotorB);

// AI Vision Color Descriptions
// AI Vision Code Descriptions
vex::aivision AIVision8(PORT3, aivision::ALL_AIOBJS);

inertial Inertial13 = inertial(PORT18);

rotation leftwheelrotation = rotation(PORT1, false);

rotation backwheelrotation = rotation(PORT2, false);

digital_out goalclamp = digital_out(Brain.ThreeWirePort.A);

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

const double pi = 3.14159265359;
const double wheeldiameter = 3.25;
const double odomwheeldiameter = 2;
const double leftwheeldisplacement = 4;
const double backwheeldisplacement = 4;
double kpdist;
double kddist;
double kidist;
double slewratedist;
double kptheta;
double kdtheta;
double kitheta;
double theta;
double dist;
double x;
double y;
bool tracking;


void PID_dist(double dist, double kpdist, double kddist, double kidist, double slewratedist) {
  bool firstdist = true;
  double errordist = 100;
  double proportionaldist = 0; 
  double derivativedist = 0;
  double integraldist = 0;
  double traveleddist = 0;
  double preverrordist = 0;
  double velocitydist = 0;
  double prevvelocitydist = 0;
  leftdrive.setPosition(0, degrees);
  rightdrive.setPosition(0, degrees);
  rightdrive.spin(forward);
  leftdrive.spin(forward);

  while (fabs(errordist) > 0.1 || velocitydist > 2) {
    prevvelocitydist = velocitydist;
    if (firstdist) {
      traveleddist = 0;
    }
    else {
      traveleddist = (leftdrive.position(degrees) + rightdrive.position(degrees)) / 2 * pi * wheeldiameter / 360 * 0.87192816635;
    }
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print(traveleddist);
    errordist = dist - traveleddist;
    proportionaldist = (errordist * kpdist);
    derivativedist = (errordist - preverrordist) * kddist;
    integraldist = (errordist + preverrordist) * kitheta;
    if (fabs(integraldist) > 12) {
      integraldist = 12;
    }
    if (fabs(integraldist) < 1) {
      integraldist = 0;
    }
    velocitydist = proportionaldist + derivativedist + integraldist;
    if ((velocitydist - prevvelocitydist) > slewratedist) {
      velocitydist = prevvelocitydist + slewratedist;
    }
    else {
      if ((velocitydist - prevvelocitydist) < (-1 * slewratedist)) {
        velocitydist = prevvelocitydist + slewratedist;
      }
    }
    leftdrive.spin(forward, velocitydist, volt);
    rightdrive.spin(forward, velocitydist, volt);
    preverrordist = errordist;
    firstdist = false;
    wait(5, msec);
    Brain.Screen.print(" time: %f", Brain.Timer.time(seconds));
  }
  leftdrive.stop(brake);
  rightdrive.stop(brake);
}

void PID_theta(double theta, double kptheta, double kdtheta, double kitheta) {
  bool firsttheta = true;
  double errortheta = 100;
  double proportionaltheta = 0; 
  double derivativetheta = 0;
  double integraltheta = 0;
  double traveledtheta = 0;
  double preverrortheta = 0;
  double velocitytheta = 0;
  Inertial13.setRotation(0, degrees);
  Inertial13.setHeading(0, degrees);
  // Inertial13.setRotation(0, degrees);
  leftdrive.setPosition(0, degrees);
  rightdrive.setPosition(0, degrees);
  rightdrive.spin(forward);
  leftdrive.spin(forward);

  while (fabs(errortheta) > 0.5) {
    if (firsttheta) {
	    traveledtheta = 0;
	  }
	  else {
      if (traveledtheta > 0) {
	      traveledtheta = Inertial13.rotation(degrees);
      }
      else {
        if (traveledtheta == 0) {
          if (theta < 0) {
	          traveledtheta = Inertial13.rotation(degrees);
          }
          else {
	          traveledtheta = Inertial13.rotation(degrees);
          }
        }
	      traveledtheta = Inertial13.rotation(degrees);
      }
	  }
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("rotation: %f", traveledtheta);
    // Controller1.Screen.print("error: %f", errortheta);
    errortheta = theta - traveledtheta;
    proportionaltheta = (errortheta * kptheta);
    derivativetheta = (errortheta - preverrortheta) * kdtheta;
    integraltheta = (errortheta + preverrortheta) * kitheta;
    if (integraltheta > 12) {
      integraltheta = 12;
    }
    if (integraltheta < 12) {
      integraltheta = -12;
    }
    if (fabs(integraltheta) < 1) {
      integraltheta = 0;
    }
    if (theta > 0) {
      if ((proportionaltheta + derivativetheta) < 0) {
        if (fabs(derivativetheta) > 1) {
          derivativetheta = -1 * proportionaltheta;
        }
      }
    }
    else {
      if ((proportionaltheta + derivativetheta) > 0) {
        if (fabs(derivativetheta) > 1) {
          derivativetheta = -1 * proportionaltheta;
        }
      }
    }

    if (fabs(errortheta) < fabs(0.05 * theta)) {
      integraltheta = 0;
    }
    else {
      if (fabs(derivativetheta + proportionaltheta) < 1) {
        integraltheta = 0;
      }
    }
    velocitytheta = proportionaltheta + derivativetheta + integraltheta;
    leftdrive.spin(forward, (velocitytheta), volt);
    rightdrive.spin(forward, -1 * (velocitytheta), volt);
    preverrortheta = errortheta;
    firsttheta = false;
    wait(2, msec);
    // Controller1.Screen.print(" time: %f", Brain.Timer.time(seconds));
  }
  leftdrive.spin(forward, 0, volt);
  rightdrive.spin(forward, 0, volt);
  leftdrive.stop(coast);
  rightdrive.stop(coast);
}

void odometry() {
  double leftwheeldist;
  double backwheeldist;
  double traveledodomtheta;
  double traveledreverseodomtheta;
  double lefthypo;
  double backhypo;
  double thetanot = 90;
  double prevx = 0;
  double prevy = 0;
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
  leftdrive.setPosition(0, degrees);
  rightdrive.setPosition(0, degrees);
  leftwheelrotation.setPosition(0, degrees);
  backwheelrotation.setPosition(0, degrees);
  // Start calibration.
  Inertial13.calibrate();
  // Print that the Inertial Sensor is calibrating while
  // waiting for it to finish calibrating.
  while(Inertial13.isCalibrating()){
      Controller1.Screen.clearScreen();
      Controller1.Screen.print("Inertial Calibrating");
      wait(50, msec);
  }

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
  while(Inertial13.isCalibrating()){
      wait(50, msec);
  }
  
  // PID_dist(distance(inches), kp, kd, ki, slewrate);
  // PID_dist(25, 6, 2.1, 0.05, 32);
  // wait(0.5, seconds);
  // PID_theta(robot-centric rotation(degrees), kp, kd, ki);
  PID_theta(-90, 0.17, 1.55, 0.08);
  // wait(0.5, seconds);
  // PID_dist(26, 6, 2.1, 0.05, 32);
  // wait(0.5, seconds);
  // PID_theta(-180, 0.22, 1.35, 0.2);
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
  // Start calibration with the calibration time set to 3 seconds.
  // Inertial13.calibrate(3);
  Inertial13.setRotation(0, degrees);
  Inertial13.setHeading(0, degrees);
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

    // Controller1.Screen.clearScreen();
    // Controller1.Screen.setCursor(1, 1);
    // Controller1.Screen.print("rotation: %f", Inertial13.rotation(degrees));
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
