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
  chassis.set_heading_constants(6.5, 0.8, 0, 9, 0);
  chassis.set_v_drive_constants(10, 1.2, 0, 10, 0);
  chassis.set_v_heading_constants(6, .4, 0, 1, 0);
  chassis.set_turn_constants(6.5, 0.8, 0, 9, 0);
  chassis.set_swing_constants(12, .3, .0013, 0, 15);

  // Each exit condition set is in the form of (settle_error, settle_time, timeout).
  chassis.set_drive_exit_conditions(0.6, 300, 5000);
  chassis.set_v_drive_exit_conditions(1.5, 300, 5000);
  chassis.set_turn_exit_conditions(0.5, 300, 3000);
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
void fast(){
  default_constants();
  chassis.heading_max_voltage = 12;
  chassis.drive_max_voltage = 12;
  chassis.turn_max_voltage = 12;
  chassis.drive_kp = 2;
  chassis.drive_kd = 1.4;
  chassis.heading_kp = .05;
  chassis.heading_kd = 0;
  chassis.turn_kp = .25;
  chassis.turn_kd = 2.2;
  chassis.drive_min_voltage = 0;
}
void fast_vel(){
  default_constants();
  chassis.heading_max_voltage = 12;
  chassis.drive_max_voltage = 12;
  chassis.turn_max_voltage = 12;
  chassis.drive_kp = 2;
  chassis.drive_kd = 1.4;
  chassis.heading_kp = .05;
  chassis.heading_kd = 0;
  chassis.turn_kp = .25;
  chassis.turn_kd = 2.2;
  chassis.v_drive_kp = 2;
  chassis.v_drive_kd = 0;
  chassis.v_heading_kp = 40;
  chassis.v_heading_kd = 0;
  chassis.drive_min_voltage = 0;
}
void test() {
  chassis.set_coordinates(0, 0, 0);
  fast();
  chassis.drive_to_point(0, 20);
  chassis.turn_to_angle(90);
  chassis.turn_to_angle(0);
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
  intakeControl.reverse = true;
  intakeControl.maxVelocity = 30;
  intakeControl.on = true;
  wait(0.5, seconds);
  intakeControl.on = false;
  intakeControl.reverse = false;
  intakeControl.maxVelocity = 100;
  intakeControl.stuckOveride = false;
  chassis.turn_to_point(-22.364, 23.553, true);
  chassis.drive_max_voltage = 9;
  chassis.drive_to_point(-22.364, 23.553);
  wait(0.3, seconds);
  goalclamp.set(true);
  chassis.turn_to_point(-23.364, 50.464);
  intakeControl.on = true;
  chassis.drive_to_point(-23.364, 50.464);
  chassis.turn_to_point(-9.15, 50.654);
  chassis.drive_to_point(-9.15, 50.654);
  chassis.turn_to_point(-17.678, 51.033, true);
  chassis.drive_to_point(-17.678, 51.033);
  chassis.turn_to_point(-17.299, 43.263);
  chassis.drive_to_point(-17.299, 43.263);
  chassis.turn_to_point(-9.15, 43.263, true);
  chassis.drive_to_point(-9.15, 43.263);
  chassis.turn_to_point(-9.718, 25.448);
  intakeControl.on = false;
  chassis.drive_max_voltage = 7;
  chassis.drive_to_point(-9.718, 25.448);
  chassis.turn_to_angle(170);
  chassis.drive_with_voltage(0,0);
}

void red_left_2_ring(){
  chassis.set_coordinates(-59.372, 23.364, -90);
  task intakeTask(intakeTaskFunction);
  intakeControl.team = false;
  intakeControl.on = false;
  odom_constants();
  intakeControl.reverse = true;
  intakeControl.maxVelocity = 30;
  intakeControl.on = true;
  wait(0.5, seconds);
  intakeControl.on = false;
  intakeControl.reverse = false;
  intakeControl.maxVelocity = 100;
  intakeControl.stuckOveride = false;
  chassis.turn_to_point(-22.795, 23.364, true);
  chassis.drive_to_point(-22.795, 23.364);
  goalclamp.set(true);
  wait(0.3, seconds);
  chassis.turn_to_point(-23.364, 50.464);
  intakeControl.on = true;
  chassis.drive_to_point(-23.364, 50.464);
  wait(1, seconds);
  chassis.turn_to_point(-23.743, 9.908);
  chassis.drive_max_voltage = 7;
  chassis.drive_to_point(-23.743, 9.908);
  intakeControl.on = false;
  chassis.drive_with_voltage(0, 0);
}

