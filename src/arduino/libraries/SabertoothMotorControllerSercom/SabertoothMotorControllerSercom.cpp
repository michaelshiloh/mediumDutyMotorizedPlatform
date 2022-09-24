#include "Arduino.h"
#include "SabertoothMotorControllerSercom.h"

Uart SabertoothMotorController::SabertoothMotorController 
	( int _motorTimeOut)
{
	motorTimeOut = _motorTimeOut;
}

void SabertoothMotorController::init() {
	stopBothMotors();
}

void SabertoothMotorController::forward(int _speed){
	controlMotor1(_speed, MYFORWARD);
	controlMotor2(_speed, MYFORWARD);
}

void SabertoothMotorController::backward(int _speed){
		controlMotor1(_speed, MYREVERSE);
		controlMotor2(_speed, MYREVERSE);
}

void SabertoothMotorController::left(int _speed){
		controlMotor1(_speed, MYFORWARD);
		controlMotor2(_speed, MYREVERSE);
}

void SabertoothMotorController::right(int _speed){
		controlMotor1(_speed, MYREVERSE);
		controlMotor2(_speed, MYFORWARD);
}

void SabertoothMotorController::tick() {
  if ((motor1OnAt != -1) && ((millis() - motor1OnAt ) > motorTimeOut )) {
    stopMotor1();
		Serial.println("motorControllerTick(): motor 1 timeout");
  }
  if ((motor2OnAt != -1) && ((millis() - motor2OnAt ) > motorTimeOut )) {
    stopMotor2();
		Serial.println("motorControllerTick(): motor 2 timeout");
  }
}

/*
   Only functions below this line
  write to the motor controller serial port
*/

/*    0 = stop
      1 = slowest
      1-63 = speed
    63-255 = full speed
*/

void SabertoothMotorController::controlMotor1(int speed, bool direction) {
  Serial.print("Motor 1, direction = ");
  Serial.println(direction);

  /*
     From the documentation:

     Sending a character between 1 and 127 will control
     motor 1. 1 is full reverse, 64 is stop and 127 is full forward.

     Character 0 (hex 0x00) is a special case.
     Sending this character will shut down both motors.
  */
  if (speed == 0) {
    stopMotor1();
  }

  if (direction == MYREVERSE) {
    speed = map(speed, 0, 255, 65, 127);
    speed = constrain(speed, 65, 127);
  } else {
    speed = map(speed, 0, 255, 63, 1);
    speed = constrain(speed, 1, 63);
  }

  Serial.print("M1 speed = ");
  Serial.println(speed);

  motorControllerSerial.write(speed);
  motor1OnAt = millis();
}

void SabertoothMotorController::controlMotor2(int speed, bool direction) {
  Serial.print("Motor 2, direction = ");
  Serial.println(direction);
  /*
     From the documentation:


     Sending a character between 128 and 255 will control motor 2. 128
     is full reverse, 192 is stop and 255 is full forward.

     Character 0 (hex 0x00) is a special case.
     Sending this character will shut down both motors.
  */
  if (speed == 0) {
    stopMotor2();
  }

  if (direction == MYREVERSE) {
    speed = map(speed, 0, 255, 191, 128);
    speed = constrain(speed, 128, 191);
  } else {
    speed = map(speed, 0, 255, 193, 255);
    speed = constrain(speed, 193, 255);
  }

  Serial.print("M2 speed = ");
  Serial.println(speed);

  motorControllerSerial.write(speed);
  motor2OnAt = millis();

}

void SabertoothMotorController::stopBothMotors() {
  Serial.println("stopBothMotors");
  motorControllerSerial.write((byte)0);
  motor1OnAt = -1;
  motor2OnAt = -1;
}

void SabertoothMotorController::stopMotor1() {
  motorControllerSerial.write(64);
  motor1OnAt = -1;
}

void SabertoothMotorController::stopMotor2() {
  motorControllerSerial.write(192);
  motor2OnAt = -1;
}
