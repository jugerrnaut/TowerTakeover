#include "main.h"
#include "motor_defs.h"
#include "okapi/api.hpp"

using namespace okapi;

//motor defs from header file
MotorDefs mo;

//defining the chassis
auto myChassis = ChassisControllerFactory::create(
  {-7, -12}, // Left motors
  {8, 6},   // Right motors
  AbstractMotor::gearset::red, // Torque gearset
  {3.25_in, 13.5_in} // 4 inch wheels, 12.5 inch wheelbase width
);

//defining the motion controller
auto profileController = AsyncControllerFactory::motionProfile(
  0.5,  // Maximum linear velocity of the Chassis in m/s
  0.3,  // Maximum linear acceleration of the Chassis in m/s/s
  3.45, // Maximum linear jerk of the Chassis in m/s/s/s
  myChassis // Chassis Controller
);

//pid turning
void turn(double angle){
  double scaled_angle = angle;
  mo.fleft_drive->move_relative(scaled_angle, 100);
  mo.bleft_drive->move_relative(scaled_angle, 100);
  mo.fright_drive->move_relative(-scaled_angle, 100);
  mo.bright_drive->move_relative(-scaled_angle, 100);
}

//part of the turning function
void waitForCompletion(){
  while((abs(mo.bright_drive->get_position() - mo.bright_drive->get_target_position()) + 
			abs(mo.bleft_drive->get_position() - mo.bleft_drive->get_target_position()) +
			abs(mo.fleft_drive->get_position() - mo.fleft_drive->get_target_position()) +
			abs(mo.fright_drive->get_position() - mo.fright_drive->get_target_position())) > 6 * 4 /* Number of drive mtrs */ ) {
		pros::Task::delay(5);
	}
}

void tilt(int power){
    mo.lift -> move(power);
}

void roller_intake(int power){
  mo.roller_right -> move(-power);
  mo.roller_left -> move(-power);
}

void movearm(int power){
    mo.arm_motor -> move(power);
}

void autonomous() {
//flipping out

//starting the intake
roller_intake(127);
//moving toward 1st column
profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{1.6_ft, 0_ft, 0_deg}}, "A");
profileController.setTarget("A");
profileController.waitUntilSettled();
//bezier turning
profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{1.5_ft, 2_ft, 0_deg}}, "B");
profileController.setTarget("B",true);
profileController.waitUntilSettled();
//moving toward the second column

//PID turning

//moving toward the goal zone

//tilting

//moving back
}

