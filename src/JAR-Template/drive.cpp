#include "vex.h"

MP mp;
/**
 * Drive constructor for the chassis.
 * Even though there's only one constructor, there can be
 * huge differences in implementation depending on the drive style
 * selected.Example Conversion
 * 
 * @param drive_setup The style of drive, such as TANK_TWO_ROTATION.
 * @param DriveL Left motor group.
 * @param DriveR Right motor group.
 * @param gyro_port IMU port.
 * @param wheel_diameter Wheel diameter in inches.
 * @param wheel_ratio External drive gear ratio.
 * @param gyro_scale Scale factor in degrees.
 * @param DriveLF_port Left front port for holonomic drives.
 * @param DriveRF_port Right front port for holonomic drives.
 * @param DriveLB_port Left back port for holonomic drives.
 * @param DriveRB_port Right back port for holonomic drives.
 * @param ForwardTracker_port Port for the forward tracker.
 * @param ForwardTracker_diameter Diameter in inches.
 * @param ForwardTracker_center_distance Horizontal distance in inches.
 * @param SidewaysTracker_port Port for the sideways tracker.
 * @param SidewaysTracker_diameter Diameter in inches.
 * @param SidewaysTracker_center_distance Vertical distance in inches.
 */

Drive::Drive(enum::drive_setup drive_setup, motor_group DriveL, motor_group DriveR, enum::gyro_setup gyro_setup, 
int gyro_one_port,  int gyro_two_port, float wheel_diameter, float wheel_ratio, float gyro_one_scale, float gyro_two_scale, 
int DriveLF_port, int DriveRF_port, int DriveLB_port, int DriveRB_port, 
int ForwardTracker_port, float ForwardTracker_diameter, float ForwardTracker_center_distance, 
int SidewaysTracker_port, float SidewaysTracker_diameter, float SidewaysTracker_center_distance) :
  driveOveride(false),
  wheel_diameter(wheel_diameter),
  wheel_ratio(wheel_ratio),
  gyro_one_scale(gyro_one_scale),
  gyro_two_scale(gyro_two_scale),
  drive_in_to_deg_ratio(wheel_ratio/360.0*M_PI*wheel_diameter),
  ForwardTracker_center_distance(ForwardTracker_center_distance),
  ForwardTracker_diameter(ForwardTracker_diameter),
  ForwardTracker_in_to_deg_ratio(M_PI*ForwardTracker_diameter/360.0),
  SidewaysTracker_center_distance(SidewaysTracker_center_distance),
  SidewaysTracker_diameter(SidewaysTracker_diameter),
  SidewaysTracker_in_to_deg_ratio(M_PI*SidewaysTracker_diameter/360.0),
  drive_setup(drive_setup),
  DriveL(DriveL),
  DriveR(DriveR),
  gyro_setup(gyro_setup),
  GyroOne(inertial(gyro_one_port)),
  GyroTwo(inertial(gyro_two_port)),
  DriveLF(abs(DriveLF_port), is_reversed(DriveLF_port)),
  DriveRF(abs(DriveRF_port), is_reversed(DriveRF_port)),
  DriveLB(abs(DriveLB_port), is_reversed(DriveLB_port)),
  DriveRB(abs(DriveRB_port), is_reversed(DriveRB_port)),
  R_ForwardTracker(ForwardTracker_port),
  R_SidewaysTracker(SidewaysTracker_port),
  E_ForwardTracker(ThreeWire.Port[to_port(ForwardTracker_port)]),
  E_SidewaysTracker(ThreeWire.Port[to_port(SidewaysTracker_port)])
{
    if (drive_setup == TANK_ONE_FORWARD_ENCODER || drive_setup == TANK_ONE_FORWARD_ROTATION || drive_setup == ZERO_TRACKER_ODOM){
      odom.set_physical_distances(ForwardTracker_center_distance, 0);
    } 
    if (drive_setup == TANK_ONE_SIDEWAYS_ENCODER || drive_setup == TANK_ONE_SIDEWAYS_ROTATION || 
    drive_setup == TANK_TWO_ENCODER || drive_setup == TANK_TWO_ROTATION ||
    drive_setup == HOLONOMIC_TWO_ENCODER || drive_setup == HOLONOMIC_TWO_ROTATION){
      odom.set_physical_distances(ForwardTracker_center_distance, SidewaysTracker_center_distance);
    }
}

/**
 * Drives each side of the chassis at the specified voltage.
 * 
 * @param leftVoltage Voltage out of 12.
 * @param rightVoltage Voltage out of 12.
 */

void Drive::drive_with_voltage(float leftVoltage, float rightVoltage){
  DriveL.spin(fwd, leftVoltage, volt);
  DriveR.spin(fwd, rightVoltage,volt);
}

/**
 * Resets default turn constants.
 * Turning includes turn_to_angle() and turn_to_point().
 * 
 * @param turn_max_voltage Max voltage out of 12.
 * @param turn_kp Proportional constant.
 * @param turn_ki Integral constant.
 * @param turn_kd Derivative constant.
 * @param turn_starti Minimum angle in degrees for integral to begin.
 */

void Drive::set_turn_constants(float turn_max_voltage, float turn_kp, float turn_ki, float turn_kd, float turn_starti){
  this->turn_max_voltage = turn_max_voltage;
  this->turn_kp = turn_kp;
  this->turn_ki = turn_ki;
  this->turn_kd = turn_kd;
  this->turn_starti = turn_starti;
} 

/**
 * Resets default drive constants.
 * Driving includes drive_distance(), drive_to_point(), and
 * holonomic_drive_to_point().
 * 
 * @param drive_max_voltage Max voltage out of 12.
 * @param drive_kp Proportional constant.
 * @param drive_ki Integral constant.
 * @param drive_kd Derivative constant.
 * @param drive_starti Minimum distance in inches for integral to begin
 */

void Drive::set_drive_constants(float drive_max_voltage, float drive_kp, float drive_ki, float drive_kd, float drive_starti){
  this->drive_max_voltage = drive_max_voltage;
  this->drive_kp = drive_kp;
  this->drive_ki = drive_ki;
  this->drive_kd = drive_kd;
  this->drive_starti = drive_starti;
} 

void Drive::set_v_drive_constants(float v_drive_max_voltage, float v_drive_kp, float v_drive_ki, float v_drive_kd, float v_drive_starti){
  this->v_drive_max_voltage = v_drive_max_voltage;
  this->v_drive_kp = v_drive_kp;
  this->v_drive_ki = v_drive_ki;
  this->v_drive_kd = v_drive_kd;
  this->v_drive_starti = v_drive_starti;
} 

