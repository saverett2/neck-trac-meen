
void StartGuiDisplay()
{
  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  tft.fillScreen(RA8875_BLACK); // Sets the color of the background.
}



void runStartWarnings() {
  int CaseValStart = 1;
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
        char iniForceRead = String("Warning: Force sensor not connected properly");
        // add enlarge function for size****
        // add position of actual text*****
        tft.textTransparent(RA8875_WHITE); // GUI white text
        tft.textWrite(iniForceRead); // Print on GUI
      }
      if (initialForce > 0.01 || intialForce < -0.01) {
        tft.fillScreen(RA8875_BLACK);
        char ForceSenString = String("Force Sensor is connected");
        tft.textEnlarge(2);
        tft.textSetCursor(10, 100+480*2/3);
        tft.textTransparent(RA8875_WHITE); // GUI white text
        tft.textWrite(iniForceRead); // Print on GUI
        // Show Continue Text Next to top button to switch to next case
        tft.textSetCursor(100, 100); // THIS NEEDS TO CHANGE TO BE NEXT TO THE TOP BUTTON
        tft.textTransparent(RA8875_WHITE); // GUI white text
        tft.textWrite(continueString); // Print on GUI

        if (buttonStateTop == HIGH) { // If continue button is pressed, then move to case 2 of the runStartWarnings function
          CaseValStart = 2;
          break;
        }
      }
      else {
        break;
      }


    case 2:
      char BedConString = String("Is the device secured to the bed and plugged in?")
                          tft.fillScreen(RA8875_BLACK);
      tft.textSetCursor(10, 100+480*2/3);
      tft.textEnlarge(2);
      tft.textTransparent(RA8875_WHITE); // GUI white text
      tft.textWrite(BedConString); // Print on GUI
      ContinueBack();
      // Choose to continue or go back
      if (buttonStateTop == HIGH) { // If continue button is pressed, then move to case 3 of the runStartWarnings function
        CaseValStart = 3;
        break;
      }
      if (buttonStateBottom == HIGH) { // If back buttin is pressed, then move to case 1 of the run StartWarnings function
        CaseValStart = 1;
        break;
      }
    case 3:
      char AngleSec = String("Are all joint clamps secured?");
      tft.fillScreen(RA8875_BLACK);
      tft.textEnlarge(2);
      tft.textSetCursor(10, 100+480*2/3);
      tft.textTransparent(RA8875_WHITE); // GUI white text
      tft.textWrite(AngleSec); // Print on GUI

      // function to display "Continue and Back"
      ContinueBack();

      // Choose to continue or go back
      if (buttonStateTop == HIGH) { // If continue button is pressed, then move to case 4 of the runStartWarnings function
        CaseValStart = 4;
        break;
      }
      if (buttonStateBottom == HIGH) { // If back buttin is pressed, then move to case 2 of the run StartWarnings function
        CaseValStart = 2;
        break;
      }
    case 4:
      char ClawStr = String("Is the Claw attached securely to the Force Sensor?");
      tft.fillScreen(RA8875_BLACK);
      tft.textEnlarge(2);
      tft.textSetCursor(10, 100+480*2/3);
      tft.textTransparent(RA8875_WHITE); // GUI white text
      tft.textWrite(ClawStr); // Print on GUI

      // function to display "Continue and Back"
      ContinueBack();

      // Choose to continue or go back
      if (buttonStateTop == HIGH) { // If continue button is pressed, then move to case 4 of the runStartWarnings function
        CaseValStart = 5;
        break;
      }
      if (buttonStateBottom == HIGH) { // If back buttin is pressed, then move to case 2 of the run StartWarnings function
        CaseValStart = 3;
        break;
      }
    case 5:
      char FinishWarning = String("Thank you for checking the device");
      tft.fillScreen(RA8875_BLACK);
      tft.textEnlarge(2);
      tft.textSetCursor(10, 100+480*2/3);
      tft.textTransparent(RA8875_WHITE); // GUI white text
      tft.textWrite(FinishWarning); // Print on GUI

      // function to display "Continue and Back"
      ContinueBack();

      // Choose to continue or go back
      if (buttonStateTop == HIGH) { // If continue button is pressed, then exit function and proceed to main loop
        return;
      }
      if (buttonStateBottom == HIGH) { // If back buttin is pressed, then move to case 4 of the run StartWarnings function
        CaseValStart = 4;
        break;
      }
    default :
      char DefaultWarning = String("Start up Warnings went to Default, Reset System");
      tft.fillScreen(RA8875_BLACK);
      tft.textEnlarge(2);
      tft.textSetCursor(10, 100+480*2/3);
      tft.textTransparent(RA8875_WHITE); // GUI white text
      tft.textWrite(DefaultWarning); // Print on GUI
  }
}


void ContinueBack() {
  // Show "Continue" Text Next to top button to switch to next case
  char contin = String("Continue");
  tft.textSetCursor(600,100+480*0/3); // THIS NEEDS TO CHANGE TO BE NEXT TO THE TOP BUTTON****
  tft.textTransparent(RA8875_WHITE); // GUI white text
  tft.textWrite(contin); // Print on GUI
  tft.textSetCursor(600,50+480*2/3); // THIS NEEDS TO CHANGE TO BE NEXT TO THE MIDDLE/BOTTOM BUTTON****
  char goBack = String("Back");
  // Show "Back" text next to middle button to switch to previous case
  tft.textTransparent(RA8875_WHITE); // GUI white text
  tft.textWrite(goBack); // Print on GUI
}
