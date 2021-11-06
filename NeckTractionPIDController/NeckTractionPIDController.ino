// This script executes a PID control for desired force on two linear actuators
// using the reading from a force sensor.
//
// Trevor Schwehr
// University of Utah
// Neck Traction Prototype

// Include I2C library
#include <Wire.h>

// Include force sensor library
#include <HX711.h>

// Include PID library
#include <PID_v2.h>

// Include NewLiquidCrystal Library for I2C
// Need to include new LCD screen
//#include <LiquidCrystal_I2C.h>



// Wil change
// Define LCD pinout
//const int  en = 2, rw = 1, rs = 0, d4 = 4, d5 = 5, d6 = 6, d7 = 7, bl = 3;

// Define LCD I2C Address - change if reqiuired
//const int i2c_addr = 0x27;

//LiquidCrystal_I2C lcd(i2c_addr, en, rw, rs, d4, d5, d6, d7, bl, POSITIVE);

// Define Pins
const int potPinL1 = 7; // Analog pin 13 for the left motor, device 1
const int potPinR2 = 6; // Analog pin 14 for the right motor, device 2

const int loadcellDOUT = 14; // Load Cell
const int loadcellSCK = 15; // Load Cell

const int potPinContr = A7; // Potentiometer Dial Pin
const int potPinM1 = A0; // Potentiometer Dial Pin
const int potPinM2 = A1; // Potentiometer Dial Pin

// Define device numbers
bool newOrders = false;
bool firstTime = true;
char message[32];
char command[5];
String commandString;
uint16_t smcDeviceNumber;
uint16_t smcDeviceNumberL1 = 1;
uint16_t smcDeviceNumberR2 = 2;
double travelSpeed;
double travelSpeedPID;
int travelSpeedPercentage;
double leftPosition;
double rightPosition;

// Define global variables
unsigned long lastTime = 0; // Starting time for PID controller (ms)
double Kp = 4; // Proportional Gain (TBD)
double Kd = 0; // Derivative Gain (TBD)
double Ki = 0.01; // Integral Gain (TBD)
double sampleFrequency = 10; //Frequency for reading force sensor and updating PID (Hz);
double sampleTime = (1 / sampleFrequency) * 1000; //Time between samples (ms)
double inputForce = 0;
double desiredForce = 0;
double currentForce = 0; // Current reading from force sensor (lb)
double IError = 0; // Integral error (lb*ms)
double oldError = 0; // Old error for PID (lb)
double pidOUT;
double m1_pos = 0;
double m2_pos = 0;

HX711 loadcell;
float calibrationFactor = -23550.f; // Found experimentally with nice load cell

// setting up display
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#define RA8875_INT 3
#define RA8875_CS 10
#define RA8875_RESET 9//any pin or nothing!
Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;







// Creat PID object
PID myPID(&currentForce, &travelSpeedPID, &desiredForce, Kp, Ki, Kd, DIRECT);

void setup() {
  Serial.begin(9600);

// Look for Display 
  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
  }

  
  Wire.begin(); 
  exitSafeStart(smcDeviceNumberL1); // Disable safe start and allow the motors to move
  exitSafeStart(smcDeviceNumberR2);
  //delay(100);
  // Start the GUI on display screen
  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  tft.fillScreen(RA8875_BLACK); // Sets the color of the background.


  //Serial.print("Taring force sensor...");
  loadcell.begin(loadcellDOUT, loadcellSCK);
  loadcell.set_scale(calibrationFactor);
  loadcell.tare(); // Number of times to average, blocking call
  //Serial.println("done");

  myPID.SetMode(MANUAL); // Turn on PID controller
  myPID.SetOutputLimits(-25, 25);
  myPID.SetSampleTime(100);
  Serial.print("time,");
  Serial.print("travel Speed,");
  Serial.print("current force,");
  Serial.print("desired force,");
  Serial.print("m1 pos,");
  Serial.println("m2 pos");
  myPID.SetMode(AUTOMATIC); //TEMPORARY TO COLLECT DATA
}

