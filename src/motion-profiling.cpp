// Define the constants
double max_velocity = 1.73; // m/s
double max_acceleration = 5.61; // m/s^2
double max_jerk = 18.1; // m/s^3

// Calculated values based on the trapezoidal profile requirements
double minimum_distance = (max_velocity * max_velocity) / max_acceleration; // Minimum distance to reach max velocity
double acceleration_time = max_velocity / max_acceleration; // Time to reach max velocity
double acceleration_distance = 0.5 * max_acceleration * (acceleration_time * acceleration_time); // Distance covered during acceleration phase

double constant_distance = distance - (2 * acceleration_distance); // Distance traveled at max velocity
double constant_time = constant_distance / max_velocity; // Time spent at max velocity

double deceleration_time = acceleration_time; // Deceleration time is equal to acceleration time
double deceleration_distance = acceleration_distance; // Deceleration distance is the same as acceleration distance

double total_time = acceleration_time + constant_time + deceleration_time; // Total time for the motion profile

// Now, determine position, velocity, and acceleration based on the time and distance
if (distance > minimum_distance) {
    if (time < acceleration_time) {
        // Acceleration phase
        position = 0.5 * max_acceleration * (time * time);
        velocity = max_acceleration * time;
        acceleration = max_acceleration;
    }
    else if (time < acceleration_time + constant_time) {
        // Constant velocity phase
        position = acceleration_distance + max_velocity * (time - acceleration_time);
        velocity = max_velocity;
        acceleration = 0;
    }
    else if (time <= total_time) {
        // Deceleration phase
        position = distance - 0.5 * max_acceleration * ((total_time - time) * (total_time - time));
        velocity = max_acceleration * (total_time - time);
        acceleration = -max_acceleration;
    }
} else {
    // If the distance is too short to reach max velocity, use a triangular profile (no constant velocity phase)
    if (time < acceleration_time) {
        // Acceleration phase
        position = 0.5 * max_acceleration * (time * time);
        velocity = max_acceleration * time;
        acceleration = max_acceleration;
    }
    else if (time <= total_time) {
        // Deceleration phase (mirroring the acceleration phase for shorter distance)
        position = distance - 0.5 * max_acceleration * ((total_time - time) * (total_time - time));
        velocity = max_acceleration * (total_time - time);
        acceleration = -max_acceleration;
    }
}
