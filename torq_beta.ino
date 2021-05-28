/******************************************************************************
*                                                                             *
*   Program : FileLogger                                                      *
*                                                                             *
*   FILE : torq_beta.ino                                                      *
*                                                                             *
*   Date : 28 / 05 / 2021                                                     *
*                                                                             *
*   Programmers : Team Software "TORQ"                                        *
*                                                                             *
******************************************************************************/

#include "hardwareInterface.h"

HardwareInterface hardwareInterface(
        13, A3, //Ultrasonic_LEFT
        8, 12, //Ultrasonic_RIGHT
        
        A1, 0, //Infrared_LEFT
        A5, 0, //Infrared_RIGHT
        A4, 0,  //Infrared_FRONT
        
        6, 5, //ENGINE
        4,
        
        3 // SERVO
        );

const int threshold = 21;

int last_condition = FRONT; //BY DEFAULT SET TO FRONT TO ROTATE BY DEFAULT ALGHORYTHM
int obstacle_trigger = 0; //GLOBAL VARIABLE TO TRACK OBSTACLE CHECKS

void setup() //INIT
{ 
  hardwareInterface.driveShaft.p_mainMotor.init();
  hardwareInterface.driveShaft.p_servoMotor.init(); 
  
  hardwareInterface.sensorPanel.Ultrasonic_LEFT.init();
  hardwareInterface.sensorPanel.Ultrasonic_RIGHT.init();

  hardwareInterface.sensorPanel.Infrared_LEFT.init();
  hardwareInterface.sensorPanel.Infrared_RIGHT.init();
  hardwareInterface.sensorPanel.Infrared_FRONT.init();
  
  Serial.begin(9600);
}

void loop() // THE ONE AND THE ONLY RUNNING LOOP, STANDART ONE
{
  if (hardwareInterface.sensorPanel.Ultrasonic_LEFT.get_distance() < threshold && hardwareInterface.sensorPanel.Ultrasonic_RIGHT.get_distance() < threshold && obstacle_trigger == 0) // Ultrasonic distance check if its less or equals than we should avoid obstacle
  {
    if (hardwareInterface.sensorPanel.Ultrasonic_LEFT.get_distance() < threshold)
    { 
      hardwareInterface.driveShaft.turnRight();
      obstacle_trigger = 1;
      
      hardwareInterface.driveShaft.p_mainMotor.set_rpm(110);
      delay(80);
      hardwareInterface.driveShaft.turnLeft();
      delay(190);
      
      last_condition = LEFT;
    }
    
    if (hardwareInterface.sensorPanel.Ultrasonic_RIGHT.get_distance() < threshold)
    {
      hardwareInterface.driveShaft.turnLeft();
      obstacle_trigger = 1;
      
      hardwareInterface.driveShaft.p_mainMotor.set_rpm(110);
      delay(80);
      hardwareInterface.driveShaft.turnRight();
      delay(190);
      
      last_condition = RIGHT;
    }
  }
  else
  {
    if (last_condition == FRONT)
    {
      if (hardwareInterface.sensorPanel.Infrared_FRONT.read_pin() == HIGH)
      {
        hardwareInterface.driveShaft.moveForward();
      }
      else
      {
        if(hardwareInterface.sensorPanel.Infrared_LEFT.read_pin() == HIGH)
        {
          last_condition = LEFT;
          hardwareInterface.driveShaft.turnLeft();
        }
        else if(hardwareInterface.sensorPanel.Infrared_RIGHT.read_pin() == HIGH)
        {
          last_condition = RIGHT;
          hardwareInterface.driveShaft.turnRight();
        }
      }
    }
    else
    {
      if (last_condition == LEFT) // exeption for long left turning condition
      {
        hardwareInterface.driveShaft.turnLeft();
        
        if(hardwareInterface.sensorPanel.Infrared_RIGHT.read_pin() == HIGH)
        {
          obstacle_trigger = 0;
          last_condition = FRONT;
        }
      }
      else if(last_condition == RIGHT) // exeption for long right turning condition
      {
        hardwareInterface.driveShaft.turnRight();
        
        if(hardwareInterface.sensorPanel.Infrared_LEFT.read_pin() == HIGH)
        {
          obstacle_trigger = 0;
          last_condition = FRONT;
        }
      }
    }
  }
}
