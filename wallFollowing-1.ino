#include "MyEncoders.h"
#include "MyServos.h"
#include "MySharpSensor.h"

float K = 0.6;  //Just have this here for now. We'll have to try all the K's
float desiredDistance = 5.0;   //Desired distance to goal
float distanceErrorFront, distanceErrorRight, uFront, uRight;  //Just initializing these variables up here
float shortSensor, rightSensor;
bool first = true;
float radius;

void setup() {
  Serial.begin(115200);
  initEncoders();
  LServo.attach(2);
  RServo.attach(3);
}

void loop() {
  delay(60);    //Delay of 60 is good enough so that the sensors will work
  if(first){
    first = false;
    //calibrate();
    EEPROM.get(0, RPSMapLeft);
    EEPROM.get(sizeof(int) * 161, RPSMapRight);

  }
  
  takeNewMeasurement(SSFRONT);
  takeNewMeasurement(SRIGHT);
 
  shortSensor = getCombinedDistance(SSFRONT);
  rightSensor = getCombinedDistance(SRIGHT);

  distanceErrorFront = shortSensor - desiredDistance;
  distanceErrorRight = desiredDistance - rightSensor;

  /*Short sensor is too close to the wall so it must turn until it is no longer so close*/
  if(shortSensor <= 5.4){
    while(shortSensor < 14){
      setSpeedsvw(0, 1);
      takeNewMeasurement(SSFRONT);
      shortSensor = getCombinedDistance(SSFRONT);
      delay(60);
    }
  }

  
  uFront = K * distanceErrorFront;
  uRight = K * distanceErrorRight;

  //Front Sensor. Saturation function
  //check if error higher than 4
  if(uFront > 4)
  {
    uFront = 4;
  }
  //check if error is lower than -4
  else if(uFront < -4)
  {
    uFront = -4;
  }
  if(uRight == 0)
    uRight = 0.0001;

  //Used to figure out if the robot is going to turn too hard where it shouldn't
  radius = uFront / uRight;

  //Another saturation function
  //Keeps the robot from turning to hard if it is close enough to a wall
  if(rightSensor <= 12 && radius < 9 && radius > -9){
    if(radius > 0)
      uRight = uFront / 9.0;
    else
      uRight = uFront / -9.0;
  }
  //Otherwise, it there is no wall that is close, it begin to turn. This is for a corner
  else if(rightSensor > 13){
    if(radius > 0)
      uRight = uFront / 2.0;
    else
      uRight = uFront / -2.0;
  }


  setSpeedsvw(uFront, uRight);

  


}
