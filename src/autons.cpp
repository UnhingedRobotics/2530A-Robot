#include "vex.h"

bool RemoteControlCodeEnabled = true;
int Brain_precision = 0, Console_precision = 0, Controller1_precision = 0, AIVisionBack_objectIndex = 0;

/**
 * Resets the constants for auton movement.
 * Modify these to change the default behavior of functions like
 * drive_distance(). For explanations of the difference between
 * drive, heading, turning, and swinging, as well as the PID and
 * exit conditions, check the docs.
 */

int intakeTaskFunction() {
    while (true) {
      // Intake mode management
      intakeControl.colorSorting();
      wait(5, msec); // Small delay to avoid resource overuse
    }

    return 0; // End of the task
}

void default_constants(){
  // Each constant set is in the form of (maxVoltage, kP, kI, kD, startI).
  chassis.set_drive_constants(10, 1.05, 0, 10, 0);
  chassis.set_heading_constants(7, 1, 0, 4, 0);
  chassis.set_v_drive_constants(10, 1.2, 0, 10, 0);
  chassis.set_v_heading_constants(6, .4, 0, 1, 0);
  chassis.set_turn_constants(7, .17, 0.02, 0.8, 15);
  chassis.set_swing_constants(12, .3, .001, 2, 15);

  // Each exit condition set is in the form of (settle_error, settle_time, timeout).
  chassis.set_drive_exit_conditions(1.5, 300, 5000);
  chassis.set_v_drive_exit_conditions(1.5, 300, 5000);
  chassis.set_turn_exit_conditions(1, 300, 3000);
  chassis.set_swing_exit_conditions(1, 300, 3000);
}

/**
 * Sets constants to be more effective for odom movements.
 * For functions like drive_to_point(), it's often better to have
 * a slower max_voltage and greater settle_error than you would otherwise.
 */

void odom_constants(){
  default_constants();
  // chassis.heading_max_voltage = 7;
  // chassis.drive_max_voltage = 10;
  // chassis.drive_kp = 0.7;
  // chassis.drive_kd = 0;
  // chassis.heading_kp = .15;
  // chassis.drive_settle_error = 3;
  // chassis.boomerang_lead = .5;
  chassis.drive_min_voltage = 0;
}

/**
 * The expected behavior is to return to the start position.
 */

void red_left_4_ring(){
  chassis.set_coordinates(-59.13, 24.567, -90);
  task intakeTask(intakeTaskFunction);
  intakeControl.team = false;
  intakeControl.on = false;
  odom_constants();
  chassis.turn_to_point(-23.773, 24.369, true);
  chassis.drive_to_point(-23.773, 24.369);
  goalclamp.set(true);
  wait(0.3, seconds);
  chassis.turn_to_point(-23.972, 51.185);
  intakeControl.on = true;
  chassis.drive_to_point(-23.972, 51.185);
  chassis.turn_to_point(-8.279, 50.788);
  chassis.drive_to_point(-8.279, 50.788);
  chassis.turn_to_point(-16.73, 42.694, true);
  chassis.drive_to_point(-16.73, 42.694);
  chassis.turn_to_point(-7.485, 43.637);
  chassis.drive_to_point(-7.485, 43.637);
  chassis.turn_to_point(-10.666, 24.311);
  intakeControl.on = false;
  chassis.drive_max_voltage = 7;
  chassis.drive_to_point(-10.666, 24.311);
  chassis.drive_with_voltage(0,0);
}
void red_left_winpoint(){
  chassis.set_coordinates(-52.575, 12.053, 180);
  task intakeTask(intakeTaskFunction);
  intakeControl.team = false;
  intakeControl.on = false;
  odom_constants();

  chassis.turn_to_point(-52.178, -0.262);
  chassis.drive_to_point(-52.178, -0.262);
  chassis.turn_to_point(-59.528, -0.262, true);
  chassis.drive_max_voltage = 7;
  chassis.drive_to_point(-59.528, -0.262);
  intakeControl.on = true;
  wait(0.5, seconds);
  intakeControl.reverse = true;
  wait(0.2, seconds);
  intakeControl.reverse = false;
  intakeControl.on = false;
  chassis.turn_to_point(-45.226, -0.461);
  chassis.drive_max_voltage = 10;
  chassis.drive_to_point(-45.226, -0.461);
  chassis.turn_to_point(-22.978, 23.574, true);
  chassis.drive_to_point(-22.978, 23.574);
  goalclamp.set(true);
  wait(0.3, seconds);
  chassis.turn_to_point(-23.376, 50.39);
  intakeControl.on = true;
  chassis.drive_to_point(-23.376, 50.39);
  chassis.turn_to_point(-10.067, 50.589);
  chassis.drive_to_point(-10.067, 50.589);
  chassis.turn_to_point(-15.232, 41.849, true);
  chassis.drive_to_point(-15.232, 41.849);
  chassis.turn_to_point(-10.266, 24.17);
  chassis.drive_max_voltage = 7;
  intakeControl.on = false;
  chassis.drive_to_point(-10.266, 24.17);
  chassis.drive_with_voltage(0,0);
}

