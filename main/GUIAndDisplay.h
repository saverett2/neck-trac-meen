// Include Display Library
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"

// Set up display
#define RA8875_INT 3
#define RA8875_CS 10
#define RA8875_RESET 9//any pin or nothing!
Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);


//*********************************Functions*********************************//


//checks if display is correctly being read by the arduino
void lookForDisplay(void){
  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
  }
}

void StartGuiDisplay()
{
  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  
  tft.fillScreen(RA8875_BLACK); // set the background the black
  tft.textEnlarge(7);
  tft.textMode();
  delay(10);
}

//Displays the starting page before the warning
void displayStartUpPage(void) {
  // Fill screen with white
  tft.fillScreen(RA8875_WHITE);

  //Write "University of Utah" in Red
  tft.textColor(RA8875_RED, RA8875_WHITE);
  tft.textSetCursor(120, 400 * 1 / 2);
  tft.textWrite("University of Utah");

  //Write "Neck Traction" in black
  tft.textEnlarge(2);
  tft.textColor(RA8875_BLACK, RA8875_WHITE);
  tft.textSetCursor(240, 560 * 1 / 2);
  tft.textWrite("Neck Tracktion");

  //add custom white Continue button
  // Show "Continue" text next to top button to switch to previous case
  tft.textColor(RA8875_BLACK, RA8875_WHITE); // BLACK text with WHITE background
  tft.textSetCursor(600, 50 + 480 * 0 / 3); // TODO: THIS NEEDS TO CHANGE TO BE NEXT TO THE TOP BUTTON****
  tft.textWrite("Continue"); // Print on GUI
  
  //turn on Top LED
  digitalWrite(topLED, HIGH);
}//end displayStartUp..

//converts a float to a string then prints the string
// at the current Text cursor location
void printValue(float values) {
  //convert float to string
  String valueString = "";
  valueString.concat(values);

  //convert string to char array
  char message[10];
  valueString.toCharArray(message, 10);

  //print message to display
  tft.textWrite( message );
}// end printValue function

//display's the current force onto the screen
void displayForce(float currentForce){
   //TODO: check if this output to the correct location
  tft.textColor(RA8875_WHITE, RA8875_BLACK);
  tft.textSetCursor(10, 100 + 480 * 2 / 3); // Needs to be moved to the top left corner//TODO:
  tft.textWrite("Current Force: ");
  tft.textSetCursor(360, 100 + 480 * 2 / 3); // Needs to be aligned with the Text of Current Force//TODO:
  
  printValue(currentForce);
}

//Displays "continue" next to the top button and turns the bottom top LED on
void displayContinue() {
  tft.textEnlarge(2);
  // Show "Continue" text next to middle button to switch to previous case
  tft.textEnlarge(2);//size
  tft.textColor(RA8875_WHITE, RA8875_BLACK); // white text with black background
  tft.textSetCursor(600, 50 + 480 * 0 / 3); // TODO: THIS NEEDS TO CHANGE TO BE NEXT TO THE TOP BUTTON****
  tft.textWrite("Continue"); // Print on GUI
  
  //turn on Top LED
  digitalWrite(topLED, HIGH);
}

//Displays "back" next to the bottom button and turns the bottom button LED on
void displayBack(void){
  // Show "Back" text next to middle button to switch to previous case
  tft.textEnlarge(2);//size
  tft.textColor(RA8875_WHITE, RA8875_BLACK); // white text with black background
  tft.textSetCursor(600, 50 + 480 * 2 / 3); // TODO: THIS NEEDS TO CHANGE TO BE NEXT TO THE MIDDLE/BOTTOM BUTTON****
  tft.textWrite("Back"); // Print on GUI
  
  //turn on Bottom LED
  digitalWrite(botLED, HIGH);
}

//displays a success message to the screen that the LoadCell is connected
void displayLoadCellSuccess(void) {
  tft.fillScreen(RA8875_BLACK);
  tft.textEnlarge(2);
  tft.textSetCursor(10, 480 * 1 / 2);
  tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
  tft.textWrite("Force Sensor is connected"); // Print on GUI
}

//Displays an error message that the loadcell isn't connected
void displayLoadCellErrorMessage(void)  {
  tft.textSetCursor(10, 480 * 2 / 3);
  tft.textColor(RA8875_WHITE, RA8875_BLACK); // White Text, in black box
  tft.textWrite("Warning: Force sensor not connected properly"); // Print on GUI
}

//dipslays an error message that the op amp has timed out
void displayOpAmpTimeOutError(void){
  tft.textEnlarge(6);
  tft.textSetCursor(200, 200);
  tft.textColor(RA8875_RED, RA8875_BLACK); // White Text, in black box
  tft.textWrite("error code: 101");

  tft.textEnlarge(3);
  tft.textSetCursor(120, 300);
  tft.textColor(RA8875_RED, RA8875_BLACK); // White Text, in black box
  tft.textWrite("OP AMP Timed out...");
}

//display op amp waiting to connect message
void displayOpAmpWaiting(void){
  tft.textEnlarge(4);
  tft.textSetCursor(100, 150);
  tft.textColor(RA8875_WHITE, RA8875_BLACK); // White Text, in black box
  tft.textWrite("Waiting for OP AMP to Connect..."); // Print on GUIGUI
}

//display a message asking if the device is secured
void displayDeviceSecuredCheck(void){
  tft.fillScreen(RA8875_BLACK);
  tft.textEnlarge(2);
  tft.textSetCursor(10, 480 * 1 / 2);
  tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
  tft.textWrite("Is the device secured to the bed and plugged in?"); // Print on GUI
}

//display a message asking if the joint clamps are secured
void displayJointClampsSecuredCheck(void){
  tft.fillScreen(RA8875_BLACK);
  tft.textEnlarge(2);
  tft.textSetCursor(10, 480 * 1 / 2);
  tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
  tft.textWrite("Are all joint clamps secured?"); // Print on GUI
}

//display a message asking if the tongs are secured
void displayTongsSecuredCheck(void){
  tft.fillScreen(RA8875_BLACK);
  tft.textEnlarge(2);
  tft.textSetCursor(10, 480 * 1 / 2);
  tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
  tft.textWrite("Is the tongs attached securely to the Force Sensor?"); // Print on GUI
}

//display a message stating device is ready
void displayDeviceReadyMessage(void){
  tft.fillScreen(RA8875_BLACK);
  tft.textEnlarge(2);
  tft.textSetCursor(10, 480 * 1 / 2);
  tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
  tft.textWrite("Device is ready: Press Continue to Begin"); // Print on GUI
}
