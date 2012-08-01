//Used for setting target coordinates - takes two significant digits and divides. 

void signChange(){
  if(GPS.lat == 'N') { 
    LatSignChange = 1; 
  }
  else {
    LatSignChange = -1; 
  }
  if(GPS.lon == 'E') {
    LonSignChange = 1; 
  }
  else {
    LonSignChange = -1; 
  }
  currentLocLat *= LatSignChange; 
  currentLocLong *= LonSignChange; 
}

void countDigits() {
  if (abs(num1) <= 100) {
    //fakeTargetLat = num1;
    sentTargetLat = num1; 
  }
  else if (abs(num1) <= 1000 && abs(num1) > 100) {
    //fakeTargetLat = num1/10;
    sentTargetLat = num1/10; 
  }
  else if (abs(num1) <= 10000 && abs(num1) > 1000) {
    //fakeTargetLat = num1/100;
    sentTargetLat = num1/100; 
  }
  else if (abs(num1) <= 100000 && abs(num1) > 10000) {
    //fakeTargetLat = num1/1000;
    sentTargetLat = num1/1000; 
  }
  else if (abs(num1) <= 1000000 && abs(num1) > 100000) {
    //fakeTargetLat = num1/10000;
    sentTargetLat = num1/10000; 
  }
  else if (abs(num1) <= 10000000 && abs(num1) > 1000000) {
    //fakeTargetLat = num1/100000;
    sentTargetLat = num1/10000; 
  }
  else if (abs(num1) <= 100000000 && abs(num1) > 10000000) {
    //fakeTargetLat = num1/1000000;
    sentTargetLat = num1/100000; 
  }
  else {
    //fakeTargetLat = num1/1000000;
    sentTargetLat = num1/1000000; 
  }

  if (abs(num2) <= 100) {
    //fakeTargetLon = num2;
    sentTargetLon = num2; 
  }
  else if (abs(num2) <= 1000 && abs(num2) > 100) {
    //fakeTargetLon = num2/10;
    sentTargetLon = num2/10; 
  }
  else if (abs(num2) <= 10000 && abs(num2) > 1000) {
    //fakeTargetLon = num2/100;
    sentTargetLon = num2/100; 
  }
  else if (abs(num2) <= 100000 && abs(num2) > 10000) {
    //fakeTargetLon = num2/1000;
    sentTargetLon = num2/1000; 
  }
  else if (abs(num2) <= 1000000 && abs(num2) > 100000) {
    //fakeTargetLon = num2/10000;
    sentTargetLon = num2/10000; 
  }
  else if (abs(num2) <= 10000000 && abs(num2) > 1000000) {
    //fakeTargetLon = num2/100000;
    sentTargetLon = num2/100000; 
  }
  else if (abs(num2) <= 100000000 && abs(num2) > 10000000) {
    //fakeTargetLon = num2/1000000;
    sentTargetLon = num2/1000000; 
  }
  else{
    //fakeTargetLon = num2/1000000;
    sentTargetLon = num2/1000000; 
  }

  //softwareserial interfered with some of the values 
  while(abs(sentTargetLat) > 100){ //this is primarily the purpose of the program, but this is just a failsafe 
    //fakeTargetLat /= 10; 
    sentTargetLat /= 10; 
  }
  while(abs(sentTargetLon) > 100) {
    //fakeTargetLon /= 10; 
    sentTargetLon /= 10; 
  }
}










