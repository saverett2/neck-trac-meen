

void setForceCommand() {
  //Serial.println("In set Force Command");
  printForceChangeMessage(setForce, desiredForce);

  //wait for cancel or continue command and update setForce
  buttonTopState = digitalRead(topButtonPin);
  buttonBottomState = digitalRead(bottomButtonPin);

  int forceDialInput = readForceDial();// This is reading the dial, should be changed to the new encoderShort function

  while (buttonTopState == 1){ //&&  (buttonBottomState == 1)) {
    //update desiredForce if it changes
    //Serial.println("We are in the while loop Force Command");

    forceDialInput = readForceDial(); // Reading the force dial continually
    Serial.println(forceDialInput);
    if (desiredForce != forceDialInput ) {
      desiredForce = forceDialInput;
      printForceChangeMessage(setForce, desiredForce);
    }
    buttonTopState = digitalRead(topButtonPin); // SET
    buttonBottomState = digitalRead(bottomButtonPin);
  }
  //if selected cancelled

  //if (buttonBottomState == 0){ // Cancel Button
  //desiredForce = setForce;
  //return;
  //}
  //if selected continue or set
  setForce = desiredForce;
  Serial.println("Set Force");
  Serial.println(setForce);
  t_ms = micros();
  t_old = t_ms / 1000000.0;
  CaseValMain = 2;
}// end setForceCommands method


void printForceChangeMessage(int oldValue, int newValue) {
  //set text size
  tft.textEnlarge(2);

  tft.textSetCursor(10, 200);
  tft.textColor(RA8875_WHITE, RA8875_BLACK);
  //print warning
  tft.textWrite("Do you want to change the force from ");
  printValue(oldValue);
  tft.textWrite(" lbs, to ");
  printValue(newValue);
  tft.textWrite(" lbs?");

  //print top button message (Set Weight)
  tft.textColor(RA8875_WHITE, RA8875_BLACK);
  tft.textSetCursor(640, 50); //tft.textSetCursor(640,50+480*0/3);
  tft.textWrite("Set");

//print middle button message (Pause)
  tft.textSetCursor(600, 50 + 480 * 2 / 3); // THIS NEEDS TO CHANGE TO BE NEXT TO THE TOP BUTTON
  tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
  tft.textWrite("Pause"); // Print on GUI
  

  //print bottom button message
  tft.textSetCursor(670, 50 + 480 * 2 / 3);
  tft.textWrite("Cancel");
  //tft.textSetCursor(640,100+480*2/3);
  //tft.textWrite("Weight");
}//end printForceChangeMessage function

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


void UpdateCurrentForce() {
  if (loadcell.is_ready()) {
    byte times = 5;
    currentForce = loadcell.get_units();
    //Serial.println(loadcell.get_units());
    //Serial.println(currentForce);
    tft.textSetCursor(10, 100 + 480 * 2 / 3); // Needs to be moved to the top left corner
    tft.textWrite("Current Force: ");
    tft.textSetCursor(360, 100 + 480 * 2 / 3); // Needs to be aligned with the Text of Current Force
    printValue(currentForce);
  }
}

// Keep for reference/////
/*
    //tft.fillScreen(RA8875_BLACK);
  //    tft.textSetCursor(600,50+480*0/3);
  //    tft.textEnlarge(1.5);
  //    tft.textColor(RA8875_WHITE, RA8875_BLACK);
  //    tft.textWrite("Top button 1.5");
  //    tft.textSetCursor(600,100+480*0/3);
  //    tft.textEnlarge(2);
  //    tft.textColor(RA8875_WHITE, RA8875_BLACK);
  //    tft.textWrite("Top button 2.0");

    tft.textSetCursor(600,50+480*1/3);
    tft.textEnlarge(1);
    tft.textColor(RA8875_WHITE, RA8875_BLACK);
    tft.textWrite("Middle button");

    tft.textSetCursor(600,50+480*2/3);
    tft.textEnlarge(2);
    tft.textColor(RA8875_WHITE, RA8875_BLACK);
    tft.textWrite("Bottom button");

*/
