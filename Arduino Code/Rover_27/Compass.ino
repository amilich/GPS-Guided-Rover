float getHeading() {
  Wire.beginTransmission(HMC6352SlaveAddress);
  Wire.write(HMC6352ReadAddress);   
  Wire.endTransmission();
  delay(6);
  Wire.requestFrom(HMC6352SlaveAddress, 2); 
  byte MSB = Wire.read();
  byte LSB = Wire.read();

  float headingSum = (MSB << 8) + LSB;
  float headingInt = headingSum / 10;   
  headingInt -= magFlux; //magnetic north vs. true north
  if(headingInt < 0){
    headingInt += 360; 
  }  
  return headingInt; 
}



