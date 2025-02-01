#include "drive-functions.h"
#include "vex.h"

using namespace vex;
// first hook 1230.4 degrees
// second hook 594.80 degrees
// third hook 1952.0 degrees
// fourth hook 2520.4 degrees

// Constructor with initializer list to initialize member variables
IntakeControl::IntakeControl() :
  hue(0.0),
  ring(false),
  team(true),
  wrongRing(false),
  ringDetected(false),
  on(false),
  holding(false),
  reverse(false),
  velocity(0),
  maxVelocity(100),
  accuracy(50),
  fullRotation(2556.0),
  firstHook(1186.80),
  secondHook(623.60),
  thirdHook(1822.40),
  fourthHook(2470.80),
  holdingPos(1000),
  hookPositions{firstHook, secondHook, thirdHook, fourthHook}
{}

void IntakeControl::colorSorting() {
    if (!on) {
      intake.stop(coast);
    }
    else {
      velocity = maxVelocity;
      if (!ringDetected) {
          aivisionsensor.takeSnapshot(aivisionsensor__bluering);
          if (aivisionsensor.objects[0].exists) {
              ring = false;
              if (team) {
                wrongRing = true;
              }
              ringDetected = true;
          } else {
              aivisionsensor.takeSnapshot(aivisionsensor__redring);
              if (aivisionsensor.objects[0].exists) {
                ring = true;
                if (!team) {
                  wrongRing = true;
                }
                ringDetected = true;
              }
          }
      }
      else {
        if (wrongRing) { 
          for (size_t i = 0; i < hookPositions.size(); i++) {
            if (fabs(fmod(intake.position(degrees), fullRotation) - hookPositions[i]) <= accuracy) {
              wait(0.1,seconds); 
              velocity = -1 * velocity;
              ringDetected = false;
              wrongRing = false;
            }
          }
        }
        if (holding) {
          for (size_t i = 0; i < hookPositions.size(); i++) {
            if (fabs(fmod(intake.position(degrees), fullRotation) - hookPositions[i]) <= accuracy) {
              on = false;
            }
          }
        }
      }
      intake.spin(forward);
      if (!reverse) {
        intake.setVelocity(velocity, percent);
      }
      else {
        intake.setVelocity(-velocity, percent);
      }
    }
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
