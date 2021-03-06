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
  0.8,  // Maximum linear velocity of the Chassis in m/s
  0.4,  // Maximum linear acceleration of the Chassis in m/s/s
  3.45, // Maximum linear jerk of the Chassis in m/s/s/s
  myChassis // Chassis Controller
);

void flipout(int power){
  mo.roller_right -> move(-power);
}
//pid turning
void turn(int angle){
  int scaled_angle = angle;
  mo.fleft_drive->move_absolute(scaled_angle, 100);
  mo.bleft_drive->move_absolute(scaled_angle, 100);
  mo.fright_drive->move_absolute(-scaled_angle, 100);
  mo.bright_drive->move_absolute(-scaled_angle, 100);
}

//part of the turning function
void waitForCompletion(){
  while((abs(mo.bright_drive->get_position() - mo.bright_drive->get_target_position()) + 
			abs(mo.bleft_drive->get_position() - mo.bleft_drive->get_target_position()) +
			abs(mo.fleft_drive->get_position() - mo.fleft_drive->get_target_position()) +
			abs(mo.fright_drive->get_position() - mo.fright_drive->get_target_position())) > 6 * 4 /* Number of drive mtrs */ ) {
		pros::Task::delay(1);
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

void move_drive_1(int right, int left) {
	mo.fright_drive -> move(right);
	mo.fleft_drive -> move(left);
	mo.bright_drive -> move(right);
	mo.bleft_drive -> move(left);
}


// Limit drivetrain speed based on the constant defined above

void autonomous() {
//PID turning

}
