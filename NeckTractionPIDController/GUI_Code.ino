
void StartGuiDisplay()
{
  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  tft.fillScreen(RA8875_BLACK); // Sets the color of the background.
  tft.textEnlarge(2);
  tft.textMode();
  delay(10);
}

void runStartWarnings() {
  int CaseValStart = 1;
  // Fill screen with black
  tft.fillScreen(RA8875_BLACK);
  tft.textColor(RA8875_WHITE, RA8875_BLACK);
  tft.textSetCursor(10, 480 * 1 / 2);
  tft.textWrite("Welcome to Neck Traction!");

  buttonTopState = digitalRead(topButtonPin);
  while (buttonTopState == 1)
  {
    //Wait until the top button is pushed
    buttonTopState = digitalRead(topButtonPin);
    //Serial.println(buttonTopState);
    Serial.println("Waiting for Top Button to be Pushed 1");
  }
  while (buttonTopState != 1) {
    buttonTopState = digitalRead(topButtonPin);
    Serial.println("Wh are in the waiting part of case 1");
  }




  switch (CaseValStart) {

    case 1:
      // Load Cell Setup
      Serial.print("Setting up force sensor...");
      loadcell.begin(loadcellDOUT, loadcellSCK);
      loadcell.set_scale(calibrationFactor);
      Serial.println("done");
      // If function is outputing 0 values consistently the load cell is not connected properly
      Serial.println(loadcell.is_ready());
      while (loadcell.is_ready()) {
        tft.textSetCursor(10, 480 * 2 / 3);
        tft.textColor(RA8875_WHITE, RA8875_BLACK); // White Text, in black box
        tft.textWrite("Warning: Force sensor not connected properly"); // Print on GUI
        Serial.println("Load cell is not connected");
        break;
      }
      Serial.println("Load cell is connected");
      //loadcell.tare(); // Number of times to average, blocking call
      tft.fillScreen(RA8875_BLACK);
      tft.textSetCursor(10, 480 * 1 / 2);
      tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
      tft.textWrite("Force Sensor is connected"); // Print on GUI
      // Show Continue Text Next to top button to switch to next case
      tft.textSetCursor(600, 50 + 480 * 0 / 3); // THIS NEEDS TO CHANGE TO BE NEXT TO THE TOP BUTTON
      tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
      tft.textWrite("Continue"); // Print on GUI
      // If continue button is pressed, then move to case 2 of the runStartWarnings function
      buttonTopState = digitalRead(topButtonPin);
      while (buttonTopState == 1)
      {
        //Wait until the top button is pushed
        buttonTopState = digitalRead(topButtonPin);
        //Serial.println(buttonTopState);
        Serial.println("Waiting for Top Button to be Pushed 1");
      }
      while (buttonTopState != 1) {
        buttonTopState = digitalRead(topButtonPin);
        Serial.println("Wh are in the waiting part of case 1");
      }
      CaseValStart = 2;

    case 2:
      Serial.println("I am in start case 2");
      tft.fillScreen(RA8875_BLACK);
      tft.textSetCursor(10, 480 * 1 / 2);
      tft.textEnlarge(2);
      tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
      tft.textWrite("Is the device secured to the bed and plugged in?"); // Print on GUI
      Serial.println("GUI show continue and back");
      ContinueBack(); // Show Continue and Go Back on Display
      // Choose to continue or go back
      // If continue button is pressed, then move to case 2 of the runStartWarnings function
      buttonTopState = digitalRead(topButtonPin);
      while (buttonTopState == 1)
      {
        //Wait until the top button is pushed
        buttonTopState = digitalRead(topButtonPin);
        //Serial.println(buttonTopState);
        Serial.println("Waiting for Top Button to be Pushed 2");
      }
      while (buttonTopState != 1) {
        buttonTopState = digitalRead(topButtonPin);
        Serial.println("Wh are in the waiting part of case 2");
      }

    case 3:
      Serial.println("I am in start case 3");
      tft.fillScreen(RA8875_BLACK);
      tft.textSetCursor(10, 480 * 1 / 2);
      tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
      tft.textWrite("Are all joint clamps secured?"); // Print on GUI

      // function to display "Continue and Back"
      ContinueBack();

      // Choose to continue or go back
      buttonTopState = digitalRead(topButtonPin);
      while (buttonTopState == 1)
      {
        //Wait until the top button is pushed
        buttonTopState = digitalRead(topButtonPin);
        //Serial.println(buttonTopState);
        Serial.println("Waiting for Top Button to be Pushed 3");
      }
      while (buttonTopState != 1) {
        buttonTopState = digitalRead(topButtonPin);
        Serial.println("Wh are in the waiting part of case 3");
      }
      // Go Back if we have another button
      if (buttonBottomState == HIGH) { // If back buttin is pressed, then move to case 2 of the run StartWarnings function
        CaseValStart = 2;
        break;
      }
    case 4:
      Serial.println("I am in start case 4");
      tft.fillScreen(RA8875_BLACK);
      tft.textEnlarge(2);
      tft.textSetCursor(10, 480 * 1 / 2);
      tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
      tft.textWrite("Is the tongs attached securely to the Force Sensor?"); // Print on GUI

      // function to display "Continue and Back"
      ContinueBack();

      // Choose to continue or go back
      buttonTopState = digitalRead(topButtonPin);
      while (buttonTopState == 1)
      {
        //Wait until the top button is pushed
        buttonTopState = digitalRead(topButtonPin);
        //Serial.println(buttonTopState);
        Serial.println("Waiting for Top Button to be Pushed 4");
      }
      while (buttonTopState != 1) {
        buttonTopState = digitalRead(topButtonPin);
        Serial.println("Wh are in the waiting part of case 4");
      }

    //if (buttonBottomState == HIGH) { // If back buttin is pressed, then move to case 2 of the run StartWarnings function
    //CaseValStart = 3;
    //break;

    case 5:
      tft.fillScreen(RA8875_BLACK);
      tft.textEnlarge(2);
      tft.textSetCursor(10, 480 * 1 / 2);
      tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
      tft.textWrite("Thank you for checking the device"); // Print on GUI

      // function to display "Continue and Back"
      ContinueBack();

      // Choose to continue or go back
      buttonTopState = digitalRead(topButtonPin);
      while (buttonTopState == 1)
      {
        //Wait until the top button is pushed
        buttonTopState = digitalRead(topButtonPin);
        //Serial.println(buttonTopState);
        Serial.println("Waiting for Top Button to be Pushed 5");
      }
      while (buttonTopState != 1) {
        buttonTopState = digitalRead(topButtonPin);
        Serial.println("We are in the waiting part of case 5");
      }
      return;

    // Switch case needs to deleted
    default :
      tft.fillScreen(RA8875_BLACK);
      tft.textEnlarge(2);
      tft.textSetCursor(10, 480 * 1 / 2);
      tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
      tft.textWrite("Start up Warnings went to Default, Reset System"); // Print on GUI
  }
}


void ContinueBack() {
  // Show "Continue" Text Next to top button to switch to next case
  tft.textSetCursor(600, 50 + 480 * 0 / 3); // THIS NEEDS TO CHANGE TO BE NEXT TO THE TOP BUTTON****
  tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
  tft.textWrite("Continue"); // Print on GUI
  tft.textSetCursor(600, 50 + 480 * 2 / 3); // THIS NEEDS TO CHANGE TO BE NEXT TO THE MIDDLE/BOTTOM BUTTON****
  // Show "Back" text next to middle button to switch to previous case
  tft.textColor(RA8875_WHITE, RA8875_BLACK); // GUI white text
  tft.textWrite("Back"); // Print on GUI
}

float IIRFilter(float newValue, float oldValue, float alpha) {
  return oldValue + newValue * alpha;
}
