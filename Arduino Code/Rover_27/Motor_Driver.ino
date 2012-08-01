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

void turnToDegree(float finalAngle) {
  int turnDirection1, turnDirection2; 
  if(finalAngle < currentHeading) {
    turnDirection1 = 1;  //LEFT
    turnDirection2 = -1; 
    Serial.println("left"); 
  }
  else{
    turnDirection1 = -1;
    turnDirection2 = 1;  
    Serial.println("right");  
  }

  if(finalAngle + headingAcc < 360 && finalAngle - headingAcc > 0) {
    while(abs(currentHeading - finalAngle) >= headingAcc) {
      currentHeading = getHeading(); 
      Serial.print(currentHeading);
      Serial.print(" degrees");
      delay(100);
      md.setM1Speed(turningSpeed * turnDirection1); 
      md.setM2Speed(turningSpeed * turnDirection2);
    }
  }
  else if(finalAngle + headingAcc >= 360 && finalAngle - headingAcc > 0) {
    while(currentHeading < finalAngle - headingAcc && currentHeading > finalAngle + headingAcc - 360) {
      currentHeading = getHeading(); 
      Serial.print(currentHeading);
      Serial.print(" degrees");
      delay(100);
      md.setM1Speed(turningSpeed * turnDirection1); 
      md.setM2Speed(turningSpeed * turnDirection2);
    }
  }
  else if(finalAngle + headingAcc < 360 && finalAngle - headingAcc < 0) {
    while(currentHeading < finalAngle - headingAcc + 360 && currentHeading > finalAngle + headingAcc) {
      currentHeading = getHeading(); 
      Serial.print(currentHeading);
      Serial.print(" degrees");
      delay(30);
      md.setM1Speed(turningSpeed * turnDirection1); 
      md.setM2Speed(turningSpeed * turnDirection2);
    } 
  }
  md.setM1Speed(turningSpeed * turnDirection1 * -1); 
  md.setM2Speed(turningSpeed * turnDirection2 * -1);
  delay(100 + 100*random(1,5)/4);
  md.setM1Speed(0); 
  md.setM2Speed(0); 
  Serial.println("Done"); 
  delay(200); 
  calibrated = false; 
}

void drive(){ //used over calibration
  float heading = getHeading(); 
  if(abs(heading - TargetTurnDegree()) < headingAcc){
    md.setM1Speed(m1DriveSpeed); 
    md.setM2Speed(m2DriveSpeed); 
  }
}

