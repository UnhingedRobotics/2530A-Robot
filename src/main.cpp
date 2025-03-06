#include "vex.h"

using namespace vex;
competition Competition;

bool swing_on = false;
bool goal_on = true;

/*---------------------------------------------------------------------------*/
/*                             VEXcode Config                                */
/*                                                                           */
/*  Before you do anything else, start by configuring your motors and        */
/*  sensors. In VEXcode Pro V5, you can do this using the graphical          */
/*  configurer port icon at the top right. In the VSCode extension, you'll   */
/*  need to go to robot-config.cpp and robot-config.h and create the         */
/*  motors yourself by following the style shown. All motors must be         */
/*  properly reversed, meaning the drive should drive forward when all       */
/*  motors spin forward.                                                     */
/*---------------------------------------------------------------------------*/
IntakeControl intakeControl; // Define Intake Control here

/*---------------------------------------------------------------------------*/
/*                             JAR-Template Config                           */
/*                                                                           */
/*  Where all the magic happens. Follow the instructions below to input      */
/*  all the physical constants and values for your robot. You should         */
/*  already have configured your motors.                                     */
/*---------------------------------------------------------------------------*/


Drive chassis(

//Pick your drive setup from the list below:
//ZERO_TRACKER_NO_ODOM
//ZERO_TRACKER_ODOM
//TANK_ONE_FORWARD_ENCODER
//TANK_ONE_FORWARD_ROTATION
//TANK_ONE_SIDEWAYS_ENCODER
//TANK_ONE_SIDEWAYS_ROTATION
//TANK_TWO_ENCODER
//TANK_TWO_ROTATION
//HOLONOMIC_TWO_ENCODER
//HOLONOMIC_TWO_ROTATION
//
//Write it here:
ZERO_TRACKER_ODOM,

//Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
//You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".

//Left Motors:
motor_group(leftdrivefront,leftdrivemid,leftdriveback),

//Right Motors:
motor_group(rightdrivefront,rightdrivemid,rightdriveback),

ONE,
//Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
PORT13,

PORT7,

//Input your wheel diameter. (4" omnis are actually closer to 4.125"):
3.18,

//External ratio, must be in decimal, in the format of input teeth/output teeth.
//If your motor has an 84-tooth gear and your wheel has a 60-tooth gear, this value will be 1.4.
//If the motor drives the wheel directly, this value is 1:
1,

//Gyro scale, this is what your gyro reads when you spin the robot 360 degrees.
//For most cases 360 will do fine here, but this scale factor can be very helpful when precision is necessary.
358.2,

360,

/*---------------------------------------------------------------------------*/
/*                                  PAUSE!                                   */
/*                                                                           */
/*  The rest of the drive constructor is for robots using POSITION TRACKING. */
/*  If you are not using position tracking, leave the rest of the values as  */
/*  they are.                                                                */
/*---------------------------------------------------------------------------*/

//If you are using ZERO_TRACKER_ODOM, you ONLY need to adjust the FORWARD TRACKER CENTER DISTANCE.

//FOR HOLONOMIC DRIVES ONLY: Input your drive motors by position. This is only necessary for holonomic drives, otherwise this section can be left alone.
//LF:      //RF:    
PORT1,     -PORT2,

//LB:      //RB: 
PORT3,     -PORT4,

//If you are using position tracking, this is the Forward Tracker port (the tracker which runs parallel to the direction of the chassis).
//If this is a rotation sensor, enter it in "PORT1" format, inputting the port below.
//If this is an encoder, enter the port as an integer. Triport A will be a "1", Triport B will be a "2", etc.
15,

//Input the Forward Tracker diameter (reverse it to make the direction switch):
2.75,

//Input Forward Tracker center distance (a positive distance corresponds to a tracker on the right side of the robot, negative is left.)
//For a zero tracker tank drive with odom, put the positive distance from the center of the robot to the right side of the drive.
//This distance is in inches:
-1,

//Input the Sideways Tracker Port, following the same steps as the Forward Tracker Port:
7,

//Sideways tracker diameter (reverse to make the direction switch):
-2.75,

//Sideways tracker center distance (positive distance is behind the center of the robot, negative is in front):
-1.375

);

int current_auton_selection = 0;

/**
 * Function before autonomous. It prints the current auton number on the screen
 * and tapping the screen cycles the selected auton by 1. Add anything else you
 * may need, like resetting pneumatic components. You can rename these autons to
 * be more descriptive, if you like.
 */
void buttonAEventHandler() {
  if (!swing_on) {
    intakeControl.reverse = false;
  }
  else {
    intakeControl.reverse = true;
  }
}

void buttonBEventHandler() {
  if (!swing_on) {
    intakeControl.reverse = true;
  }
  else {
    intakeControl.reverse = false;
  }
}

