/******************************************************************
  This is an example for the Adafruit RA8875 Driver board for TFT displays
  ---------------> http://www.adafruit.com/products/1590
  The RA8875 is a TFT driver for up to 800x480 dotclock'd displays
  It is tested to work with displays in the Adafruit shop. Other displays
  may need timing adjustments and are not guanteed to work.

  Adafruit invests time and resources providing this open
  source code, please support Adafruit and open-source hardware
  by purchasing products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, check license.txt for more information.
  All text above must be included in any redistribution.
 ******************************************************************/


#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"

// Library only supports hardware SPI at this time
// Connect SCLK to UNO Digital #13 (Hardware SPI clock)
// Connect MISO to UNO Digital #12 (Hardware SPI MISO)
// Connect MOSI to UNO Digital #11 (Hardware SPI MOSI)
#define RA8875_INT 3
//#define RA8875_CLK 52
#define RA8875_CS 10
#define RA8875_RESET 9

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;

////////// Three Button Intialize
// constants won't change. They're used here to set pin numbers:
const int buttonPinTop = // Pin of Top Push button (Start/Stop) Function
  const int buttonPinMid = // Pin of middle Push button (Continue) Through Prompts
    const int buttonPinBottom = // Pin of bottom Push button (Reset) Back to starting position
      const int ledPin =  13;      //Should be changed to the dial pin

// variables will change:
int buttonTopState = 0;  // variable for reading the pushbutton status
int buttonMidState = 0;
int buttonBottomState = 0;

// Intial values
int CaseValMain = 1;
int CaseValStart = 1;
long









void setup()
{
  Serial.begin(9600);
  Serial.println("RA8875 start");

  /* Initialize the display using 'RA8875_480x80', 'RA8875_480x128', 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
  }
  ///////// Button Initialization
  // initialize the pushbutton pin as an input:
  pinMode(buttonPinTop, INPUT);
  pinMode(buttonPinMid, INPUT);
  pinMode(buttonPinBottom, INPUT);

  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  tft.fillScreen(RA8875_BLACK);

  /* Switch to text mode */
  tft.textMode();
  tft.cursorBlink(32);


  /* Set a solid for + bg color ... */

  /* ... or a fore color plus a transparent background */


  /* Set the cursor location (in pixels) */
  // this is where the text will be displayed

  tft.textSetCursor(10, 10);

  /* Render some text! */
  char string[15] = "Hello, World! ";
  tft.textTransparent(RA8875_WHITE);
  tft.textWrite(string);
  tft.textColor(RA8875_WHITE, RA8875_RED);
  tft.textWrite(string);
  tft.textTransparent(RA8875_CYAN);
  tft.textWrite(string);
  tft.textTransparent(RA8875_GREEN);
  tft.textWrite(string);
  tft.textColor(RA8875_YELLOW, RA8875_CYAN);
  tft.textWrite(string);
  tft.textColor(RA8875_BLACK, RA8875_MAGENTA);
  tft.textWrite(string);

  /* Change the cursor location and color ... */
  tft.textSetCursor(100, 100);
  tft.textTransparent(RA8875_RED);
  /* If necessary, enlarge the font */
  tft.textEnlarge(1);
  /* ... and render some more text! */
  tft.textWrite(string);
  tft.textSetCursor(100, 150);
  tft.textEnlarge(2);
  tft.textWrite(string);
}




void loop()
{
  //// Start up Sequence

  // Show Welcome to Neck Traction
switch 
case 1:
  runStartWarnings();
  if StartWarnings();
  break;


  ///// Read Button State for changes
  buttonTopState = digitalRead(buttonPinTop);
  buttonMidState = digitalRead(buttonPinMid);
  buttonBottomState = digitalRead(buttonPinBottom);

  if (buttonTopState == HIGH) {
    // Some action is preformed
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}

void runStartWarnings() {

  // Fill screen with black
  tft.fillScreen(RA8875_BLACK);
  char Welcome = String("Welcome to Neck Traction!"); // Welcome String
  tft.textTransparent(RA8875_WHITE);
  //******* Millis time delay?
  // check if load cell is connected
  float initialForce = scale.read_average(10), 1);
  // Check if initial force read out remains zero
  // This would mean that the load cell is not connected properly
  switch (CaseValStart) {

  case 1:

    if (initialForce > -0.01 && initialForce < 0.01) { // If function is outputing 0 values consistently the load cell is not connected properly
        char iniForceRead = String(" Warning: Force sensor not connected properly");
        tft.textTransparent(RA8875_WHITE); // GUI white text
        tft.textWrite(iniForceRead); // Print on GUI 
    if(initialForce > 0.01){
      
    }
        
        
    }
    char 
       

      case 2:

      }



  }
