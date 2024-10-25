#include "vex.h"

using namespace vex;

// Constructor with initializer list to initialize member variables
IntakeControl::IntakeControl() : 
  hue(0.0), 
  ring(false), 
  team(true), 
  ringdetected(false), 
  intakeon(false), 
  intakevelocity(0) 
{}

void IntakeControl::colorSorting() {
  hue = opticalsensor.hue();



  if (intakeon) {
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
    if (!ringdetected) {
      intakevelocity = 60;
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
  } else {
    intakevelocity = 0;
  }
  intake.setVelocity(intakevelocity, percent);
}
