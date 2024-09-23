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
motor intakeMotorA = motor(PORT4, ratio6_1, true);
motor intakeMotorB = motor(PORT1, ratio6_1, false);
motor_group intake = motor_group(intakeMotorA, intakeMotorB);

motor leftdriveMotorA = motor(PORT15, ratio6_1, false);
motor leftdriveMotorB = motor(PORT12, ratio6_1, false);
motor leftdriveMotorC = motor(PORT13, ratio6_1, false);
motor_group leftdrive = motor_group(leftdriveMotorA, leftdriveMotorB, leftdriveMotorC);

motor rightdriveMotorA = motor(PORT14, ratio6_1, true);
motor rightdriveMotorB = motor(PORT16, ratio6_1, true);
motor rightdriveMotorC = motor(PORT19, ratio6_1, true);
motor_group rightdrive = motor_group(rightdriveMotorA, rightdriveMotorB, rightdriveMotorC);

// AI Vision Color Descriptions
// AI Vision Code Descriptions
vex::aivision AIVisionFront(PORT8, aivision::ALL_AIOBJS);
vex::aivision AIVisionBack(PORT9, aivision::ALL_AIOBJS);

inertial Inertial13 = inertial(PORT18);

rotation leftwheelrotation = rotation(PORT4, false);

rotation backwheelrotation = rotation(PORT2, false);

optical opticalSensor = optical(PORT3);

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

int Brain_precision = 0, Console_precision = 0, Controller1_precision = 0, AIVisionBack_objectIndex = 0;

const double pi = 3.14159265359;
const double wheeldiameter = 3.25;
const double odomwheeldiameter = 2;
const double leftwheeldisplacement = 4;
const double backwheeldisplacement = 4;
const double rpmdrive = 450;
double kpdist;
double kddist;
double kidist;
double timeouttheta;
double slewratedist;
double kptheta;
double kdtheta;
double kitheta;
double theta;
double dist;
double x;
double y;
bool tracking;


