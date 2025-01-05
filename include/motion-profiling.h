#pragma once
#include "vex.h"
#include <vector>
#include <cmath>
#include <utility>

class MP {
public:
  const double adjust_velocity;
  const double adjust_voltage;
  const double max_velocity;
  const double max_acceleration;
  const double max_jerk;
  const double max_angular_velocity;       // Maximum angular velocity (rad/s)
  const double max_angular_acceleration;   // Maximum angular acceleration (rad/s^2)
  const double wheelbase;                  // Distance between wheels (meters)
  const double deadband;

  double minimum_distance;
  double acceleration_time;
  double deadband_time;
  double acceleration_distance;
  double deadband_distance;
  double constant_distance;
  double constant_time;
  double total_time;
  double triangular_time;
  double time;
  double decel_time;
  double velocity;
  double angular_velocity;

  double angular_acceleration_time;
  double angular_acceleration_distance;
  double angular_minimum_distance;
  double angular_constant_distance;
  double angular_constant_time;
  double angular_total_time;
  double angular_triangular_time;

  double x, y, theta;                      // Robot pose

  std::vector<std::pair<double, double>> bezier_control_points;

  MP();

  void trapezoid_initialize(double distance);
  void trapezoid_velocity(double distance);
  void sigmoid_initialize(double distance);
  void sigmoid_velocity(double distance);

  void initialize_2d(std::vector<std::pair<double, double>> control_points, double angle);
  void angular_initialize(double angle);
  void calculate_2d_velocity(double distance, double angle);
  double calculate_angular_velocity(double angle);
  std::pair<double, double> velocity_2d();
  std::pair<double, double> bezier_point(double t);
  std::pair<double, double> bezier_derivative(double t);
  double binomial_coefficient(int n, int k);
  void update_pose();
};
