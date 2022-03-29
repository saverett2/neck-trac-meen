
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
//#include "Fonts/FreeMonoBoldOblique12pt7b.h"
//#include <Fonts/FreeSans9pt7b.h>

//*** Define Pins ***//

// Motor Pins
const int potPinL1 = A7; // Analog pin 13 for the left motor, device 1
const int potPinR2 = A6; // Analog pin 14 for the right motor, device 2


// Load Cell Pins
const int loadcellDOUT = 11; // Load Cell
const int loadcellSCK = 12; // Load Cell

// Dial Pins
// Intitalized in the encoderShort tab
// PIN_IN1 is 19
// PIN_IN2 is 18

// Button Pins
const int topButtonPin = 5;
const int midButtonPin = 6;
const int bottomButtonPin = 7;

//Define State for Buttons
boolean buttonTopState = 0;  // variable for reading the pushbutton status
boolean buttonMidState = 0;
boolean buttonBottomState = 0;

// Button LED Pins
const int TopLED = 31;
const int MidLED = 32;
const int BotLED = 33;


// Define device numbers for motor
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
float potPinL1_reading[] = {0, 0, 0};
float potPinR2_reading[] = {0, 0, 0};
float AvgPotRead = 0;
float DistPosition;
// Define variable for PID
unsigned long lastTime = 0; // Starting time for PID controller (ms)


double Kp = 3.5; // Proportional Gain (TBD)
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

// Difference Desired and Current Force
double DiffDesired; // Ramp up ramp down functions

// Button Debounce Variables
int lastButtonStateTop = 1;
int lastButtonStateBot = 1;
int lastButtonStateMid = 1;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers
int readingTop = 1;
int readingMid = 1;
int readingBot = 1;

// Variables to display page once
int previousDisplayed = 1;
int previousCas = 0;
int CaseValStart  = constrain(CaseValStart, 1, 6); // Constrain the varibles to case 1 to 6, change if additional warnings are added.
int writeback = 1;


// Initial the Set Force and Main Case Variables
int setForce = 1;
int CaseValMain = 1;

//time variables
unsigned long t_ms = 0;
double t = 0;//current time
double t_old = 0;//previous time
int fillBlack3 = 1;
double initialForce = 0;

float resetTime; // The time in which reset system button is pushed
int motorResetDelay = 30000; // 30 seconds to pull the motors back and then reset
int Resetpin = 35; // The Pin to reset the ardunio

// Load Cell and calibration factor
HX711 loadcell;
float calibrationFactor = 23550.f; // Found experimentally with Merryweather's load cell

// Initialize and set up display
#define RA8875_INT 3
#define RA8875_CS 10
#define RA8875_RESET 9//any pin or nothing!
Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;


// Creat PID object
PID myPID(&currentForce, &travelSpeedPID, &desiredForce, Kp, Ki, Kd, DIRECT);



// ************************* void setup *************************

void setup() {
  Serial.begin(9600);
  //TEST CODE
  //while(1==1)
  //{
  //  setMotorSpeed(smcDeviceNumberL1, 50);
  //  setMotorSpeed(smcDeviceNumberR2, 50);
  //}
  //Initialize Pin Modes Buttons
  pinMode(topButtonPin, INPUT_PULLUP);
  pinMode(midButtonPin, INPUT_PULLUP);
  pinMode(bottomButtonPin, INPUT_PULLUP);
  digitalWrite(Resetpin, HIGH);
  pinMode(Resetpin, OUTPUT);

  // Intialize LED Pin Mode
  pinMode(TopLED, OUTPUT);
  pinMode(MidLED, OUTPUT);
  pinMode(BotLED, OUTPUT);

  // Look for Display
  // This should be commented out if there are problems with the motor
  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
    // Turn off system, check display/screen connections and display driver board.
    // Can write script to light button LEDs in order to signify that the screen is not connected properly.
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
  //Needed to run the motors

  // Start the GUI display screen
  StartGuiDisplay();
  //Serial.println("StartGUI");

  //Serial.println("Start Warnings");
  runStartWarnings(); // Goes through a safety check, found in GUI_Code


  //// THIS
  // Set up Display to print Desired and Current Force
  delay(1000);
  tft.fillScreen(RA8875_BLACK);
  tft.textEnlarge(2);

  //set up force display
  tft.textSetCursor(10, 100 + 480 * 2 / 3);
  tft.textWrite("Desired Force: ");

  myPID.SetMode(MANUAL); // Turn on PID controller
  myPID.SetOutputLimits(-100, 100);
  myPID.SetSampleTime(100); // If the loop is faster than the sample time it
  //Serial.print("time,");
  //Serial.print("travel Speed,");
  //Serial.print("current force,");
  //Serial.print("desired force,");
  //Serial.print("m1 pos,");
  //Serial.println("m2 pos");

  // myPID.SetMode(AUTOMATIC); //TEMPORARY TO COLLECT DATA

}



