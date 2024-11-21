#pragma once
#include "vex.h"

class MP {
public:
  const double adjust_velocity; // Conversion factor for velocity adjustment
  const double max_velocity;    // Max velocity (m/s)
  const double max_acceleration; // Max acceleration (m/s^2)
  const double max_jerk;        // Max jerk (m/s^3)

  // Motion profiling parameters
  double minimum_distance;      // Minimum distance for acceleration phase
  double acceleration_time;     // Time to reach max velocity
  double acceleration_distance; // Distance covered during acceleration phase
  double constant_distance;     // Distance during constant velocity phase
  double constant_time;         // Time during constant velocity phase
  double total_time;            // Total time for the motion profile
  double triangular_time;       // Time for triangular motion profile
  double time;                  // Current time
  double velocity;              // Current velocity
  double decel_time;            // Deceleration time

  MP(); // Constructor with initializer list

  // Function to initialize motion profiling parameters
  void trapezoid_initialize(double distance);

  // Function to compute trapezoidal velocity profile
  void trapezoid_velocity(double distance);
};