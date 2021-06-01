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

#define MAX_SPEED 500
#define DEFAULT_SPEED 160
#define TURNING_SPEED 140
#define OBSTACLE_SPEED 120
#define UP_HILL_SPEED 300
#define DOWN_HILL_SPEED 30

const int threshold = 22;

int current_speed = 0;
 
int last_condition = FRONT; //BY DEFAULT SET TO FRONT TO ROTATE BY DEFAULT ALGHORYTHM

int obstacle_trigger = 0; //GLOBAL VARIABLE TO TRACK OBSTACLE CHECKS


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
  
  hardwareInterface.driveShaft.powerStart();
}

void loop() // THE ONE AND THE ONLY RUNNING LOOP, STANDART ONE
{
  
  Serial.print("\n");
        Serial.print("0-LEFT | 1-RIGHT | 2-FRONT : ");
        Serial.print(last_condition);
  if (hardwareInterface.sensorPanel.Ultrasonic_LEFT.get_distance() < threshold && obstacle_trigger == 0) // Ultrasonic distance check if its less or equals than we should avoid obstacle
  {
    if (hardwareInterface.sensorPanel.Ultrasonic_LEFT.get_distance() < threshold && hardwareInterface.sensorPanel.Ultrasonic_LEFT.get_distance() != 0)
    { Serial.print("\n");
        Serial.print("LUS");
        Serial.print(hardwareInterface.sensorPanel.Ultrasonic_LEFT.get_distance());
      
      
      hardwareInterface.driveShaft.stopMovement();
      hardwareInterface.driveShaft.p_mainMotor.rotate_anti_clockwise();
      delay(1100);
      
      hardwareInterface.driveShaft.turnRight(MAX_SPEED, 18);
      obstacle_trigger = 1;
      delay(1150);
      hardwareInterface.driveShaft.turnLeft(120, 9);
      delay(800);
      
      last_condition = LEFT;
    }
  }
  else
  {   
  if(hardwareInterface.sensorPanel.Infrared_LEFT.read_pin() == HIGH && hardwareInterface.sensorPanel.Infrared_RIGHT.read_pin() == HIGH && hardwareInterface.sensorPanel.Infrared_FRONT.read_pin() == HIGH)
  {
    hardwareInterface.driveShaft.stopMovement();
  }
  else
  { 
    if (last_condition == FRONT)
    { 
      if (hardwareInterface.sensorPanel.Infrared_FRONT.read_pin() == HIGH)
      {
        current_speed = DEFAULT_SPEED;
        Serial.print("\n");
        Serial.print("STATE: ");
        Serial.print(last_condition);
        hardwareInterface.driveShaft.moveForward(current_speed);
      }
      else
      {
        if(hardwareInterface.sensorPanel.Infrared_LEFT.read_pin() == HIGH)
        {
          current_speed = TURNING_SPEED;
          Serial.print("\n");
        Serial.print("STATE: ");
        Serial.print(last_condition);
          last_condition = LEFT;
          hardwareInterface.driveShaft.turnLeft(current_speed, 7);
        }
        else if(hardwareInterface.sensorPanel.Infrared_RIGHT.read_pin() == HIGH)
        {
          current_speed = TURNING_SPEED;
          Serial.print("\n");
          Serial.print("STATE: ");
          Serial.print(last_condition);
          last_condition = RIGHT;
          hardwareInterface.driveShaft.turnRight(current_speed, 7);
        }
      }
    }
    else
    {
      if (last_condition == LEFT) // exeption for long left turning condition
      {
        current_speed = TURNING_SPEED ;
        hardwareInterface.driveShaft.turnLeft(current_speed, 18);
        Serial.print("\n");
        Serial.print("STATE: ");
        Serial.print(last_condition);
        if(hardwareInterface.sensorPanel.Infrared_FRONT.read_pin() == HIGH)
        {
          obstacle_trigger = 0;
          last_condition = FRONT;
        }
      }
      else if(last_condition == RIGHT) // exeption for long right turning condition
      {
        current_speed = TURNING_SPEED ;
        hardwareInterface.driveShaft.turnRight(current_speed, 18);
        Serial.print("\n");
        Serial.print("STATE: ");
        Serial.print(last_condition);
        if(hardwareInterface.sensorPanel.Infrared_FRONT.read_pin() == HIGH)
        {
          obstacle_trigger = 0;
          last_condition = FRONT;
        }
      }
    }
  }
  }
}
