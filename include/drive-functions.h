#pragma once

#include <array> // For std:array

class IntakeControl {
public:
  bool auto_on = false;
  int time_waited;
  double hue;
  bool ring;          // true = red, false = blue
  bool team;          // true = red team, false = blue team
  bool wrongRing;
  bool ringDetected;
  bool on;
  bool holding;
  bool reverse;
  int velocity;
  int maxVelocity;
  double accuracy;
  double fullRotation;
  const double firstHook;
  const double secondHook;
  const double thirdHook;
  const double fourthHook;
  const double holdingPos; 
  bool stuckOveride; 
  const std::array<double, 4> hookPositions;

  IntakeControl(); // Constructor with an initializer list

  // Functions for controlling intake behavior
  void colorSorting();              // Color-sorting logic
};


void healthCheck();
