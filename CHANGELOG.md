# Changelog

## [1.0.0] - 2024-9-2
### Added
- Initial release, custom PD lateral controller and PD angular controller.
- Odometry code.
- Fixes for syntax issues with code.

## [1.0.1] - 2024-9-6
### Changed
- Controller, added deadzones.
- Updated constants.
- Fixed infinite loop errors.

## [1.1.0] - 2024-9-7
### Added
- Integral to PID for angular and lateral controllers.
- Slew to lateral controller.
- Weights and biases to gyro measurements.

## [1.1.1] - 2024-9-8
### Updated
- Slewrate bias.
- Motor and sensor ports.
- Changed stopping to brake.

## [1.1.2] - 2024-9-21
### Updated
- Limits to derivative.
- Motor and sensor ports.
- Sensor naming.

## [1.2.0] - 2024-9-22
### Added
- Deadband to controller.
- Quadratic controller scheme.

## [1.2.1] - 2024-9-23 to 2024-9-29
### Updated
- Variable naming.
- Correct adjustments for distances with lateral PID.
- Logs for fixing lateral PID.

## [2.0.0] - 2024-10-15
### Added
- Rebased to JAR Template.
- Correct names of motors and sensors.

## [2.1.0] - 2024-10-16
### Added
- Controller Auton Switcher.
- Two team color sorting with intake.

## [2.1.1] - 2024-10-18
### Updated
- Ring Detection Logic.
- Ring position storing.

## [2.1.2] - 2024-10-19
### Updated
- Color sorting to be a class.
- Definitions of drive functions file.
- Header file for drive functions.

## [2.1.3] - 2024-10-20
### Updated
- Color sorting detection logic.
- Optical sensor detection.

## [2.2.0] - 2024-10-24
### Added
- Quadratic Controller.
- Various driving related fixes.

## [2.3.0] - 2024-10-28
### Added
- Pure Pursuit algorithm with odometry.
- Color detection code changed again.
- Four Bar Arm.
- Four Bar Arm PID.

## [2.4.0] - 2024-10-28
### Added
- State Loops with controller.
- Exit conditions for PID.
- Controller buttons.

## [2.5.0] - 2024-11-1 to 2024-11-2
### Added
- Initial Auton Routine.
- Corrected Auton names.
- Goal Clamp Controller code.

## [2.5.1] - 2024-11-3
### Updated
- Timing with controller buttons.
- Event handlers to controller to make code more efficient. 
- Four Bar PID to run in the background.
- Auton to get winpoint.

## [2.6.0] - 2024-11-9
### Added
- Initial Motion Profiling config.
- Fixed odometry config to run with drivetrain encoders.

## [2.6.1] - 2024-11-10
### Updated
- Velocity conversion  with motion profiling.
- Function for feed forward loop and PID.
- Robot motor and snesor ports.
- Velocity of intake.

## [2.6.2] - 2024-11-13
### Updated
- Auton routes such as for elims.
- Motion profiling computation.

## [2.6.3] - 2024-11-15
### Updated
- Auton code and defentions.
- Auton routes, added basic skills.
- Feed forward and PID controller with trapezoidal motion profile function.
- Object detection moved from optical sensor to AI vision sensor.

## [2.6.4] - 2024-11-18
### Updated
- Auton routes.
- Completely added skills route.

## [2.7.0] - 2024-11-19 to 2024-11-23
### Added
- Testing to motion profiling.
- Motion profiling constant tuning.

## [2.7.1] - 2024-11-24
### Updated
- Motion profiling deadband.
- Various changes to comply with Apache License.
- Fixing this file.
