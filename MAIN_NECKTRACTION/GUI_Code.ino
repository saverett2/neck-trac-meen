
void StartGuiDisplay()
{
  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  tft.fillScreen(RA8875_WHITE); // set the background the white
  tft.textEnlarge(7);
  tft.textMode();
  //tft.setFont(&FreeSans9pt7b);
  delay(10);
}

void runStartWarnings() {

  CaseValStart = 1;
  // Fill screen with black

  tft.fillScreen(RA8875_WHITE);
  //tft.setFont(&FreeSans9pt7b);
  tft.textColor(RA8875_RED, RA8875_WHITE);
  tft.textSetCursor(120, 400 * 1 / 2);
  tft.textWrite("University of Utah");

  tft.textEnlarge(2);
  //tft.fillScreen(RA8875_WHITE);
  tft.textColor(RA8875_BLACK, RA8875_WHITE);
  tft.textSetCursor(240, 560 * 1 / 2);
  tft.textWrite("Neck Tracktion");

  tft.textEnlarge(2);
  // Show "Continue" Text Next to top button to switch to next case
  tft.textSetCursor(600, 50 + 480 * 0 / 3);// THIS NEEDS TO CHANGE TO BE NEXT TO THE TOP BUTTON****
  tft.textColor(RA8875_BLACK, RA8875_WHITE); // GUI white text
  tft.textWrite("Continue"); // Print on GUI


  buttonTopState = digitalRead(topButtonPin);

  while (buttonTopState == 1)
  {
    // Top button is lit up
    digitalWrite(TopLED, HIGH);
    digitalWrite(MidLED, LOW);
    digitalWrite(BotLED, LOW);

    //Wait until the top button is pushed
    buttonTopState = digitalRead(topButtonPin);
    Serial.println(buttonTopState);
    Serial.println("Waiting for Top Button to be Pushed 1");
  }
  while (buttonTopState != 1) {
    buttonTopState = digitalRead(topButtonPin);
    Serial.println("We are in the waiting part of case 1");
    Serial.println(buttonTopState);
  }



  while (CaseValStart != 6) {
    CaseValStart  = constrain(CaseValStart, 1, 6);
    int readingTop = digitalRead(topButtonPin);
    int readingMid = digitalRead(midButtonPin);
    int readingBot = digitalRead(bottomButtonPin);


    // If the switch changed, due to noise or pressing:
    if ((readingTop != lastButtonStateTop) || (readingBot !=  lastButtonStateBot)) {
      // reset the debouncing timer
      lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
      // whatever the reading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state:

      // if the button state has changed:
      if ((readingTop != buttonTopState) || (readingBot != buttonBottomState) || (readingMid != buttonMidState)) {
        buttonTopState = readingTop;
        buttonBottomState = readingBot;
        buttonMidState = readingMid;

        // only toggle the case if the Button State is pressed ( State == 0)
        if ((buttonMidState == 0) && (buttonBottomState == 1)) {
          CaseValMain = 1;
          return;
        }
        if ((buttonTopState == 0) && (buttonBottomState == 1)) {
          CaseValStart = CaseValStart + 1;
          //ledState = !ledState;
        }
        if ((buttonBottomState == 0) && ( buttonTopState == 1)) {
          CaseValStart = CaseValStart - 1;
          //ledState = !ledState;
        }
      }
    }

    // set the case:
    ///////digitalWrite(ledPin, ledState);
    writeback = 2;
    switch (CaseValStart) {

      case 1:

        digitalWrite(TopLED, HIGH);
        digitalWrite(MidLED, LOW);
        digitalWrite(BotLED, LOW);

        // Load Cell Setup
        ///////////////////////// Needs to be moved to the beginning set up.

        if (previousCas != 1) {
          Serial.print("Setting up force sensor...");
          loadcell.begin(loadcellDOUT, loadcellSCK);
          loadcell.set_scale(calibrationFactor);
          Serial.println("done");
          // If function is outputing 0 values consistently the load cell is not connected properly
          Serial.println(loadcell.is_ready());
          tft.fillScreen(RA8875_BLACK);
          int loadcelltimeold = millis();
          while (loadcell.is_ready() != true) {
            tft.textEnlarge(4);
            tft.textSetCursor(100, 150);
            tft.textColor(RA8875_WHITE, RA8875_BLACK); // White Text, in black box
            tft.textWrite("Waiting for OP AMP to Connect..."); // Print on GUIGUI
          }

          // Tongs should not be attached yet

          tft.fillScreen(RA8875_BLACK);
          tft.textEnlarge(2);
          tft.textSetCursor(10, 480 * 1 / 2);
          tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
          tft.textWrite("Is the load cell sensor plugged in?"); // Print on GUI
          // Show Continue Text Next to top button to switch to next case
          tft.textSetCursor(600, 50 + 480 * 0 / 3); // THIS NEEDS TO CHANGE TO BE NEXT TO THE TOP BUTTON
          tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
          tft.textWrite("Continue"); // Print on GUI
          loadcell.tare(); // Number of times to average, blocking call
          previousCas = 1;
        }
        break;

      case 2:
        digitalWrite(TopLED, HIGH);
        digitalWrite(MidLED, LOW);
        digitalWrite(BotLED, HIGH);

        if (previousCas != 2) {
          tft.fillScreen(RA8875_BLACK);
          tft.textEnlarge(2);
          tft.textSetCursor(10, 480 * 1 / 2);
          tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
          tft.textWrite("Is the device secured to the bed and plugged in?"); // Print on GUI
          ContinueBack(); // Show Continue and Go Back on Display
          previousCas = 2;
        }
        break;
      // Choose to continue or go back

      case 3:
        digitalWrite(TopLED, HIGH);
        digitalWrite(MidLED, LOW);
        digitalWrite(BotLED, HIGH);

        if (previousCas != 3) {
          Serial.println("I am in start case 3");
          tft.fillScreen(RA8875_BLACK);
          tft.textEnlarge(2);
          tft.textSetCursor(10, 480 * 1 / 2);
          tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
          tft.textWrite("Are all joint clamps secured?"); // Print on GUI
          // function to display "Continue and Back"
          ContinueBack();
          previousCas = 3;
        }
        break;

      case 4:

        digitalWrite(TopLED, HIGH);
        digitalWrite(MidLED, LOW);
        digitalWrite(BotLED, HIGH);


        if (previousCas != 4) {
          Serial.println("I am in start case 4");
          tft.fillScreen(RA8875_BLACK);
          tft.textEnlarge(2);
          tft.textSetCursor(10, 480 * 1 / 2);
          tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
          tft.textWrite("Are the tongs attached securely");
          tft.textSetCursor(10, 290); // Needs to be checked for location
          tft.textWrite("to the Force Sensor?"); // Print on GUI

          // function to display "Continue and Back"
          ContinueBack();
          previousCas = 4;
        }
        break;

      case 5:

        digitalWrite(TopLED, HIGH);
        digitalWrite(MidLED, LOW);
        digitalWrite(BotLED, HIGH);


        if (previousCas != 5) {
          tft.fillScreen(RA8875_BLACK);
          tft.textEnlarge(2);
          tft.textSetCursor(10, 480 * 1 / 2);
          tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
          tft.textWrite("Device is ready: Press Continue to Begin"); // Print on GUI

          // function to display "Continue and Back"
          ContinueBack();
          previousCas = 5;
        }
        break;
    }
    // Goes at the end
    // save the reading. Next time through the loop, it'll be the lastButtonState:
    lastButtonStateTop = readingTop;
    lastButtonStateMid = readingMid;
    lastButtonStateBot = readingBot;
  }
}


