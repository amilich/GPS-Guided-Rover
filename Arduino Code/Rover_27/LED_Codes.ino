//Various LED codes (pin 13 LED). Described on tab one. 

void setLEDS(float incomingAngle){
  float ledNum = incomingAngle/360; 
  ledNum *= 1000; 
  ledNum = map(ledNum, 0, 1000, 0, 10); 
  if(ledNum == 10){
    ledNum = 0;  
  }
  for(int ii = 0; ii < 10; ii++){
    if(ii != ledNum){
      digitalWrite(LEDS[ii], LOW);  
    }
    else{
      digitalWrite(LEDS[ii], HIGH); 
    }
  }
}

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


