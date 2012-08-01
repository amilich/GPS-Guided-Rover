//TRIG AND MATH FUNCTIONS BELOW. 
/* 
 The coordDistance float uses the Haversine formula to find the distance between two latitude and longitude points.
 It finds the vertical and horizontal distance between the car's current and target location. 
 This data aids the arduino in approximating the tangent of the angle relative to true north. 
 */

float TargetTurnDegree(){
  float latLength = coordDist(currentLocLat, currentLocLong, sentTargetLat, currentLocLong, 1); 
  float lonLength = coordDist(currentLocLat, currentLocLong, currentLocLat, sentTargetLon, 1);
  if(latLength == 0)
    latLength == 0.0001; 
  if(lonLength == 0)
    lonLength == 0.0001; 
  turningAngle = degrees(atan(lonLength/latLength));
  if(sentTargetLat < currentLocLat){
    if(sentTargetLon > currentLocLong){
      turningAngle = 180 - turningAngle;
    }
    else{
      turningAngle = -180 + turningAngle;
    }
  }
  else {
    if(sentTargetLon > currentLocLong){
      turningAngle = turningAngle;
    }
    else{
      turningAngle = -turningAngle;
    }
  }
  if(turningAngle > 0){
    return turningAngle; 
  }
  else{
    return 360 + turningAngle;    
  }
}

float coordDist(float currentLat, float currentLon, float targetLat, float targetLon, float unit_conversion){
  //Pythagorean formula (not as accurate)

  /*float latDiff = abs(currentLat - targetLat); 
   float longDiff = abs(currentLon - targetLon); 
   latDiff = pow(latDiff, 2); 
   longDiff = pow(longDiff, 2); 
   float total = latDiff + longDiff; 
   return sqrt(total); */

  //Haversine formula

  float dLat = radians(targetLat - currentLat);
  float dLon = radians(targetLon - currentLon);
  float a = sin(dLat/2.0f)*sin(dLat / 2.0f) + cos(radians(currentLat)) * cos(radians(targetLat)) *
    sin(dLon/2.0f)*sin(dLon/2.0f);

  float d = 2.0f * atan2(sqrt(a), sqrt(1.0f - a));
  return d * Earth_Radius * unit_conversion;
} 