void buttonYEventHandler() {
  if (swing_on == true) {
    intakeControl.reverse = false;
    swing_on = false;
    swingarm.set(false);
    leftdrivefront.stop(coast);
    leftdrivemid.stop(coast);
    leftdriveback.stop(coast);
    rightdrivefront.stop(coast);
    rightdrivemid.stop(coast);
    rightdriveback.stop(coast);
  }
  else{
    intakeControl.reverse = true;
    swing_on = true;
    swingarm.set(true);
    intakeControl.holding = false;
    leftdrivefront.stop(hold);
    leftdrivemid.stop(hold);
    leftdriveback.stop(hold);
    rightdrivefront.stop(hold);
    rightdrivemid.stop(hold);
    rightdriveback.stop(hold);
  }
}

void buttonL1EventHandler() {
  if (goal_on == true) {
    goal_on = false;
    goalclamp.set(true);
  }
  else {
    goal_on = true;
    goalclamp.set(false);
  }
}
void buttonR1EventHandler() {
  if (!intakeControl.on) {
    intakeControl.on = true;
  }
  else {
    intakeControl.on = false;
  }
}
void buttonR2EventHandler() {
  if (!intakeControl.roller_on) {
    intakeControl.roller_on = true;
  }
  else {
    intakeControl.roller_on = false;
  }
}

int intakeTaskFunctionUser() {
    while (true) {
      intakeControl.colorSorting();
      // Controller1.Screen.clearScreen();
      // Controller1.Screen.setCursor(1, 1);
      // Controller1.Screen.print(intake.position(degrees));
      wait(10, msec); // Small delay to avoid resource overuse
    }

    return 0; // End of the task
}

void pre_auton() {
  // Initializing Robot Configuration. DO NOT REMOVE
  vexcodeInit();
  default_constants();
  chassis.set_coordinates(0, 0, 0);
  thread healthTask(healthCheck);
  intake.spin(forward);
  intake.setVelocity(0, percent);
  intake.setStopping(coast);
  while(!intakeControl.auto_on){
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(5, 20, "JAR Template v1.2.0");
    Brain.Screen.printAt(5, 40, "Battery Percentage:");
    Brain.Screen.printAt(5, 60, "%d", Brain.Battery.capacity());
    Brain.Screen.printAt(5, 80, "Chassis X:");
    Brain.Screen.printAt(5, 100, "%f", chassis.get_X_position());
    Brain.Screen.printAt(5, 120, "Chassis Y:");
    Brain.Screen.printAt(5, 140, "%f", chassis.get_Y_position());
    Brain.Screen.printAt(5, 160, "Selected Auton:");
    switch(current_auton_selection){
      case 0:
        Brain.Screen.printAt(5, 180, "Test");
        break;
      case 1:
        Brain.Screen.printAt(5, 180, "Red Left 4 Ring");
        break;
      case 2:
        Brain.Screen.printAt(5, 180, "Red Left 2 Ring");
        break;
      case 3:
        Brain.Screen.printAt(5, 180, "Red Left Winpoint");
        break;
      case 4:
        Brain.Screen.printAt(5, 180, "Red Right 2 Ring");
        break;
      case 5:
        Brain.Screen.printAt(5, 180, "Blue Right 4 Ring");
        break;
      case 6:
        Brain.Screen.printAt(5, 180, "Blue Right 2 Ring");
        break;
      case 7:
        Brain.Screen.printAt(5, 180, "Blue Right Winpoint");
        break;
      case 8:
        Brain.Screen.printAt(5, 180, "Blue Left 2 Ring");
        break;
      case 9:
        Brain.Screen.printAt(5, 180, "Skills");
        break;
    }
    if(Brain.Screen.pressing()){
      while(Brain.Screen.pressing()) {}
      current_auton_selection ++;
    } else if (current_auton_selection == 10){
      current_auton_selection = 0;
    }
    task::sleep(10);
  }
}

/**
 * Auton function, which runs the selected auton. Case 0 is the default,
 * and will run in the brain screen goes untouched during preauton. Replace
 * drive_test(), for example, with your own auton function you created in
 * autons.cpp and declared in autons.h.
 */

void autonomous(void) {
  intakeControl.auto_on = true;
  switch(current_auton_selection){ 
    case 0:
      test();
      break;
    case 1:
      red_left_4_ring();
      break;
    case 2:
      red_left_2_ring();
      break;
    case 3:         
      red_left_winpoint();
      break;
    case 4:
      red_right_2_ring();
      break;
    case 5:
      blue_right_4_ring();
      break;
    case 6:
      blue_right_2_ring();
      break;
    case 7:         
      blue_right_winpoint();
      break;
    case 8:
      blue_left_2_ring();
      break;
    case 9:
      skills();
      break;
 }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
void usercontrol(void) {
  intakeControl.auto_on = false;
  thread intakeTask(intakeTaskFunctionUser);
  Controller1.ButtonA.pressed(buttonAEventHandler);
  Controller1.ButtonB.pressed(buttonBEventHandler);
  Controller1.ButtonL1.pressed(buttonL1EventHandler);
  Controller1.ButtonR1.pressed(buttonR1EventHandler);
  Controller1.ButtonR2.pressed(buttonR2EventHandler);
  Controller1.ButtonY.pressed(buttonYEventHandler);
  while (1) {
    chassis.control_tank(chassis.driveOveride);
    wait(10, msec); // Sleep the task for a short amount of time to prevent wasted resources
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
