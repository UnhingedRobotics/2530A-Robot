#include "drive-functions.h"
#include "vex.h"

using namespace vex;


void healthCheck() {
  while (true) {
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1,1);
    // Controller1.Screen.print(opticalsensor.hue());
    Controller1.Screen.print(intake.position(degrees));
    if (intake.temperature(fahrenheit) >= 110) {
      // Controller1.Screen.clearScreen();
      // Controller1.Screen.setCursor(1,1);
      // Controller1.Screen.print("Intake");
    }

    if (leftdrivefront.temperature(fahrenheit) >= 100) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(2,1);
      Controller1.Screen.print("LeftDriveFront");
    }
    
    if (leftdriveback.temperature(fahrenheit) >= 100) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(3,1);
      Controller1.Screen.print("LeftDriveBack");
    }

    if (rightdrivefront.temperature(fahrenheit) >= 100) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(4,1);
      Controller1.Screen.print("RightDriveFront");
    }

    if (rightdriveback.temperature(fahrenheit) >= 100) {
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,1);
      Controller1.Screen.print("RightDriveBack");
    }
    wait(5, seconds);
  }
}
