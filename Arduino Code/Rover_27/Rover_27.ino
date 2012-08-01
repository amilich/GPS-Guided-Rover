//This program is covered under the Attribution-NonCommercial-ShareAlike 3.0 Unported license. 
//See liscence on http://bobulisk.github.com/GPS-Guided-Rover/

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
//8: turn formula corrected. 
//9: eeprom work begun, extensive notes added. Revised turning calibration.
//10: new loop structure. 
//11: completely rewritten driving code.
//12: rewritten motor code. 
//13: rewritten GPS code. 
//14: removed delays in program. 
//15: added compass support. 
//16: added EEPROM support. 
//17: fixed compass turning code. 
//18: new turning/calibration. 
//19: added compass calibration. 
//20: finalized calibration. 
//21: corrected latitude/longitude functions. 
//22: completely new turning function. 
//23: finalized compass directed turning. 
//24: new turning reverse delay. 
//25: turning formula tweaked. 
//26: added support for directional LEDs. 

/* Principal Features: 
 * LED directional indicator
 * EEPROM support
 * DMS -> decimal
 * Digital compass (calibrated)
 * Feedback loop design
 */

//LED Codes: 
//2 fast blinks: coordinate setting false (probably no GPS fix) 
//3 fast blinks: motors not calibrated
//10 blinks: starting drive 
//constant blinking: motor fault

#include "DualMC33926MotorShield.h"
#include <StringToAnything.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h> //not used but necessary to use Adafruit_GPS
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
int LEDS[10] = {
  26, 27, 28, 29, 30, 31, 22, 23, 24, 25}; 
boolean debug = false; 
boolean compCalibrated = false; 

boolean debugGPS = false; 

boolean readEEPROM = true; 
boolean setCoord = false; 

boolean writeLatLong = true; 
boolean goDriveToLoc = false; 
boolean calibrated = false; 
boolean startDrive = false; 
boolean goToAngle = false; 
boolean beginCalibration = true; 
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
int m1DriveSpeed = 125; 
int m2DriveSpeed = 125; 
int turningSpeed = 60; 
int LatSignChange; 
int LonSignChange; 

int headingCount = 0; 
int address = 0;
unsigned int baseAddr = 0;

int compassAddress = 0x42 >> 1;
int enterCalibration = 0x43;
int exitCalibration = 0x45;

int HMC6352SlaveAddress = 0x42; //compass I2C address
int HMC6352ReadAddress = 0x41;
int headingValue;
int magFlux = 13; 
int headingAvg = 4; 
int calibTime = 5000; 
int headingAcc = 10; 

void setup(){
  Serial.begin(115200);
  Wire.begin();
  md.init(); 
  HMC6352SlaveAddress = HMC6352SlaveAddress >> 1;
  pinMode(errorLed, OUTPUT); 
  Serial.println("Input coordinates.");   
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //Does not need all data
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); //5HZ is the max speed for RMCGGA  
  useInterrupt(true);

  for(int ii = 22; ii <= 52; ii++){
    pinMode(ii, OUTPUT);   
  } 
  for(int ii = 0; ii < 2; ii++){
    for(int ii = 0; ii < 10; ii++){
      digitalWrite(LEDS[ii], HIGH); 
      delay(250); 
      digitalWrite(LEDS[ii], LOW); 
    }
  }

  for(int ii = 0; ii < 2; ii++){
    for(int ii = 0; ii < 10; ii ++){
      digitalWrite(LEDS[ii], HIGH);  
    }
    delay(400); 
    for(int ii = 0; ii < 10; ii ++){
      digitalWrite(LEDS[ii], LOW);  
    }
    delay(400); 
  }

  digitalWrite(50, HIGH);
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
  if(beginCalibration) {
    float compTimer = millis(); 
    Serial.println("Calibrating"); 
    if(!compCalibrated) {
      Serial.println("Calibrating compass"); 
      compCalibrated = true;  
      Wire.beginTransmission(compassAddress);
      Wire.write('C'); //start calibration 
      Wire.endTransmission();
      delayMicroseconds(10); 
    }
    while(millis() < compTimer + calibTime){
      md.setM1Speed(110); 
      md.setM2Speed(-110); 
      Serial.println("Calibrating"); 
      delay(30); 
    }
    Wire.beginTransmission(compassAddress);
    Wire.write('E'); 
    Wire.endTransmission();
    delay(14); 
    md.setM1Speed(0); 
    md.setM2Speed(0); 
    beginCalibration = false; 
  }

  stopIfFault(); //protects motor driver (gives motor controller time to reset/cooldown)
  if(millis() <  6000 + calibTime){
    interpretSerial(); //do not use EEPROM, use input coordinates
  }
  else if(readEEPROM && !setCoord){ //if nothing is input, read from eeprom
    Serial.println("Reading previous coordinates."); 
    double readLat; 
    double readLong; 
    EEPROM_readAnything(baseAddr, readLat); 
    EEPROM_readAnything(baseAddr + 4, readLong); //+4 to read next double
    sentTargetLat = readLat; 
    sentTargetLon = readLong; 
    Serial.println(); 
    Serial.print("Read value 1: "); 
    Serial.println(readLat, 5); 
    Serial.print("Read value 2: "); 
    Serial.println(readLong, 5); 
    setCoord = false; 
    startDrive = true; 
    readEEPROM = false; 
  }
  currentLocLat = calculateCurrentLocation(GPS.latitude); 
  currentLocLong = calculateCurrentLocation(GPS.longitude); 
  signChange(); 
  if(startDrive){
    while(coordDist(currentLocLat, currentLocLong, sentTargetLat, sentTargetLon, 1) > 2){ //drive until under two meters away
      setLEDS(TargetTurnDegree() - currentHeading); 
      stopIfFault(); 
      if (! usingInterrupt) {
        char c = GPS.read();
        if (GPSECHO)
          if (c) UDR0 = c;
      }
      if (GPS.newNMEAreceived()) {  
        if (!GPS.parse(GPS.lastNMEA()))  
          return;  
      }
      if (timer > millis())  timer = millis();
      if (millis() - timer > 2000) { 
        counter ++; 
        timer = millis(); 
        printData(); 
        currentHeading = getHeading(); 
        stopIfFault(); //if motors stall

        if(counter == 2 && GPS.fix == 1){
          getLatLong(); 
          Serial.print("Current lat: "); 
          Serial.println(currentLocLat, 5); 
          Serial.print("Current lon: "); 
          Serial.println(currentLocLong, 5); 
          Serial.print("Distance: "); 
          Serial.println(coordDist(currentLocLat, currentLocLong, sentTargetLat, sentTargetLon, 1)); 
          Serial.print("Targ heading: "); 
          Serial.println(TargetTurnDegree()); 
          currentHeading = getHeading(); 
          if(abs(currentHeading - TargetTurnDegree()) > headingAcc){ //now must turn
            turnToDegree(TargetTurnDegree()); 
            Serial.println("TURNING"); 
          }
          else{
            drive();  
          }
          counter = 0; 
        }
      }
      else{
        while(GPS.fix == 0){
          Serial.println("no connection"); 
          delay(6); 
        }  
      }
    }
    Serial.println("ARRIVED"); 
    delay(300); 
  }
}





























