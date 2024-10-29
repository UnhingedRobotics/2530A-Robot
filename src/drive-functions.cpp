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
  if (hue < 30) {
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("red");
    ring = true;
  }
  if (hue > 170) {
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.clearScreen();
    Controller1.Screen.print("blue");
    ring = false;
  }
  if (intakeon) {
    // Color detection logic
    if (opticalsensor.isNearObject()) {
      if (!team) {
        if (distancesensor.objectDistance(inches) < 4) {
          Controller1.Screen.setCursor(1, 1);
          Controller1.Screen.clearScreen();
          Controller1.Screen.print("object found");
          wait(40, msec);
          intakevelocity = 0;
          intake.stop(brake);
          wait(20, msec);
          intake.spin(forward);
          ring = false;
          ringdetected = false;
        }
        else {
          if (distancesensor.objectDistance(inches) < 4) {
            Controller1.Screen.setCursor(1, 1);
            Controller1.Screen.clearScreen();
            Controller1.Screen.print("object found");
            wait(40, msec);
            intakevelocity = 0;
            intake.stop(brake);
            wait(20, msec);
            intake.spin(forward);
            ring = true;
            ringdetected = false;
          }
        }
      }
      ringdetected = true;
      intakevelocity = 80;
    } 
    if (!ringdetected) {
      intakevelocity = 60;
    }
  } else {
    intakevelocity = 0;
  }
  intake.setVelocity(intakevelocity, percent);
}