/**
 * Resets default heading constants.
 * Heading control keeps the robot facing the right direction
 * and is part of drive_distance(), drive_to_point(), and
 * holonomic_drive_to_point.
 * 
 * @param heading_max_voltage Max voltage out of 12.
 * @param heading_kp Proportional constant.
 * @param heading_ki Integral constant.
 * @param heading_kd Derivative constant.
 * @param heading_starti Minimum angle in degrees for integral to begin.
 */

void Drive::set_heading_constants(float heading_max_voltage, float heading_kp, float heading_ki, float heading_kd, float heading_starti){
  this->heading_max_voltage = heading_max_voltage;
  this->heading_kp = heading_kp;
  this->heading_ki = heading_ki;
  this->heading_kd = heading_kd;
  this->heading_starti = heading_starti;
}

void Drive::set_v_heading_constants(float v_heading_max_voltage, float v_heading_kp, float v_heading_ki, float v_heading_kd, float v_heading_starti){
  this->v_heading_max_voltage = v_heading_max_voltage;
  this->v_heading_kp = v_heading_kp;
  this->v_heading_ki = v_heading_ki;
  this->v_heading_kd = v_heading_kd;
  this->v_heading_starti = v_heading_starti;
}

/**
 * Resets default swing constants.
 * Swing control holds one side of the drive still and turns with the other.
 * Only left_swing_to_angle() and right_swing_to_angle() use these constants.
 * 
 * @param swing_max_voltage Max voltage out of 12.
 * @param swing_kp Proportional constant.
 * @param swing_ki Integral constant.
 * @param swing_kd Derivative constant.
 * @param swing_starti Minimum angle in degrees for integral to begin.
 */

void Drive::set_swing_constants(float swing_max_voltage, float swing_kp, float swing_ki, float swing_kd, float swing_starti){
  this->swing_max_voltage = swing_max_voltage;
  this->swing_kp = swing_kp;
  this->swing_ki = swing_ki;
  this->swing_kd = swing_kd;
  this->swing_starti = swing_starti;
} 

/**
 * Resets default turn exit conditions.
 * The robot exits when error is less than settle_error for a duration of settle_time, 
 * or if the function has gone on for longer than timeout.
 * 
 * @param turn_settle_error Error to be considered settled in degrees.
 * @param turn_settle_time Time to be considered settled in milliseconds.
 * @param turn_timeout Time before quitting and move on in milliseconds.
 */

void Drive::set_turn_exit_conditions(float turn_settle_error, float turn_settle_time, float turn_timeout){
  this->turn_settle_error = turn_settle_error;
  this->turn_settle_time = turn_settle_time;
  this->turn_timeout = turn_timeout;
}

/**
 * Resets default drive exit conditions.
 * The robot exits when error is less than settle_error for a duration of settle_time, 
 * or if the function has gone on for longer than timeout.
 * 
 * @param drive_settle_error Error to be considered settled in inches.
 * @param drive_settle_time Time to be considered settled in milliseconds.
 * @param drive_timeout Time before quitting and move on in milliseconds.
 */

void Drive::set_drive_exit_conditions(float drive_settle_error, float drive_settle_time, float drive_timeout){
  this->drive_settle_error = drive_settle_error;
  this->drive_settle_time = drive_settle_time;
  this->drive_timeout = drive_timeout;
}

void Drive::set_v_drive_exit_conditions(float v_drive_settle_error, float v_drive_settle_time, float v_drive_timeout){
  this->v_drive_settle_error = v_drive_settle_error;
  this->v_drive_settle_time = v_drive_settle_time;
  this->v_drive_timeout = v_drive_timeout;
}

/**
 * Resets default swing exit conditions.
 * The robot exits when error is less than settle_error for a duration of settle_time, 
 * or if the function has gone on for longer than timeout.
 * 
 * @param swing_settle_error Error to be considered settled in degrees.
 * @param swing_settle_time Time to be considered settled in milliseconds.
 * @param swing_timeout Time before quitting and move on in milliseconds.
 */

void Drive::set_swing_exit_conditions(float swing_settle_error, float swing_settle_time, float swing_timeout){
  this->swing_settle_error = swing_settle_error;
  this->swing_settle_time = swing_settle_time;
  this->swing_timeout = swing_timeout;
}

/**
 * Gives the drive's absolute heading with Gyro correction.
 * 
 * @return Gyro scale-corrected heading in the range [0, 360).
 */

float Drive::get_rotation(){ 
  if (gyro_setup==ONE){
    return((GyroOne.rotation()*360.0/gyro_one_scale));
  }
  else {
    return( ( (GyroOne.rotation()*360.0/gyro_one_scale + GyroTwo.rotation()*360.0/gyro_two_scale) / 2 ) );
  }
}

float Drive::get_heading(){ 
  if (gyro_setup==ONE){
    return(reduce_0_to_360(GyroOne.rotation()*360.0/gyro_one_scale));
  }
  else {
    return(reduce_0_to_360((GyroOne.rotation()*360.0/gyro_one_scale + GyroTwo.rotation()*360.0/gyro_two_scale)/2));
  }
}

/**
 * Gets the motor group's position and converts to inches.
 * 
 * @return Left position in inches.
 */

float Drive::get_left_position_in(){
  return( DriveL.position(deg)*drive_in_to_deg_ratio );
}

float Drive::get_left_velocity_ins(){
  return( (DriveL.velocity(dps)*drive_in_to_deg_ratio * M_PI * wheel_diameter) / 360 );
}

/**
 * Gets the motor group's position and converts to inches.
 * 
 * @return Right position in inches.
 */

float Drive::get_right_position_in(){
  return( DriveR.position(deg)*drive_in_to_deg_ratio );
}

float Drive::get_right_velocity_ins(){
  return( (DriveR.velocity(dps)*drive_in_to_deg_ratio * M_PI * wheel_diameter) / 360  );
}

/**
 * Stops both sides of the drive with the desired mode.
 * 
 * @param mode hold, brake, or stop
 */

void Drive::drive_stop(vex::brakeType mode){
  DriveL.stop(mode);
  DriveR.stop(mode);
}

