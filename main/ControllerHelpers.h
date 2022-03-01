
//check if op amp has timed out 
void checkOpAmpTimeout(void){//TODO: make this code event driven without while loops
  int loadcelltimeold = millis();
  while (loadcell.is_ready() != true) {
    while ((millis() - loadcelltimeold) > 5000) {//TODO: switch this times with the const int
      displayOpAmpTimeOutError();
    }
    displayOpAmpWaiting();
  }
}// timeout

//tares the load cell. Displays error message if loadcell isn't connected
void tareLoadCell(void){
  if ( !loadcell.is_ready() ){
    delay(50);//TODO: remove this if it doesn't cause flashing

    //while loadcell isn't ready, display error message
    displayLoadCellErrorMessage();

    //wait till loadcell is plugged in    }
    while (!loadcell.is_ready()) { }
  }//end if check
  
  //Tare/zero the loadcell
  loadcell.tare(); // Number of times to average, blocking call
  return;
}

//startup sequence going throught the warnings and checks for setting
// up the robot. 
void setupWarningSequence(void){
  //check if any "continue" or "back" has been selected
  if(topButtonSelected){  setupPageCase = setupPageCase + 1; }//move forward
  if(botButtonSelected){   setupPageCase--; }//move backwards //TODO: confirm if this notation works
  constrain(setupPageCase, 0, 6);//TODO: change the upper max

  //if button has been pressed to change the display screen, enter switch case. Otherwise skip it
  if (prevSetupPageCase != setupPageCase){
    switch( setupPageCase ){
      case 0:
        displayStartUpPage();
        break;
        
      case 1:
        //setup loadcell stuff
        loadcell.begin(loadcellDOUT, loadcellSCK);
        loadcell.set_scale(calibrationFactor);

        //check if op amp is connected or if it's timed out
        checkOpAmpTimeout();

        //if loadcell isn't ready, print warning//TODO: maybe put a small delay if it flashes//TODO: make this it's own function
        tareLoadCell();
        displayLoadCellSuccess();
        
        //Display Buttons
        displayContinue();
        displayBack();
        break;
      
      case 2:
        //display message asking if device is secured to bed
        displayDeviceSecuredCheck();
      
        //Display Buttons
        displayContinue();
        displayBack();
        break;
      case 3:
        //display a message asking if the joint clamps are secured
        displayJointClampsSecuredCheck();
      
        //Display Buttons
        displayContinue();
        displayBack();
        break;
      case 4:
        //display a message asking if the tongs are secured
        displayTongsSecuredCheck();
      
        //Display Buttons
        displayContinue();
        displayBack();
        break;
      case 5:
        //display a message stating device is ready
        displayDeviceReadyMessage();

        //Display Buttons
        displayContinue();
        displayBack();
        break;
      case 6:
        //exits the starting up event
        settingUpRobot = false;

        //Display Buttons
        displayContinue();
        displayBack();
        break;
    }//end switch case

    //update which case was last
    prevSetupPageCase = setupPageCase;
  }//end if
}//end setupWarningSequence

//reads and displays the current force in from the loadcell.
//  If the loadcell is disconected, displays an error
void updateCurrentForce(void) {
  //If loadcell isn't connected, display error
  if ( !loadcell.is_ready() ) { displayLoadCellErrorMessage(); }

  //read force
  byte times = 5; //reads times many times and gets the average
  currentForce = loadcell.get_units();

  //print to GUI
  displayForce( currentForce );//TODO: maybe make currentForce local
}//end updateCurrentForce
