//Probably rename this physical interface or something//TODO

//reset Pin
const int resetPin = 35;

// Button LED Pins
const int topLED = 31;
const int midLED = 32;
const int botLED = 33;

// Button Pins
const int topButtonPin = 5;
const int midButtonPin = 6;
const int bottomButtonPin = 7;

//Define State for Buttons
boolean buttonTopState = 0;  // variable for reading the pushbutton status
boolean buttonMidState = 0;   //TODO: I think these should start as high
boolean buttonBottomState = 0;

//set up serial communication
uint16_t tx, ty;//TODO: move this to wherever Wire.h goes

//All Timeing //TODO probably split up timing between types
unsigned int timeCurr;
unsigned int loadCellTimeOld;//TODO: maybe take out "old"
unsigned int buttonTime;

const int opAmpTimeout = 5000;//op amp times out in 5 seconds of inactivity
const int buttonTimer = 500;//.5 sec

//TODO: Name this section
byte setupPageCase = 0;//variable to track which startup sequence page we are on
byte prevSetupPageCase = 0;//variable to track which startup sequence page was last on


//*********************************Functions*********************************//
//*********************************Buttons
//Set the pinmodes for the buttons
void initializeButtonPins(void) {
  pinMode(topButtonPin, INPUT_PULLUP);
  pinMode(midButtonPin, INPUT_PULLUP);
  pinMode(bottomButtonPin, INPUT_PULLUP);
}

//reads the states of the buttons
void readButtons(void) {
  bool tops[20];
  bool mids[20];
  bool bots[20];

  for( int index = 0; index < 20; index++){
    tops[index] = digitalRead(topButtonPin);
    mids[index] = digitalRead(midButtonPin);
    bots[index] = digitalRead(bottomButtonPin);
  }

  int topSum = 0;
  int midSum = 0;
  int botSum = 0;


  for( int index = 0; index < 20; index++){
    topSum = topSum + tops[index];
    midSum += mids[index];
    botSum += bots[index];
  }

  buttonTopState = topSum/20; //TODO: verify this math works. truncation might make .9 = 0;?
  buttonMidState = midSum/20;
  buttonBottomState = botSum/20;

//  Serial.print("      top button: ");
//  Serial.println(buttonTopState);
//  Serial.print("mid button: ");
//  Serial.println(buttonMidState);
//  Serial.print("bot button: ");
//  Serial.println(buttonBottomState);
}

//returns if the top button was clicked
bool topButtonSelected(void){
  //if top button was clicked;
  if (buttonTopState == LOW){   return true;  }
  
  //if button not selected
  return false;
}

//returns if the top button was clicked
bool midButtonSelected(void){
  //if top button was clicked;
  if (buttonMidState == LOW){   return true;  }
  
  //if button not selected
  return false;
}

//returns if the bottom button was clicked
bool botButtonSelected(void){
  //if top button was clicked;
  if (buttonBottomState == LOW){   return true;  }
  
  //if button not selected
  return false;
}

//*********************************LEDs
//set the pinmodes for the LEDs on the buttons
void initializeLEDPins(void)  {
  pinMode(topLED, OUTPUT);
  pinMode(midLED, OUTPUT);
  pinMode(botLED, OUTPUT);
}
