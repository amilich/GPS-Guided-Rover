/*****************************************************************************/
/*////////////////////GPS Rover: BlueStampEngineering 2012///////////////////*/
/*///////////////////Created and written by Andrew M.////////////////////////*/
/********************************Copyright 2012*******************************/

//Change log: 
//1: updated to work with motors; functions added.
//2: no more microseconds in latitude/longitude conversion.
//3: finalized motor calibration, turning to angle.
//4: debugging option added (must be changed in code). 
//5: updated failsafes, corrected latitude/longitude conversion formula.
//6: error codes added.
//7: resolved turning and calibration bugs.
//8: turn formula corrected***.
//9: eeprom work begun, extensive notes added. Revised turning calibration.
//10: new loop structure. 
//11: completely rewritten driving code.
//12: rewritten motor code. 
//13: rewritten GPS code. 
//14: removed delays in program. 
//15: added compass support. 
//16: added EEPROM support. 

//LED Code: 
//2 fast blinks: coordinate setting false (probably no GPS fix) 
//3 fast blinks: motors not calibrated
//10 blinks: starting drive 
//constant blinking: motor fault

#include "DualMC33926MotorShield.h"
#include <StringToAnything.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <EEPROMAnything.h>
#include <Wire.h>
#include <EEPROM.h>

Adafruit_GPS GPS(&Serial1);
#define GPSECHO  true
boolean usingInterrupt = false;
void useInterrupt(boolean);
#define errorLed  13
#define Earth_Radius 6371000 

DualMC33926MotorShield md;

boolean debug = false; 
boolean debugGPS = false; 

boolean readEEPROM = true; 
boolean setCoord = false; 

boolean writeLatLong = true; 
boolean goDriveToLoc = false; 
boolean calibrated = false; 
boolean startDrive = false; 
boolean goToAngle = false; 
double sentTargetLat = 0; 
double sentTargetLon = 0; 
float turnTime = 1500; //90 degrees
float turningAngle; 
float headingTotal = 0; 
double currentLocLat; 
double currentLocLong; 
double currentHeading; 
double fakeTargetLat = 0; 
double fakeTargetLon = 0; 
double num1, num2; 
String incomingData, data1, data2; 
int inputData = 0; 
int restartTime = 15000; 
int calcCount = 0; 
int checkCalc = 4; //The number of times to check the calculations -1 (the first time is usually incorrect).
int calibrationSpeed = 200; //speed used when finding current angle
int m1DriveSpeed = 300; 
int m2DriveSpeed = 300; 
int turningSpeed = 100; 
int LatSignChange; 
int LonSignChange; 

int headingCount = 0; 
int address = 0;
unsigned int baseAddr = 0;

int HMC6352SlaveAddress = 0x42;
int HMC6352ReadAddress = 0x41;
int headingValue;
int magFlux = 13; 
int headingAvg = 4; 

void setup(){
  Serial.begin(115200);
  Wire.begin();
  md.init(); 
  HMC6352SlaveAddress = HMC6352SlaveAddress >> 1;
  pinMode(errorLed, OUTPUT); 
  Serial.println("Input coordinates.");   
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); //5HZ is the max speed for RMCGGA  
  useInterrupt(true);
  delay(1000);
}

SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  if (GPSECHO)
    if (c) UDR0 = c;  
}

void useInterrupt(boolean v) {
  if (v) {
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } 
  else {
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}


uint32_t timer = millis();
float counter = 0; 
void loop(){
  stopIfFault(); //protects motor driver (in theory, should stop program, but now just gives it time to reset)
  if(millis() < 6000){
    interpretSerial(); 
  }
  else if(readEEPROM && !setCoord){
    Serial.println("Reading previous coordinates."); 
    double readLat; 
    double readLong; 
    EEPROM_readAnything(baseAddr, readLat);
    EEPROM_readAnything(baseAddr + 4, readLong); 
    sentTargetLat = readLat; 
    sentTargetLon = readLong; 
    Serial.println(); 
    Serial.print("Read value 1: "); 
    Serial.println(readLat, 4); 
    Serial.print("Read value 2: "); 
    Serial.println(readLong, 4); 
    setCoord = false; 
    startDrive = true; 
    readEEPROM = false; 
  }
  if(startDrive){
    while(coordDist(currentLocLat, currentLocLong, sentTargetLat, sentTargetLon, 1) > 2){
      if (! usingInterrupt) {
        char c = GPS.read();
        if (GPSECHO)
          if (c) UDR0 = c;
      }
      if (GPS.newNMEAreceived()) {  
        if (!GPS.parse(GPS.lastNMEA()))  
          return;  
      }

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
      headingTotal += headingInt; 
      headingCount ++; 
      if(headingCount == headingAvg){
        currentHeading = headingTotal/headingCount;
        headingCount = 0; 
        headingTotal = 0; 
        //Serial.print("Averaged Heading: "); 
        //Serial.println(currentHeading); 
      } 

      if (timer > millis())  timer = millis();
      if (millis() - timer > 2000) { 
        counter ++; 
        timer = millis(); 
        printData(); 

        stopIfFault();
        //if(GPS.fix == 1){
        signChange(); 
        Serial.print("Current lat: "); 
        Serial.println(currentLocLat, 5); 
        Serial.print("Current lon: "); 
        Serial.println(currentLocLong, 5); 
        Serial.print("Distance: "); 
        Serial.println(coordDist(currentLocLat, currentLocLong, sentTargetLat, sentTargetLon, 1)); 
        if(counter == 2 && GPS.fix == 1){
          //if(true){
          if(calibrated){
            //currentHeading = GPS.angle; 
            //Serial.print("Heading: "); 
            //Serial.println(currentHeading);
            currentLocLat = calculateCurrentLocation(GPS.latitude); 
            currentLocLong = calculateCurrentLocation(GPS.longitude); 
            turnToDegree(TargetTurnDegree()); 
            Serial.println("TURNING"); 
          }
          else{
            calibrateCurrentAngle();  
          }
          Serial.print("Targ heading: "); 
          Serial.println(TargetTurnDegree()); 
          counter = 0; 
        }
      }
      else{
        while(GPS.fix == 0){
          //Serial.println("no connection"); 
          //delay(6); 
        }  
      }
    }
    Serial.println("ARRIVED"); 
    delay(300); 
  }
}






