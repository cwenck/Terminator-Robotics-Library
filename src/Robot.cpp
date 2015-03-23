/*
 * Robot.cpp
 *
 *  Created on: Feb 8, 2015
 *      Author: cwenck
 */

#include "TRL/Robot.h"

namespace TRL {
///////////////////////////////
//CONTSTRUCTORS + DESTRUCTORS//
///////////////////////////////

Robot Robot::instance = Robot();

Controller Robot::master_controller = Controller(Master_Controller);
Controller Robot::slave_controller = Controller(Slave_Controller);

void Robot::initializeMotors() {
	frontRightDrive = Motor(MotorPort_10, FrontRight);
	frontLeftDrive = Motor(MotorPort_9, FrontLeft);
	backRightDrive = Motor(MotorPort_2, BackRight, true);
	backLeftDrive = Motor(MotorPort_1, BackLeft);

	frontRightLift = Motor(MotorPort_3, FrontRight);
	backRightLift = Motor(MotorPort_5, BackRight);
	frontLeftLift = Motor(MotorPort_4, FrontLeft, true);
	backLeftLift = Motor(MotorPort_6, BackLeft, true);

	intakeMotor = Motor(MotorPort_7, Intake);
	intakeArmMotor = Motor(MotorPort_8, Other);

	Motor* liftMotors[4] = { &frontRightLift, &frontLeftLift, &backRightLift,
			&backLeftLift };
	Motor* driveMotors[4] = { &frontRightDrive, &frontLeftDrive,
			&backRightDrive, &backLeftDrive };

	setDriveMotors(driveMotors, 4);	//last number is the number of drive motors
	setLiftMotors(liftMotors, 4);	//last number is the number of lift motors
	setIntakeMotors(&intakeMotor, &intakeArmMotor);
	println(LOG, "Robot", "initializeMotors", "Motors Initialized.");
}

Robot::Robot() {
	//Controller Deadzone
	controllerDeadzoneMagnitude = 10;

	//Init Controller Inputs
	y_drive_stick = Ch3;
	x_drive_stick = Ch4;

	lift_up = Btn5D;
	lift_down = Btn5U;

	claw_rotate = Ch1;

	claw_open = Btn6U;
	claw_close = Btn6D;

	orientation_forward = Btn7D;
	orientation_backward = Btn7U;

	//Init the default lift power
	liftPower = 127;
	clawArmPower = 127;
	clawOpenPower = 127;
	clawClosePower = 127;

	//Init the defualt orientaiton
	orientation = BackwardOrientation;

	//Init the number of diive and lift motors
	numDriveMotors = 0;
	numLiftMotors = 0;

	//Init the motor pointers as null
	clawMotor = NULL;
	clawArmMotor = NULL;

	initializeMotors();
}

Robot::~Robot() {
	// TODO Auto-generated destructor stub
}

////////////////////
//CONTROLLER INPUT//
////////////////////

void Robot::handleInput(InputControlMode controlMode) {
//	handleDriveOrientation(controlMode);
//	handleDrive(controlMode);
//	handleLift(controlMode);
//	handleClaw(controlMode);
}

//void Robot::handleDriveOrientation(InputControlMode controlMode) {
//	switch (controlMode) {
//	case MasterControllerOnly:
//		if (master_controller.getValue(orientation_forward) == 1) {
//			setDriveOrientation(ForwardOrientation);
//		} else if (master_controller.getValue(orientation_backward) == 1) {
//			setDriveOrientation(BackwardOrientation);
//		}
//		break;
//	case NormalOverwritesPartnerContoller:
//		if (master_controller.getValue(orientation_forward) == 1) {
//			setDriveOrientation(ForwardOrientation);
//		} else if (master_controller.getValue(orientation_backward) == 1) {
//			setDriveOrientation(BackwardOrientation);
//		} else if (slave_controller.getValue(orientation_forward) == 1) {
//			setDriveOrientation(ForwardOrientation);
//		} else if (slave_controller.getValue(orientation_backward) == 1) {
//			setDriveOrientation(BackwardOrientation);
//		}
//		break;
//	}
//}

void Robot::driveOrientationInputController(InputControlMode controlMode) {
	bool masterForwardActive = master_controller.isInputActive(
			orientation_forward);
	bool masterBackwawrdActive = master_controller.isInputActive(
			orientation_backward);
	bool slaveForwardActive = slave_controller.isInputActive(
			orientation_forward);
	bool slaveBackwardActive = slave_controller.isInputActive(
			orientation_backward);

	bool masterActive = masterForwardActive || masterBackwawrdActive;
	bool slaveActive = slaveForwardActive || slaveBackwardActive;

	switch (controlMode) {
	case MasterOnly:
		driveOrientationController(master_controller);
		return;
	case SlaveOnly:
		driveOrientationController(slave_controller);
		return;
	case MasterAndSlaveEqualPriority:
		if (masterActive && !slaveActive) {
			driveOrientationController(master_controller);
		} else if (!masterActive && slaveActive) {
			driveOrientationController(slave_controller);
		} else if (masterActive && slaveActive) {
			//Dont send any input if both are trying to control it
		}
		return;
	case MasterHigherPriority:
		if (masterActive && !slaveActive) {
			driveOrientationController(master_controller);
		} else if (!masterActive && slaveActive) {
			driveOrientationController(slave_controller);
		} else if (masterActive && slaveActive) {
			driveOrientationController(master_controller);
		}
		return;
	case SlaveHigherPriority:
		if (masterActive && !slaveActive) {
			driveOrientationController(master_controller);
		} else if (!masterActive && slaveActive) {
			driveOrientationController(slave_controller);
		} else if (masterActive && slaveActive) {
			driveOrientationController(slave_controller);
		}
		return;
	}
}

void Robot::driveOrientationController(Controller &controller) {
	if (controller.isInputActive(orientation_forward)) {
		setDriveOrientation(ForwardOrientation);
	} else if (controller.getValue(orientation_backward) == 1) {
		setDriveOrientation(BackwardOrientation);
	}
}

void Robot::driveInputController(InputControlMode controlMode) {
	bool yMasterActiveDrive = master_controller.isInputActive(y_drive_stick);
	bool xMasterActiveDrive = master_controller.isInputActive(x_drive_stick);

	bool yMasterActiveStrafe = master_controller.isInputActive(y_strafe_stick);
	bool xMasterActiveStrafe = master_controller.isInputActive(x_strafe_stick);

	bool ySlaveActiveDrive = slave_controller.isInputActive(y_drive_stick);
	bool xSlaveActiveDrive = slave_controller.isInputActive(x_drive_stick);

	bool ySlaveActiveStrafe = slave_controller.isInputActive(y_strafe_stick);
	bool xSlaveActiveStrafe = slave_controller.isInputActive(x_strafe_stick);

	bool masterDriveActive = xMasterActiveDrive || yMasterActiveDrive;
	bool masterStrafeActive = xMasterActiveStrafe || yMasterActiveStrafe;
	bool masterActive = masterDriveActive || masterStrafeActive;

	bool slaveDriveActive = xSlaveActiveDrive || ySlaveActiveDrive;
	bool slaveStrafeActive = xSlaveActiveStrafe || ySlaveActiveStrafe;
	bool slaveActive = slaveDriveActive || slaveStrafeActive;

	switch (controlMode) {
	case MasterOnly:
		driveController(master_controller);
		return;
	case SlaveOnly:
		driveController(slave_controller);
		return;
	case MasterAndSlaveEqualPriority:
		if (masterActive && !slaveActive) {
			driveController(master_controller);
		} else if (!masterActive && slaveActive) {
			driveController(slave_controller);
		} else if (masterActive && slaveActive) {
			//Dont send any input if both are trying to control it
		} else if (!masterActive && !slaveActive) {
			stopDriveMotors();
		}
		return;
	case MasterHigherPriority:
		if (masterActive && !slaveActive) {
			driveController(master_controller);
		} else if (!masterActive && slaveActive) {
			driveController(slave_controller);
		} else if (masterActive && slaveActive) {
			driveController(master_controller);
		} else if (!masterActive && !slaveActive) {
			stopDriveMotors();
		}
		return;
	case SlaveHigherPriority:
		if (masterActive && !slaveActive) {
			driveController(master_controller);
		} else if (!masterActive && slaveActive) {
			driveController(slave_controller);
		} else if (masterActive && slaveActive) {
			driveController(slave_controller);
		} else if (!masterActive && !slaveActive) {
			stopDriveMotors();
		}
		return;
	}
}

void Robot::driveController(Controller &controller) {
	short y = controller.getValue(y_drive_stick);
	short x = controller.getValue(x_drive_stick);

	if (abs(x) < abs(controllerDeadzoneMagnitude)
			&& abs(y) < abs(controllerDeadzoneMagnitude)) {
		return;
	}

	if (abs(y) > abs(x)) {
		drive(y, ManualDrive);
	} else if (abs(x) >= abs(y)) {
		drive(x, ManualTurn);
	}
}

//void Robot::handleDrive(InputControlMode controlMode) {
//	short yMain = master_controller.getValue(y_drive_stick);
//	short xMain = master_controller.getValue(x_drive_stick);
//
//	short yPartner = slave_controller.getValue(y_drive_stick);
//	short xPartner = slave_controller.getValue(x_drive_stick);
//
//	switch (controlMode) {
//	case MasterControllerOnly:
//		if (abs(xMain) < abs(controllerDeadzoneMagnitude)
//				&& abs(yMain) < abs(controllerDeadzoneMagnitude)) {
//			return;
//		}
//
//		if (abs(yMain) > abs(xMain)) {
//			drive(yMain, ManualDrive);
//		} else if (abs(xMain) >= abs(yMain)) {
//			drive(xMain, ManualTurn);
//		}
//		break;
//	case NormalOverwritesPartnerContoller:
//		if (!(abs(xMain) < abs(controllerDeadzoneMagnitude)
//				&& abs(yMain) < abs(controllerDeadzoneMagnitude))) {
//			if (abs(yMain) > abs(xMain)) {
//				drive(yMain, ManualDrive);
//			} else if (abs(xMain) >= abs(yMain)) {
//				drive(xMain, ManualTurn);
//			}
//		} else if (!(abs(xPartner) < abs(controllerDeadzoneMagnitude)
//				&& abs(yPartner) < abs(controllerDeadzoneMagnitude))) {
//			if (abs(yPartner) > abs(xPartner)) {
//				drive(yPartner, ManualDrive);
//			} else if (abs(xPartner) >= abs(yPartner)) {
//				drive(xPartner, ManualTurn);
//			}
//		} else {
//			stopDriveMotors();
//		}
//		break;
//	}
//}
//
////returns false if the lift should be set to 0
//void Robot::handleLift(InputControlMode controlMode) {
//	switch (controlMode) {
//	case MasterControllerOnly:
//		if (master_controller.getValue(lift_up) == 1) {
//			lift(liftPower, Up);
//		} else if (master_controller.getValue(lift_down) == 1) {
//			lift(liftPower, Down);
//		} else {
//			stopLift();
//		}
//		break;
//	case NormalOverwritesPartnerContoller:
//		if (master_controller.getValue(lift_up) == 1) {
//			lift(liftPower, Up);
//		} else if (master_controller.getValue(lift_down) == 1) {
//			lift(liftPower, Down);
//		} else if (slave_controller.getValue(lift_up) == 1) {
//			lift(liftPower, Up);
//		} else if (slave_controller.getValue(lift_down) == 1) {
//			lift(liftPower, Down);
//		} else {
//			stopLift();
//		}
//		break;
//	}
//}
//
//void Robot::handleClaw(InputControlMode controlMode) {
//	switch (controlMode) {
//	case MasterControllerOnly:
//		clawArm(master_controller.getValue(claw_rotate), ManualClawArmControl);
//
//		if (master_controller.getValue(claw_open) == 1) {
//			claw(clawOpenPower, OpenClaw);
//		} else if (master_controller.getValue(claw_close) == 1) {
//			claw(clawClosePower, CloseClaw);
//		} else {
//			stopClaw();
//		}
//		break;
//	case NormalOverwritesPartnerContoller:
//		if (master_controller.getValue(claw_rotate) != 0) {
//			clawArm(master_controller.getValue(claw_rotate), ManualClawArmControl);
//		} else if (slave_controller.getValue(claw_rotate) != 0) {
//			clawArm(slave_controller.getValue(claw_rotate),
//					ManualClawArmControl);
//		} else {
//			stopClawArm();
//		}
//
//		if (master_controller.getValue(claw_open) == 1) {
//			claw(clawOpenPower, OpenClaw);
//		} else if (master_controller.getValue(claw_close) == 1) {
//			claw(clawClosePower, CloseClaw);
//		} else if (slave_controller.getValue(claw_open) == 1) {
//			claw(clawOpenPower, OpenClaw);
//		} else if (slave_controller.getValue(claw_close) == 1) {
//			claw(clawClosePower, CloseClaw);
//		} else {
//			stopClaw();
//		}
//		break;
//	}

//	if (controller->getValue(cl) == 1) {
//		clawArm(clawArmPower, RotateArmLeft);
//	} else if (controller->getValue(claw_rotate_right) == 1) {
//		clawArm(clawArmPower, RotateArmRight);
//	} else {
//		stopClawArm();
//	}
//}

///////////////////////////////
//SET MOTOR POINTER FUNCTIONS//
///////////////////////////////

bool Robot::setDriveMotors(Motor* driveMotors[], char numDriveMotors) {
	if (numDriveMotors > 6) {
		printf("[Error] Too many drive motors in array\n\r");
		return false;
	}

	for (int i = 0; i < numDriveMotors; i++) {
		this->driveMotors[i] = driveMotors[i];
	}
	this->numDriveMotors = numDriveMotors;
	return true;
}

bool Robot::setLiftMotors(Motor* liftMotors[], char numLiftMotors) {
	if (numLiftMotors > 10) {
		printf("[Error] Too many lift motors in array\n\r");
		return false;
	}

	for (int i = 0; i < numLiftMotors; i++) {
		this->liftMotors[i] = liftMotors[i];
	}
	this->numLiftMotors = numLiftMotors;
	return true;
}

void Robot::setIntakeMotors(Motor* claw, Motor* clawArm) {
	this->clawMotor = claw;
	this->clawArmMotor = clawArm;
}

///////////////////////////////
//DRIVE ORIENTATION FUNCTIONS//
///////////////////////////////

void Robot::setDriveOrientation(DriveOrientation orientation) {
	this->orientation = orientation;
}

void Robot::reverseDriveOrientation() {
	if (orientation == ForwardOrientation) {
		orientation = BackwardOrientation;
	} else if (orientation == BackwardOrientation) {
		orientation = ForwardOrientation;
	}
}

///////////////////
//DRIVE FUNCTIONS//
///////////////////

void Robot::drive(int power, DriveDirection dir) {
	if (orientation == BackwardOrientation) {
		switch (dir) {
		case ManualDrive:
			power = -power;
			break;
		case ManualTurn:
			//Nothing needs to be done
			break;
		case ManualStrafe:
			power = -power;
			break;
		case DriveForward:
			dir = DriveBackward;
			break;
		case DriveBackward:
			dir = DriveForward;
			break;
		case TurnLeft:
			//Nothing needs to be done
			break;
		case TurnRight:
			//Nothing needs to be done
			break;
		case StrafeLeft:
			dir = StrafeRight;
			break;
		case StrafeRight:
			dir = StrafeLeft;
			break;
		}
	}

	switch (dir) {
	case ManualDrive:
		for (int i = 0; i < numDriveMotors; i++) {
			Motor* motor = driveMotors[i];
			motor->setPower(power);
		}
		break;
	case ManualTurn:
		for (int i = 0; i < numDriveMotors; i++) {
			Motor* motor = driveMotors[i];
			MotorLocationSide side = motor->getLocationSide();
			switch (side) {
			case LeftSide:
				motor->setPower(power);
				break;
			case RightSide:
				motor->setPower(-power);
				break;
			default:
				printf(
						"[Warning] A motor on the drive does not have it's location properly set.\n\r");
				break;
			}
		}
		break;
	case ManualStrafe:
		for (int i = 0; i < numDriveMotors; i++) {
			Motor* motor = driveMotors[i];
			MotorLocation location = motor->getLocation();
			switch (location) {
			case FrontRight:
				motor->setPower(-power);
				break;
			case FrontLeft:
				motor->setPower(power);
				break;
			case BackRight:
				motor->setPower(power);
				break;
			case BackLeft:
				motor->setPower(-power);
			default:
				printf(
						"[Warning] A motor on the drive does not have it's location properly set for strafing.\n\r");
				break;
			}
		}
		break;
	case DriveForward:
		power = abs(power);
		for (int i = 0; i < numDriveMotors; i++) {
			Motor* motor = driveMotors[i];
			motor->setPower(power);
		}
		break;
	case DriveBackward:
		power = -abs(power);
		for (int i = 0; i < numDriveMotors; i++) {
			Motor* motor = driveMotors[i];
			motor->setPower(power);
		}
		break;
	case TurnLeft:
		power = abs(power);
		for (int i = 0; i < numDriveMotors; i++) {
			Motor* motor = driveMotors[i];
			MotorLocationSide side = motor->getLocationSide();
			switch (side) {
			case LeftSide:
				motor->setPower(-power);
				break;
			case RightSide:
				motor->setPower(power);
				break;
			default:
				printf(
						"[Warning] A motor on the drive does not have it's location properly set.\n\r");
			}
		}
		break;
	case TurnRight:
		power = abs(power);
		for (int i = 0; i < numDriveMotors; i++) {
			Motor* motor = driveMotors[i];
			MotorLocationSide side = motor->getLocationSide();
			switch (side) {
			case LeftSide:
				motor->setPower(power);
				break;
			case RightSide:
				motor->setPower(-power);
				break;
			default:
				printf(
						"[Warning] A motor on the drive does not have it's location properly set.\n\r");
				break;
			}
		}
		break;
	case StrafeLeft:
		power = abs(power);
		for (int i = 0; i < numDriveMotors; i++) {
			Motor* motor = driveMotors[i];
			MotorLocation location = motor->getLocation();
			switch (location) {
			case FrontRight:
				motor->setPower(power);
				break;
			case FrontLeft:
				motor->setPower(-power);
				break;
			case BackRight:
				motor->setPower(-power);
				break;
			case BackLeft:
				motor->setPower(power);
			default:
				printf(
						"[Warning] A motor on the drive does not have it's location properly set for strafing.\n\r");
				break;
			}
		}
		break;
	case StrafeRight:
		power = abs(power);
		for (int i = 0; i < numDriveMotors; i++) {
			Motor* motor = driveMotors[i];
			MotorLocation location = motor->getLocation();
			switch (location) {
			case FrontRight:
				motor->setPower(-power);
				break;
			case FrontLeft:
				motor->setPower(power);
				break;
			case BackRight:
				motor->setPower(power);
				break;
			case BackLeft:
				motor->setPower(-power);
			default:
				printf(
						"[Warning] A motor on the drive does not have it's location properly set for strafing.\n\r");
				break;
			}
		}
		break;
	}
}

void Robot::stopDriveMotors() {
	for (int i = 0; i < numDriveMotors; i++) {
		driveMotors[i]->stop();
	}
}

//////////////////
//LIFT FUNCTIONS//
//////////////////

void Robot::lift(int power, LiftDirection dir) {
	switch (dir) {
	case ManualLift:
		for (int i = 0; i < numLiftMotors; i++) {
			liftMotors[i]->setPower(power);
		}
		break;
	case Up:
		power = abs(power);
		for (int i = 0; i < numLiftMotors; i++) {
			liftMotors[i]->setPower(power);
		}
		break;
	case Down:
		power = abs(power);
		for (int i = 0; i < numLiftMotors; i++) {
			liftMotors[i]->setPower(-power);
		}
		break;
	}
}

void Robot::stopLift() {
	lift(0, ManualLift);
}

//////////////////
//CLAW FUNCTIONS//
//////////////////

void Robot::claw(int power, ClawDirection dir) {
	switch (dir) {
	case ManualClawControl:
		clawMotor->setPower(power);
		break;
	case OpenClaw:
		power = abs(power);
		clawMotor->setPower(power);
		break;
	case CloseClaw:
		power = abs(power);
		clawMotor->setPower(-power);
		break;
	}
}

void Robot::stopClaw() {
	clawMotor->stop();
}

//////////////////////
//CLAW ARM FUNCTIONS//
//////////////////////

void Robot::clawArm(int power, ClawArmDirection dir) {
	switch (dir) {
	case ManualClawArmControl:
		clawArmMotor->setPower(power);
		break;
	case RotateArmLeft:
		power = abs(power);
		clawArmMotor->setPower(-power);
		break;
	case RotateArmRight:
		power = abs(power);
		clawArmMotor->setPower(power);
		break;
	}
}

void Robot::stopClawArm() {
	clawArmMotor->stop();
}

}
/* namespace TRL */
