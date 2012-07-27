//Various LED codes (pin 13 LED). Described on tab one. 

void blink(int count){
  for(int jj = 0; jj < count; jj++){
    digitalWrite(errorLed, HIGH); 
    delay(200); 
    digitalWrite(errorLed, LOW); 
    delay(800); 
  }
}

void arrived(){
  Serial.println("Arrived.");
  blink(15);  
}

void coordSettingFailed(){
  Serial.println("Could not set coordinates"); 
  blink(8); 
}

void motorFault(){
  Serial.println("Motor driver fault"); 
  digitalWrite(errorLed, HIGH); 
  delay(200); 
  digitalWrite(errorLed, LOW); 
  delay(200); 
}

void uncalibratedMotors(){
  Serial.println("Motors not calibrated"); 
  blink(3); 
  delay(800); 
}

void startingDrive(){
  Serial.println("Starting drive"); 
  blink(10); 
  delay(800); 
}


