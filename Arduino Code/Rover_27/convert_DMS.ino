//needed to use GPS data - convert from DMS to decimal

float calculateCurrentLocation(double inputCoord){    
  //conversion below
  uint16_t wholeDegrees = 0.01*inputCoord;
  return wholeDegrees + (inputCoord - 100.0*wholeDegrees)/60.0;
}

void getLatLong(){
  currentLocLat = calculateCurrentLocation(GPS.latitude); 
  currentLocLong = calculateCurrentLocation(GPS.longitude); 
  signChange();  
}

