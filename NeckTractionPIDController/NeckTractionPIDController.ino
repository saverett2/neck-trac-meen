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

// Include Display Library
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"

//*** Define Pins ***//

// Motor Pins
const int potPinL1 = 7; // Analog pin 13 for the left motor, device 1
const int potPinR2 = 6; // Analog pin 14 for the right motor, device 2

// Load Cell Pins
const int loadcellDOUT = 11; // Load Cell
const int loadcellSCK = 12; // Load Cell
// Dial Pins
//
//
//

// Button Pins
const int topButtonPin = 5;
const int midButtonPin = 6;
const int bottomButtonPin = 7;

//Define State for Buttons
boolean buttonTopState = 0;  // variable for reading the pushbutton status
boolean buttonMidState = 0;
boolean buttonBottomState = 0;


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
// Intial Value needed to set force to make motors run
int setForce = 1;
int CaseValMain = 1;
//time variables
unsigned long t_ms = 0;
double t = 0;                   //current time
double t_old = 0;               //previous encoder time
int fillBlack = 1;
double initialForce = 0;

HX711 loadcell;
float calibrationFactor = -23550.f; // Found experimentally with Merryweather's load cell

// Set up display
#define RA8875_INT 3
#define RA8875_CS 10
#define RA8875_RESET 9//any pin or nothing!
Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;


// Creat PID object
PID myPID(&currentForce, &travelSpeedPID, &desiredForce, Kp, Ki, Kd, DIRECT);

void setup() {
  Serial.begin(9600);
//while(1==1)
//{
//  setMotorSpeed(smcDeviceNumberL1, 50); 
//  setMotorSpeed(smcDeviceNumberR2, 50);   
//}

  //Initialize Pin Modes Buttons
  pinMode(topButtonPin, INPUT_PULLUP);
  pinMode(midButtonPin, INPUT_PULLUP);
  pinMode(bottomButtonPin, INPUT_PULLUP);

  // Look for Display
  // This should be commented out if there are problems with the motor
  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
    ////////////////////////////////
  }
  // Setup Encoder for Dial
  setupEncoder();


  // Exit Safe Start
  Serial.println("Start Exit Safe Start");
  void exitSafeStart(uint8_t smcDeviceNumber);
  Serial.println("End Exit Safe Start");
  Wire.begin();
  exitSafeStart(smcDeviceNumberL1); // Disable safe start and allow the motors to move
  exitSafeStart(smcDeviceNumberR2);
  //delay(100);

  // Start the GUI display screen
  StartGuiDisplay();
  Serial.println("StartGUI");

  Serial.println("Start Warnings");
  runStartWarnings(); // Goes through a safety check, found in GUI_Code

  // Set up Display to print Desired and Current Force

  delay(1000);
  tft.fillScreen(RA8875_BLACK);
  tft.textEnlarge(2);
  //set up force display
  tft.textSetCursor(10, 100 + 480 * 2 / 3);
  tft.textWrite("Desired Force: ");

  myPID.SetMode(MANUAL); // Turn on PID controller
  myPID.SetOutputLimits(-25, 25);
  myPID.SetSampleTime(100); // If the loop is faster than the sample time it
  //Serial.print("time,");
  //Serial.print("travel Speed,");
  //Serial.print("current force,");
  //Serial.print("desired force,");
  //Serial.print("m1 pos,");
  //Serial.println("m2 pos");

  // myPID.SetMode(AUTOMATIC); //TEMPORARY TO COLLECT DATA

}

