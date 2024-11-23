#include "motion-profiling.h" // Use the correct header file name
#include <cmath>              // For mathematical operations like sqrt

MP::MP() :
  adjust_velocity(200.93641618497), // m/s to volts
  max_velocity(1.73),            // m/s
  max_acceleration(5.61),        // m/s^2
  max_jerk(18.1),                // m/s^3
  minimum_distance(0),           // Minimum distance for acceleration phase
  acceleration_time(0),          // Time to reach max velocity
  acceleration_distance(0),      // Distance covered during acceleration phase
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
  distance *= 0.0254;
  // Time to reach max velocity
  acceleration_time = max_velocity / max_acceleration;

  // Distance covered during acceleration phase
  acceleration_distance = 0.5 * max_acceleration * (acceleration_time * acceleration_time);

  // Minimum distance required to reach max velocity
  minimum_distance = 2 * acceleration_distance;

  if (distance >= minimum_distance) {
    // Compute constant motion phase distance and time
    constant_distance = distance - 2 * acceleration_distance;
    constant_time = constant_distance / max_velocity;

    // Total time for motion profiling
    total_time = 2 * acceleration_time + constant_time;
  } else {
    // Triangular profile when distance < minimum_distance
    triangular_time = sqrt(distance / max_acceleration);
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

  // Check if the distance allows reaching max velocity
  if (distance >= minimum_distance) {
    if (time < acceleration_time) {
      // Acceleration phase
      velocity = max_acceleration * time;
    } else if (time < acceleration_time + constant_time) {
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
