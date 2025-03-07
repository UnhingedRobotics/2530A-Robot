#include "intake.h"
#include "vex.h"

using namespace vex;

int aivisionsensor_objectIndex = 0;
// Constructor with initializer list to initialize member variables

IntakeControl::IntakeControl() :
  curRing(none),
  alliance(red),
  hue(0.0),
  wrongRing(false),
  on(false),
  roller_on(false),
  holding(false),
  reverse(false),
  velocity(0),
  maxVelocity(100),
  accuracy(10),
  chainLinks(73),
  gearDiameter(1.2896161417),
  gearRatio(2),
  fullRotation((360 * (chainLinks * 65.5 / 170) / M_PI * gearDiameter) / gearRatio),
  firstHook(242),
  secondHook(780),
  thirdHook(1822.40),
  fourthHook(2470.80),
  holdingPos(1000),
  stuckOveride(true),
  hookPositions{firstHook, secondHook, thirdHook, fourthHook}
{}

void IntakeControl::detectColor() {
  aivisionsensor.takeSnapshot(aivision::ALL_AIOBJS);
  if (aivisionsensor.objects[aivisionsensor_objectIndex].id == blueRing) {
      curRing = blue;
      if (alliance == red) {
        wrongRing = true;
      }

  }
  else {
    if (aivisionsensor.objects[aivisionsensor_objectIndex].id == blueRing) {
      curRing = red;
      if (alliance == blue) {
        wrongRing = true;
      }
    }
  }
  if (curRing == none) {
    if (reduce_0_to_360(fabs(opticalsensor.hue() - 10)) <= 0) {
      curRing = red;
      if (alliance == blue) {
        wrongRing = true;
      }
    }
    else {
      if (fabs(opticalsensor.hue() - 10) <= 240) {
        curRing = red;
        if (alliance == blue) {
          wrongRing = true;
        }
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
  if (roller_on) {
    roller.spin(forward);
    roller.setVelocity(100, percent);
  }
  else {
    roller.stop(coast);
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
