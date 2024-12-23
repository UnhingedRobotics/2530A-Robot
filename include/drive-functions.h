#pragma once

#include <string> // For std::to_string

// Intake modes enumeration
enum Mode {
  INTAKE_COLOR_SORT,
  WALLSTAKE_HOLDING
};

class IntakeControl {
public:
  bool auto_on = false;
  int time_waited;
  double hue;
  bool ring;          // true = red, false = blue
  bool team;          // true = red team, false = blue team
  bool ringdetected;
  bool intakeon;
  bool holding;
  int intakeVelocity;
  int intakeMaxVelocity;
  double accuracyIntake;
  double intakeFullRotation;
  double intakeFirstHook;
  double intakeSecondHook;
  double intakeHoldingPos;
  Mode mode;          // Current mode of the intake system

  IntakeControl(); // Constructor with an initializer list

  // Functions for controlling intake behavior
  void setMode(Mode newMode);       // Sets the intake mode
  void colorSorting();              // Color-sorting logic
  void intakeMove();

private:
  // Helper functions for modularized actions
  void updateControllerScreen(const char* message); // Updates the controller screen
};


void healthCheck();
std::string toString(double value);

class FishControl {
public:
  bool pid_stop;
  int pos_num;
  float arm_max_voltage;
  float arm_kp;
  float arm_ki;
  float arm_kd;
  float arm_starti;
  float arm_settle_error;
  float arm_settle_time;
  float arm_timeout;
  float angle_wanted_pos;

  // Functions for setting arm PID constants and exit conditions
  void set_arm_constants(float arm_max_voltage, float arm_kp, float arm_ki, float arm_kd, float arm_starti);
  void set_arm_exit_conditions(float arm_settle_error, float arm_settle_time, float arm_timeout);
  
  // Functions for moving arm to a specified angle
  void move_to_angle(float angle);
  void move_to_angle(float angle, float arm_max_voltage);
  void move_to_angle(float angle, float arm_max_voltage, float arm_settle_error, float arm_settle_time, float arm_timeout);
  void move_to_angle(float angle, float arm_max_voltage, float arm_settle_error, float arm_settle_time, float arm_timeout, float arm_kp, float arm_ki, float arm_kd, float arm_starti);
};
