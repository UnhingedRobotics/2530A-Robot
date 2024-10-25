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
};
