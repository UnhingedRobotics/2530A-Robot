#pragma once

class IntakeControl {
public:
  double hue;
  bool ring;          // true = red, false = blue
  bool team;          // true = red team, false = blue team
  bool ringdetected;
  bool intakeon;
  int intakevelocity;

  IntakeControl(); // Constructor with an initializer list
  void colorSorting();
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

  ArmControl(); // Constructor
  void set_arm_constants(float arm_max_voltage, float arm_kp, float arm_ki, float arm_kd, float arm_starti);
  void move_to_angle(float angle);
  void move_to_angle(float angle, float arm_max_voltage);
  void move_to_angle(float angle, float arm_max_voltage, float arm_settle_error, float arm_settle_time, float arm_timeout);
  void move_to_angle(float angle, float arm_max_voltage, float arm_settle_error, float arm_settle_time, float arm_timeout, float arm_kp, float arm_ki, float arm_kd, float arm_starti);
};
