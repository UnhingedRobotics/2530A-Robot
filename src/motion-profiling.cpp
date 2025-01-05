#include "motion-profiling.h"

// Constructor with initializer list
MP::MP() :
  adjust_velocity(6.93641618497),
  adjust_voltage(0.144166666667),
  max_velocity(1.73),
  max_acceleration(5.61),
  max_jerk(18.1),
  max_angular_velocity(2.0), // Maximum angular velocity (rad/s)
  max_angular_acceleration(6.0), // Maximum angular acceleration (rad/s^2)
  wheelbase(0.5), // Distance between wheels (meters)
  x(0), y(0), theta(0), // Initial pose
  velocity(0), 
  angular_velocity(0),  
  deadband(0),
  minimum_distance(0),
  acceleration_time(0),
  deadband_time(0),
  acceleration_distance(0),
  deadband_distance(1),
  constant_distance(0),
  constant_time(0),
  total_time(0),
  triangular_time(0),
  time(0),
  decel_time(0)
{}
void MP::initialize_2d(std::vector<std::pair<double, double>> control_points, double angle) {
  // Store control points
  bezier_control_points = control_points;

  // Calculate total path length by discretizing the curve
  double path_length = 0.0;
  for (double t = 0; t <= 1.0; t += 0.01) {
    auto current_point = bezier_point(t);
    auto next_point = bezier_point(t + 0.01);
    path_length += std::hypot(next_point.first - current_point.first, next_point.second - current_point.second);
  }

  // Initialize 1D motion profile with total path length
  trapezoid_initialize(path_length);
  angular_initialize(angle); // Angular profile
}


void MP::angular_initialize(double angle) {
  angle = angle * (M_PI / 180.0); // Convert angle to radians
  angular_acceleration_time = max_angular_velocity / max_angular_acceleration;
  angular_acceleration_distance = 0.5 * max_angular_acceleration * pow(angular_acceleration_time, 2);
  angular_minimum_distance = 2 * angular_acceleration_distance;

  if (abs(angle) > angular_minimum_distance) {
    angular_constant_distance = abs(angle) - 2 * angular_acceleration_distance;
    angular_constant_time = angular_constant_distance / max_angular_velocity;
    angular_total_time = 2 * angular_acceleration_time + angular_constant_time;
  } else {
    angular_triangular_time = sqrt(abs(angle) / max_angular_acceleration);
    angular_total_time = 2 * angular_triangular_time;
  }
}

void MP::calculate_2d_velocity(double distance, double angle) {
  trapezoid_velocity(distance); // Linear velocity
  angular_velocity = calculate_angular_velocity(angle); // Angular velocity
}

double MP::calculate_angular_velocity(double angle) {
  angle = angle * (M_PI / 180.0); // Convert angle to radians
  double current_time = Brain.Timer.time(seconds);

  if (abs(angle) > angular_minimum_distance) {
    if (current_time < angular_acceleration_time) {
      return max_angular_acceleration * current_time * (angle < 0 ? -1 : 1);
    } else if (current_time < angular_acceleration_time + angular_constant_time) {
      return max_angular_velocity * (angle < 0 ? -1 : 1);
    } else if (current_time <= angular_total_time) {
      double decel_time = angular_total_time - current_time;
      return max_angular_acceleration * decel_time * (angle < 0 ? -1 : 1);
    }
  } else {
    if (current_time < angular_triangular_time) {
      return max_angular_acceleration * current_time * (angle < 0 ? -1 : 1);
    } else if (current_time <= angular_total_time) {
      double decel_time = angular_total_time - current_time;
      return max_angular_acceleration * decel_time * (angle < 0 ? -1 : 1);
    }
  }
  return 0;
}
std::pair<double, double> MP::velocity_2d() {
  time = Brain.Timer.time(seconds); // Get current time
  trapezoid_velocity(1.0);         // Update velocity for current time step

  // Map current time to parameter t based on total_time
  double t = time / total_time;
  if (t > 1.0) t = 1.0;

  // Get Bézier curve position and derivative
  auto position = bezier_point(t);
  auto derivative = bezier_derivative(t);

  // Calculate heading angle (theta)
  double theta = std::atan2(derivative.second, derivative.first);

  // Return velocity and heading as a pair
  return {velocity, theta};
}

// Bézier curve position at parameter t
std::pair<double, double> MP::bezier_point(double t) {
  double x = 0.0, y = 0.0;
  int n = bezier_control_points.size() - 1;
  for (int i = 0; i <= n; ++i) {
    double coefficient = binomial_coefficient(n, i) * pow(1 - t, n - i) * pow(t, i);
    x += coefficient * bezier_control_points[i].first;
    y += coefficient * bezier_control_points[i].second;
  }
  return {x, y};
}

// Bézier curve derivative at parameter t
std::pair<double, double> MP::bezier_derivative(double t) {
  double dx = 0.0, dy = 0.0;
  int n = bezier_control_points.size() - 1;
  for (int i = 0; i < n; ++i) {
    double coefficient = binomial_coefficient(n - 1, i) * pow(1 - t, n - 1 - i) * pow(t, i);
    dx += coefficient * (bezier_control_points[i + 1].first - bezier_control_points[i].first);
    dy += coefficient * (bezier_control_points[i + 1].second - bezier_control_points[i].second);
  }
  return {dx, dy};
}

// Calculate binomial coefficient (n choose k)
double MP::binomial_coefficient(int n, int k) {
  if (k == 0 || k == n) return 1;
  if (k > n) return 0;
  double result = 1.0;
  for (int i = 1; i <= k; ++i) {
    result *= (n - i + 1) / i;
  }
  return result;
}


void MP::update_pose() {
  double dt = Brain.Timer.time(seconds) - time;
  time = Brain.Timer.time(seconds);

  // Calculate new position based on linear and angular velocity
  x += velocity * cos(theta) * dt;
  y += velocity * sin(theta) * dt;
  theta += angular_velocity * dt;

  // Normalize theta to -pi to pi
  theta = fmod(theta + M_PI, 2 * M_PI) - M_PI;
}

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
