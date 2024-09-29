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
motor leftdriveMotorB = motor(PORT2, ratio6_1, false);
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

rotation backwheelrotation = rotation(PORT12, false);

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
const double rmpintake = 450;

double averagevoltagedist;
double dist;
double kpdist;
double kddist;
double kidist;

void PID_dist(double dist, double kpdist, double kddist, double kidist, double averagevoltagedist) {
	bool firstdist = true;
	double errordist = 100;
	double proportionaldist = 0; 
	double derivativedist = 0;
	double integraldist = 0;
	double traveldist = 0;
	double preverrordist = 0;
	double velocitydist = 0;
	double starttimedist = Brain.Timer.time(seconds);
	double timeoutdist = fabs(dist / ((averagevoltagedist / 12) * rpmdrive * pi * wheeldiameter / 60)); 
	leftdrive.resetPosition();
	rightdrive.resetPosition();
	rightdrive.spin(forward);
	leftdrive.spin(forward);

	while (fabs(errordist) > 0.01) {

		if (Brain.Timer.time(seconds) - starttimedist > timeoutdist) {
			break;
		}
		if (firstdist) {
			errordist = 0;
		}
		else {
			traveldist = (leftdrive.position(degrees) + rightdrive.position(degrees)) / 2 * pi * wheeldiameter / 360 * 0.87192816635;
		}

		errordist = dist - traveldist;
		proportionaldist = (errordist * kpdist);
		derivativedist = (errordist - preverrordist) * kddist;
		integraldist += errordist;

		if (fabs(integraldist) < 0.5) {
			integraldist = 0;
		}
		if (integraldist > 3) {
			integraldist = 3;
		}
		if (integraldist < -3) {
			integraldist = -3;
		}
		velocitydist = proportionaldist + derivativedist + integraldist * kidist;
		leftdrive.spin(forward, velocitydist, volt);
		rightdrive.spin(forward, velocitydist, volt);
		preverrordist = errordist;
		firstdist = false;
		wait(2, msec);
	}
	leftdrive.spin(forward, 0, volt);
	rightdrive.spin(forward, 0, volt);
	leftdrive.stop(brake);
	rightdrive.stop(brake);
	errordist = 0;
}

double averagevoltagetheta;
double kptheta;
double kdtheta;
double kitheta;
double theta;

void PID_theta(double theta, double kptheta, double kdtheta, double kitheta, double averagevoltagetheta) {
	bool firsttheta = true;
	double errortheta = 100;
	double proportionaltheta = 0; 
	double derivativetheta = 0;
	double integraltheta = 0;
	double traveledtheta = 0;
	double preverrortheta = 0;
	double velocitytheta = 0;
	double starttimetheta = Brain.Timer.time(seconds);
	double timeouttheta = fabs(theta * (pi / 180) / averagevoltagetheta / 12 * rpmdrive * 2 * pi / 60); 
	leftdrive.setPosition(0, degrees);
	rightdrive.setPosition(0, degrees);
	rightdrive.spin(forward);
	leftdrive.spin(forward);

	while (fabs(errortheta) > 0.05) {
		if (Brain.Timer.time(seconds) - starttimetheta > timeouttheta) {
			break;
		}
		if (firsttheta) {
			errortheta = 0;
		}
		else {
			traveledtheta = Inertial13.rotation(degrees);
		}
		errortheta = theta - traveledtheta;
		proportionaltheta = (errortheta * kptheta);
		derivativetheta = (errortheta - preverrortheta) * kdtheta;
		integraltheta += errortheta;
		if (fabs(integraltheta) < 0.5) {
			integraltheta = 0;
		}
		if (integraltheta > 3) {
			integraltheta = 3;
		}
		if (integraltheta < -3) {
			integraltheta = -3;
		}
		velocitytheta = proportionaltheta + derivativetheta + integraltheta * kitheta;
		leftdrive.spin(forward, (velocitytheta), volt);
		rightdrive.spin(forward, -1 * (velocitytheta), volt);
		preverrortheta = errortheta;
		firsttheta = false;
		wait(2, msec);
	}
	leftdrive.spin(forward, 0, volt);
	rightdrive.spin(forward, 0, volt);
	leftdrive.stop(brake);
	rightdrive.stop(brake);
	errortheta = 0;
}

double x;
double y;
bool tracking;
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

