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
  chassis.set_drive_constants(10, 1.2, 0, 10, 0);
  chassis.set_heading_constants(6, .4, 0, 1, 0);
  chassis.set_v_drive_constants(10, 1.2, 0, 10, 0);
  chassis.set_v_heading_constants(6, .4, 0, 1, 0);
  chassis.set_turn_constants(12, .4, .03, 3, 15);
  chassis.set_swing_constants(12, .3, .001, 2, 15);
<<<<<<< HEAD
  fishControl.set_arm_constants(12, .13, 0, 0.1, 0);
=======
  fishControl.set_arm_constants(12, .12, 0, 0.1, 0);
>>>>>>> 8959322eb653ff705eaa83418afd620b3e2e75d3

  // Each exit condition set is in the form of (settle_error, settle_time, timeout).
  chassis.set_drive_exit_conditions(1.5, 300, 5000);
  chassis.set_v_drive_exit_conditions(1.5, 300, 5000);
  chassis.set_turn_exit_conditions(1, 300, 3000);
  chassis.set_swing_exit_conditions(1, 300, 3000);
<<<<<<< HEAD
  fishControl.set_arm_exit_conditions(6, 100, 100);
=======
  fishControl.set_arm_exit_conditions(18, 200, 200);
>>>>>>> 8959322eb653ff705eaa83418afd620b3e2e75d3
}

/**
 * Sets constants to be more effective for odom movements.
 * For functions like drive_to_point(), it's often better to have
 * a slower max_voltage and greater settle_error than you would otherwise.
 */

void odom_constants(){
  default_constants();
  chassis.heading_max_voltage = 10;
  chassis.drive_max_voltage = 8;
  chassis.drive_settle_error = 3;
  chassis.boomerang_lead = .5;
  chassis.drive_min_voltage = 0;
}

/**
 * The expected behavior is to return to the start position.
 */

void red_left_elims(){
  fishMech.resetPosition();
  task intakeTask(intakeTaskFunction);
  intakeControl.team = true;
  intakeControl.intakeon = false;
<<<<<<< HEAD
  chassis.drive_distance(18);
  chassis.turn_to_angle(90);
  intakeControl.intakeMaxVelocity = 20;
  intakeControl.intakeon = true;
  fishControl.move_to_angle(150);
  intakeControl.intakeon = false;
  intakeControl.intakeMaxVelocity = 70;
  fishControl.move_to_angle(90);
  chassis.drive_distance(-5);
  chassis.turn_to_angle(50);
  chassis.drive_distance(-45);
=======
  chassis.drive_distance(14);
  chassis.turn_to_angle(90);
  intakeControl.intakeon = true;
  fishControl.move_to_angle(150);
  wait(0.1, seconds);
  intakeControl.intakeon = false;
  chassis.drive_distance(-5);
  fishControl.move_to_angle(160);
  chassis.turn_to_angle(45);
  chassis.drive_distance(-47);
>>>>>>> 8959322eb653ff705eaa83418afd620b3e2e75d3
  goalclamp.set(true);
  wait(0.1, seconds);
  chassis.turn_to_angle(180);
  intakeControl.intakeon = true;
<<<<<<< HEAD
  chassis.drive_distance(28);
  wait(0.2, seconds);
  intakeControl.intakeon = false;
  chassis.turn_to_angle(270);
  intakeControl.intakeon = true;
  chassis.drive_distance(22);
  wait(0.4, seconds);
  chassis.drive_distance(-13);
  chassis.turn_to_angle(300);
  chassis.drive_distance(14);
  wait(0.4, seconds);
  chassis.turn_to_angle(0);
  chassis.drive_distance(20);
  wait(2, seconds);
=======
  chassis.drive_distance(30);
>>>>>>> 8959322eb653ff705eaa83418afd620b3e2e75d3
  intakeControl.intakeon = false;
}

