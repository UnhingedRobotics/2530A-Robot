#include "drive-functions.h"
#include "vex.h"

using namespace vex;

// Constructor with initializer list to initialize member variables
IntakeControl::IntakeControl() :
  hue(0.0),
  ring(false),
  team(true),
  ringdetected(false),
  intakeon(false),
  holding(false),
  intakevelocity(0),
  mode(INTAKE_COLOR_SORT)
{}

void IntakeControl::setMode(Mode newMode) {
  mode = newMode;
}

void IntakeControl::colorSorting() { 
  if (intakeon) {
    // hue = opticalsensor.hue();

    
    if (!ringdetected) {
      aivisionsensor.takeSnapshot(aivisionsensor__bluering);
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,1);
      Controller1.Screen.print("red");
      if (aivisionsensor.objects[0].exists) {
        ring = true;
        ringdetected = true;
        intakevelocity = 75;
        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print("red");
      }
      else {
        ring = false;
      }
      aivisionsensor.takeSnapshot(aivisionsensor__bluering);
      if (aivisionsensor.objects[0].exists) {
        ring = false;
        ringdetected = true;
        intakevelocity = 75;
        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print("blue");
      }
      else {
        ring = true;
      }
      // if (hue < 30) { // for optical
	  }
    
    if (!ringdetected) {
      // Controller1.Screen.clearScreen();
      // Controller1.Screen.setCursor(1,1);
      // Controller1.Screen.print("ring not detected");
      intakevelocity = 75;
    }
    if (distancesensor.objectDistance(inches) < 2) {
      if (ringdetected) {
        if (team) {
          if (!ring) {
            if ((intake.position(degrees) >= 99 && intake.position(degrees) <= 101) || (intake.position(degrees) % 100 == 99 || intake.position(degrees) % 100 == 0 || intake.position(degrees) % 100 == 1)) {
			  intake.resetPosition();
			  intake.spin(reverse);
			  intake.setVelocity(100, percent);
            }
          }
        }
        else {
          if (ring) {
            if ((intake.position(degrees) >= 99 && intake.position(degrees) <= 101) || (intake.position(degrees) % 100 == 99 || intake.position(degrees) % 100 == 0 || intake.position(degrees) % 100 == 1)) {
			  intake.resetPosition();
			  intake.spin(reverse);
			  intake.setVelocity(100, percent);
            }
          }
        }
        ringdetected = false;
      }
      if (holding) {
        intake.stop(brake);
        intake.spin(forward);
        intakevelocity = -50;
        intake.setVelocity(intakevelocity, percent);
        intakeon = false;
        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print("held");  
      }
	  }
  }

  else {
    intakevelocity = 0;
  }
  intake.spin(forward);
  intake.setVelocity(intakevelocity, percent);
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1,1);
  Controller1.Screen.print(intake.position(degrees));  
}


void IntakeControl::update() {
  switch (mode) {
    case INTAKE_COLOR_SORT:
      if (auto_on) {
        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print("auto on");  
        time_waited = time_waited + 1;
        if (time_waited >= 10) {
          intakeon = true;
          time_waited = 0;
          Controller1.Screen.clearScreen();
          Controller1.Screen.setCursor(1,1);
          Controller1.Screen.print("time");  
        }   
      }
      colorSorting();
      break;
    case WALLSTAKE_HOLDING:
      if (distancesensor.objectDistance(inches) < 2) {
        intake.stop(brake);
		    intake.spin(forward);
		    intake.setVelocity(0, percent);
      }
      break;
  }
}

void ArmControl::set_arm_exit_conditions(float arm_settle_error, float arm_settle_time, float arm_timeout){
  this->arm_settle_error = arm_settle_error;
  this->arm_settle_time = arm_settle_time;
  this->arm_timeout = arm_timeout;
}

void ArmControl::set_arm_constants(float arm_max_voltage, float arm_kp, float arm_ki, float arm_kd, float arm_starti) {
  this->arm_max_voltage = arm_max_voltage;
  this->arm_kp = arm_kp;
  this->arm_ki = arm_ki;
  this->arm_kd = arm_kd;
  this->arm_starti = arm_starti;
}

void ArmControl::move_to_angle(float angle) {
  move_to_angle(angle, arm_max_voltage, arm_settle_error, arm_settle_time, arm_timeout, arm_kp, arm_ki, arm_kd, arm_starti);
}

void ArmControl::move_to_angle(float angle, float arm_max_voltage) {
  move_to_angle(angle, arm_max_voltage, arm_settle_error, arm_settle_time, arm_timeout, arm_kp, arm_ki, arm_kd, arm_starti);
}

void ArmControl::move_to_angle(float angle, float arm_max_voltage, float arm_settle_error, float arm_settle_time, float arm_timeout) {
  move_to_angle(angle, arm_max_voltage, arm_settle_error, arm_settle_time, arm_timeout, arm_kp, arm_ki, arm_kd, arm_starti);
}

void ArmControl::move_to_angle(float angle, float arm_max_voltage, float arm_settle_error, float arm_settle_time, float arm_timeout, float arm_kp, float arm_ki, float arm_kd, float arm_starti) {
  PID armPID((angle - fourBar.position(degrees)), arm_kp, arm_ki, arm_kd, arm_starti, arm_settle_error, arm_settle_time, arm_timeout);
  while (!pid_stop) {   
    float error = angle - fourBar.position(degrees);
    float output = armPID.compute(error);
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print(angle - fourBar.position(degrees));
    output = clamp(output, -arm_max_voltage, arm_max_voltage);
    fourBar.setVelocity(output, percent);
    task::sleep(10);
  }
  pid_stop = false;
  fourBar.stop(brake);
  fourBar.spin(forward);
  fourBar.setVelocity(0, percent);
}

void healthCheck() {
  while (true) {
    if (intake.temperature(fahrenheit) >= 105) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,1);
      Controller1.Screen.print("Intake too hot!");
      Controller1.rumble("..--");
    }

    if (leftdrivefront.temperature(fahrenheit) >= 105) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,2);
      Controller1.Screen.print("LeftDriveFront too hot!");
      Controller1.rumble("..--");
    }
    
    if (leftdriveback.temperature(fahrenheit) >= 105) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,3);
      Controller1.Screen.print("LeftDriveBack too hot!");
      Controller1.rumble("..--");
    }

    if (rightdrivefront.temperature(fahrenheit) >= 105) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,4);
      Controller1.Screen.print("RightDriveFront too hot!");
      Controller1.rumble("..--");
    }

    if (rightdriveback.temperature(fahrenheit) >= 105) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,1);
      Controller1.Screen.print("RightDriveBack too hot!");
      Controller1.rumble("..--");
    }
    wait(1, seconds);
  }
}
