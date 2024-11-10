// Define the constants
const double adjust_velocity = 6.93641618497; // m/s to volts
double max_velocity = 1.73; // m/s
double max_acceleration = 5.61; // m/s^2
double max_jerk = 18.1; // m/s^3

// Minimum distance required to reach max velocity
double minimum_distance = (max_velocity * max_velocity) / max_acceleration; 

// Time to reach max velocity
double acceleration_time = max_velocity / max_acceleration; 

// Distance covered during acceleration phase
double acceleration_distance = 0.5 * max_acceleration * (acceleration_time * acceleration_time); 

// Check if the distance is large enough to reach max velocity
if (distance >= minimum_distance) {
    // If the distance is large enough to reach max velocity
    double constant_distance = distance - 2 * acceleration_distance; // Distance traveled at max velocity
    double constant_time = constant_distance / max_velocity; // Time spent at max velocity
    
    // Total time for the trapezoidal profile
    double total_time = acceleration_time + constant_time + acceleration_time;
    
    // Now determine position, velocity, and acceleration based on the time and distance
    if (time < acceleration_time) {
        // Acceleration phase
        position = 0.5 * max_acceleration * (time * time);
        velocity = max_acceleration * time * adjust_velocity;
        acceleration = max_acceleration;
    }
    else if (time < acceleration_time + constant_time) {
        // Constant velocity phase
        position = acceleration_distance + max_velocity * (time - acceleration_time);
        velocity = max_velocity * adjust_velocity;
        acceleration = 0;
    }
    else if (time <= total_time) {
        // Deceleration phase
        position = distance - 0.5 * max_acceleration * ((total_time - time) * (total_time - time));
        velocity = max_acceleration * (total_time - time) * adjust_velocity;
        acceleration = -max_acceleration;
    }
} else {
    // If the distance is too short to reach max velocity, use a triangular profile
    double triangular_time = sqrt(distance / max_acceleration); // Time for triangular profile (half acceleration time)
    
    // For triangular profile (acceleration and deceleration only)
    if (time < triangular_time) {
        // Acceleration phase
        position = 0.5 * max_acceleration * (time * time);
        velocity = max_acceleration * time * adjust_velocity;
        acceleration = max_acceleration;
    }
    else if (time <= 2 * triangular_time) {
        // Deceleration phase (mirroring the acceleration phase)
        position = distance - 0.5 * max_acceleration * ((2 * triangular_time - time) * (2 * triangular_time - time));
        velocity = max_acceleration * (2 * triangular_time - time) * adjust_velocity;
        acceleration = -max_acceleration;
    }
}