void red_left_winpoint(){
  chassis.set_coordinates(-52.575, 12.053, 180);
  task intakeTask(intakeTaskFunction);
  intakeControl.team = false;
  intakeControl.on = false;
  odom_constants();
  intakeControl.reverse = true;
  intakeControl.maxVelocity = 30;
  intakeControl.on = true;
  wait(0.5, seconds);
  intakeControl.on = false;
  intakeControl.reverse = false;
  intakeControl.maxVelocity = 100;
  intakeControl.stuckOveride = false;
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
  wait(0.6, seconds);
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
  intakeControl.reverse = true;
  intakeControl.maxVelocity = 30;
  intakeControl.on = true;
  wait(0.5, seconds);
  intakeControl.on = false;
  intakeControl.reverse = false;
  intakeControl.maxVelocity = 100;
  intakeControl.stuckOveride = false;
  // chassis.turn_to_point(-23.773, -23.503, true);
  chassis.drive_to_point(-23.773, -23.503);
  chassis.drive_max_voltage = 4;
  goalclamp.set(true);
  wait(0.3, seconds);
  chassis.turn_to_point(-23.972, -46.743);
  intakeControl.on = true;
  chassis.drive_to_point(-23.972, -46.743);
  wait(1, seconds);
  chassis.turn_to_point(-23.376, -10.194);
  chassis.drive_max_voltage = 7;
  chassis.drive_to_point(-23.376, -10.194);
  intakeControl.on = false;
  chassis.drive_with_voltage(0, 0);
}

void blue_right_4_ring(){
  chassis.set_coordinates(59.076, 23.553, 90);
  task intakeTask(intakeTaskFunction);
  intakeControl.team = true;
  intakeControl.on = false;
  odom_constants();
  intakeControl.reverse = true;
  intakeControl.maxVelocity = 30;
  intakeControl.on = true;
  wait(0.5, seconds);
  intakeControl.on = false;
  intakeControl.reverse = false;
  intakeControl.maxVelocity = 100;
  intakeControl.stuckOveride = false;
  chassis.turn_to_point(22.499, 23.553, true);
  chassis.drive_to_point(22.499, 23.553);
  goalclamp.set(true);
  wait(0.3, seconds);
  chassis.turn_to_point(24.205, 51.033);
  intakeControl.on = true;
  chassis.drive_to_point(24.205, 51.033);
  chassis.turn_to_point(11.318, 50.843);
  chassis.drive_to_point(11.318, 50.843);
  chassis.turn_to_point(18.899, 50.843, true);
  chassis.drive_to_point(18.899, 50.843);
  chassis.turn_to_point(19.088, 42.126);
  chassis.drive_to_point(19.088, 42.126);
  chassis.turn_to_point(10.37, 42.126);
  chassis.drive_to_point(10.37, 42.126);
  chassis.turn_to_point(9.612, 22.226);
  chassis.drive_max_voltage = 7;
  chassis.drive_to_point(9.612, 22.226);
  intakeControl.on = false;
  chassis.drive_with_voltage(0, 0);
}
void blue_right_2_ring() {
  chassis.set_coordinates(60.024, 23.743, 90);
  task intakeTask(intakeTaskFunction);
  intakeControl.team = true;
  intakeControl.on = false;
  odom_constants();
  intakeControl.reverse = true;
  intakeControl.maxVelocity = 30;
  intakeControl.on = true;
  wait(0.5, seconds);
  intakeControl.on = false;
  intakeControl.reverse = false;
  intakeControl.maxVelocity = 100;
  intakeControl.stuckOveride = false;
  chassis.turn_to_point(21.931, 23.553, true);
  chassis.drive_to_point(21.931, 23.553);
  goalclamp.set(true);
  wait(0.3, seconds);
  chassis.turn_to_point(23.447, 52.36);
  intakeControl.on = true;
  chassis.drive_to_point(23.447, 52.36);
  wait(1, seconds);
  chassis.turn_to_point(23.447, 10.666);
  chassis.drive_max_voltage = 7;
  chassis.drive_to_point(23.447, 10.666);
  intakeControl.on = false;
  chassis.drive_with_voltage(0, 0);
}

