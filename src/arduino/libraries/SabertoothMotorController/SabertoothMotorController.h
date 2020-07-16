/*
   Motor controller for medium duty motorized platform. 
	 Communicates with the Sabertooth2x12 motor controller from Dimensional 
	 Engineering using a software serial port.

   Based on simpleMotorControlWebPage from Telepresence robot

	 Created by Michael Shiloh

  20 June 2020 - MS - incorporated into integrated program
  11 June 2020 - MS - Created
  29 June 2020 - MS - add a timeout for each command
	04 July 2020 - MS - moved to its own repository
	15 July 2020 - MS - created library

  TODO
  - Handle millis overflowing (long = ? seconds)
  - All those hard coded numbers in motor control should be consts
	- Add an example to show how to initialize and control!

Instructions

   Set Motor Controller DIP switch:
	(non-lithium battery, simplified serial, single controller, 9600 baud)
   1 - on
   2 - off
   3 - on
   4 - off
   5 - on
   6 - on
*/

#ifndef SabertoothMotorController_h
#define SabertoothMotorController_h

#include "Arduino.h"
#include <SoftwareSerial.h>

class SabertoothMotorController 
{
  public:

    SabertoothMotorController (int _rxPin, int _txPin, int _motorTimeOut);
		void init();
		void forward(int _speed);
		void backward(int _speed);
		void left(int _speed);
		void right(int _speed);
		void tick();

	private:

		long motorTimeOut = 90; // stop after this many milliseconds
		long motor1OnAt = -1; // -1 means not running
		long motor2OnAt = -1; //
		SoftwareSerial *motorControllerSerialPort;
		int txPin, rxPin;
		// for controlling the motors
		const int MYFORWARD = 0;
		const int MYREVERSE = 1;
		int forwardSpeed = 80;

		void controlMotor1(int speed, bool direction);
		void controlMotor2(int speed, bool direction);
		void stopBothMotors();
		void stopMotor1();
		void stopMotor2();
};

#endif
