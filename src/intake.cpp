#include "intake.h"
#include "vex.h"

using namespace vex;

// Constructor with initializer list to initialize member variables

IntakeControl::IntakeControl() :
  curRing(none),
  alliance(red),
  hue(0.0),
  wrongRing(false),
  on(false),
  holding(false),
  reverse(false),
  velocity(0),
  maxVelocity(100),
  accuracy(10),
  fullRotation(2496.0),
  firstHook(1156.80),
  secondHook(657.60),
  thirdHook(1822.40),
  fourthHook(2470.80),
  holdingPos(1000),
  stuckOveride(true),
  hookPositions{firstHook, secondHook, thirdHook, fourthHook}
{}

void IntakeControl::detectColor() {
  aivisionsensor.takeSnapshot(aivisionsensor__bluering);
  if (aivisionsensor.objects[0].exists) {
      curRing = blue;
      if (alliance == red) {
        wrongRing = true;
      }
    
  } else {
    aivisionsensor.takeSnapshot(aivisionsensor__redring);
    if (aivisionsensor.objects[0].exists) {
      curRing = red;
      if (alliance == blue) {
        wrongRing = true;
      }
    }
  }
}
void IntakeControl::colorSorting() {
  if (!on) {
    intake.stop(coast);
  }
  else {
    velocity = maxVelocity;
    if (curRing == none) {
      detectColor();
    }
    else {
      if (wrongRing) { 
        for (size_t i = 0; i < hookPositions.size(); i++) {
          if (fabs(fmod(intake.position(degrees), fullRotation) - hookPositions[i]) <= accuracy) {
            velocity = -1 * velocity;
            curRing = none;
            wrongRing = false;
          }
        }
      }
      else {
        if (holding) {
          for (size_t i = 0; i < hookPositions.size(); i++) {
            if (fabs(fmod(intake.position(degrees), fullRotation) - hookPositions[i]) <= accuracy) {
              on = false;
            }
          }
        }
      }
    }
    intake.spin(forward);
    if (reverse) {
      velocity = -velocity;
    }
    else {
      if (velocity < 0) {
        intake.setVelocity(velocity, percent);
        wait(0.1,seconds); 
      }
    }
    if (!stuckOveride) {
      antiJam();
    }
    else {
      intake.setVelocity(velocity, percent);
    }
  }
}
void IntakeControl::antiJam() {
  if (intake.velocity(rpm) > 50) {
    intake.setVelocity(velocity, percent);
  }
  else {
    intake.setVelocity(-velocity, percent);
    wait(0.3,seconds); 
  }
}