void blue_right_winpoint(){

}

void blue_left_2_ring(){
  chassis.set_coordinates(58.886, -23.636, 90);
  task intakeTask(intakeTaskFunction);
  intakeControl.team = true;
  intakeControl.on = false;
  odom_constants();
  intakeControl.reverse = true;
  intakeControl.maxVelocity = 30;
  intakeControl.on = true;
  wait(0.5, seconds);
  intakeControl.on = false;
  intakeControl.reverse = false;
  intakeControl.maxVelocity = 100;
  intakeControl.stuckOveride = false;
  chassis.turn_to_point(22.636, -23.257, true);
  chassis.drive_to_point(22.636, -23.257);
  goalclamp.set(true);
  wait(0.3, seconds);
  chassis.turn_to_point(23.636, -48.084);
  intakeControl.on = true;
  chassis.drive_to_point(23.636, -48.084);
  wait(1, seconds);
  chassis.turn_to_point(23.826, -10.56);
  chassis.drive_max_voltage = 7;
  chassis.drive_to_point(23.826, -10.56);
  intakeControl.on = false;
  chassis.drive_with_voltage(0, 0);
}

void skills(){
  chassis.set_coordinates(-60.063, 23.537, -90);
  odom_constants();
  chassis.turn_to_point(-47.089, 23.137, true);
  chassis.drive_to_point(-47.089, 23.137);
  goalclamp.set(true);
  wait(1, seconds);
  chassis.turn_to_point(-47.089, 47.089);
  chassis.drive_to_point(-47.089, 47.089);
  chassis.drive_max_voltage = 11;
  chassis.turn_to_point(-57.666, 57.476, true);
  chassis.drive_to_point(-57.66, 57.476);
  goalclamp.set(false);
  wait(1, seconds);
  chassis.drive_max_voltage = 10;
  chassis.turn_to_point(-47.089, 47.289);
  chassis.drive_to_point(-47.089, 47.289);
  chassis.turn_to_point(-47.089, -23.169, true);
  chassis.drive_to_point(-47.089, -23.169);
  goalclamp.set(true);
  wait(1, seconds);
  chassis.turn_to_point(-47.289, -46.922, true);
  chassis.drive_to_point(-47.289, -46.922);
  chassis.turn_to_point(-58.045, -57.37, true);
  chassis.drive_max_voltage = 11;
  chassis.drive_to_point(-58.045, -57.37);
  goalclamp.set(false);
  wait(1, seconds);
  chassis.turn_to_point(-38.714, -37.471);
  chassis.drive_max_voltage = 10;
  chassis.drive_to_point(-38.714, -37.471);
  chassis.turn_to_point(46.189, -37.282);
  chassis.drive_to_point(46.189, -37.282);
  chassis.turn_to_point(58.507, -23.636, true);
  chassis.drive_to_point(58.507, -23.636);
  goalclamp.set(true);
  wait(1, seconds);
  chassis.turn_to_point(57.749, -57.56, true);
  chassis.drive_to_point(57.749, -57.56);
  chassis.turn_to_angle(-45);
  goalclamp.set(false);
  wait(1, seconds);
  chassis.turn_to_point(55.854, -4.008, true);
  chassis.drive_max_voltage = 12;
  chassis.drive_to_point(58.498, -4.008);
  chassis.turn_to_point(65.899, 62.214);
  chassis.drive_max_voltage = 12;
  chassis.drive_to_point(65.899, 62.214);
  chassis.drive_with_voltage(0,0);
}
