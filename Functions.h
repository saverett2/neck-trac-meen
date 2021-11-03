#include "HX711.h"

void runStartWarnings(){
  //black screen

  //put up logo or name of device

  //take initial measurements of encocoder's, force, and 
    //if force sensor not plugged in, throw warning until fixed
    //if emergency stop is on, put up warning

  //Add skip button in top corner

  //starts "checks"
  //button in bottom corner to procede

  //is robot connected to bed?
  char message[] = "Is the Robot attached to the bed?";
  tft.textColor(RA8875_WHITE, RA8875_BLACK);
  tft.textWrite(string);
  //is robot plugged into the wall?

  //is pin locked for the robot so it's not moving side to side?

  //is head claw correctly installed on patient's head?

  //is head claw connected to the load sensor?

  //

  //Checks done

  //Show battery percentage
  
}//end runStartWarnings

bool emergencyStopEngaged(){
  //Check if emergency stop is engaged
  //emergency stop should automatically turn off the motors
  //Display Warning saying it's pressed

  //set something up to safely come out of it. 
}//end emergencyStopEngaged

void printValue(float values){
  //convert float to string
  String valueString = "";
  valueString.concat(values);

  //convert string to char array
  char message[10];
  valueString.toCharArray(message, 10);

  //print message to display
  tft.textWrite( message );
}// end printValue function



//start/stop program, starts running linear acuators


void setForceCommands(int desiredForce){
  printForceChangeMessage(setForce, desiredForce);

  //wait for cancel or continue command and updatet setForce
  buttonStateTop = digitalRead(topButtonPin);
  buttonStateBottom = digitalRead(bottomButtonPin);
  while(buttonStateTop != HIGH &&  buttonStateBottom != HIGH){
    //update desiredForce if it changes
    if(desiredForce != analogRead(forceDialPin) ){
      desiredForce = analogRead(forceDialPin);
      printForceChangeMessage(setForce, desiredForce);
    }
    buttonStateTop = digitalRead(topButtonPin);
    buttonStateBottom = digitalRead(bottomButtonPin);
  }

  //if selected cancelled
  if (buttonStateTop == HIGH){
    desiredForce = setForce
    return;
  } else {//if selected continue
    //run force change
    setForce = desiredForce;
  }
}


void printForceChangeMessage(int oldValue, int newValue){
  tft.textColor(RA8875_WHITE, RA8875_BLACK);
  //print warning function
  char messageString[] = "Do you want to change the force from ";
  tft.textWrite(messageString);
  printValue(oldValue);
  char messageString[] = " lbs, to ";
  tft.textWrite(messageString);
  printValue(newValue);
  char messageString[] = " lbs?";
  tft.textWrite(messageString);

  //print top button message
  //print bottom button message
}//end printForceChangeMessage function
