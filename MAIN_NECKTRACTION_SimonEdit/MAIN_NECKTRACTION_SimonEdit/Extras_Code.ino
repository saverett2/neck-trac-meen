/*
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
      if ((readingTop != buttonTopState) || (readingBot != buttonBottomState) ||(readingMid != buttonMidState)) {
        buttonTopState = readingTop;
        buttonBottomState = readingBot;
        buttonMidState = readingMid;

        
        // only toggle the case if the Button State is pressed ( State == 0)
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
//
//
//
//
//          // Change to Desired Force
//      Serial.println("We are in Main Case 1");
//      tft.textSetCursor(10, 100 + 480 * 2 / 3);
//      tft.textWrite("Desired Force: ");
//      tft.textSetCursor(360, 100 + 480 * 2 / 3);
//      printValue(setForce);//
//
//      //Read dial
//      //int desiredForce = forceDial.read(); // Replace with InterruptRotator Code for smoother dial readings

*/
//      //check change in force input
