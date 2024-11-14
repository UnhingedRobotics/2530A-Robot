// motion-profiling.h

#pragma once
#include "vex.h"

// Constants for motion profiling
extern const double adjust_velocity;      // Conversion factor for velocity adjustment
extern const double max_velocity;         // Max velocity (m/s)
extern const double max_acceleration;     // Max acceleration (m/s^2)
extern const double max_jerk;             // Max jerk (m/s^3)

// Global variables for motion profiling calculations
extern double minimum_distance;           // Minimum distance for acceleration phase
extern double acceleration_time;          // Time to reach max velocity
extern double acceleration_distance;      // Distance covered during acceleration phase

// Function to initialize motion profiling parameters
void trapezoid_initialize();

// Function to compute trapezoidal velocity profile
void trapezoid_velocity(double distance);