/**
 * Turns the robot to a field-centric angle.
 * Optimizes direction, so it turns whichever way is closer to the 
 * current heading of the robot.
 * 
 * @param angle Desired angle in degrees.
 */

void Drive::turn_to_angle(float angle){
  turn_to_angle(angle, turn_max_voltage, turn_settle_error, turn_settle_time, turn_timeout, turn_kp, turn_ki, turn_kd, turn_starti);
}

void Drive::turn_to_angle(float angle, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout, float turn_kp, float turn_ki, float turn_kd, float turn_starti){
  PID turnPID(angle - get_rotation(), turn_kp, turn_ki, turn_kd, turn_starti, turn_settle_error, turn_settle_time, turn_timeout);
  while( !turnPID.is_settled() ){
    float error = angle - get_rotation();
    float output = turnPID.compute(error);
    output = clamp(output, -turn_max_voltage, turn_max_voltage);
    drive_with_voltage(output, -output);
    task::sleep(10);
  }
  drive_with_voltage(0,0);
}

void Drive::turn_to_heading(float angle){
  turn_to_angle(angle, turn_max_voltage, turn_settle_error, turn_settle_time, turn_timeout, turn_kp, turn_ki, turn_kd, turn_starti);
}

void Drive::turn_to_heading(float angle, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout, float turn_kp, float turn_ki, float turn_kd, float turn_starti){
  PID turnPID(reduce_negative_180_to_180(angle - get_rotation()), turn_kp, turn_ki, turn_kd, turn_starti, turn_settle_error, turn_settle_time, turn_timeout);
  while( !turnPID.is_settled() ){
    float error = reduce_negative_180_to_180(angle - get_rotation());
    float output = turnPID.compute(error);
    output = clamp(output, -turn_max_voltage, turn_max_voltage);
    drive_with_voltage(output, -output);
    task::sleep(10);
  }
  drive_with_voltage(0,0);
}

// TRUE = RIGHT
void Drive::turn_to_absolute_rotation(float angle){
  turn_to_angle(angle, turn_max_voltage, turn_settle_error, turn_settle_time, turn_timeout, turn_kp, turn_ki, turn_kd, turn_starti);
}

void Drive::turn_to_absolute_rotation(float angle, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout, float turn_kp, float turn_ki, float turn_kd, float turn_starti){
  PID turnPID(angle - get_heading(), turn_kp, turn_ki, turn_kd, turn_starti, turn_settle_error, turn_settle_time, turn_timeout);
  while( !turnPID.is_settled() ){
    float error = angle - get_heading();
    float output = turnPID.compute(error);
    output = clamp(output, -turn_max_voltage, turn_max_voltage);
    drive_with_voltage(output, -output);
    task::sleep(10);
  }
  drive_with_voltage(0,0);
}

void Drive::turn_to_localized_rotation(float angle){
  turn_to_angle(angle, turn_max_voltage, turn_settle_error, turn_settle_time, turn_timeout, turn_kp, turn_ki, turn_kd, turn_starti);
}

void Drive::turn_to_localized_rotation(float angle, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout, float turn_kp, float turn_ki, float turn_kd, float turn_starti){
  float initAngle = get_rotation();
  PID turnPID(angle, turn_kp, turn_ki, turn_kd, turn_starti, turn_settle_error, turn_settle_time, turn_timeout);
  while( !turnPID.is_settled() ){
    float error = get_rotation() - initAngle - angle;
    float output = turnPID.compute(error);
    output = clamp(output, -turn_max_voltage, turn_max_voltage);
    drive_with_voltage(output, -output);
    task::sleep(10);
  }
  drive_with_voltage(0,0);
}

/**
 * Drives the robot a given distance with a given heading.
 * Drive distance does not optimize for direction, so it won't try
 * to drive at the opposite heading from the one given to get there faster.
 * You can control the heading, but if you choose not to, it will drive with the
 * heading it's currently facing. It uses the average of the left and right
 * motor groups to calculate distance driven.
 * 
 * @param distance Desired distance in inches.
 * @param heading Desired heading in degrees.
 */

