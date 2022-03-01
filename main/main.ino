#include "18v15G2Functions.h"
#include "General.h"
#include "ForceSensor.h"
#include "GUIAndDisplay.h"
#include "PIDStuff.h"
#include "ControllerHelpers.h"

//States of robot
bool settingUpRobot = true;  //going through the startup sequence
bool changingWeight = false; // in the process of setting and changing weight
bool mainPage = false;       // main page that displays weight, nothing is moving or anything

void setup() {
  Serial.begin(9600);

  //initialize Pin Modes
  initializeButtonPins();
  initializeLEDPins();

  //check if display is connected to the board
  lookForDisplay();//this should be commented out if there are problems with the motor

  // Setup Encoder for Dial
  setupEncoder();

  // Exit Safe Start
  void exitSafeStart(uint8_t smcDeviceNumber);
  Wire.begin();
  exitSafeStart(smcDeviceNumberL1); // Disable safe start and allow the motors to move
  exitSafeStart(smcDeviceNumberR2);  //TODO: find out exactly what this does

  //setup PID for force
  setupPID();

  //setup gui and start display
  StartGuiDisplay();//sets up setting so we can write to the screen
  displayStartUpPage();
  //Display Buttons
   displayContinue();
  
}//end setup()

void loop() {
  timeCurr = millis();//current time in milli seconds

  //read buttons every half second //TODO: optimize this time maybe even do debounce here. 
  if ( (timeCurr - buttonTime) > buttonTimer ) {
    //reset time til next buttonClick
    buttonTime = timeCurr;
    
    readButtons();
  }//end button check

  //if robot is still being set up 
  if( settingUpRobot ) {
    setupWarningSequence();

    /if device is no longer being set up, go to main page
    if ( !settingUpRobot ) { mainPage = true;}
  }
  
  //display current force in appropriate location //TODO: maybe put this first or in an if timer
  updateCurrentForce();

  if ( mainPage ){
    if(topButtonSelected){  setupPageCase = setupPageCase + 1; }//move forward
    if(botButtonSelected){   setupPageCase--; }//move backwards
  }

  //reset the device
  // if bottom button is pressed

  //pausing the device
  if ( !settingUpRobot && !buttonMidState ){ //if not in setup mode, and middle button is selected
    //TODO: this
  }
  
  // put your main code here, to run repeatedly:
  //Serial.println(oldpin);
  myPID.SetMode(MANUAL);//TODO: check were this goes
  setSpeedBoth(travelSpeed);//TODO: check where this goes. 
  
}