void loop() {
  t_ms = micros();
  t = t_ms / 1000000.0;
  //Code to Test Motors for PID Controller
  //m1_pos = (float(analogRead(potPinM1)) - 34) * 150 / 842.0;
  //m2_pos = (float(analogRead(potPinM2)) - 32) * 150 / 840;
  //desiredForce = analogRead(potPinContr) / 1024.0 * 200.0; //TEMPORARY TO COLLECT DATA


  // Inputs to Serial Monitor: <Drive Both,50,50>  // This input in serial monitor drives motors forward
  //                           <Drive Both,-50,-50> // This input in serial monitor retracts motors back

  // Code needed to read inputs from serial monitor
  //RecvWithStartEndMarkers(); // Read data from serial monitor

  // Check if load cell is ready to give measurement
  if (loadcell.is_ready()) {
    byte times = 5;
    currentForce = loadcell.get_units();
    Serial.println(loadcell.get_units());
    Serial.println(currentForce);
  }



  switch (CaseValMain)
  {
    case 1:
      // Change to Desired Force
      Serial.println("We are in Main Case 1");
      tft.textSetCursor(10, 100 + 480 * 2 / 3);
      tft.textWrite("Desired Force: ");
      tft.textSetCursor(360, 100 + 480 * 2 / 3);
      printValue(setForce);//
      
      //Read dial
      //int desiredForce = forceDial.read(); // Replace with InterruptRotator Code for smoother dial readings
      //check change in force input
      if (desiredForce < setForce) {
        setForceCommand();
      }
      if (desiredForce >= setForce) {
        CaseValMain = 2;
      }
      break;

    case 2: // Additional Cases can be added to add progressive checks to stop motors and ask for prompt to continue
    
    // The Main Course
    
    while(currentForce < desiredForce){
// Pause/Unpause -  Stop Motor
// Start Motor
// Display Force Continue


      
    }
      // Apply Set Force and Start Motors
      Serial.println("We are in Main Case 2");
      // Show Current and Desired Force on Display

/*    Could be replaced with 
 *     
      // Add the new load cell read out
      tft.textSetCursor(10, 100 + 480 * 2 / 3);
      tft.textWrite("Current Force: ");
      tft.textSetCursor(360, 100 + 480 * 2 / 3);
      printValue(currentForce);
      tft.textSetCursor(10, 100 + 480 * 1 / 3);
      tft.textWrite("Desired Force: ");
      tft.textSetCursor(360, 100 + 480 * 1 / 3);
      printValue(setForce);
*/


      Serial.print("Before Desired Force ");
      Serial.println(desiredForce);
      Serial.print("Before Current Force");
      Serial.println(currentForce);
      myPID.SetMode(AUTOMATIC);
      Serial.print("After Desired Force ");
      Serial.println(desiredForce);
      Serial.print("After Current Force");
      Serial.println(currentForce);
      
      if (currentForce >= desiredForce) {
        // Could change this out for the PauseContinue function
        Serial.println("STOP MOTOR");
        myPID.SetMode(MANUAL);
        stopBoth();
        CaseValMain = 3; // Send to the reset case.
        break;
      }
      break;

    case 3:
      Serial.println("We are in Main Case 3");
      if (fillBlack == 1) {
        tft.fillScreen(RA8875_BLACK);
        fillBlack = 0;
      }
      tft.textSetCursor(10, 100 + 480 * 2 / 3);
      tft.textWrite("Reached Desired Force. Reset when ready ");
      tft.textSetCursor(600, 50 + 480 * 0 / 3);
      tft.textTransparent(RA8875_WHITE);
      tft.textWrite("Reset");

      buttonTopState = digitalRead(topButtonPin);
      while (buttonTopState == 1)
      {
        //Wait until the top button is pushed
        buttonTopState = digitalRead(topButtonPin);
        //Serial.println(buttonTopState);
        Serial.println("Waiting for Top Button MainCase3");
      }
      while (buttonTopState != 1) {
        buttonTopState = digitalRead(topButtonPin);
        CaseValMain = 4;
        t_old = t;
        Serial.println("Waiting for Button push MainCase3");
      }
      break;

    case 4:
      // reset linear actuators
      travelSpeed = -50;
      setSpeedBoth(travelSpeed);
      break;
  }

  // Only used for PID Calibration
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
    Serial.println(commandString);

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

  //Replace with GUI code functions

  //lcd.setCursor(0,0);
  //lcd.print("Set F:  ");
  //lcd.print(desiredForce);
  //lcd.setCursor(0,1);
  //lcd.print("Meas F: ");
  //lcd.print(currentForce);

  // Send Measured Force from Load Cell to GUI
  //tft.textSetCursor(10, 10); // tft.setCursor(X_axis, Y_axis);
  //tft.textColor(RA8875_BLACK,RA8875_GREEN); // This sets the color the text
  //tft.textEnlarge(2); // This enlarges the text size
  //tft.setFontScale(3); Not sure if this function works
  //Serial.print("Measured force: ");
  //Serial.print(loadcell.get_units(), 1);


  //delay(100); // Operate at roughly 10 Hz
}
