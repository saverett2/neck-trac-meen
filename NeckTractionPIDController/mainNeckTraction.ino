
#include <Encoder.h>

const int forceDialPin = 4;
const int topButtonPin = 5;
const int midButtonPin = 6;
const int bottomButtonPin = 7;
Encoder forceDial(18, 19);



boolean buttonTopState = 0;  // variable for reading the pushbutton status
boolean buttonMidState = 0;
boolean buttonBottomState = 0;

void setup() {
  // set pins
    Serial.begin(9600);
  Serial.println("RA8875 start");

  /* Initialize the display using 'RA8875_480x80', 'RA8875_480x128', 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_800x480) ) {
    Serial.println("RA8875 Not Found!");
    while (1);
  }

  tft.textEnlarge(1);

  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  tft.fillScreen(RA8875_BLACK);

  tft.textMode();
  
  delay(1000);
  tft.fillScreen(RA8875_BLACK);
  tft.textEnlarge(2);
  //set up force display
  tft.textSetCursor(10, 100+480*2/3);
  tft.textWrite("Desired Force: ");
}

void loop() {
  // put your main code here, to run repeatedly:

  //update desired force
  tft.textSetCursor(10, 100+480*2/3);
  tft.textWrite("Desired Force: ");
  tft.textSetCursor(360, 100+480*2/3);
  printValue(setForce);
  delay(10);

    //read dial
  int desiredForce = forceDial.read(); 
  //check change in force input
  if (desiredForce != setForce){
     setForceCommand(desiredForce);
  }
}

void setForceCommand(int desiredForce){
  printForceChangeMessage(setForce, desiredForce);

  //wait for cancel or continue command and update setForce
  buttonTopState = digitalRead(topButtonPin);
  buttonBottomState = digitalRead(bottomButtonPin);
  
  int forceDialInput = forceDial.read();
  while(buttonTopState != HIGH &&  buttonBottomState != HIGH){
    //update desiredForce if it changes
    forceDialInput = forceDial.read(); // Update to InterruptRotator
    if(desiredForce != forceDialInput ){
      desiredForce = forceDialInput;
      printForceChangeMessage(setForce, desiredForce);
    }
    buttonTopState = digitalRead(topButtonPin);
    buttonBottomState = digitalRead(bottomButtonPin);
  }
  //if selected cancelled
  if (buttonTopState == HIGH){
    desiredForce = setForce;
    return;
  } 
  //if selected continue or set
  //run force change
  setForce = desiredForce;
  t_ms = micros();
  t_old = t_ms/1000000.0;
  CaseValMain = 2;
}// end setForceCommands method


void printForceChangeMessage(int oldValue, int newValue){
  //set text size
  tft.textEnlarge(2);
  
  tft.textSetCursor(10,200);
  tft.textColor(RA8875_WHITE, RA8875_BLACK);
  //print warning 
  tft.textWrite("Do you want to change the force from ");
  printValue(oldValue);
  tft.textWrite(" lbs, to ");
  printValue(newValue);
  tft.textWrite(" lbs?");

  //print top button message
  tft.textColor(RA8875_WHITE, RA8875_BLACK);
  tft.textSetCursor(640,50);//tft.textSetCursor(640,50+480*0/3);
  tft.textWrite("Cancel");
  
  //print bottom button message
  tft.textSetCursor(670,50+480*2/3);
  tft.textWrite("Set");
  tft.textSetCursor(640,100+480*2/3);
  tft.textWrite("Weight");
}//end printForceChangeMessage function

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

    //tft.fillScreen(RA8875_BLACK);
//    tft.textSetCursor(600,50+480*0/3);
//    tft.textEnlarge(1.5);
//    tft.textColor(RA8875_WHITE, RA8875_BLACK);
//    tft.textWrite("Top button 1.5");
//    tft.textSetCursor(600,100+480*0/3);
//    tft.textEnlarge(2);
//    tft.textColor(RA8875_WHITE, RA8875_BLACK);
//    tft.textWrite("Top button 2.0");
//
//    tft.textSetCursor(600,50+480*1/3);
//    tft.textEnlarge(1);
//    tft.textColor(RA8875_WHITE, RA8875_BLACK);
//    tft.textWrite("Middle button");
//
//    tft.textSetCursor(600,50+480*2/3);
//    tft.textEnlarge(2);
//    tft.textColor(RA8875_WHITE, RA8875_BLACK);
//    tft.textWrite("Bottom button");
