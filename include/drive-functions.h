#pragma once

#include <string> // For std::to_string

// Intake modes enumeration
enum Mode {
  INTAKE_COLOR_SORT,
  WALLSTAKE_HOLDING
  // HIGH_WALLSTAKE_SCORING,
  // ALLIANCE_WALLSTAKE_SCORING
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
  int intakevelocity;
  Mode mode;          // Current mode of the intake system

  IntakeControl(); // Constructor with an initializer list

  // Functions for controlling intake behavior
  void setMode(Mode newMode);       // Sets the intake mode
  void colorSorting();              // Color-sorting logic
  void update();                    // Update intake behavior based on mode

private:
  // Helper functions for modularized actions
  void updateControllerScreen(const char* message); // Updates the controller screen
  void resetIntake();                               // Resets the intake mechanism
};


void healthCheck();
std::string toString(double value);