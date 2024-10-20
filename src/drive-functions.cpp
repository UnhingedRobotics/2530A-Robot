#include "vex.h"

using namespace vex;

enum gameElements {
	mobileGoal,
	redRing,
	blueRing,
};

bool RemoteControlCodeEnabled = true;
int Brain_precision = 0, Console_precision = 0, Controller1_precision = 0, AIVisionBack_objectIndex = 0;
class IntakeControl {
  void colorSorting() {
    hue = opticalsensor.hue();

    // Color detection logic
    if (opticalsensor.isNearObject()) {
      if (hue < 30) {
        Controller1.Screen.setCursor(1, 1);
        Controller1.Screen.clearScreen();
        Controller1.Screen.print("red");
        ring = true;
      } else if (hue > 170) {
        Controller1.Screen.setCursor(1, 1);
        Controller1.Screen.clearScreen();
        Controller1.Screen.print("blue");
        ring = false;
      }
      ringdetected = true;
      intakevelocity = 80;
    }

    if (intakeon) {
      intake.setVelocity(intakevelocity, percent);
      if (!ringdetected) {
        intakevelocity = 60;
      }
    } // NICE
    else {
      intakevelocity = 0;
    }
    // Team and object detection logic
    if (!team) {
      if (ring && distancesensor.objectDistance(inches) < 2) {
        Controller1.Screen.setCursor(1, 1);
        Controller1.Screen.clearScreen();
        Controller1.Screen.print("object found");
        wait(40, msec);
        intake.stop(brake);
        wait(20, msec);
        intake.spin(forward);
        intake.setVelocity(0, percent);
        ring = false;
        ringdetected = false;
      }
    } else {
      if (!ring && distancesensor.objectDistance(inches) < 2) {
        Controller1.Screen.setCursor(1, 1);
        Controller1.Screen.clearScreen();
        Controller1.Screen.print("object found");
        wait(40, msec);
        intake.stop(brake);
        wait(20, msec);
        intake.spin(forward);
        intake.setVelocity(0, percent);
        ring = true;
        ringdetected = false;
      }
    }

  }

};
