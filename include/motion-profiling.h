#pragma once
#include "vex.h"

class MP {
public:
  const double adjust_velocity;
  const double adjust_voltage;
  const double max_velocity;
  const double max_acceleration;
  const double max_jerk;
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
  double velocity;
  double decel_time;

  MP();

  void trapezoid_initialize(double distance);
  void trapezoid_velocity(double distance);
  void sigmoid_initialize(double distance);
  void sigmoid_velocity(double distance);
};
