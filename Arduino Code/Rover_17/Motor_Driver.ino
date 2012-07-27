//All motor functions are here, including error response, turning, and driving. 

void stopIfFault() {
  if (md.getFault()) {
    float restartTimer = millis(); 
    while(restartTimer > millis() + restartTime){
      Serial.println("Fault: motors restarting.");
      md.setM1Speed(0); 
      md.setM2Speed(0);
      //motorFault(); 
    }
  }
}

void calibrateCurrentAngle(){
  long currentTime = millis(); 
  while(currentTime + 1000 > millis()){
    md.setM1Speed(calibrationSpeed); 
    md.setM2Speed(calibrationSpeed); 
  } 
  md.setM1Speed(0); 
  md.setM2Speed(0); 
  calibrated = true; 
}

void turnToDegree(float finalAngle){
  int turnDirection1, turnDirection2; 
  if(finalAngle < currentHeading){
    turnDirection1 = 1;  //LEFT
    turnDirection2 = -1; 
    Serial.println("left"); 
  }
  else{
    turnDirection1 = -1;
    turnDirection2 = 1;  
    Serial.println("right");  
  }
    long currentTime = millis();  
while(currentTime > millis() + 5000 && abs(currentHeading - finalAngle) > 4){
    Wire.beginTransmission(HMC6352SlaveAddress);
    Wire.write(HMC6352ReadAddress);     
    Wire.endTransmission();
    delay(6);
    Wire.requestFrom(HMC6352SlaveAddress, 2); 
    byte MSB = Wire.read();
    byte LSB = Wire.read();

    float headingSum = (MSB << 8) + LSB; 
    float headingInt = headingSum / 10; 

    if(headingInt < 360 - magFlux){
      headingInt += magFlux; 
    }
    else if(headingInt > 360 - magFlux && headingInt < 360){
      float difference = 360 - headingInt; 
      headingInt = magFlux - difference; 
    } 

    currentHeading = headingInt; 
    Serial.println(currentHeading); 
    md.setM1Speed(turningSpeed * turnDirection1); 
    md.setM2Speed(turningSpeed * turnDirection2);
  }
  md.setM1Speed(0); 
  md.setM2Speed(0); 
  Serial.println("Done"); 
  delay(200); 
  calibrated = false; 
}

void drive(){
  if(coordDist(currentLocLat, currentLocLong, sentTargetLat, sentTargetLon, 1) < 2000){
    md.setM1Speed(m1DriveSpeed); 
    md.setM2Speed(m2DriveSpeed); 
  } 
}





















