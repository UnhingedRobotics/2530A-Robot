#include "motion-profiling.h"
#include <cmath> // For sqrt() and pow()

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
  deadband_distance(1),
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

  if (distance > 0) {
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
  } else {
    if (distance >= minimum_distance) {
      constant_distance = (distance * -1) - (2 * acceleration_distance) - deadband_distance;
      constant_time = constant_distance / max_velocity;
      total_time = 2 * acceleration_time + constant_time + deadband_time;
    } else {
      triangular_time = sqrt(((distance * -1) + deadband_distance) / max_acceleration);
      total_time = 2 * triangular_time;
    }
  }
}

void MP::trapezoid_velocity(double distance) {
  distance *= 0.0254; // Convert distance from inches to meters
  time = Brain.Timer.time(seconds); // Get current time
  velocity = 0.0;

  if (distance > 0) {
    if (distance >= minimum_distance) {
      if (time < deadband_time) {
        velocity = max_acceleration * time; // Deadband phase
      } else if (time < deadband_time + acceleration_time) {
        velocity = max_acceleration * (time - deadband_time); // Acceleration phase
      } else if (time < deadband_time + acceleration_time + constant_time) {
        velocity = max_velocity; // Constant velocity phase
      } else if (time <= total_time) {
        decel_time = total_time - time;
        velocity = max_acceleration * decel_time; // Deceleration phase
      }
    } else {
      if (time < triangular_time) {
        velocity = max_acceleration * time; // Acceleration phase
      } else if (time <= total_time) {
        decel_time = total_time - time;
        velocity = max_acceleration * decel_time; // Deceleration phase
      }
    }
  } else {
    if (distance <= minimum_distance) {
      if (time < deadband_time) {
        velocity = max_acceleration * time * -1; // Deadband phase
      } else if (time < deadband_time + acceleration_time) {
        velocity = max_acceleration * (time - deadband_time); // Acceleration phase
      } else if (time < deadband_time + acceleration_time + constant_time) {
        velocity = max_velocity * -1; // Constant velocity phase
      } else if (time <= total_time) {
        decel_time = total_time - time;
        velocity = max_acceleration * decel_time * -1; // Deceleration phase
      }
    } else {
      if (time < triangular_time) {
        velocity = max_acceleration * time * -1; // Acceleration phase
      } else if (time <= total_time) {
        decel_time = total_time - time;
        velocity = max_acceleration * decel_time * -1; // Deceleration phase
      }
    }
  }
  velocity *= adjust_velocity; // Adjust velocity using the conversion factor
}

void MP::sigmoid_initialize(double distance) {
  Brain.Timer.reset();
  distance *= 0.0254; // Convert distance from inches to meters

  double jerk_phase_time = max_acceleration / max_jerk; // Time to reach max acceleration using max jerk
  acceleration_time = max_velocity / max_acceleration;

  double jerk_distance = 0.5 * max_jerk * pow(jerk_phase_time, 3); // Distance covered during jerk phase
  acceleration_distance = 0.5 * max_acceleration * pow(acceleration_time, 2) - 2 * jerk_distance;

  double full_acceleration_distance = 2 * jerk_distance + acceleration_distance;
  minimum_distance = 2 * full_acceleration_distance;

  if (distance >= minimum_distance) {
    constant_distance = distance - 2 * full_acceleration_distance;
    constant_time = constant_distance / max_velocity;
    total_time = 2 * acceleration_time + constant_time;
  } else {
    triangular_time = sqrt(distance / max_acceleration);
    total_time = 2 * triangular_time;
  }
}

void MP::sigmoid_velocity(double distance) {
  distance *= 0.0254; // Convert distance from inches to meters
  time = Brain.Timer.time(seconds);
  velocity = 0.0;

  double jerk_phase_time = max_acceleration / max_jerk;

  if (distance >= minimum_distance) {
    if (time < jerk_phase_time) {
      velocity = 0.5 * max_jerk * pow(time, 2); // Initial jerk phase
    } else if (time < jerk_phase_time + acceleration_time) {
      double t = time - jerk_phase_time;
      velocity = max_acceleration * t + 0.5 * max_jerk * pow(jerk_phase_time, 2); // Acceleration
    } else if (time < total_time - acceleration_time - jerk_phase_time) {
      velocity = max_velocity; // Constant velocity
    } else if (time < total_time - jerk_phase_time) {
      double t = time - (total_time - acceleration_time - jerk_phase_time);
      velocity = max_velocity - max_acceleration * t; // Deceleration
    } else if (time <= total_time) {
      double t = total_time - time;
      velocity = 0.5 * max_jerk * pow(t, 2); // Final jerk phase
    }
  } else {
    if (time < triangular_time) {
      velocity = 0.5 * max_jerk * pow(time, 2); // Triangular acceleration phase
    } else if (time <= total_time) {
      double t = total_time - time;
      velocity = 0.5 * max_jerk * pow(t, 2); // Triangular deceleration phase
    }
  }
  velocity *= adjust_velocity; // Adjust velocity using the conversion factor
}
