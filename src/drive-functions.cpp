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
  intakevelocity(0) 
{}

// ArmControl constructor
ArmControl::ArmControl() : 
  arm_max_voltage(0), arm_kp(0), arm_ki(0), arm_kd(0), arm_starti(0),
  arm_settle_error(0), arm_settle_time(0), arm_timeout(0)
{}

void IntakeControl::colorSorting() {
  hue = opticalsensor.hue();
  // Add color detection logic here
  // (code remains the same as provided in the original post)
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
