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
  intakeSecondHook(2349.6)
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

void IntakeControl::resetIntake() {
    intake.setVelocity(-intakeMaxVelocity, percent);

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
      // Check if position is a multiple of 1300
      if (intakeInitPos >= intakeFirstHook) {
        if (fmod(intake.position(degrees), intakeFullRotation) >= intakeSecondHook - accuracyIntake || fmod(intake.position(degrees), intakeFullRotation) <= intakeFirstHook - accuracyIntake) {
          if (ringdetected) {
            if ((team && !ring) || (!team && ring)) {
              resetIntake();
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
              resetIntake();
              updateControllerScreen("ring removed");
              ringdetected = false;
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

void IntakeControl::update() {
  switch (mode) {
    case INTAKE_COLOR_SORT:
      if (auto_on) {
        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print("auto on");  
        time_waited = time_waited + 1;
        if (time_waited >= 10) {
          intakeon = true;
          time_waited = 0;
          Controller1.Screen.clearScreen();
          Controller1.Screen.setCursor(1,1);
          Controller1.Screen.print("time");  
        }   
      }
      colorSorting();
      break;
    case WALLSTAKE_HOLDING:
      if (distancesensor.objectDistance(inches) < 2) {
        intake.stop(brake);
		    intake.spin(forward);
		    intake.setVelocity(0, percent);
      }
      break;
  }
}

void healthCheck() {
  while (true) {
    if (intake.temperature(fahrenheit) >= 105) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,1);
      Controller1.Screen.print("Intake too hot!");
      Controller1.rumble("..--");
    }

    if (leftdrivefront.temperature(fahrenheit) >= 105) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,2);
      Controller1.Screen.print("LeftDriveFront too hot!");
      Controller1.rumble("..--");
    }
    
    if (leftdriveback.temperature(fahrenheit) >= 105) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,3);
      Controller1.Screen.print("LeftDriveBack too hot!");
      Controller1.rumble("..--");
    }

    if (rightdrivefront.temperature(fahrenheit) >= 105) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,4);
      Controller1.Screen.print("RightDriveFront too hot!");
      Controller1.rumble("..--");
    }

    if (rightdriveback.temperature(fahrenheit) >= 105) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,1);
      Controller1.Screen.print("RightDriveBack too hot!");
      Controller1.rumble("..--");
    }
    wait(1, seconds);
  }
}