// ************************* void loop *************************



void loop() {
  t_ms = micros();
  t = t_ms / 1000000.0;
  potPinL1_reading[1] = analogRead(potPinL1);
  potPinR2_reading[1] = analogRead(potPinR2);
  PrintSerialMonitor();
  //Code to Test Motors for PID Controller
  //m1_pos = (float(analogRead(potPinM1)) - 34) * 150 / 842.0;
  //m2_pos = (float(analogRead(potPinM2)) - 32) * 150 / 840;
  //desiredForce = analogRead(potPinContr) / 1024.0 * 200.0; //TEMPORARY TO COLLECT DATA

  // Inputs to Serial Monitor: <Drive Both,50,50>  // This input in serial monitor drives motors forward
  //                           <Drive Both,-50,-50> // This input in serial monitor retracts motors back
  // Code needed to read inputs from serial monitor
  //RecvWithStartEndMarkers(); // Read data from serial monitor

  // Check if load cell is ready to give measurement and display measurement

  UpdateCurrentForce();



  switch (CaseValMain) {

    case 1:

      setForceCommand();
      break;

    case 2: // Additional Cases can be added to add progressive checks to stop motors and ask for prompts to continue

      // The Main Course


      //////// READING BUTTON CODE START
      // Read Button Input
      readingTop = digitalRead(topButtonPin);
      readingMid = digitalRead(midButtonPin);
      readingBot = digitalRead(bottomButtonPin);

      // Three buttons are lit up
      digitalWrite(TopLED, HIGH);
      digitalWrite(MidLED, HIGH);
      digitalWrite(BotLED, HIGH);

      // If the switch changed, due to noise or pressing:
      if ((readingMid != lastButtonStateMid) || (readingBot !=  lastButtonStateBot)) {
        // reset the debouncing timer
        lastDebounceTime = millis();
      }

      // millis time delay
      if ((millis() - lastDebounceTime) > debounceDelay) {
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // if the button state has changed:
        if ((readingTop != buttonTopState) || (readingBot != buttonBottomState) || (readingMid != buttonMidState)) {
          buttonTopState = readingTop;
          buttonBottomState = readingBot;
          buttonMidState = readingMid;


          // If mid button is pushed then pause, send back to case 1 to change desired force
          if ((buttonMidState == 0) && (buttonBottomState == 1)) {
            //Serial.println("STOP MOTOR");
            myPID.SetMode(MANUAL);
            stopBoth();
            CaseValMain = 1;
            break;
          }
          // If bottom button is pushed, reset the system, send to case 3
          if ((buttonBottomState == 0) && (buttonMidState == 1)) {
            //Serial.println("STOP MOTOR");
            //Serial.println("Bottom Button is Pushed");
            myPID.SetMode(MANUAL);
            stopBoth();
            CaseValMain = 3;
            break;
            //ledState = !ledState;
          }
        }
      }
      ///////// READ BUTTON CODE END

      // Apply Set Force and Start Motors
      //Serial.println("We are in Main Case 2");
      // Show Current and Desired Force on Display
      //Serial.print("Before Desired Force ");
      //Serial.println(desiredForce);
      //Serial.print("Before Current Force");
      //Serial.println(currentForce);

      //myPID.SetMode(AUTOMATIC);
      //Serial.print("After Desired Force ");
      //Serial.println(desiredForce);
      //Serial.print("After Current Force");
      //Serial.println(currentForce);

      // Ramp up,Ramp Down

      // Calculate the Diff
//      DiffDesired = desiredForce - currentForce;
//      if ((abs(DiffDesired) <= 0.01 * desiredForce)) {
//        // Could change this out for the PauseContinue function
//        //Serial.println("STOP MOTOR");
//        myPID.SetMode(MANUAL);
//        stopBoth();
//        CaseValMain = 1; // Send back to change force and wait until new force is set
//        // or set new case to tell doctors that force is reached.
//        break;
//      }
//      if (DiffDesired < 0) {
//        travelSpeed = -1;
//      }
//      else {
//        travelSpeed = 1;
//      }
//      if (abs(DiffDesired) >= 0.5 * desiredForce ) {
//        travelSpeed = 25 * travelSpeed;
//      }
//      if ((abs(DiffDesired) < 0.5 * desiredForce) && (abs(DiffDesired) >= 0.1 * desiredForce)) {
//        travelSpeed = 20 * travelSpeed;
//      }
//      if (abs(DiffDesired) < 0.1 * desiredForce ) {
//        travelSpeed = 15 * travelSpeed;
//      }

      //      if ((potPinL1_reading <= 200)) { // tenative needs to be test********
      //        // Could change this out for the PauseContinue function
      //        Serial.println("STOP MOTOR");
      //        myPID.SetMode(MANUAL);
      //        stopBoth();
      //        CaseValMain = 1; // Send back to change force and wait until new force is set
      //        // or set new case to tell doctors that force is reached.
      //        break;
      //      }




      myPID.SetMode(AUTOMATIC);
      //setSpeedBoth(travelSpeed);

      lastButtonStateTop = readingTop;
      lastButtonStateMid = readingMid;
      lastButtonStateBot = readingBot;
      break;


    case 3:
      Serial.println("We are in Main Case 3");

      if (previousDisplayed != 2) {
        // Turn on Active Buttons
        digitalWrite(TopLED, HIGH);
        digitalWrite(MidLED, HIGH);
        digitalWrite(BotLED, HIGH);

        tft.fillScreen(RA8875_BLACK);
        tft.textEnlarge(2);
        tft.textSetCursor(10, 50);
        tft.textColor(RA8875_WHITE, RA8875_BLACK);
        tft.textWrite("Confirm the reset");

        //tft.textSetCursor(10, 100);
        //tft.textColor(RA8875_WHITE, RA8875_BLACK);
        //tft.textWrite("Press Reset Again");

        tft.textSetCursor(625, 480 * 0 / 3); // Change position to match Top Button
        tft.textTransparent(RA8875_WHITE);
        tft.textEnlarge(2);
        tft.textWrite("Reset");
        tft.textSetCursor(625, 50 + 480 * 0 / 3);
        tft.textTransparent(RA8875_WHITE);
        tft.textEnlarge(1);
        tft.textWrite("Actuators");

        tft.textSetCursor(600, 50 + 480 * 1 / 3); // Change Position to match Middle Button
        tft.textTransparent(RA8875_WHITE);
        tft.textEnlarge(2);
        tft.textWrite("Cancel");

        tft.textSetCursor(625, 480 * 2 / 3); // Change position to match Bottom Button
        tft.textTransparent(RA8875_WHITE);
        tft.textWrite("Reset");
        tft.textEnlarge(1);
        tft.textSetCursor(625, 50 + 480 * 2 / 3);
        tft.textWrite("System");
        tft.textEnlarge(2);

        previousDisplayed = 2;
      }
      // Read Either Reset Button for confirmation or Cancel to go back to case 1 to set force again.
      // Read Button Input
      readingTop = digitalRead(topButtonPin);
      readingMid = digitalRead(midButtonPin);
      readingBot = digitalRead(bottomButtonPin);

      //Serial.println("printing button state");
      //Serial.println(buttonTopState);
      //Serial.println(buttonMidState);
      //Serial.println(buttonBottomState);
      //Serial.println(readingTop);
      //Serial.println(readingMid);
      //Serial.println(readingBot);

      // If the switch changed, due to noise or pressing:
      if ((readingMid != lastButtonStateMid) || (readingBot !=  lastButtonStateBot) || (readingTop != lastButtonStateTop)) {
        // reset the debouncing timer
        lastDebounceTime = millis();
        //Serial.println("reset last debounce time");
      }
      //Serial.println("After first if");
      // millis time delay
      if ((millis() - lastDebounceTime) > debounceDelay) {
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:
        //Serial.println("After second if");
        // if the button state has changed:
        if ((readingTop != buttonTopState) || (readingBot != buttonBottomState) || (readingMid != buttonMidState)) {
          //Serial.println("After button state change");
          buttonTopState = readingTop;
          buttonBottomState = readingBot;
          buttonMidState = readingMid;

          if ((buttonTopState == 0) && (buttonBottomState == 1)) {
            CaseValMain = 5;
            resetTime = millis();
            break;
          }

          // If mid button is pushed then pause, send back to case 1 to change desired force
          if ((buttonMidState == 0) && (buttonBottomState == 1)) {
            CaseValMain = 1;
            break;
            //
          }
          // If bottom button is pushed, reset the system, send to case 3
          if ((buttonBottomState == 0) && ( buttonMidState == 1)) {
            //Serial.println("We are going to main case 4");
            CaseValMain = 4;
            resetTime = millis();
            break;
            //ledState = !ledState;
          }
        }
      }

      lastButtonStateTop = readingTop;
      lastButtonStateMid = readingMid;
      lastButtonStateBot = readingBot;
      break;

    case 4: //Reset the entire system (pull actuators to 0, resent board)
      //Serial.println("We are in Main Case 4");
      // reset linear actuators
      //Serial.println("Pull Back the Motors");
      potPinL1_reading[1] = analogRead(potPinL1);
      potPinR2_reading[1] = analogRead(potPinR2);
      if (previousDisplayed != 3) {
        tft.fillScreen(RA8875_BLACK);
        tft.textEnlarge(2);
        tft.textSetCursor(10, 100);
        tft.textColor(RA8875_WHITE, RA8875_BLACK);
        tft.textWrite("Please wait, resetting system...");
        previousDisplayed = 3;
      }
      //if ((potPinL1_reading >= 400) || (potPinR2_reading >= 400)) {
      myPID.SetMode(MANUAL);
      travelSpeed = -100;
      setSpeedBoth(travelSpeed);
      //}
      //      else if ((potPinL1_reading < 400) || (potPinR2_reading < 400)) {
      //        myPID.SetMode(MANUAL);
      //        stopBoth();
      //      }
      if ((millis() - resetTime) >= motorResetDelay) { // Reset the Entire System
        //Serial.println("I am going to reset");
        digitalWrite(Resetpin, LOW);
      }
      break;
    // Need to code a way to reset the system after actuators have reached their starting points



    case 5: // This case is to *RESET ACTUATORS ONLY*
      //Serial.println("I am in main case 5");
      if (previousDisplayed != 3) {
        tft.fillScreen(RA8875_BLACK);
        tft.textEnlarge(2);
        tft.textSetCursor(10, 100);
        tft.textColor(RA8875_WHITE, RA8875_BLACK);
        tft.textWrite("Please wait, resetting actuators...");
        previousDisplayed = 3;
      }

      //potPinL1_reading = analogRead(potPinL1);
      //potPinR2_reading = analogRead(potPinR2);
      //if ((potPinL1_reading >= 400) || (potPinR2_reading >= 400)) { // Make this a function to call here
      myPID.SetMode(MANUAL);
      travelSpeed = -100;
      setSpeedBoth(travelSpeed);
      //}
      //else if ((potPinL1_reading < 400) || (potPinR2_reading < 400)) {
      //myPID.SetMode(MANUAL);
      //stopBoth();
      //}

      if ((millis() - resetTime) <= motorResetDelay) {
        //Serial.println("The Actuators are going home");
      }
      else {
        CaseValMain = 1;
      }
      break;
  }




  if (myPID.GetMode() == AUTOMATIC) {
    myPID.Compute(); // Compute with each loop
    travelSpeedPID = constrain(travelSpeedPID, -100, 50); // constrain travel speed to be within reasonable value
    travelSpeedPercentage = (int) travelSpeedPID; // Cast to int to send to motors
    setSpeedBoth(travelSpeedPercentage); // set speed of the motors
    //Serial.println();
    //Serial.println(travelSpeedPercentage);
    //Serial.println();
  }
  // Only used for PID Calibration
  /*
    Serial.print(millis() / 1000.0);
    Serial.print(", ");
    Serial.print(travelSpeedPID);
    Serial.print(", ");
    Serial.print(currentForce);
    Serial.print(", ");
    Serial.print(desiredForce);
    Serial.print(", ");
    Serial.print(m1_pos);
    Serial.print(", ");
    Serial.println(m2_pos);

  */
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
