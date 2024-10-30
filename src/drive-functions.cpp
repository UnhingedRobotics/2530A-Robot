#include "drive-functions.h"
#include "vex.h"

using namespace vex;

// Constructor with initializer list to initialize member variables
IntakeControl::IntakeControl() :
  hue(0.0),
  ring(false),
  team(true),
  ringdetected(false),
  intakeon(false),
  intakevelocity(0),
  mode(INTAKE_COLOR_SORT)
{}

void IntakeControl::setMode(Mode newMode) {
  mode = newMode;
  if (mode == WALLSTAKE_HOLDING || mode == HIGH_WALLSTAKE_SCORING || mode == ALLIANCE_WALLSTAKE_SCORING) {
    intakeon = false; // Stop intake when switching to wallstake modes
  }
}

void IntakeControl::colorSorting() {
  if (mode == INTAKE_COLOR_SORT && intakeon) {
    hue = opticalsensor.hue();
    if (hue < 30) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.print("red");
      ring = true;
    } else if (hue > 170) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.print("blue");
      ring = false;
    }

    if (opticalsensor.isNearObject()) {
      ringdetected = true;
      intakevelocity = 80;
    }
    if (!ringdetected) {
      intakevelocity = 60;
    }
  } else if (mode == WALLSTAKE_HOLDING && distancesensor.objectDistance(inches) < 4) {
    intakevelocity = 0;
    intake.stop(brake);
  } else {
    intakevelocity = 0;
  }
  intake.setVelocity(intakevelocity, percent);
}

void ArmControl::set_arm_exit_conditions(float arm_settle_error, float arm_settle_time, float arm_timeout){
  this->arm_settle_error = arm_settle_error;
  this->arm_settle_time = arm_settle_time;
  this->arm_timeout = arm_timeout;
}

void ArmControl::set_arm_constants(float arm_max_voltage, float arm_kp, float arm_ki, float arm_kd, float arm_starti) {
  this->arm_max_voltage = arm_max_voltage;
  this->arm_kp = arm_kp;
  this->arm_ki = arm_ki;
  this->arm_kd = arm_kd;
  this->arm_starti = arm_starti;
}

void ArmControl::move_to_angle(float angle) {
  move_to_angle(angle, arm_max_voltage, arm_settle_error, arm_settle_time, arm_timeout, arm_kp, arm_ki, arm_kd, arm_starti);
}

void ArmControl::move_to_angle(float angle, float arm_max_voltage) {
  move_to_angle(angle, arm_max_voltage, arm_settle_error, arm_settle_time, arm_timeout, arm_kp, arm_ki, arm_kd, arm_starti);
}

void ArmControl::move_to_angle(float angle, float arm_max_voltage, float arm_settle_error, float arm_settle_time, float arm_timeout) {
  move_to_angle(angle, arm_max_voltage, arm_settle_error, arm_settle_time, arm_timeout, arm_kp, arm_ki, arm_kd, arm_starti);
}

void ArmControl::move_to_angle(float angle, float arm_max_voltage, float arm_settle_error, float arm_settle_time, float arm_timeout, float arm_kp, float arm_ki, float arm_kd, float arm_starti) {
  PID armPID(angle - fourBar.position(degrees), arm_kp, arm_ki, arm_kd, arm_starti, arm_settle_error, arm_settle_time, arm_timeout);
  while (!armPID.is_settled()) {
    float error = angle - fourBar.position(degrees);
    float output = armPID.compute(error);
    output = arm_max_voltage; // Assign output directly based on PID
    fourBar.setVelocity(output, percent);
    task::sleep(10);
  }
}

void IntakeControl::update() {
  switch (mode) {
    case INTAKE_COLOR_SORT:
      colorSorting();
      break;
    case WALLSTAKE_HOLDING:
      if (distancesensor.objectDistance(inches) < 4) {
        intake.stop(brake);
      }
      break;
    case HIGH_WALLSTAKE_SCORING:
    case ALLIANCE_WALLSTAKE_SCORING:
      intake.stop(brake);
      break;
  }
}