void red_right_2_ring(){
  chassis.set_coordinates(-59.13, -23.701, -90);
  task intakeTask(intakeTaskFunction);
  intakeControl.team = false;
  intakeControl.on = false;
  odom_constants();
  chassis.turn_to_point(-23.773, -23.503, true);
  chassis.drive_to_point(-23.773, -23.503);
  goalclamp.set(true);
  wait(0.3, seconds);
  chassis.turn_to_point(-23.972, -46.743);
  intakeControl.on = true;
  chassis.drive_to_point(-23.972, -46.743);
  chassis.turn_to_point(-23.376, -10.194);
  chassis.drive_max_voltage = 7;
  chassis.drive_to_point(-23.376, -10.194);
  intakeControl.on = false;
  chassis.drive_with_voltage(0, 0);
}

void blue_right_4_ring(){
}

void blue_right_winpoint(){
}

void blue_left_2_ring(){
}

void skills(){
  task intakeTask(intakeTaskFunction);
  intakeControl.team = true;
  intakeControl.on = false;
  chassis.set_coordinates(-58.866, -0.016, 90);
  chassis.turn_to_point(-47.432, -23.826);
  chassis.drive_to_point(-47.432, -23.826);
  chassis.turn_to_point(-23.174, -23.257);
  chassis.drive_to_point(-23.174, -23.257);
  chassis.turn_to_point(-23.553, -47.326);
  chassis.drive_to_point(-23.553, -47.326);
  chassis.turn_to_point(-47.243, -47.136);
  chassis.drive_to_point(-47.243, -47.136);
  chassis.turn_to_point(-58.993, -47.136);
  chassis.drive_to_point(-58.993, -47.136);
  chassis.turn_to_point(-47.243, -59.076);
  chassis.drive_to_point(-47.243, -59.076);
  chassis.turn_to_point(-56.67, -56.303);
  chassis.drive_to_point(-56.67, -56.303);
  chassis.turn_to_point(-47.053, 23.553);
  chassis.drive_to_point(-47.053, 23.553);
  chassis.turn_to_point(-23.364, 23.553);
  chassis.drive_to_point(-23.364, 23.553);
  chassis.turn_to_point(-23.364, 47.243);
  chassis.drive_to_point(-23.364, 47.243);
  chassis.turn_to_point(-46.864, 47.243);
  chassis.drive_to_point(-46.864, 47.243);
  chassis.turn_to_point(-58.803, 47.243);
  chassis.drive_to_point(-58.803, 47.243);
  chassis.turn_to_point(-47.053, 59.372);
  chassis.drive_to_point(-47.053, 59.372);
  chassis.turn_to_point(-56.87, 56.87);
  chassis.drive_to_point(-56.87, 56.87);
  chassis.turn_to_point(0.136, 58.993);
  chassis.drive_to_point(0.136, 58.993);
  chassis.turn_to_point(23.636, 47.432);
  chassis.drive_to_point(23.636, 47.432);
  chassis.turn_to_point(47.321, -0.216);
  chassis.drive_to_point(47.321, -0.216);
  chassis.turn_to_point(47.895, -9.233);
  chassis.drive_to_point(47.895, -9.233);
  chassis.turn_to_point(58.318, -0.515);
  chassis.drive_to_point(58.318, -0.515);
  chassis.turn_to_point(59.834, 23.364);
  chassis.drive_to_point(59.834, 23.364);
  chassis.turn_to_point(57.101, 57.269);
  chassis.drive_to_point(57.101, 57.269);
  chassis.turn_to_point(23.768, -23.369);
  chassis.drive_to_point(23.768, -23.369);
  chassis.turn_to_point(23.826, -46.757);
  chassis.drive_to_point(23.826, -46.757);
  chassis.turn_to_point(56.502, -56.303);
  chassis.drive_to_point(56.502, -56.303);
  chassis.drive_with_voltage(0,0);
}