void Drive::drive_distance(float distance){
  drive_distance(distance, get_rotation(), drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_distance(float distance, float heading){
  drive_distance(distance, heading, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}


void Drive::drive_distance(float distance, float heading, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti){
  PID drivePID(distance, drive_kp, drive_ki, drive_kd, drive_starti, drive_settle_error, drive_settle_time, drive_timeout);
  PID headingPID(reduce_negative_180_to_180(heading - get_rotation()), heading_kp, heading_ki, heading_kd, heading_starti);
  float start_average_position = (get_left_position_in()+get_right_position_in())/2.0;
  float average_position = start_average_position;
  while(drivePID.is_settled() == false){
    float average_position = (get_left_position_in()+get_right_position_in())/2.0;
    float drive_error = distance+start_average_position-average_position;
    float heading_error = reduce_negative_180_to_180(heading - get_rotation());
    float drive_output = drivePID.compute(drive_error);
    float heading_output = headingPID.compute(heading_error);
    task::sleep(10);
    drive_output = clamp(drive_output, -drive_max_voltage, drive_max_voltage);
    heading_output = clamp(heading_output, -heading_max_voltage, heading_max_voltage);
    drive_with_voltage(drive_output+heading_output, drive_output-heading_output);
  }
}

void Drive::drive_velocity(float drive_output, float heading_output){
  drive_velocity(drive_output, heading_output, v_drive_max_voltage, v_heading_max_voltage, v_drive_settle_error, v_drive_settle_time, v_drive_timeout, v_drive_kp, v_drive_ki, v_drive_kd, v_drive_starti, v_heading_kp, v_heading_ki, v_heading_kd, v_heading_starti);
}

void Drive::drive_velocity(float drive_output, float heading_output, float v_drive_max_voltage, float v_heading_max_voltage, float v_drive_settle_error, float v_drive_settle_time, float v_drive_timeout, float v_drive_kp, float v_drive_ki, float v_drive_kd, float v_drive_starti, float v_heading_kp, float v_heading_ki, float v_heading_kd, float v_heading_starti){
  PID driveVelocityPID(drive_output, v_drive_kp, v_drive_ki, v_drive_kd, v_drive_starti, v_drive_settle_error, v_drive_settle_time, v_drive_timeout);
  PID headingVelocityPID(heading_output, v_heading_kp, v_heading_ki, v_heading_kd, v_heading_starti);
  const float wheelbase = 13.25; // in inches
  float average_velocity = (get_left_velocity_ins()+get_right_velocity_ins()) / 2;
  float average_angular_velocity = (get_right_velocity_ins()-get_left_velocity_ins())/wheelbase;
  float drive_velocity_error = drive_output - average_velocity;
  float heading_velocity_error = heading_output - average_angular_velocity;
  drive_output = driveVelocityPID.compute(drive_velocity_error);
  heading_output = headingVelocityPID.compute(heading_velocity_error);
  drive_output = clamp(drive_output, -v_drive_max_voltage, v_drive_max_voltage);
  heading_output = clamp(heading_output, -v_heading_max_voltage, v_heading_max_voltage);
  drive_with_voltage(left_voltage_scaling(drive_output, heading_output), right_voltage_scaling(drive_output, heading_output));
}

/**
 * Drives the robot a given distance with a given heading.
 * Drive distance does not optimize for direction, so it won't try
 * to drive at the opposite heading from the one given to get there faster.
 * You can control the heading, but if you choose not to, it will drive with the
 * heading it's currently facing. It uses the average of the left and right
 * motor groups to calculate distance driven.
 * Also uses motion profiling.
 * 
 * @param distance Desired distance in inches.
 * @param heading Desired heading in degrees.
 */
void Drive::drive_distance_mp(float distance) {
  drive_distance_mp(distance, get_rotation(), heading_max_voltage, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_distance_mp(float distance, float heading) {
  drive_distance_mp(distance, heading, heading_max_voltage, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_distance_mp(float distance, float heading, float heading_max_voltage, float heading_kp, float heading_ki, float heading_kd, float heading_starti) {
  // Initialize PID controllers
  PID headingPID(reduce_negative_180_to_180(heading - get_rotation()), heading_kp, heading_ki, heading_kd, heading_starti);

  mp.sigmoid_initialize(distance);
  float start_average_position = (get_left_position_in() + get_right_position_in()) / 2.0;
  float average_position = start_average_position;

  // Motion profile variables
  float velocity = 0.0; // Initialize velocity
  float drive_output = 0.0;
  float prev_drive_output = 0.0;

  while (drive_output != 0) {
    float heading_error = reduce_negative_180_to_180(heading - get_rotation());

    // Compute PID outputs
    float heading_output = headingPID.compute(heading_error);

    mp.sigmoid_velocity(distance); // Call to motion profile

    // Use profile velocity for feedforward
    velocity = mp.velocity; // Access motion profile velocity
    float feedforward = velocity; // Use feedforward as needed
    float desired_drive_output = feedforward;

    // Clamp outputs to the max voltages
    drive_output = clamp(drive_output, -drive_max_voltage, drive_max_voltage);
    heading_output = clamp(heading_output, -heading_max_voltage, heading_max_voltage);
    // Send calculated voltages to motors
    drive_with_voltage(drive_output + heading_output, drive_output - heading_output);

    // Update previous drive output
    prev_drive_output = drive_output;

    // Wait to prevent fast looping
    task::sleep(10);
  }
}

/**
 * Turns to a given angle with only one side of the drivetrain.
 * Like turn_to_angle(), is optimized for turning the shorter
 * direction.
 * 
 * @param angle Desired angle in degrees.
 */

void Drive::left_swing_to_angle(float angle){
  left_swing_to_angle(angle, swing_max_voltage, swing_settle_error, swing_settle_time, swing_timeout, swing_kp, swing_ki, swing_kd, swing_starti);
}

void Drive::left_swing_to_angle(float angle, float swing_max_voltage, float swing_settle_error, float swing_settle_time, float swing_timeout, float swing_kp, float swing_ki, float swing_kd, float swing_starti){
  PID swingPID(reduce_negative_180_to_180(angle - get_rotation()), swing_kp, swing_ki, swing_kd, swing_starti, swing_settle_error, swing_settle_time, swing_timeout);
  while(swingPID.is_settled() == false){
    float error = reduce_negative_180_to_180(angle - get_rotation());
    float output = swingPID.compute(error);
    output = clamp(output, -turn_max_voltage, turn_max_voltage);
    DriveL.spin(fwd, output, volt);
    DriveR.stop(hold);
    task::sleep(10);
  }
}

void Drive::right_swing_to_angle(float angle){
  right_swing_to_angle(angle, swing_max_voltage, swing_settle_error, swing_settle_time, swing_timeout, swing_kp, swing_ki, swing_kd, swing_starti);
}

void Drive::right_swing_to_angle(float angle, float swing_max_voltage, float swing_settle_error, float swing_settle_time, float swing_timeout, float swing_kp, float swing_ki, float swing_kd, float swing_starti){
  PID swingPID(reduce_negative_180_to_180(angle - get_rotation()), swing_kp, swing_ki, swing_kd, swing_starti, swing_settle_error, swing_settle_time, swing_timeout);
  while(swingPID.is_settled() == false){
    float error = reduce_negative_180_to_180(angle - get_rotation());
    float output = swingPID.compute(error);
    output = clamp(output, -turn_max_voltage, turn_max_voltage);
    DriveR.spin(reverse, output, volt);
    DriveL.stop(hold);
    task::sleep(10);
  }
}

/**
 * Depending on the drive style, gets the tracker's position.
 * 
 * @return The tracker position.
 */

float Drive::get_ForwardTracker_position(){
  if (drive_setup==ZERO_TRACKER_ODOM || drive_setup == TANK_ONE_SIDEWAYS_ENCODER || drive_setup == TANK_ONE_SIDEWAYS_ROTATION){
    return((get_right_position_in()+get_left_position_in())/2);
  }
  if (drive_setup==TANK_ONE_FORWARD_ENCODER || drive_setup == TANK_TWO_ENCODER || drive_setup == HOLONOMIC_TWO_ENCODER){
    //return(get_right_position_in());
    return(E_ForwardTracker.position(deg)*ForwardTracker_in_to_deg_ratio);
  }else{
    // return(get_right_position_in());
    return(R_ForwardTracker.position(deg)*ForwardTracker_in_to_deg_ratio);
  }
}

/**
 * Depending on the drive style, gets the tracker's position.
 * 
 * @return The tracker position.
 */

float Drive::get_SidewaysTracker_position(){
  if (drive_setup==TANK_ONE_FORWARD_ENCODER || drive_setup == TANK_ONE_FORWARD_ROTATION || drive_setup == ZERO_TRACKER_ODOM){
    return(0);
  }else if (drive_setup == TANK_TWO_ENCODER || drive_setup == HOLONOMIC_TWO_ENCODER || drive_setup == TANK_ONE_SIDEWAYS_ENCODER){
    return(E_SidewaysTracker.position(deg)*SidewaysTracker_in_to_deg_ratio);
  }else{
    return(R_SidewaysTracker.position(deg)*SidewaysTracker_in_to_deg_ratio);
  }
}

/**
 * Background task for updating the odometry.
 */

void Drive::position_track(){
  while(1){
    odom.update_position(get_ForwardTracker_position(), get_SidewaysTracker_position(), get_rotation());
    task::sleep(5);
  }
}

/**
 * Resets the robot's heading.
 * For example, at the beginning of auton, if your robot starts at
 * 45 degrees, so set_heading(45) and the robot will know which way 
 * it's facing.
 * 
 * @param orientation_deg Desired heading in degrees.
 */

void Drive::set_heading(float orientation_deg){
  if (gyro_setup==ONE){
    GyroOne.setRotation(orientation_deg*gyro_one_scale/360.0, deg);
  }
  else {
    GyroTwo.setRotation(orientation_deg*gyro_two_scale/360.0, deg);
  }
}

/**
 * Resets the robot's coordinates and heading.
 * This is for odom-using robots to specify where the bot is at the beginning
 * of the match.
 * 
 * @param X_position Robot's x in inches.
 * @param Y_position Robot's y in inches.
 * @param orientation_deg Desired heading in degrees.
 */

void Drive::set_coordinates(float X_position, float Y_position){
  odom.set_position(X_position, Y_position, get_rotation(), get_ForwardTracker_position(), get_SidewaysTracker_position());
}
void Drive::set_coordinates(float X_position, float Y_position, float orientation_deg){
  odom.set_position(X_position, Y_position, orientation_deg, get_ForwardTracker_position(), get_SidewaysTracker_position());
  set_heading(orientation_deg);
  odom_task = task(position_track_task);
}

/**
 * Gets the robot's x.
 * 
 * @return The robot's x position in inches.
 */

float Drive::get_X_position(){
  return(odom.X_position);
}

/**
 * Gets the robot's y.
 * 
 * @return The robot's y position in inches.
 */

float Drive::get_Y_position(){
  return(odom.Y_position);
}

/**
 * Drives to a specified point on the field.
 * Uses the double-PID method, with one for driving and one for heading correction.
 * The drive error is the euclidean distance to the desired point, and the heading error
 * is the turn correction from the current heading to the desired point. Uses optimizations
 * like driving backwards whenever possible and scaling the drive output with the cosine
 * of the angle to the point.
 * 
 * @param X_position Desired x position in inches.
 * @param Y_position Desired y position in inches.
 */

void Drive::drive_to_point(float X_position, float Y_position){
  drive_to_point(X_position, Y_position, drive_min_voltage, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_to_point(float X_position, float Y_position, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti){
  float start_drive_error = hypot(X_position - get_X_position(), Y_position - get_Y_position());
  float start_angle = to_deg(atan2(X_position-get_X_position(), Y_position-get_Y_position()));
  float start_reverse_angle = start_angle;
  float start_heading_error = reduce_negative_180_to_180(start_angle - get_rotation());
  if (start_reverse_angle > 0) {
	start_reverse_angle = -(180 - start_reverse_angle);
  }
  else {
	start_reverse_angle = (180 + start_reverse_angle);
  }
  float start_reverse_heading_error = reduce_negative_180_to_180(start_reverse_angle - get_rotation());
  if (fabs(start_reverse_heading_error) < fabs(start_heading_error)) {
    start_heading_error = start_reverse_heading_error;
    start_drive_error = -start_drive_error;
  }
  PID headingPID(start_heading_error, heading_kp, heading_ki, heading_kd, heading_starti);
  PID drivePID(start_drive_error, drive_kp, drive_ki, drive_kd, drive_starti, drive_settle_error, drive_settle_time, drive_timeout);
  while(!drivePID.is_settled()){
    float drive_error = hypot(X_position-get_X_position(), Y_position-get_Y_position());
    float angle = to_deg(atan2((X_position-get_X_position()),(Y_position-get_Y_position())));
    float reverse_angle = angle;
    float heading_error = reduce_negative_180_to_180(angle - get_rotation());
    if (reverse_angle > 0) {
      reverse_angle = -(180 - angle);
    }
    else {
      reverse_angle = (180 + angle);
    }
    float reverse_heading_error = reduce_negative_180_to_180(reverse_angle - get_heading());
    if (fabs(reverse_heading_error) < fabs(heading_error)) {
      heading_error = reverse_heading_error;
      drive_error = -drive_error;
    }
    float drive_output = drivePID.compute(drive_error);
    float heading_scale_factor = cos(to_rad(heading_error));
    drive_output*=heading_scale_factor;
    heading_error = reduce_negative_90_to_90(heading_error);
    float heading_output = headingPID.compute(heading_error);
    drive_output = clamp(drive_output, -fabs(heading_scale_factor)*drive_max_voltage, fabs(heading_scale_factor)*drive_max_voltage);
    heading_output = clamp(heading_output, -heading_max_voltage, heading_max_voltage);
    drive_output = clamp_min_voltage(drive_output, drive_min_voltage);
    drive_with_voltage(left_voltage_scaling(drive_output, heading_output), right_voltage_scaling(drive_output, heading_output));
    task::sleep(10);
  }
  chassis.drive_with_voltage(0,0);
}

void Drive::drive_to_point_non_adjust(float X_position, float Y_position){
  drive_to_point(X_position, Y_position, drive_min_voltage, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_to_point_non_adjust(float X_position, float Y_position, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti){
  float start_drive_error = hypot(X_position - get_X_position(), Y_position - get_Y_position());
  float start_angle = to_deg(atan2(X_position-get_X_position(), Y_position-get_Y_position()));
  float start_reverse_angle = start_angle;
  float start_heading_error = reduce_negative_180_to_180(start_angle - get_heading());
  if (start_reverse_angle > 0) {
	start_reverse_angle = -(180 - start_reverse_angle);
  }
  else {
	start_reverse_angle = (180 + start_reverse_angle);
  }
  float start_reverse_heading_error = reduce_negative_180_to_180(start_reverse_angle - get_heading());
  if (fabs(start_reverse_heading_error) < fabs(start_heading_error)) {
    start_heading_error = start_reverse_heading_error;
    start_drive_error = -start_drive_error;
  }
  PID headingPID(start_heading_error, heading_kp, heading_ki, heading_kd, heading_starti);
  PID drivePID(start_drive_error, drive_kp, drive_ki, drive_kd, drive_starti, drive_settle_error, drive_settle_time, drive_timeout);
  while(!drivePID.is_settled()){
    float drive_error = hypot(X_position-get_X_position(), Y_position-get_Y_position());
    float heading_error = reduce_negative_180_to_180(start_angle - get_heading());
    float reverse_heading_error = reduce_negative_180_to_180(start_reverse_angle - get_heading());
    if (fabs(reverse_heading_error) < fabs(heading_error)) {
      heading_error = reverse_heading_error;
      drive_error = -drive_error;
    }
    float drive_output = drivePID.compute(drive_error);
    float heading_scale_factor = cos(to_rad(heading_error));
    drive_output*=heading_scale_factor;
    heading_error = reduce_negative_90_to_90(heading_error);
    float heading_output = headingPID.compute(heading_error);
    drive_output = clamp(drive_output, -fabs(heading_scale_factor)*drive_max_voltage, fabs(heading_scale_factor)*drive_max_voltage);
    heading_output = clamp(heading_output, -heading_max_voltage, heading_max_voltage);
    drive_output = clamp_min_voltage(drive_output, drive_min_voltage);
    drive_with_voltage(left_voltage_scaling(drive_output, heading_output), right_voltage_scaling(drive_output, heading_output));
    task::sleep(10);
  }
  chassis.drive_with_voltage(0,0);
}
void Drive::drive_to_point_velocity(float X_position, float Y_position){
  drive_to_point(X_position, Y_position, drive_min_voltage, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_to_point_velocity(float X_position, float Y_position, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti){
  float start_drive_error = hypot(X_position - get_X_position(), Y_position - get_Y_position());
  float start_angle = to_deg(atan2(X_position-get_X_position(), Y_position-get_Y_position()));
  float start_reverse_angle = start_angle;
  float start_heading_error = reduce_negative_180_to_180(start_angle - get_heading());
  if (start_reverse_angle > 0) {
	start_reverse_angle = -(180 - start_reverse_angle);
  }
  else {
	start_reverse_angle = (180 + start_reverse_angle);
  }
  float start_reverse_heading_error = reduce_negative_180_to_180(start_reverse_angle - get_heading());
  if (fabs(start_reverse_heading_error) < fabs(start_heading_error)) {
    start_heading_error = start_reverse_heading_error;
    start_drive_error = -start_drive_error;
  }
  PID headingPID(start_heading_error, heading_kp, heading_ki, heading_kd, heading_starti);
  PID drivePID(start_drive_error, drive_kp, drive_ki, drive_kd, drive_starti, drive_settle_error, drive_settle_time, drive_timeout);
  while(!drivePID.is_settled()){
    float drive_error = hypot(X_position-get_X_position(), Y_position-get_Y_position());
    float angle = to_deg(atan2((X_position-get_X_position()),(Y_position-get_Y_position())));
    float reverse_angle = angle;
    float heading_error = reduce_negative_180_to_180(angle - get_heading());
    if (reverse_angle > 0) {
      reverse_angle = -(180 - angle);
    }
    else {
      reverse_angle = (180 + angle);
    }
    float reverse_heading_error = reduce_negative_180_to_180(reverse_angle - get_heading());
    if (fabs(reverse_heading_error) < fabs(heading_error)) {
      heading_error = reverse_heading_error;
      drive_error = -drive_error;
    }
    float drive_output = drivePID.compute(drive_error);
    float heading_scale_factor = cos(to_rad(heading_error));
    drive_output*=heading_scale_factor;
    heading_error = reduce_negative_90_to_90(heading_error);
    float heading_output = headingPID.compute(heading_error);
    drive_output = clamp(drive_output, -fabs(heading_scale_factor)*drive_max_voltage, fabs(heading_scale_factor)*drive_max_voltage);
    heading_output = clamp(heading_output, -heading_max_voltage, heading_max_voltage);
    drive_output = clamp_min_voltage(drive_output, drive_min_voltage);
	drive_velocity(drive_output, heading_output);
    task::sleep(10);
  }
  chassis.drive_with_voltage(0,0);
}
/**
 * Drives to a specified point and orientation on the field.
 * Uses a boomerang controller. The carrot point is back from the target
 * by the same distance as the robot's distance to the target, times the lead. The
 * robot always tries to go to the carrot, which is constantly moving, and the
 * robot eventually gets into position. The heading correction is optimized to only
 * try to reach the correct angle when drive error is low, and the robot will drive 
 * backwards to reach a pose if it's faster. .5 is a reasonable value for the lead. 
 * The setback parameter is used to glide into position more effectively. It is
 * the distance back from the target that the robot tries to drive to first.
 * 
 * @param X_position Desired x position in inches.
 * @param Y_position Desired y position in inches.
 * @param angle Desired orientation in degrees.
 * @param lead Constant scale factor that determines how far away the carrot point is. 
 * @param setback Distance in inches from target by which the carrot is always pushed back.
 * @param drive_min_voltage Minimum voltage on the drive, used for chaining movements.
 */

void Drive::drive_to_pose(float X_position, float Y_position, float angle){
  drive_to_pose(X_position, Y_position, angle, boomerang_lead, boomerang_setback, drive_min_voltage, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_to_pose(float X_position, float Y_position, float angle, float lead, float setback, float drive_min_voltage){
  drive_to_pose(X_position, Y_position, angle, lead, setback, drive_min_voltage, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_to_pose(float X_position, float Y_position, float angle, float lead, float setback, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage){
  drive_to_pose(X_position, Y_position, angle, lead, setback, drive_min_voltage, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}


void Drive::drive_to_pose(float X_position, float Y_position, float angle, float lead, float setback, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout){
  drive_to_pose(X_position, Y_position, angle, lead, setback, drive_min_voltage, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_to_pose(float X_position, float Y_position, float angle, float lead, float setback, float drive_min_voltage, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti){
  float target_distance = hypot(X_position-get_X_position(),Y_position-get_Y_position());
  PID drivePID(target_distance, drive_kp, drive_ki, drive_kd, drive_starti, drive_settle_error, drive_settle_time, drive_timeout);
  PID headingPID(to_deg(atan2(X_position-get_X_position(),Y_position-get_Y_position()))-get_rotation(), heading_kp, heading_ki, heading_kd, heading_starti);
  bool line_settled = is_line_settled(X_position, Y_position, angle, get_X_position(), get_Y_position());
  bool prev_line_settled = is_line_settled(X_position, Y_position, angle, get_X_position(), get_Y_position());
  bool crossed_center_line = false;
  bool center_line_side = is_line_settled(X_position, Y_position, angle+90, get_X_position(), get_Y_position());
  bool prev_center_line_side = center_line_side;
  while(!drivePID.is_settled()){
    line_settled = is_line_settled(X_position, Y_position, angle, get_X_position(), get_Y_position());
    if(line_settled && !prev_line_settled){ break; }
    prev_line_settled = line_settled;

    center_line_side = is_line_settled(X_position, Y_position, angle+90, get_X_position(), get_Y_position());
    if(center_line_side != prev_center_line_side){
      crossed_center_line = true;
    }

    target_distance = hypot(X_position-get_X_position(),Y_position-get_Y_position());

    float carrot_X = X_position - sin(to_rad(angle)) * (lead * target_distance + setback);
    float carrot_Y = Y_position - cos(to_rad(angle)) * (lead * target_distance + setback);

    float drive_error = hypot(carrot_X-get_X_position(),carrot_Y-get_Y_position());
    float heading_error = reduce_negative_180_to_180(to_deg(atan2(carrot_X-get_X_position(),carrot_Y-get_Y_position()))-get_rotation());

    if (drive_error<drive_settle_error || crossed_center_line || drive_error < setback) { 
      heading_error = reduce_negative_180_to_180(angle-get_rotation()); 
      drive_error = target_distance;
    }
    
    float drive_output = drivePID.compute(drive_error);

    float heading_scale_factor = cos(to_rad(heading_error));
    drive_output*=heading_scale_factor;
    heading_error = reduce_negative_90_to_90(heading_error);
    float heading_output = headingPID.compute(heading_error);

    drive_output = clamp(drive_output, -fabs(heading_scale_factor)*drive_max_voltage, fabs(heading_scale_factor)*drive_max_voltage);
    heading_output = clamp(heading_output, -heading_max_voltage, heading_max_voltage);

    drive_output = clamp_min_voltage(drive_output, drive_min_voltage);

    drive_with_voltage(left_voltage_scaling(drive_output, heading_output), right_voltage_scaling(drive_output, heading_output));
    task::sleep(10);
  }
}

/**
 * Turns to a specified point on the field.
 * Functions similarly to turn_to_angle() except with a point. The
 * extra_angle_deg parameter turns the robot extra relative to the 
 * desired target. For example, if you want the back of your robot
 * to point at (36, 42), you would run turn_to_point(36, 42, 180).
 * 
 * @param X_position Desired x position in inches.
 * @param Y_position Desired y position in inches.
 * @param extra_angle_deg Angle turned past the desired heading in degrees.
 */

void Drive::turn_to_point(float X_position, float Y_position){
  turn_to_point(X_position, Y_position, false, turn_max_voltage, turn_settle_error, turn_settle_time, turn_timeout, turn_kp, turn_ki, turn_kd, turn_starti);
}

void Drive::turn_to_point(float X_position, float Y_position, bool reversed){
  turn_to_point(X_position, Y_position, reversed, turn_max_voltage, turn_settle_error, turn_settle_time, turn_timeout, turn_kp, turn_ki, turn_kd, turn_starti);
}

void Drive::turn_to_point(float X_position, float Y_position, bool reversed, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout, float turn_kp, float turn_ki, float turn_kd, float turn_starti){
  if (!reversed) {
    float start_angle = to_deg(atan2((X_position-get_X_position()),(Y_position-get_Y_position())));
    float start_error = reduce_negative_180_to_180(start_angle - get_heading());
    PID turnPID(start_error, turn_kp, turn_ki, turn_kd, turn_starti, turn_settle_error, turn_settle_time, turn_timeout);
    while(!turnPID.is_settled()){
      float angle = to_deg(atan2((X_position-get_X_position()),(Y_position-get_Y_position())));
      float error = reduce_negative_180_to_180(angle - get_rotation());
      float output = turnPID.compute(error);
      output = clamp(output, -turn_max_voltage, turn_max_voltage);
      drive_with_voltage(output, -output);
      task::sleep(10);
    }
  }
  else {
    float start_angle = to_deg(atan2(X_position-get_X_position(), Y_position-get_Y_position()));
    if (start_angle > 0) {
	  start_angle = -(180 - start_angle);
    }
    else {
	  start_angle = (180 + start_angle);
    }
    float start_error = reduce_negative_180_to_180(start_angle - get_rotation());
    PID turnPID(start_error, turn_kp, turn_ki, turn_kd, turn_starti, turn_settle_error, turn_settle_time, turn_timeout);
    while(!turnPID.is_settled()){
      float angle = to_deg(atan2((X_position-get_X_position()),(Y_position-get_Y_position())));
      if (angle > 0) {
	angle = -(180 - angle);
      }
      else {
	angle = (180 + angle);
      }
      float error = reduce_negative_180_to_180(angle - get_rotation());
      float output = turnPID.compute(error);
      output = clamp(output, -turn_max_voltage, turn_max_voltage);
      drive_with_voltage(output, -output);
      task::sleep(10);
    }
  }
  chassis.drive_with_voltage(0,0);
}
/**
 * Drives and turns simultaneously to a desired pose.
 * Uses two PID loops, one drive and one heading to drive and turn
 * at the same time. Optimized to turn the quicker direction and only
 * exits once both PID loops have settled. It uses the heading constants
 * for heading but the turn exit conditions to settle.
 * 
 * @param X_position Desired x position in inches.
 * @param Y_position Desired y position in inches.
 * @param angle Desired ending angle in degrees.
 */

void Drive::holonomic_drive_to_pose(float X_position, float Y_position){
  holonomic_drive_to_pose(X_position, Y_position, get_rotation(), drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::holonomic_drive_to_pose(float X_position, float Y_position, float angle){
  holonomic_drive_to_pose(X_position, Y_position, angle, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::holonomic_drive_to_pose(float X_position, float Y_position, float angle, float drive_max_voltage, float heading_max_voltage){
  holonomic_drive_to_pose(X_position, Y_position, angle, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::holonomic_drive_to_pose(float X_position, float Y_position, float angle, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout){
  holonomic_drive_to_pose(X_position, Y_position, angle, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::holonomic_drive_to_pose(float X_position, float Y_position, float angle, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti){
  PID drivePID(hypot(X_position-get_X_position(),Y_position-get_Y_position()), drive_kp, drive_ki, drive_kd, drive_starti, drive_settle_error, drive_settle_time, drive_timeout);
  PID turnPID(angle-get_rotation(), heading_kp, heading_ki, heading_kd, heading_starti, turn_settle_error, turn_settle_time, turn_timeout);
  while( !(drivePID.is_settled() && turnPID.is_settled()) ){
    float drive_error = hypot(X_position-get_X_position(),Y_position-get_Y_position());
    float turn_error = reduce_negative_180_to_180(angle-get_rotation());

    float drive_output = drivePID.compute(drive_error);
    float turn_output = turnPID.compute(turn_error);

    drive_output = clamp(drive_output, -drive_max_voltage, drive_max_voltage);
    turn_output = clamp(turn_output, -heading_max_voltage, heading_max_voltage);

    float heading_error = atan2(Y_position-get_Y_position(), X_position-get_X_position());

    DriveLF.spin(fwd, drive_output*cos(to_rad(get_rotation()) + heading_error - M_PI/4) + turn_output, volt);
    DriveLB.spin(fwd, drive_output*cos(-to_rad(get_rotation()) - heading_error + 3*M_PI/4) + turn_output, volt);
    DriveRB.spin(fwd, drive_output*cos(to_rad(get_rotation()) + heading_error - M_PI/4) - turn_output, volt);
    DriveRF.spin(fwd, drive_output*cos(-to_rad(get_rotation()) - heading_error + 3*M_PI/4) - turn_output, volt);
    task::sleep(10);
  }
}

/**
 * Controls a chassis with left stick throttle and right stick turning.
 * Default deadband is 5.
 */

void Drive::control_arcade(){
  float throttle = deadband(controller(primary).Axis3.value(), 5);
  float turn = deadband(controller(primary).Axis1.value(), 5);
  DriveL.spin(fwd, to_volt(throttle+turn), volt);
  DriveR.spin(fwd, to_volt(throttle-turn), volt);
}

/**
 * Controls a chassis with left stick throttle and right stick turning.
 * Default deadband is 5.
 * Also squares the movement to allow fine movements.
 */

void Drive::control_arcade_squared(){
  float throttle = deadband_squared(controller(primary).Axis3.value(), 5);
  float turn = deadband_squared(controller(primary).Axis1.value(), 5);
  DriveL.spin(fwd, to_volt(throttle+turn), volt);
  DriveR.spin(fwd, to_volt(throttle-turn), volt);
}

/**
 * Controls a chassis with left stick throttle and strafe, and right stick turning.
 * Default deadband is 5.
 */

void Drive::control_holonomic(){
  float throttle = deadband(controller(primary).Axis3.value(), 5);
  float turn = deadband(controller(primary).Axis1.value(), 5);
  float strafe = deadband(controller(primary).Axis4.value(), 5);
  DriveLF.spin(fwd, to_volt(throttle+turn+strafe), volt);
  DriveRF.spin(fwd, to_volt(throttle-turn-strafe), volt);
  DriveLB.spin(fwd, to_volt(throttle+turn-strafe), volt);
  DriveRB.spin(fwd, to_volt(throttle-turn+strafe), volt);
}

/**
 * Controls a chassis with left stick throttle and strafe, and right stick turning.
 * Default deadband is 5.
 * Also squares the movement to allow fine movements.
 */

void Drive::control_holonomic_squared(){
  float throttle = deadband_squared(controller(primary).Axis3.value(), 5);
  float turn = deadband_squared(controller(primary).Axis1.value(), 5);
  float strafe = deadband_squared(controller(primary).Axis4.value(), 5);
  DriveLF.spin(fwd, to_volt(throttle+turn+strafe), volt);
  DriveRF.spin(fwd, to_volt(throttle-turn-strafe), volt);
  DriveLB.spin(fwd, to_volt(throttle+turn-strafe), volt);
  DriveRB.spin(fwd, to_volt(throttle-turn+strafe), volt);
}

/**
 * Controls a chassis with left stick left drive and right stick right drive.
 * Default deadband is 5.
 */

void Drive::control_tank(float overide){
  if (!overide) {
    float leftthrottle = deadband(controller(primary).Axis3.value(), 5);
    float rightthrottle = deadband(controller(primary).Axis2.value(), 5);
    DriveL.spin(fwd, to_volt(leftthrottle), volt);
    DriveR.spin(fwd, to_volt(rightthrottle), volt);
  }
}

/**
 * Controls a chassis with left stick left drive and right stick right drive.
 * Default deadband is 5.
 * Also squares the movement to allow fine movements.
 */

void Drive::control_tank_squared(){
  float leftthrottle = deadband_squared(controller(primary).Axis3.value(), 5);
  float rightthrottle = deadband_squared(controller(primary).Axis2.value(), 5);
  DriveL.spin(fwd, to_volt(leftthrottle), volt);
  DriveR.spin(fwd, to_volt(rightthrottle), volt);
}

void Drive::pid_control_tank() {
  float leftthrottle = to_volt(controller(primary).Axis3.value());
  float rightthrottle = to_volt(controller(primary).Axis2.value());
  PID leftVelocityPID(leftthrottle, v_drive_kp, v_drive_ki, v_drive_kd, v_drive_starti, v_drive_settle_error, v_drive_settle_time, v_drive_timeout);
  PID rightVelocityPID(rightthrottle, v_drive_kp, v_drive_ki, v_drive_kd, v_drive_starti, v_drive_settle_error, v_drive_settle_time, v_drive_timeout);
  float left_velocity_error = leftthrottle - get_left_velocity_ins();
  float right_velocity_error = rightthrottle - get_right_velocity_ins();
  leftthrottle = leftVelocityPID.compute(left_velocity_error);
  rightthrottle = rightVelocityPID.compute(right_velocity_error);

  DriveL.spin(fwd, to_volt(leftthrottle), volt);
  DriveR.spin(fwd, to_volt(rightthrottle), volt);
}

/**
 * Tracking task to run in the background.
 */

int Drive::position_track_task(){
  chassis.position_track();
  return(0);
}