void red_left_winpoint(){
  task intakeTask(intakeTaskFunction);
  intakeControl.team = true;
  intakeControl.intakeon = false;
  chassis.drive_distance_mp(-53);
  goalclamp.set(true);
  wait(0.3, seconds);
  intakeControl.intakeon = true;
  chassis.drive_distance_mp(8);
  chassis.turn_to_angle(90);
  chassis.drive_distance_mp(34);
  wait(.5, seconds);
  chassis.drive_distance_mp(-11);
  chassis.turn_to_angle(180);
  chassis.drive_distance_mp(14);
  wait(.5, seconds);
  chassis.turn_to_angle(255);
  chassis.drive_distance_mp(15);
  intakeControl.intakeon = false;
}

void blue_left_elims(){
  task intakeTask(intakeTaskFunction);
  intakeControl.team = false;
  intakeControl.intakeon = false;
  chassis.drive_distance_mp(-53);
  goalclamp.set(true);
  wait(0.3, seconds);
  intakeControl.intakeon = true;
  chassis.drive_distance_mp(8);
  chassis.turn_to_angle(-90);
  chassis.drive_distance_mp(30);
  chassis.drive_distance_mp(4);
  wait(.5, seconds);
  chassis.drive_distance_mp(-11);
  chassis.turn_to_angle(-180);
  chassis.drive_distance_mp(14);
  wait(.5, seconds);
  chassis.turn_to_angle(-255);
  chassis.drive_distance_mp(15);
  intakeControl.intakeon = false;
}

void blue_left_winpoint(){
  task intakeTask(intakeTaskFunction);
  intakeControl.team = false;
  intakeControl.intakeon = false;
  chassis.drive_distance_mp(-53);
  goalclamp.set(true);
  wait(0.3, seconds);
  intakeControl.intakeon = true;
  chassis.drive_distance_mp(8);
  chassis.turn_to_angle(-90);
  chassis.drive_distance_mp(30);
  chassis.drive_distance_mp(4);
  wait(.5, seconds);
  intakeControl.intakeon = true;
  chassis.drive_distance_mp(-33);
  chassis.turn_to_angle(45);
  goalclamp.set(false);
  chassis.drive_distance_mp(37);
  wait(.3, seconds);
  intakeControl.intakeon = false;
  intakeControl.intakeon = true;
  chassis.drive_distance_mp(10);
  wait(.2, seconds);
  intakeControl.intakeon = false;
  chassis.turn_to_angle(90);
  chassis.drive_distance_mp(25);
  chassis.turn_to_angle(0);
  chassis.drive_distance_mp(-40);
  goalclamp.set(true);
  intakeControl.intakeon = true;
  chassis.turn_to_angle(-90);
  chassis.drive_distance_mp(17);
  intakeControl.intakeon = false;
}

void skills(){
  task intakeTask(intakeTaskFunction);
  intakeControl.team = true;
  intakeControl.intakeon = false;
  chassis.drive_distance_mp(-25);
  goalclamp.set(true);
  wait(0.3, seconds);
  chassis.drive_distance_mp(14);
  intakeControl.intakeon = true;
  chassis.turn_to_angle(90);
  chassis.drive_distance_mp(32);
  chassis.turn_to_angle(240);
  chassis.drive_distance_mp(-17);
  chassis.turn_to_angle(190);
  chassis.drive_distance_mp(-8);
  goalclamp.set(false);
  intakeControl.intakeon = false;
  chassis.drive_distance_mp(18);
  chassis.turn_to_angle(270);
  intakeControl.intakeon = true;
  chassis.drive_distance_mp(70);
  chassis.turn_to_angle(90);
  chassis.drive_distance_mp(-35);
  intakeControl.intakeon = false;
  goalclamp.set(true);
  wait(0.3, seconds);
  intakeControl.intakeon = true;
  chassis.turn_to_angle(-90);
  chassis.drive_distance_mp(32);
  wait(0.1, seconds);
  chassis.drive_distance_mp(8);
  chassis.drive_distance_mp(-5);
  chassis.turn_to_angle(150);
  chassis.drive_distance_mp(-15);
  goalclamp.set(false);
}
