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
  intakeReverse(false),
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
    if (!intakeReverse) {
      intake.setVelocity(intakeVelocity, percent);
    }
    else {
      intake.setVelocity(-intakeVelocity, percent);
    }
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
    if (intake.temperature(fahrenheit) >= 110) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,1);
      Controller1.Screen.print("Intake");
    }

    if (leftdrivefront.temperature(fahrenheit) >= 100) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(2,1);
      Controller1.Screen.print("LeftDriveFront");
    }
    
    if (leftdriveback.temperature(fahrenheit) >= 100) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(3,1);
      Controller1.Screen.print("LeftDriveBack");
    }

    if (rightdrivefront.temperature(fahrenheit) >= 100) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(4,1);
      Controller1.Screen.print("RightDriveFront");
    }

    if (rightdriveback.temperature(fahrenheit) >= 100) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,1);
      Controller1.Screen.print("RightDriveBack");
    }
    wait(5, seconds);
  }
}
