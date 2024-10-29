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
    if (opticalsensor.isNearObject()) {
      ringdetected = true;
      intakevelocity = 80;
    }
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
    if (!ringdetected) {
      intakevelocity = 60;
    }
  } else {
    intakevelocity = 0;
  }
  intake.setVelocity(intakevelocity, percent);
}

void ArmControl::set_arm_constants(float arm_max_voltage, float arm_kp, float arm_ki, float arm_kd, float arm_starti){
  this->arm_max_voltage = arm_max_voltage;
  this->arm_kp = arm_kp;
  this->arm_ki = arm_ki;
  this->arm_kd = arm_kd;
  this->arm_starti = arm_starti;
} 
void ArmControl::move_to_angle(float angle){
  move_to_angle(angle, arm_max_voltage, arm_settle_error, arm_settle_time, arm_timeout, arm_kp, arm_ki, arm_kd, arm_starti);
}

void ArmControl::move_to_angle(float angle, float arm_max_voltage){
  move_to_angle(angle, arm_max_voltage, arm_settle_error, arm_settle_time, arm_timeout, arm_kp, arm_ki, arm_kd, arm_starti);
}

void ArmControl::move_to_angle(float angle, float arm_max_voltage, float arm_settle_error, float arm_settle_time, float arm_timeout){
  move_to_angle(angle, arm_max_voltage, arm_settle_error, arm_settle_time, arm_timeout, arm_kp, arm_ki, arm_kd, arm_starti);
}

void Arm::move_to_angle(float angle, float arm_max_voltage, float arm_settle_error, float arm_settle_time, float arm_timeout, float arm_kp, float arm_ki, float arm_kd, float arm_starti){
  PID armPID(reduce_negative_180_to_180(angle - get_absolute_heading()), arm_kp, arm_ki, arm_kd, arm_starti, arm_settle_error, arm_settle_time, arm_timeout);
  while( !armPID.is_settled() ){
    float error = angle - fourBar.position(percent);
    float output = armPID.compute(error);
    output = arm_max_voltage;
    fourBar.setVelocity(output, percent);
    task::sleep(10);
  }
}

