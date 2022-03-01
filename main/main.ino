#include "18v15G2Functions.h"
#include "encoderShort.h"
#include "General.h"
#include "ForceSensor.h"
#include "GUIAndDisplay.h"
#include "PIDStuff.h"
#include "ControllerHelpers.h"

void setup() {
  Serial.begin(9600);

  //initialize Pin Modes
  initializeButtonPins();
  initializeLEDPins();

  //setup reset pin //you can click the pin on the mega's shield
  digitalWrite(resetPin, HIGH);
  pinMode(resetPin, OUTPUT);

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
  
}//end setup()

void loop() {
  timeCurr = millis();//current time in milli seconds

  //read buttons every half second //TODO: optimize this time maybe even do debounce here. 
  if ( (timeCurr - buttonTime) > buttonTimer ) {
    //reset time til next buttonClick
    buttonTime = timeCurr;
    
    readButtons();
  }else{
    setButtonsOff();
  }

  //if robot is still being set up 
  if( settingUpRobot ) {
    setupWarningSequence();

    //if device is no longer being set up, go to main page
    if ( !settingUpRobot ) { mainPage = true;}
  }
  
  //display current force in appropriate location //TODO: maybe put this first or in an if timer
    //if time is ready and loadcell has been tared;
  if ( (timeCurr - forceTime) > forceTimer && (!settingUpRobot || setupPageCase > 2) ){
    //reset time til next force reading
    forceTime = timeCurr;

    //print current force to the gui
    updateCurrentForce();
  }

  if ( mainPage ){
    //
    if(topButtonSelected() ){  changingForce = true; }//move forward

    //reset
    if(botButtonSelected() ){   setupPageCase--; }//move backwards
  }

  if ( changingForce ) {
    setForceCommand();
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
