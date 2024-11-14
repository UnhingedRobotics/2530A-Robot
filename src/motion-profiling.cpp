#include "vex.h"

// motion_profiling.cpp

// Define the constants
const double adjust_velocity = 6.93641618497; // m/s to volts
const double max_velocity = 1.73; // m/s
const double max_acceleration = 5.61; // m/s^2
const double max_jerk = 18.1; // m/s^3

double minimum_distance;
double acceleration_time;
double acceleration_distance;

void trapezoid_initialize() {
  // Minimum distance required to reach max velocity
  minimum_distance = (max_velocity * max_velocity) / max_acceleration;

  // Time to reach max velocity
  acceleration_time = max_velocity / max_acceleration;

  // Distance covered during acceleration phase
  acceleration_distance = 0.5 * max_acceleration * (acceleration_time * acceleration_time);
}

void trapezoid_velocity(double distance) {
  distance = distance * 0.0254; // Convert inches to meters
  double time = Brain.Timer.time(seconds);
  double velocity = 0.0; // Initialize velocity variable
  
  // Check if distance allows reaching max velocity
  if (distance >= minimum_distance) {
    double constant_distance = distance - 2 * acceleration_distance;
    double constant_time = constant_distance / max_velocity;
    double total_time = 2 * acceleration_time + constant_time;

    // Calculate values based on time
    if (time < acceleration_time) {
      // double position = 0.5 * max_acceleration * time * time;
      velocity = max_acceleration * time;
      // double acceleration = max_acceleration;
      // Use position, velocity, and acceleration for motor control here if needed
    } else if (time < acceleration_time + constant_time) {
      // double position = acceleration_distance + max_velocity * (time - acceleration_time);
      velocity = max_velocity;
      // double acceleration = 0;
      // Use position, velocity, and acceleration for motor control here if needed
    } else if (time <= total_time) {
      double decel_time = total_time - time;
      // double position = distance - 0.5 * max_acceleration * decel_time * decel_time;
      velocity = max_acceleration * decel_time;
      // double acceleration = -max_acceleration;
      // Use position, velocity, and acceleration for motor control here if needed
    }
  } else {
    double triangular_time = sqrt(distance / max_acceleration);
    if (time < triangular_time) {
      // double position = 0.5 * max_acceleration * time * time;
      velocity = max_acceleration * time;
      // double acceleration = max_acceleration;
      // Use position, velocity, and acceleration for motor control here if needed
    } else if (time <= 2 * triangular_time) {
      double decel_time = 2 * triangular_time - time;
      // double position = distance - 0.5 * max_acceleration * decel_time * decel_time;
      velocity = max_acceleration * decel_time;
      // double acceleration = -max_acceleration;
      // Use position, velocity, and acceleration for motor control here if needed
    }
  }

  velocity *= adjust_velocity; // Apply velocity adjustment factor
}