void ContinueBack() {
  tft.textEnlarge(2);
  // Show "Continue" Text Next to top button to switch to next case
  tft.textSetCursor(600, 50 + 480 * 0 / 3);// THIS NEEDS TO CHANGE TO BE NEXT TO THE TOP BUTTON****
  tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
  tft.textWrite("Continue"); // Print on GUI
  tft.textSetCursor(600, 50 + 480 * 2 / 3); // THIS NEEDS TO CHANGE TO BE NEXT TO THE MIDDLE/BOTTOM BUTTON****
  // Show "Back" text next to middle button to switch to previous case

  if (writeback != 1) {
    tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
    tft.textWrite("Back"); // Print on GUI
    writeback = 2;
  }
}


void PauseContinue() { // If the pauseButtonState == 0 move to this function and stop the device.
  myPID.SetMode(MANUAL);
  stopBoth();
  // Start moving the device to the desiredForce. This could go back to the set Force Command function
}


void PrintSerialMonitor() {
  // Time
  Serial.print(t);
  Serial.print("  ");
  // Force
  Serial.print(currentForce);
  Serial.print("  ");
  
  // Position
  //Low-Pass filter
  potPinL1_reading[2] = IIRFilter( potPinL1_reading[0], potPinL1_reading[1]);
  potPinR2_reading[2] = IIRFilter( potPinR2_reading[0], potPinR2_reading[1]);
  
  //assign previous val as the currnet value. 
  potPinL1_reading[0] = potPinL1_reading[2];
  potPinR2_reading[0] = potPinR2_reading[2];

  //average potentiometer values and convert to distance (mm)
  AvgPotRead = ((potPinL1_reading[2] + potPinR2_reading[2]) / 2.0 );
  DistPosition = (152.0 / 856.0) * ((float)AvgPotRead - 883.0) + 152.0; //(mm)

  //print distance to serial display
  Serial.print(DistPosition);
  Serial.println(" ");
}

//Filters values to reduce noise with a 
// rolling IIR filter. Increase alpha to 
// increase damping/filtering
float IIRFilter(float oldVal, float currVal){
  float alpha = 0.5875;
  return oldVal*alpha + currVal*(1-alpha);
}
