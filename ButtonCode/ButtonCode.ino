//I, Seth Polevoi, wrote two different button read methods.
//Use whichever one is better, or, if you want to use both, change the 
//method names. 
//Code has not been tested. 
//Code also doesn't update the button state variables. not sure if we want to do that.

//global variables from the main code just to allow this code to compile.
// Button Pins
const int topButtonPin = 5;
const int midButtonPin = 6;
const int bottomButtonPin = 7;

//Define State for Buttons
boolean buttonTopState = 0;  // variable for reading the pushbutton status
boolean buttonMidState = 0;
boolean buttonBottomState = 0;

void setup(){
}
void loop(){
}


//This function waits for any of the 3 buttons to be clicked. 
//Returns the pin of the button that was clicked. 
int buttonCode(void){
  bool prevButtonTopState, prevButtonMidState, prevButtonBottomState = LOW;
  bool bottomClick, midClick, topClick = LOW;
  while (topClick || midClick || bottomClick){
    if ( prevButtonTopState != digitalRead(topButtonPin) ){
      return topButtonPin;
    }else if ( prevButtonMidState != digitalRead(midButtonPin) ){
      return midButtonPin;
    }else if ( prevButtonBottomState != digitalRead(bottomButtonPin) ){
      return bottomButtonPin;
    }
  }//endWhile
}//end buttonCode


//This function waits till the given button is clicked
//returns TRUE/HIGH when the button is clicked. 
bool buttonCode(int buttonPin){
  bool currButtonState;
  bool prevStateVal = digitalRead(buttonPin);
  while( prevStateVal != currButtonState ){
    currButtonState = digitalRead(buttonPin);
  }
  return currButtonState;
}
