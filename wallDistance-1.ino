#include "MyEncoders.h"
#include "MyServos.h"
#include "MySharpSensor.h"
#include <EEPROM.h>

float K = 0.6;  //Just have this here for now. We'll have to try all the K's
float desiredDistance = 5.0;   //Desired distance to goal
float distanceError, u;  //Just initializing these variables up here
float shortSensor;
bool first = true;

void setup() {
  Serial.begin(115200);
  initEncoders();
  LServo.attach(2);
  RServo.attach(3);
}

void loop() {
  delay(80);    //Delay of 80 is good enough so that the sensors will work
  if(first){
    first = false;
    //calibrate();
    EEPROM.get(0, RPSMapLeft);
    EEPROM.get(sizeof(int) * 161, RPSMapRight);
  }
  
  takeNewMeasurement(0);
 
  shortSensor = getCombinedDistance(0);

  distanceError = shortSensor - desiredDistance;

  
  u = K * distanceError;

  //check if error higher than 6
  if(u > 6)
  {
    setSpeedsIPS(6, 6);
  }
  //check if error is lower than -6
  else if(u < -6)
  {
    setSpeedsIPS(-6, -6);
  }

  //if it's between the two just use the error
  else
  {
    setSpeedsIPS(u, u);
  }

}
