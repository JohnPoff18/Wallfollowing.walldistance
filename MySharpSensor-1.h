int N = 5;
float SLEFTarr[5];
float SSFRONTarr[5];
float SRIGHTarr[5];
float SLONGarr[5];
int SLEFTpos = 0, SSFRONTpos = 0, SRIGHTpos = 0, SLONGpos = 0;

#ifndef __MY_SHARP_SENSOR__
#define __MY_SHARP_SENSOR__

//this function takes as input the value obtained by a short distance sensor,
//and it should return its respective value in inches
float shortToInches(int value){
  float voltage, distanceInCm, trueDistance;
  float convertToCm = 0.3937;

  voltage = value / 205.0;     //Dividing by 205 since 1V = 205 units
  distanceInCm = 12.5864 * pow(voltage, -1.043);
  trueDistance = distanceInCm * convertToCm;

  return trueDistance;
}

//this function takes as input the value obtained by the long distance sensor,
//and it should return its respective value in inches
float longToInches(int value){
  float voltage, distanceInCm, trueDistance;
  float convertToCm = 0.3937;
  float inSqrt;
  float distanceInverse;

  voltage = value / 205.0;     //Dividing by 205 since 1V = 205 units
 /* distanceInCm = 61.9176 * pow(voltage, -1.096);     //Found equation using excel
  trueDistance = distanceInCm * convertToCm;*/

  inSqrt = -1987.6 * voltage + 5816.333496;
  distanceInverse = (-77.124 + sqrt(inSqrt)) / 993.8;
  distanceInCm = -1.0 / distanceInverse;
  trueDistance = distanceInCm * convertToCm;
  
  

  return trueDistance;
}


//the following two functions are meant for combining (taking the mean, median or etc) multiple measurements

//this function should read and store a new measurement taken with sensor given by the parameter 'sensor'
#define SLEFT   1
#define SSFRONT 0
#define SRIGHT  2
#define SLONG   3
void takeNewMeasurement(int sensor)
{
  if(sensor == SLEFT){
    int val  = analogRead(SLEFT);
    SLEFTarr[SLEFTpos] = shortToInches(val);
    SLEFTpos++;
    SLEFTpos = SLEFTpos % N; 
  }
  else if(sensor == SSFRONT){
    int val  = analogRead(SSFRONT);
    SSFRONTarr[SSFRONTpos] = shortToInches(val);
    SSFRONTpos++;
    SSFRONTpos = SSFRONTpos % N;
  }
  else if(sensor == SRIGHT){
    int val  = analogRead(SRIGHT);
    SRIGHTarr[SRIGHTpos] = shortToInches(val);
    SRIGHTpos++;
    SRIGHTpos = SRIGHTpos % N;
  }
  else if(sensor == SLONG){
    int val  = analogRead(SLONG);
    SLONGarr[SLONGpos] = longToInches(val);
    SLONGpos++;
    SLONGpos = SLONGpos % N;
  }
   
}


//this function should combine the last N measurement taken with sensor 'sensor' and return the result.
//You are free to chose the value of N
float getCombinedDistance(int sensor)
{
  float count = 0;
  float sum = 0;
  int i;
  
  if(sensor == SLEFT){
    for(i = 0; i < N; i++){
      if(SLEFTarr[i] != 0){
        sum += SLEFTarr[i];
        count++;
      }
    }
    return sum / count;
  }
  else if(sensor == SSFRONT){
    for(i = 0; i < N; i++){
      if(SSFRONTarr[i] != 0){
        sum += SSFRONTarr[i];
        count++;
      }
    }
    return sum / count;
  }
  else if(sensor == SRIGHT){
    for(i = 0; i < N; i++){
      if(SRIGHTarr[i] != 0){
        sum += SRIGHTarr[i];
        count++;
      }
    }
    return sum / count;
  }
  else if(sensor == SLONG){
    for(i = 0; i < N; i++){
      if(SLONGarr[i] != 0){
        sum += SLONGarr[i];
        count++;
      }
    }
    return sum / count;
  }
}

#endif