void PID_dist(double dist, double kpdist, double kddist, double kidist) {
  bool firstdist = true;
  double errordist = 100;
  double proportionaldist = 0; 
  double derivativedist = 0;
  double integraldist = 0;
  double traveldist = 0;
  double preverrordist = 0;
  double velocitydist = 0;

  // Assuming the wheel diameter is in inches
  // double wheelcircumference = pi * wheeldiameter; // Circumference in inches

  // Calculate the time required to turn the specified angle

  // double timeToMove = (fabs(dist)) * (wheelcircumference / (rpmdrive * 0.5 * 60.0)); // Time in seconds
  leftdrive.setPosition(0, degrees);
  rightdrive.setPosition(0, degrees);
  rightdrive.spin(forward);
  leftdrive.spin(forward);

  // Start timer
  double starttime = Brain.Timer.time(seconds);

  while (fabs(errordist) > 0.05) {
      // Timeout check
      // if (Brain.Timer.time(seconds) - starttime > timeToMove) {
          // break; // Exit the loop if the timeout is reached
      // }

    if (firstdist) {
      traveldist = 0;
    }
    else {
      traveldist = (leftdrive.position(degrees) + rightdrive.position(degrees)) / 2 * pi * wheeldiameter / 360 * 0.87192816635;
    }
      
      Controller1.Screen.clearScreen();
      // controller1.screen.setCursor(1, 1);
      // controller1.screen.print("rotation: %f", traveldist);
      // controller1.screen.print("error: %f", errordist);
      errordist = dist - traveldist;
      proportionaldist = (errordist * kpdist);
      derivativedist = (errordist - preverrordist) * kddist;
      integraldist = (errordist + preverrordist) * kidist;
      if (integraldist > 3) {
          integraldist = 3;
      }
      if (integraldist < -3) {
          integraldist = -3;
      }
      
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print("integral: %f", integraldist);
      Controller1.Screen.setCursor(2, 1);
      Controller1.Screen.print("proportional: %f", proportionaldist);
      Controller1.Screen.setCursor(3, 1);
      Controller1.Screen.print("derivative: %f", derivativedist);
      velocitydist = proportionaldist + derivativedist + integraldist;
      leftdrive.spin(forward, (velocitydist), volt);
      rightdrive.spin(forward, (velocitydist), volt);
      preverrordist = errordist;
      firstdist = false;
      wait(2, msec);
      // controller1.screen.print(" time: %f", brain.timer.time(seconds));

  }
  leftdrive.setVelocity(0, percent);
  rightdrive.setVelocity(0, percent);
  leftdrive.stop(brake);
  rightdrive.stop(brake);
}
void PID_theta(double theta, double kptheta, double kdtheta, double kitheta, double timeouttheta) {
  bool firsttheta = true;
  double errortheta = 100;
  double proportionaltheta = 0; 
  double derivativetheta = 0;
  double integraltheta = 0;
  double traveledtheta = 0;
  double preverrortheta = 0;
  double velocitytheta = 0;
  double starttime = Brain.Timer.time(seconds);
  // Inertial13.setRotation(0, degrees);
  // Inertial13.setHeading(0, degrees);
  // Inertial13.setRotation(0, degrees);
  leftdrive.setPosition(0, degrees);
  rightdrive.setPosition(0, degrees);
  rightdrive.spin(forward);
  leftdrive.spin(forward);

  while (fabs(errortheta) > 1) {
    if (Brain.Timer.time(seconds) - starttime > timeouttheta) {
      break; // Exit the loop if the timeout is reached
    }
    if (firsttheta) {
	    traveledtheta = 0;
	  }
	  else {
	    traveledtheta = Inertial13.rotation(degrees);
	  }
    Controller1.Screen.clearScreen();
    // Controller1.Screen.setCursor(1, 1);
    // Controller1.Screen.print("rotation: %f", traveledtheta);
    // Controller1.Screen.print("error: %f", errortheta);
    errortheta = theta - traveledtheta;
    proportionaltheta = (errortheta * kptheta);
    derivativetheta = (errortheta - preverrortheta) * kdtheta;
    integraltheta = (errortheta + preverrortheta) * kitheta;
    if (integraltheta > 3) {
        integraltheta = 3;
    }
    if (integraltheta < -3) {
        integraltheta = -3;
    }
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("theta: %f", traveledtheta);
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.print("proportional: %f", proportionaltheta);
    // Controller1.Screen.setCursor(3, 1);
    // Controller1.Screen.print("derivative: %f", derivativetheta);
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
  intake.stop(hold);
  // PID_theta(-45, 1.7, 1.55, 0.1);

  PID_theta(45, 0.12, 0.01, 0.01, 3); 

  leftdrive.resetPosition();
  rightdrive.resetPosition();
  rightdrive.setVelocity(60, percent);
  leftdrive.setVelocity(60, percent);
  leftdrive.spinFor(reverse, 600, vex::rotationUnits::deg, false);
  rightdrive.spinFor(reverse, 600, vex::rotationUnits::deg, true);
  rightdrive.setVelocity(0, percent);
  leftdrive.setVelocity(0, percent);
  PID_theta(-45, 0.12, 0.01, 0.01, 3); 
  rightdrive.setVelocity(60, percent);
  leftdrive.setVelocity(60, percent);
  leftdrive.spinFor(forward, 700, vex::rotationUnits::deg, false);
  rightdrive.spinFor(forward, 700, vex::rotationUnits::deg, true);
  rightdrive.setVelocity(0, percent);
  leftdrive.setVelocity(0, percent);
  // PID_dist(3, 1.1, 0.0, 0.00);

  // PID_dist(-3, 0.7, 0.15, 0); 
  // PID_theta(0, 0.05, 0.15, 0.05);
  // PID_dist(6, 0.7, 0.15, 0);
  // intake.spinFor(forward, 10, vex::rotationUnits::deg, true);
  // intake.spinFor(reverse, 360, vex::rotationUnits::deg, false);
  // PID_dist(-1, 0.7, 0.15, 0); 
  // PID_dist(1, 0.7, 0.15, 0);
  // PID_dist(-6, 0.7, 0.15, 0); 


  // PID_dist(distance(inches), kp, kd, ki, slewrate);
  // PID_dist(25, 6, 2.1, 0.05, 32);
  // wait(0.5, seconds);
  // PID_theta(robot-centric rotation(degrees), kp, kd, ki);

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
  double hue;
  bool team = true; // blue team = true, red team= false
  bool ring = false; // detects if wrong ring is detected
  double intakepos;
  Inertial13.setRotation(0, degrees);
  Inertial13.setHeading(0, degrees);
  rightdrive.spin(forward);
  leftdrive.spin(forward);
  rightdrive.setVelocity(0, percent);
  leftdrive.setVelocity(0, percent);
  intake.setPosition(0, degrees);
  opticalSensor.setLightPower(100, percent);
  constexpr int32_t DEADBAND = 5;

  while (1) {
    int32_t rightvelocity = Controller1.Axis2.position();
    int32_t leftvelocity = Controller1.Axis3.position();

    // Apply deadband
    if (abs(leftvelocity) < DEADBAND) leftvelocity = 0;
    if (abs(rightvelocity) < DEADBAND) rightvelocity = 0;

    // Apply exponential scaling
    leftvelocity = (leftvelocity != 0) ? (leftvelocity > 0 ? pow(leftvelocity, 2) : -pow(-leftvelocity, 2)) : 0;
    rightvelocity = (rightvelocity != 0) ? (rightvelocity > 0 ? pow(rightvelocity, 2) : -pow(-rightvelocity, 2)) : 0;

    // Scale the velocity to a percentage range (0-100)
    leftvelocity = fmin(fmax(leftvelocity / 127.0 * 100, -100.0), 100.0);
    rightvelocity = fmin(fmax(rightvelocity / 127.0 * 100, -100.0), 100.0);

    rightdrive.setVelocity(rightvelocity, percent);
    leftdrive.setVelocity(leftvelocity, percent);

    // Calculate intake velocity based on drivetrain speed
    int32_t intakeVelocity;
    if (!ring) {
      if (rightvelocity == 0 && leftvelocity == 0) {
        // Robot is stationary
        intakeVelocity = 30; // Start at 30%
      } else {
        // Scale intake based on the maximum drivetrain speed
        int32_t maxDrivetrainSpeed = (abs(rightvelocity) + abs(leftvelocity)) / 2;
        intakeVelocity = 30 + (maxDrivetrainSpeed * (100 - 30) / 100); // Scale to 100%
      }
    }
    else {
      if (22 <= intake.position(degrees)) {
        intake.stop(brake);
        ring = false;
      }


    }

    hue = opticalSensor.hue();

    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print(hue);
    if (team) {
      if (hue < 20) {
        ring = true;
        intake.resetPosition();
      }
      else {
        if (!ring) {
        ring = false;        
        }
      }
    }
    else {
      if (hue < 200) {
        ring = true;
      }
      else {
        ring = false;
      }
    }

    // Set the intake velocity
    if (Controller1.ButtonR1.pressing()) {
      intake.spin(forward, intakeVelocity, percent);
    }
    if (intake.velocity(percent) > 30) {
      intake.spin(forward, intakeVelocity, percent);
    }

    if (Controller1.ButtonR2.pressing()) {
      intake.stop();
    }
    if (Controller1.ButtonL1.pressing()) {
      goalclamp.set(true);
    }
    if (Controller1.ButtonL2.pressing()) {
      goalclamp.set(false);
    }

    AIVisionBack.takeSnapshot(aivision::ALL_AIOBJS);
    if (AIVisionBack.objectCount > 0 && AIVisionBack.objects[AIVisionBack_objectIndex].width > 70.0) {
      if (AIVisionBack.objects[AIVisionBack_objectIndex].id == mobileGoal) {
        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("Mobile Goal");
        goalclamp.set(true);
      }
      if (AIVisionBack.objects[AIVisionBack_objectIndex].id == redRing) {
        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("Redring");
      }
      if (AIVisionBack.objects[AIVisionBack_objectIndex].id == blueRing) {
        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("Bluering");
      }
    }

    wait(10, msec); // Sleep the task for a short amount of time to prevent wasted resources.
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
