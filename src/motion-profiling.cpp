#include "motion-profiling.h"
#include <cmath> // For sqrt()

// Constructor with initializer list
MP::MP() :
  adjust_velocity(6.93641618497), // Conversion factor (m/s to volts)
  adjust_voltage(0.144166666667),  // Conversion factor for deadband adjustment
  max_velocity(1.73),              // Max velocity (m/s)
  max_acceleration(5.61),          // Max acceleration (m/s^2)
  max_jerk(18.1),                  // Max jerk (m/s^3)
  deadband(0),                     // Deadband in volts
  minimum_distance(0),             // Initialize motion profile parameters
  acceleration_time(0),
  deadband_time(0),
  acceleration_distance(0),
  deadband_distance(0),
  constant_distance(0),
  constant_time(0),
  total_time(0),
  triangular_time(0),
  time(0),
  velocity(0),
  decel_time(0)
{}

void MP::trapezoid_initialize(double distance) {
  Brain.Timer.reset();
  distance *= 0.0254; // Convert distance from inches to meters

  // Time to reach max velocity
  acceleration_time = max_velocity / max_acceleration;
  
  // Time for deadband phase
  deadband_time = (deadband * adjust_voltage) / max_acceleration;

  // Distance covered during deadband phase
  deadband_distance = 0.5 * max_acceleration * (deadband_time * deadband_time);

  // Distance covered during acceleration phase (excluding deadband)
  acceleration_distance = 0.5 * max_acceleration * (acceleration_time * acceleration_time);

  // Minimum distance required to reach max velocity, including deadband
  minimum_distance = 2 * acceleration_distance + deadband_distance;

  if (distance >= minimum_distance) {
    // Compute constant motion phase distance and time
    constant_distance = distance - (2 * acceleration_distance) - deadband_distance;
    constant_time = constant_distance / max_velocity;

    // Total time for motion profiling
    total_time = 2 * acceleration_time + constant_time + deadband_time;
  } else {
    // Triangular profile when distance < minimum_distance
    triangular_time = sqrt((distance + deadband_distance) / max_acceleration);
    total_time = 2 * triangular_time;
  }
}

void MP::trapezoid_velocity(double distance) {
  // Convert distance from inches to meters
  distance *= 0.0254;

  // Get the current time in seconds
  time = Brain.Timer.time(seconds);

  // Reset velocity
  velocity = 0.0;

  if (distance >= minimum_distance) {
    if (time < deadband_time) {
      // Deadband phase: Adjust velocity based on deadband
      velocity = max_acceleration * time;
    } else if (time < deadband_time + acceleration_time) {
      // Acceleration phase
      velocity = max_acceleration * (time - deadband_time);
    } else if (time < deadband_time + acceleration_time + constant_time) {
      // Constant velocity phase
      velocity = max_velocity;
    } else if (time <= total_time) {
      // Deceleration phase
      decel_time = total_time - time;
      velocity = max_acceleration * decel_time;
    }
  } else {
    if (time < triangular_time) {
      // Acceleration phase for triangular profile
      velocity = max_acceleration * time;
    } else if (time <= total_time) {
      // Deceleration phase for triangular profile
      decel_time = total_time - time;
      velocity = max_acceleration * decel_time;
    }
  }

  // Adjust velocity using the conversion factor
  velocity *= adjust_velocity;
}

