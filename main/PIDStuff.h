// Include PID library
#include <PID_v2.h>

double Kp = 4; // Proportional Gain (TBD)
double Kd = 0.1; // Derivative Gain (TBD)
double Ki = 1; // Integral Gain (TBD)

double IError = 0; // Integral error (lb*ms)
double oldError = 0; // Old error for PID (lb)
double pidOUT;

double travelSpeedPID;
int travelSpeedPercentage; //TODO: find better place for this

//Position//TODO: maybe make this a different header file
double leftPosition;//TODO: i think this is for Position PID
double rightPosition;

double m1_pos = 0;
double m2_pos = 0;//TODO: check if left position and m1_pos are redundant variables

// initialize PID object
PID myPID(&currentForce, &travelSpeedPID, &desiredForce, Kp, Ki, Kd, DIRECT);


//*********************************Functions*********************************//

void setupPID(void) {
  myPID.SetMode(MANUAL); // Turn on PID controller
  myPID.SetOutputLimits(-100, 100);
  myPID.SetSampleTime(100); // If the loop is faster than the sample time it
  
  // myPID.SetMode(AUTOMATIC); //TEMPORARY TO COLLECT DATA //TODO: check if this can be removed
}

void updatePID(void){
  if (myPID.GetMode() == AUTOMATIC) {
    myPID.Compute(); // Compute with each loop
    travelSpeedPID = constrain(travelSpeedPID, -100, 100); // constrain travel speed to be within reasonable value
    travelSpeedPercentage = (int) travelSpeedPID; // Cast to int to send to motors
    setSpeedBoth(travelSpeedPercentage); // set speed of the motors
    //Serial.println();
    //Serial.println(travelSpeedPercentage);
    //Serial.println();
  }
}

void PauseContinue() { // If the pauseButtonState == 0 move to this function and stop the device.
  myPID.SetMode(MANUAL);
  stopBoth();
  // Start moving the device to the desiredForce. This could go back to the set Force Command function
}
