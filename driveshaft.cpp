/******************************************************************************
*                                                                             *
*   Program : TORQ beta                                                       *
*                                                                             *
*   FILE : driveshaft.cpp                                                     *
*                                                                             *
*   Date : 28 / 05 / 2021                                                     *
*                                                                             *
*   Programmers : Team Software "TORQ"                                        *
*                                                                             *
******************************************************************************/

#include "driveShaft.h"

DriveShaft::DriveShaft(byte m_epin, byte m_inapin, byte m_inbpin, byte m_s_pin) : p_mainMotor(m_epin, m_inapin, m_inbpin), p_servoMotor(m_s_pin)
{
}

void DriveShaft::moveForward()
{
  p_servoMotor.reset();
	p_mainMotor.set_rpm(180);
	p_mainMotor.rotate_clockwise();
}

void DriveShaft::turnLeft()
{
  p_servoMotor.reset();
  p_servoMotor.rotate_left(18);
	p_mainMotor.set_rpm(145);
	p_mainMotor.rotate_clockwise();
}

void DriveShaft::turnRight()
{
  p_servoMotor.reset();
  p_servoMotor.rotate_right(18);
	p_mainMotor.set_rpm(145);
	p_mainMotor.rotate_clockwise();
}

void DriveShaft::stopMovement()
{
	p_servoMotor.reset();
	p_mainMotor.rotate_stop();
}
