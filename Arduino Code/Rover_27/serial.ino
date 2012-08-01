void serialEvent(){
  data1 = ""; 
  data2 = ""; 
  inputData = 0; 
  while (Serial.available() > 0) {
    char c = Serial.read();
    incomingData += c; //Serial data read in characters  - add to a string
  }
  char dataArray[incomingData.length()+1]; 
  incomingData.toCharArray(dataArray, sizeof(dataArray)); 
  for(int ii = 0; ii < incomingData.length()+1; ii++){ //now make data into something useful
    if((dataArray[ii] == '\n') || (dataArray[ii] == ',')){ //time for a new word/coordinate
      inputData ++; 
    } 
    else if (dataArray[ii] != '(' && dataArray[ii] != ')'){
      if(inputData == 0) {
        data1 += dataArray[ii];
      }
      else if(inputData == 1) {   
        data2 += dataArray[ii];    
      }
    }
  }
  setCoord = true; //coordinates set, time to move on
  calcCount = 0; //counter to ensure angle/location calculation occurs five times (to negate errors) 
}

void interpretSerial(){
  if(setCoord){
    char dataArray1[data1.length() + 1]; //to turn data1, data2 into coordinates
    char dataArray2[data2.length() + 1]; 
    data1.toCharArray(dataArray1, sizeof(dataArray1));
    data2.toCharArray(dataArray2, sizeof(dataArray2));
    num1 = atol(dataArray1); //character array to integer - NOT final coordinates
    num2 = atol(dataArray2);
    countDigits(); //interprets incoming data, divides appropriately (in division)
    if(writeLatLong){
      Serial.print(sentTargetLat, 7);
      Serial.print(","); 
      Serial.println(sentTargetLon, 7); 
      delay(500); 
    }
    EEPROM_writeAnything(baseAddr, sentTargetLat);
    EEPROM_writeAnything(baseAddr+4, sentTargetLon);
  }
  if(setCoord){
    calcCount ++; //calculate angle five times - first is usually incorrect (data is still being processed)
    if(debug){ //can be turned on or off - not critical information (but can be source of errors)
      Serial.print("Angle: ");
      Serial.println(TargetTurnDegree()); 
      Serial.print("Current lat: "); 
      Serial.print(currentLocLat); 
      Serial.print("     "); 
      Serial.print("Current long: "); 
      Serial.println(currentLocLong); 
    }

    if(calcCount == checkCalc){ //turn off all preliminary target calculations, no more writing to serial
      writeLatLong = false; 
      setCoord = false; 
      startDrive = true; 
      //startingDrive(); //LED code (Error_Codes)
    }
  } 
}





