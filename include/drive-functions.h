#pragma once

// Intake modes enumeration
enum Mode {
  INTAKE_COLOR_SORT,
  WALLSTAKE_HOLDING,
  HIGH_WALLSTAKE_SCORING,
  ALLIANCE_WALLSTAKE_SCORING
};

class IntakeControl {
public:
  double hue;
  bool ring;          // true = red, false = blue
  bool team;          // true = red team, false = blue team
  bool ringdetected;
  bool intakeon;
  int intakevelocity;
  Mode mode;          // Current mode of the intake system

  IntakeControl(); // Constructor with an initializer list

  // Functions for controlling intake behavior
  void setMode(Mode newMode);       // Sets the intake mode
  void colorSorting();              // Color-sorting logic
  void update();                    // Update intake behavior based on mode
};

class ArmControl {
public:
  float arm_max_voltage;
  float arm_kp;
  float arm_ki;
  float arm_kd;
  float arm_starti;
  float arm_settle_error;
  float arm_settle_time;
  float arm_timeout;

  // Functions for setting arm PID constants and exit conditions
  void set_arm_constants(float arm_max_voltage, float arm_kp, float arm_ki, float arm_kd, float arm_starti);
  void set_arm_exit_conditions(float arm_settle_error, float arm_settle_time, float arm_timeout);
  
  // Functions for moving arm to a specified angle
  void move_to_angle(float angle);
  void move_to_angle(float angle, float arm_max_voltage);
  void move_to_angle(float angle, float arm_max_voltage, float arm_settle_error, float arm_settle_time, float arm_timeout);
  void move_to_angle(float angle, float arm_max_voltage, float arm_settle_error, float arm_settle_time, float arm_timeout, float arm_kp, float arm_ki, float arm_kd, float arm_starti);
};