double hue;
bool intakeon;
int32_t intakeVelocity;
const bool team = false; // blue team = true, red team = false
const int32_t jamtimeout = 3;
double starttimeintake = 0;
double starttimecolor = 0;
double currentposintake;
double previousposintake;
bool ring = false; // detects if wrong ring is detected
void intakeauton() {
	opticalSensor.setLightPower(100, percent);
	while (intakeon) {
		currentposintake = intake.position(degrees);
		if (fabs(fabs(currentposintake) - fabs(previousposintake)) < 5) {
			if (starttimeintake == 0) {
				starttimeintake = Brain.Timer.time(seconds);
			}
			if (Brain.Timer.time(seconds) - starttimeintake >= jamtimeout) {
				intake.stop(coast);
			  	intake.spin(forward, 0, percent);
				wait(10, msec);
				intake.spinFor(reverse, 80, vex::rotationUnits::deg, true);
				// wait(100, msec);
			}
		}
		else {
			starttimeintake = 0;
		}
		hue = opticalSensor.hue();
		if (!ring) {

			if (team) {
				if (hue < 20) {
					ring = true;
					starttimecolor = Brain.Timer.time(seconds);
				}
				else {
					ring = false;        
				}
			}
			else {
				if (hue > 170) {
					ring = true;
					starttimecolor = Brain.Timer.time(seconds);
				}
				else {
					ring = false;
				}
			}
			intakeVelocity = 55; // Start at 30%

		}
		else {
			intake.stop(brake);
			wait(1.2, seconds);
			intake.spin(forward, 100, percent);
			ring = false;
		}
		Controller1.Screen.clearScreen();
		Controller1.Screen.print(hue);
		intake.spin(forward, intakeVelocity, percent);
		previousposintake = currentposintake;
		wait(2, msec);
	}
  intake.spin(forward, 0, percent);
	intakeVelocity = 0;
	hue = 0;
	ring = false;
	currentposintake = 0;
	previousposintake = 0;
	starttimeintake = 0;
	opticalSensor.setLightPower(0, percent);
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
	Inertial13.calibrate();
	intake.stop(hold);
	leftdrive.resetPosition();
	rightdrive.resetPosition();
	// Print that the Inertial Sensor is calibrating while
	// waiting for it to finish calibrating.
	while(Inertial13.isCalibrating()){
		Controller1.Screen.clearScreen();
		Controller1.Screen.print("Inertial Calibrating");
		wait(50, msec);
	}
	opticalSensor.setLightPower(0, percent);
	Inertial13.setRotation(0, degrees);
	Inertial13.setHeading(0, degrees);
	hue = 0;
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
	leftdrive.resetPosition();
	rightdrive.resetPosition();
	intakeon = false;
	// intakeon = false;
	// intakeon = true;
	// vex::thread intakeTast(intakeauton);
  goalclamp.set(false);
	PID_dist(-38, 0.5, 0.09, 0, 5);
  wait(1, seconds);
  goalclamp.set(true);
  wait(0.2, seconds);
	PID_theta(-30, 0.15, 0.11, 0.04, 5);
  wait(0.2, seconds);
	PID_dist(52, 0.37, 0.09, 0, 5);
  wait(0.2, seconds);
  // leftdrive.spinFor(forward, 80, vex::rotationUnits::deg, false);
	PID_theta(20, 0.7, 0.11, 0.04, 5);
	PID_dist(14, 1.1, 0.09, 0, 5);
	intake.spinFor(reverse, 80, vex::rotationUnits::deg, true);
  wait(0.2, seconds);
	PID_dist(-15, 1, 0.11, 0, 4);
	intake.spinFor(forward, 600, vex::rotationUnits::deg, false);
	intake.stop(coast);
  wait(0.2, seconds);
  PID_theta(-30, 0.7, 0.11, 0.04, 5);
  wait(0.2, seconds);
	PID_dist(-8, 0.6, 0.11, 0, 5);
  wait(0.2, seconds);
	PID_theta(-100, 0.5, 0.11, 0.04, 5);
  wait(0.2, seconds);
	intakeon = true;
	vex::thread intakeTast(intakeauton);
  PID_dist(26, 0.5, 0.11, 0, 5);
  wait(0.2, seconds);
  PID_theta(-200, 0.3, 0.11, 0.04, 5);
  wait(0.2, seconds);
	PID_dist(20, 0.5, 0.09, 0, 5);
  intakeon = false;
	// intakeon = false;
	// PID_theta(robot-centric rotation(degrees), kp, kd, ki, average voltage(0-12));
	// PID_dist(robot-centric distance(inches), kp, kd, ki, average voltage(0-12));
	// intake.spinFor(forward, 10, vex::rotationUnits::deg, true);
	// intake.spinFor(reverse, 360, vex::rotationUnits::deg, false);
	// PID_dist(distance(inches), kp, kd, ki, slewrate);
	// wait(0.5, seconds);

	// wait(0.5, seconds);
	// PID_dist(26, 6, 2.1, 0.05, 32);
	// wait(0.5, seconds);
	// PID_theta(-180, 0.22, 1.35, 0.2);
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
    bool intakeready = false;
    starttimecolor = 0;
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

        currentposintake = intake.position(degrees);
        if (intakeVelocity >= 30) {
            if (fabs(fabs(currentposintake) - fabs(previousposintake)) < 5) {
                if (starttimeintake == 0) {
                    starttimeintake = Brain.Timer.time(seconds);
                }
                if (Brain.Timer.time(seconds) - starttimeintake >= jamtimeout) {
                    intake.stop(coast);
                    intake.spinFor(reverse, 30, vex::rotationUnits::deg, true);
                }
            }
            else {
                starttimeintake = 0;
            }
        }

        // Calculate intake velocity based on drivetrain speed
        if (intakeready) {
            if (!ring) {
				if (starttimecolor == 0) {
					if (team) {
						if (hue < 20) {
							ring = true;
							starttimecolor = Brain.Timer.time(seconds);
						}
						else {
							ring = false;        
						}
					}
					else {
						if (hue > 170) {
							ring = true;
							starttimecolor = Brain.Timer.time(seconds);
						}
						else {
							ring = false;
						}
					}
					if (rightvelocity == 0 && leftvelocity == 0) {
						// Robot is stationary
						intakeVelocity = 55; // Start at 30%
					} else {
						// Scale intake based on the maximum drivetrain speed
						int32_t maxDrivetrainSpeed = (abs(rightvelocity) + abs(leftvelocity)) / 2;
						intakeVelocity = 30 + (maxDrivetrainSpeed * (100 - 30) / 100); // Scale to 100%
					}
				}
            }
            else {
                // Color detected sequence (non-blocking control)
                if (starttimecolor == 0) {
                    starttimecolor = Brain.Timer.time(seconds);
                }

                // Stop intake immediately after detecting the color
                if (Brain.Timer.time(seconds) - starttimecolor <= 1) {
                    intake.stop(brake);
                }
                // Run motor at 60% for 2 seconds
                else if (Brain.Timer.ti	me(seconds) - starttimecolor <= 2) {
                    intake.spin(forward, 20, percent);
                }
                // Stop the motor again after 2 seconds
                else if (Brain.Timer.time(seconds) - starttimecolor <= 6) {
                    intake.stop(brake);
                }
                // Reset the sequence after 5 seconds
                else if (Brain.Timer.time(seconds) - starttimecolor >= 9) {
                    intake.stop(brake);
                    ring = false;
                    starttimecolor = 0;
                }
            }
            // Normal intake control if no color is detected
            if (starttimecolor == 0) {
                intake.spin(forward, intakeVelocity, percent);
            }
        }
        else {
            intake.spin(forward, 0, percent);
        }

        hue = opticalSensor.hue();

        // Set the intake velocity
        if (Controller1.ButtonR1.pressing()) {
            intakeready = true;
        }

        if (Controller1.ButtonR2.pressing()) {
            intakeready = false;
        }
        if (Controller1.ButtonL1.pressing()) {
            goalclamp.set(true);
        }
        if (Controller1.ButtonL2.pressing()) {
            goalclamp.set(false);
        }
        if (Controller1.ButtonA.pressing()) {
            ring = false;
			starttimecolor = 0;
        }
        if (Controller1.ButtonB.pressing()) {
			intake.spin(reverse, 50, percent);
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

        previousposintake = currentposintake;

        // Sleep the task for a short amount of time to prevent wasted resources.
        wait(5, msec);
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
	// pre_auton();
	
	intakeon = true;
	vex::thread intakeTast(intakeauton);
	// Prevent main from exiting with an infinite loop.
	while (true) {
		wait(100, msec);
	}
}
