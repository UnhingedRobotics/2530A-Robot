#pragma once

class IntakeControl {
public:
  double hue;
  bool ring;          // true = red, false = blue
  bool team;          // true = red team, false = blue team
  bool ringdetected;
  bool intakeon;
  int intakevelocity;

  // Constructor with an initializer list
  IntakeControl();

  void colorSorting();


  void turn_to_angle(float angle);
  void turn_to_angle(float angle, float turn_max_voltage);
  void turn_to_angle(float angle, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout);
  void turn_to_angle(float angle, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout, float turn_kp, float turn_ki, float turn_kd, float turn_starti);
};
