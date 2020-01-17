#include "main.h"
#include "motor_defs.h"

MotorDefs m; //imported class for the motors

const double DRIVE_SPEED = 1; // Changes the maximum drive speed (range between 0 and 1)
const int DRIVE_THRESHOLD  = 12; // Controls the minimum power that can be assigned to drive motors

pros::Controller master(pros::E_CONTROLLER_MASTER);

void move_drive(int right, int left) {
	m.fright_drive -> move(right);
	m.fleft_drive -> move(left);
	m.bright_drive -> move(right);
	m.bleft_drive -> move(left);
}

void move_lift(int power){
	m.lift -> move(power);
}

void move_lift_velocity(int power){
	m.lift -> move_velocity(power);
}

void move_roller(int power){
	m.roller_right -> move(power);
	m.roller_left -> move(power);
}

void move_arm(int power){
	m.arm_motor -> move(power);
}

void armmove(void*arm){
	int up_power;
	int down_power;

	while (true){
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
			move_arm(-50);
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
				pros::Task::delay(1);
			}
		move_arm(0);
		}
		else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
			move_arm(100);
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
				pros::Task::delay(1);
			};
		move_arm(24); //made this value
		}
		pros::Task::delay(10);
	}
}

void tilter(void*tilter){
	while (true){
	//if the up button is pressed
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) == 1){
			move_lift_velocity(110); //from 127
			while (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) == 1){
				pros::Task::delay(10);
			}
			move_lift(10);

		}
		else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) == 1){
			move_lift(-85); //from 127
			while (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) == 1){
				pros::Task::delay(10);
			}
			move_lift(0);
		}
		pros::Task::delay(10);
	}
}

void drive(void*drive_void){
	int left_y;  // Left joystick Y
	int right_x; // Right joystick X
	int right_power;
	int left_power;
	while (true){
		// Assign the values from the controller's joysticks to variables
		left_y = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y); //forward backward
		right_x = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X); //left right
		
		// Filter joystick values to prevent the drive from responding to miscalibrated joysticks
		if (std::abs(left_y) < DRIVE_THRESHOLD) {
			left_y = 0;
		}
		if(std::abs(right_x) < DRIVE_THRESHOLD) {
			right_x = 0;
		}
		left_power = left_y - right_x;
		right_power = left_y + right_x;

		// Limit drivetrain speed based on the constant defined above
		right_power = (int) (right_power * DRIVE_SPEED);
		left_power = (int) (left_power * DRIVE_SPEED);

		// Assign power to the motors
		move_drive(left_power, right_power);
	}
}


void roller(void*roller_void){
	//0=stop,1=forward,-1=slowback,-2=back
	int state = 0;
	while (true){
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
			if (state == 1){
				move_roller(0);
				state = 0;
			}
			else{
				move_roller(127);
				state = 1;
			}
			pros::Task::delay(250);
		}
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
			if (state == -2){
				move_roller(0);
				state = 0;
			}
			else{
				move_roller(-127);
				state = -2;
			}
			pros::Task::delay(250);
		}
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
			if (state == -1){
				move_roller(0);
				state = 0;
			}
			else{
				move_roller(-50);
				state = -1;
			}
			pros::Task::delay(250);
		}
	}
}
void opcontrol() {
	pros::Task driveTask (drive,(void*)"PROS",TASK_PRIORITY_DEFAULT,TASK_STACK_DEPTH_DEFAULT,"drive");
	pros::Task rollerTask (roller,(void*)"PROS",TASK_PRIORITY_DEFAULT,TASK_STACK_DEPTH_DEFAULT,"rollers");
	pros::Task tilterTask (tilter,(void*)"PROS",TASK_PRIORITY_DEFAULT,TASK_STACK_DEPTH_DEFAULT,"tilter");
	pros::Task armTask (armmove,(void*)"PROS",TASK_PRIORITY_DEFAULT,TASK_STACK_DEPTH_DEFAULT,"arm");
}
