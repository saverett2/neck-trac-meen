#include "Functions.h"
#include "HX711.h"

int forceDialPin = 0;
int setForce = 0;

void setup() {
  // set pins
  pinMode(forceDialPin, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  //read dial
  int desiredForce = analogRead(forceDialPin); 
  //check change in force input
  if (desiredForce != setForce){
    Functions.setForceCommand(desiredForce);
  }
   

}