void loop() {
  m1_pos = (float(analogRead(potPinM1)) - 34) * 150 / 842.0;
  m2_pos = (float(analogRead(potPinM2)) - 32) * 150 / 840;
  desiredForce = analogRead(potPinContr) / 1024.0 * 200.0; //TEMPORARY TO COLLECT DATA
  RecvWithStartEndMarkers(); // Read data from serial monitor

  if (loadcell.is_ready()) { // check to see if load cell is ready to give a measurement
    byte times = 5;
    currentForce = loadcell.get_units();
    Serial.println(loadcell.get_units());
    Serial.println(currentForce);
  }
  if (myPID.GetMode() == AUTOMATIC) {
    myPID.Compute(); // Compute with each loop
    travelSpeedPID = constrain(travelSpeedPID, -100, 100); // constrain travel speed to be within reasonable value
    travelSpeedPercentage = (int) travelSpeedPID; // Cast to int to send to motors
    setSpeedBoth(travelSpeedPercentage); // set speed of the motors
  }
  Serial.print(millis() / 1000.0);
  Serial.print(",");
  Serial.print(travelSpeedPID);
  Serial.print(",");
  Serial.print(currentForce);
  Serial.print(",");
  Serial.print(desiredForce);
  Serial.print(",");
  Serial.print(m1_pos);
  Serial.print(",");
  Serial.println(m2_pos);

  // If there are new orders for the robot, execute them
  if (newOrders) {
    ParseData(); // Parse orders

    commandString = String(command);
    //    Serial.println(commandString);

    if (commandString.equals("Desired Force")) {
      desiredForce = inputForce;
      Serial.print("Desired force: ");
      Serial.println(desiredForce);
    }
    else if (commandString.equals("Stop")) {
      stopBoth();
    }
    else if (commandString.equals("Left Pos")) {
      leftPosition = getPosition(potPinL1);
      Serial.println(leftPosition);
    }
    else if (commandString.equals("Right Pos")) {
      rightPosition = getPosition(potPinR2);
      Serial.println(rightPosition);
    }
    else if (commandString.equals("Force Meas")) {
      Serial.println(currentForce);
    }
    else if (commandString.equals("PID Mode")) {
      Serial.println("Starting PID");
      myPID.SetMode(AUTOMATIC); // Turn on PID controller
    }
    else if (commandString.equals("PID Off")) {
      Serial.println("PID stopping");
      myPID.SetMode(MANUAL); // Turn on PID controller
      travelSpeedPID = 0;
      travelSpeed = 0;
      setSpeedBoth(0);
    }
    else if (commandString.equals("Drive Both")) {
      Serial.println("Driving both");
      Serial.print("Speed Percentage: "); Serial.println(travelSpeed);
      setSpeedBoth(travelSpeed);
    }
    else if (commandString.equals("Stop Both")) {
      Serial.println("Stopping both");
      stopBoth();
    }
    else if (commandString.equals("Tare")) {
      Serial.println("Taring force sensor...");
      loadcell.tare(); // Number of times to average, blocking call
    }

    newOrders = false; // Once orders have been executed, set boolean to false
  }
  // For I2C LCD Screen, Not used, replace with RX
  
  //lcd.setCursor(0,0);
  //lcd.print("Set F:  ");
  //lcd.print(desiredForce);
  //lcd.setCursor(0,1);
  //lcd.print("Meas F: ");
  //lcd.print(currentForce);

  // Send Measured Force from Load Cell to GUI
  tft.textSetCursor(10, 10); // tft.setCursor(X_axis, Y_axis); 
  tft.textColor(RA8875_BLACK,RA8875_GREEN); // This sets the color the text
  tft.textEnlarge(1); // This enlarges the text size
  //tft.setFontScale(3); Not sure if this function works
  Serial.print("Measured force: ");
  Serial.print(loadcell.get_units(), 1);

// Show desired Force force from Potentiometer/Dial on GUI
  
// Seths CODE
  

  //delay(100); // Operate at roughly 10 Hz
}


/*
 * Motor Control Functions
 */

 
// This function disables safe start and is required for the motors to move. 
// Must be called when the controller restarts and after any error. 
void exitSafeStart(uint8_t smcDeviceNumber)
{
  Wire.beginTransmission(smcDeviceNumber);
  Wire.write(0x83);  // Exit safe start
  Wire.endTransmission();
}
 

