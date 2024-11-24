#pragma once
#include "vex.h"

class MP {
public:
  const double adjust_velocity;  // Conversion factor for velocity adjustment
  const double adjust_voltage;   // Conversion factor for deadband adjustment
  const double max_velocity;     // Max velocity (m/s)
  const double max_acceleration; // Max acceleration (m/s^2)
  const double max_jerk;         // Max jerk (m/s^3)
  const double deadband;         // Deadband in volts

  // Motion profiling parameters
  double minimum_distance;       // Minimum distance for acceleration phase
  double acceleration_time;      // Time to reach max velocity
  double deadband_time;          // Time to handle deadband adjustment
  double acceleration_distance;  // Distance covered during acceleration phase
  double deadband_distance;      // Distance covered during deadband phase
  double constant_distance;      // Distance during constant velocity phase
  double constant_time;          // Time during constant velocity phase
  double total_time;             // Total time for the motion profile
  double triangular_time;        // Time for triangular motion profile
  double time;                   // Current time
  double velocity;               // Current velocity
  double decel_time;             // Deceleration time

  MP(); // Constructor with initializer list

  // Function to initialize motion profiling parameters
  void trapezoid_initialize(double distance);

  // Function to compute trapezoidal velocity profile with deadband
  void trapezoid_velocity(double distance);
};
