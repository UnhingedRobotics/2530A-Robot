#include "drive-functions.h"
#include "vex.h"
#include <sstream> // For std::ostringstream

using namespace vex;
double intakeInitPos;

// Constructor with initializer list to initialize member variables
IntakeControl::IntakeControl() :
  hue(0.0),
  ring(false),
  team(true),
  ringdetected(false),
  intakeon(false),
  holding(false),
  intakeVelocity(0),
  intakeMaxVelocity(80),
  mode(INTAKE_COLOR_SORT),
  accuracyIntake(125),
  intakeFullRotation(2486.4),
  intakeFirstHook(1054.8),
  intakeSecondHook(2349.6),
  intakeHoldingPos(700)
{}

// Converts a numeric value to a string
std::string toString(double value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// Member function of IntakeControl
void IntakeControl::updateControllerScreen(const char* message) {
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print(message);
}


void IntakeControl::colorSorting() {
    if (!intakeon) {
      intakeVelocity = 0;
    }
    else {
      intakeVelocity = intakeMaxVelocity;
      if (!ringdetected) {
          aivisionsensor.takeSnapshot(aivisionsensor__bluering);
          if (aivisionsensor.objects[0].exists) {
              ring = false;
              ringdetected = true;
              intakeInitPos = fmod(intake.position(degrees), intakeFullRotation);
              updateControllerScreen("blue");
          } else {
              aivisionsensor.takeSnapshot(aivisionsensor__redring);
              if (aivisionsensor.objects[0].exists) {
                ring = true;
                ringdetected = true;
                intakeInitPos = fmod(intake.position(degrees), intakeFullRotation);
                updateControllerScreen("red");
              }
          }
      }
	  if (holding) {
        if (fmod(intake.position(degrees), intakeFullRotation) >= intakeHoldingPos - accuracyIntake) {
          if (ringdetected) {
            if ((team && !ring) || (!team && ring)) {
    		  intake.setVelocity(-intakeMaxVelocity, percent);
              updateControllerScreen("ring removed");
              ringdetected = false;
            }
			else {
    		  intake.setVelocity(0, percent);
			  intakeon = false;
              updateControllerScreen("ring held");
              ringdetected = false;
			}
          }
        }     
	  }
	  else {
        if (intakeInitPos >= intakeFirstHook) {
          if (fmod(intake.position(degrees), intakeFullRotation) >= intakeSecondHook - accuracyIntake || fmod(intake.position(degrees), intakeFullRotation) <= intakeFirstHook - accuracyIntake) {
            if (ringdetected) {
              if ((team && !ring) || (!team && ring)) {
    			intake.setVelocity(-intakeMaxVelocity, percent);
                updateControllerScreen("ring removed");
                ringdetected = false;
              }
            }
          }     
        }
        else {
          if (fmod(intake.position(degrees), intakeFullRotation) >= intakeFirstHook - accuracyIntake) {
            if (ringdetected) {
              if ((team && !ring) || (!team && ring)) {
    			intake.setVelocity(-intakeMaxVelocity, percent);
                updateControllerScreen("ring removed");
                ringdetected = false;
              }
            }
          }     
        }
	  }


    }
    intake.spin(forward);
    intake.setVelocity(intakeVelocity, percent);
}

void IntakeControl::intakeMove() {
    if (!intakeon) {
      intakeVelocity = 0;
    }
    else {
      intakeVelocity = intakeMaxVelocity;
    }
    intake.spin(forward);
    intake.setVelocity(intakeVelocity, percent);
}

void healthCheck() {
  while (true) {
    if (intake.temperature(fahrenheit) >= 100) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,1);
      Controller1.Screen.print("Intake too hot!");
      Controller1.rumble("..--");
    }

    if (leftdrivefront.temperature(fahrenheit) >= 100) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,2);
      Controller1.Screen.print("LeftDriveFront too hot!");
      Controller1.rumble("..--");
    }
    
    if (leftdriveback.temperature(fahrenheit) >= 100) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,3);
      Controller1.Screen.print("LeftDriveBack too hot!");
      Controller1.rumble("..--");
    }

    if (rightdrivefront.temperature(fahrenheit) >= 100) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,4);
      Controller1.Screen.print("RightDriveFront too hot!");
      Controller1.rumble("..--");
    }

    if (rightdriveback.temperature(fahrenheit) >= 100) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,1);
      Controller1.Screen.print("RightDriveBack too hot!");
      Controller1.rumble("..--");
    }
    wait(1, seconds);
  }
}

void FishControl::set_arm_exit_conditions(float arm_settle_error, float arm_settle_time, float arm_timeout){
  this->arm_settle_error = arm_settle_error;
  this->arm_settle_time = arm_settle_time;
  this->arm_timeout = arm_timeout;
}

void FishControl::set_arm_constants(float arm_max_voltage, float arm_kp, float arm_ki, float arm_kd, float arm_starti) {
  this->arm_max_voltage = arm_max_voltage;
  this->arm_kp = arm_kp;
  this->arm_ki = arm_ki;
  this->arm_kd = arm_kd;
  this->arm_starti = arm_starti;
}

void FishControl::move_to_angle(float angle) {
  move_to_angle(angle, arm_max_voltage, arm_settle_error, arm_settle_time, arm_timeout, arm_kp, arm_ki, arm_kd, arm_starti);
}

void FishControl::move_to_angle(float angle, float arm_max_voltage) {
  move_to_angle(angle, arm_max_voltage, arm_settle_error, arm_settle_time, arm_timeout, arm_kp, arm_ki, arm_kd, arm_starti);
}

void FishControl::move_to_angle(float angle, float arm_max_voltage, float arm_settle_error, float arm_settle_time, float arm_timeout) {
  move_to_angle(angle, arm_max_voltage, arm_settle_error, arm_settle_time, arm_timeout, arm_kp, arm_ki, arm_kd, arm_starti);
}

void FishControl::move_to_angle(float angle, float arm_max_voltage, float arm_settle_error, float arm_settle_time, float arm_timeout, float arm_kp, float arm_ki, float arm_kd, float arm_starti) {
  PID fishPID((angle - fishMech.position(degrees)), arm_kp, arm_ki, arm_kd, arm_starti, arm_settle_error, arm_settle_time, arm_timeout);
  while (!pid_stop) {   
    float error = angle - fishMech.position(degrees);
    float output = fishPID.compute(error);
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print(angle - fishMech.position(degrees));
    output = clamp(output, -arm_max_voltage, arm_max_voltage);
    fishMech.setVelocity(output, percent);
    task::sleep(10);
  }
  pid_stop = false;
  fishMech.stop(brake);
  fishMech.spin(forward);
  fishMech.setVelocity(0, percent);
}
