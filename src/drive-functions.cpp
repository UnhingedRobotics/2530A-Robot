#include "drive-functions.h"
#include "vex.h"
#include <sstream> // For std::ostringstream

using namespace vex;

// Constructor with initializer list to initialize member variables
IntakeControl::IntakeControl() :
  hue(0.0),
  ring(false),
  team(true),
  ringdetected(false),
  intakeon(false),
  holding(false),
  intakevelocity(0),
  mode(INTAKE_COLOR_SORT)
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
    intake.setVelocity(-80, percent);

}

void IntakeControl::colorSorting() {
    if (!intakeon) {
      intakevelocity = 0;
    }
    else {
      intakevelocity = 80;
      if (intake.position(degrees) >= 2000) {
        ringdetected = false;
        
      }
      if (!ringdetected) {
          aivisionsensor.takeSnapshot(aivisionsensor__bluering);
          if (aivisionsensor.objects[0].exists) {
              ring = false;
              ringdetected = true;
              if (team) {
                intake.resetPosition();
              }
              // updateControllerScreen("blue");
          } else {
              aivisionsensor.takeSnapshot(aivisionsensor__redring);
              if (aivisionsensor.objects[0].exists) {
                ring = true;
                ringdetected = true;
                if (!team) {
                  intake.resetPosition();
                }
                // updateControllerScreen("red");
              }
          }
      }
      // Check if position is a multiple of 1300
      if (intake.position(degrees) >= 350) {
          if (ringdetected) {
            if ((team && !ring) || (!team && ring)) {
              resetIntake();
              updateControllerScreen("ring removed");
              ringdetected = false;
            }

            if (holding) {
                intake.stop(brake);
                intake.spin(forward);
                intakevelocity = -50;
                intake.setVelocity(intakevelocity, percent);
                intakeon = false;
            }
          }
          

      }

    }
    intake.spin(forward);
    intake.setVelocity(intakevelocity, percent);
    // updateControllerScreen(toString(intake.position(degrees)).c_str());
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
