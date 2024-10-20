#pragma once

class IntakeControl;

class IntakeControl {
public:
  double hue;
  bool ring = false; // true = red, false = blue
  bool team = true;  // true = red team, false = blue team
  bool ringdetected = false;
  bool intakeon = false;
  int intakevelocity;
};