// This function sets the speed of the motor. 
void setMotorSpeed(uint8_t smcDeviceNumber, int16_t percentSpeed)
{
  uint8_t cmd = 0x85;  // Motor forward
  if (percentSpeed < 0)
  {
    cmd = 0x86;  // Motor reverse
    percentSpeed = -percentSpeed;
  }
  Wire.beginTransmission(smcDeviceNumber);
  Wire.write(cmd);
  Wire.write(0);
  Wire.write(percentSpeed);
  Wire.endTransmission();
}
 

// This function reads how long it has been since the last time the motor was reset or powered up. 
uint16_t readUpTime(uint8_t smcDeviceNumber)
{
  Wire.beginTransmission(smcDeviceNumber);
  Wire.write(0xA1);  // Command: Get variable
  Wire.write(28);    // Variable ID: Up time (low)
  Wire.endTransmission();
  Wire.requestFrom(smcDeviceNumber, (uint8_t)2);
  uint16_t upTime = Wire.read();
  upTime |= Wire.read() << 8;
  return upTime;
}

// This function reads the value of the potentiometer from the selected arduino pin and converts it to position using calibrated values
double getPosition(uint16_t motorPotPin){
  double val = analogRead(motorPotPin); 
  double postn = 0.0069*val -0.1978; // Convert count to position, values determined by experiment using a reference voltage of 4.79 V
  return postn; 
}

// This function sets the speed of both motors. 
// Note device numbers must be named in main scrip.
void setSpeedBoth(uint16_t spd){
  setMotorSpeed(smcDeviceNumberL1, spd); 
  setMotorSpeed(smcDeviceNumberR2, spd); 
}

// This function sets the maximum speed between 0 and 3200 that the motor can go forward or backward. 
// Note: This speed cannot be above the absolute maximum motor speed programmed through the motor driver through USB.
// Possible returns: 
// 0 - No problem setting the limit
// 1 - Unable to set forward limit because of hard motor limit settings
// 2 - Unable to set reverse limit because of hard motor limit settings
// 3 - Unable to set forward and reverse limit because of hard motor limit settings
int setMaxSpeed(uint8_t smcDeviceNumber, uint16_t maxSpeed){
  // convert max speed to two bytes
  byte firstByte = maxSpeed & 0x1F; 
  byte secondByte = maxSpeed >> 5 & 0x7F; 
  
  Wire.beginTransmission(smcDeviceNumber);
  Wire.write(0xA2); // Command Byte
  Wire.write(firstByte); 
  Wire.write(secondByte);
  int check = Wire.endTransmission(); 
  return check; 
}

// This function checks the current error of the motor. 
// See data sheet for error output meanings
int checkError(uint8_t smcDeviceNumber){
  Wire.beginTransmission(smcDeviceNumber); 
  Wire.write(0xA1); // Command byte
  Wire.write(0); // ID for error 
  Wire.endTransmission(); 
  Wire.requestFrom(smcDeviceNumber, (uint8_t)2);
  uint16_t error = Wire.read(); 
  error |= Wire.read() << 8; 
  return error; 
}

// This function stops the motor and will make controller susceptible to a safe-start violation if it is enabled 
void stopMotor(uint8_t smcDeviceNumber){
  Wire.beginTransmission(smcDeviceNumber); 
  Wire.write(0xE0); // Stop motor
  Wire.endTransmission(); 
}

// This function will stop both motors and make the controllers susceptible to safe-start violations if enabled.
void stopBoth(){
  stopMotor(smcDeviceNumberL1); 
  stopMotor(smcDeviceNumberR2); 
}


/*
 * Functions to Read Serial Monitor
 */
 
void RecvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
    
    while (Serial.available() > 0 && newOrders == false) {
        rc = Serial.read();
        //Serial.println(rc); 
        if (recvInProgress == true) {
            if (rc != endMarker) {
                message[ndx] = rc;
                ndx++;
            }
            else {
                message[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newOrders = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}


void ParseData() {      // split the data into its parts

    char* strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(message,",");      // get the first part - the string
    strcpy(command, strtokIndx); // copy it to command
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    inputForce = (int) atof(strtokIndx);     // convert this part to uint8_t

    strtokIndx = strtok(NULL, ",");
    travelSpeed = (int) atof(strtokIndx);     // convert this part to a signed int
}
