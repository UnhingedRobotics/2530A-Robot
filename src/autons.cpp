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
  chassis.set_drive_constants(10, 1.1, 0, 10, 0);
  chassis.set_heading_constants(7, 1, 0, 2, 0);
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

void red_left_elims(){
  chassis.set_coordinates(-59.13, -23.701, -90);
  task intakeTask(intakeTaskFunction);
  intakeControl.team = false;
  intakeControl.intakeon = false;
  odom_constants();
  chassis.turn_to_point(-58.733, -23.701, false);
  chassis.drive_to_point(-22.78, -23.701);
  goalclamp.set(true);
  wait(0.3, seconds);
  chassis.turn_to_point(-22.78, -46.545);
  intakeControl.intakeon = true;
  chassis.drive_to_point(-22.78, -46.545);
  chassis.turn_to_point(-23.376, -13.372);
  chassis.drive_to_point(-23.376, -13.372);
  intakeControl.intakeon = false;
  chassis.drive_with_voltage(0,0);
}

void red_left_winpoint(){
  task intakeTask(intakeTaskFunction);
  intakeControl.team = true;
  intakeControl.intakeon = false;
  chassis.drive_distance(-45);
  goalclamp.set(true);
  wait(0.2, seconds);
  chassis.turn_to_angle(-83);
  intakeControl.intakeMaxVelocity = 70;
  intakeControl.intakeon = true;
  chassis.drive_distance(20);
  chassis.turn_to_angle(-20);
  chassis.drive_distance(40);
  wait(0.2, seconds);
  chassis.turn_to_angle(-170);
  chassis.turn_to_angle(-230);
  chassis.drive_distance(-10);
  intakeControl.intakeon = false;
}

void blue_left_elims(){
  chassis.set_coordinates(-59.13, 24.567, -90);
  task intakeTask(intakeTaskFunction);
  intakeControl.team = false;
  intakeControl.intakeon = false;
  odom_constants();
  chassis.turn_to_point(-23.773, 24.369, true);
  chassis.drive_to_point(-23.773, 24.369);
  goalclamp.set(true);
  wait(0.3, seconds);
  chassis.turn_to_point(-23.972, 51.185);
  intakeControl.intakeon = true;
  chassis.drive_to_point(-23.972, 51.185);
  chassis.turn_to_point(-8.279, 50.788);
  chassis.drive_to_point(-8.279, 50.788);
  chassis.turn_to_point(-16.73, 42.694, true);
  chassis.drive_to_point(-16.73, 42.694);
  chassis.turn_to_point(-7.485, 43.637);
  chassis.drive_to_point(-7.485, 43.637);
  chassis.turn_to_point(-10.666, 24.311);
  intakeControl.intakeon = false;
  chassis.drive_to_point(-10.666, 24.311);
  chassis.drive_with_voltage(0,0);


}

void blue_left_winpoint(){
}

void skills(){
  task intakeTask(intakeTaskFunction);
  intakeControl.team = true;
  intakeControl.intakeon = false;
  chassis.set_coordinates(-60.888, 23.364, 90);

  chassis.turn_to_point(-46.864, 23.364);
  chassis.drive_to_point(-46.864, 23.364);
  chassis.turn_to_point(-22.226, 22.606);
  chassis.drive_to_point(-22.226, 22.606);
  chassis.turn_to_point(-22.226, 47.432);
  chassis.drive_to_point(-22.226, 47.432);
  chassis.turn_to_point(0.326, 56.718);
  chassis.drive_to_point(0.326, 56.718);
  chassis.turn_to_point(-0.053, 51.222);
  chassis.drive_to_point(-0.053, 51.222);
  chassis.turn_to_point(-46.485, 47.243);
  chassis.drive_to_point(-46.485, 47.243);
  chassis.turn_to_point(-46.485, 59.182);
  chassis.drive_to_point(-46.485, 59.182);
  chassis.turn_to_point(-58.993, 59.751);
  chassis.drive_to_point(-58.993, 59.751);
  chassis.turn_to_point(-54.634, -0.136);
  chassis.drive_to_point(-54.634, -0.136);
  chassis.turn_to_point(-46.674, -24.774);
  chassis.drive_to_point(-46.674, -24.774);
  chassis.turn_to_point(-22.985, -23.068);
  chassis.drive_to_point(-22.985, -23.068);
  chassis.turn_to_point(-23.174, -47.326);
  chassis.drive_to_point(-23.174, -47.326);
  chassis.turn_to_point(-59.561, -46.947);
  chassis.drive_to_point(-59.561, -46.947);
  chassis.turn_to_point(-46.864, -59.645);
  chassis.drive_to_point(-46.864, -59.645);
  chassis.turn_to_point(-60.13, -60.213);
  chassis.drive_to_point(-60.13, -60.213);
  chassis.turn_to_point(0.136, -56.423);
  chassis.drive_to_point(0.136, -56.423);
  chassis.turn_to_point(23.636, -23.447);
  chassis.drive_to_point(23.636, -23.447);
  chassis.turn_to_point(45.241, -2.032);
  chassis.drive_to_point(45.241, -2.032);
  chassis.turn_to_point(23.826, -47.516);
  chassis.drive_to_point(23.826, -47.516);
  chassis.turn_to_point(59.076, -46.947);
  chassis.drive_to_point(59.076, -46.947);
  chassis.turn_to_point(48.653, -58.507);
  chassis.drive_to_point(48.653, -58.507);
  chassis.turn_to_point(60.213, -60.213);
  chassis.drive_to_point(60.213, -60.213);
  chassis.turn_to_point(38.04, 9.529);
  chassis.drive_to_point(38.04, 9.529);
  chassis.turn_to_point(58.318, 22.226);
  chassis.drive_to_point(58.318, 22.226);
  chassis.turn_to_point(57.56, 45.726);
  chassis.drive_to_point(57.56, 45.726);
  chassis.turn_to_point(57.939, 58.424);
  chassis.drive_to_point(57.939, 58.424);
  chassis.drive_with_voltage(0,0);


}
