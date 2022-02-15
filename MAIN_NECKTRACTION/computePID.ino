  // This function will compute the output of a PID controller using the force input from the OP-312
// force sensor and driving the HDA6-30 linear actuators
// Inputs: Desired force in pounds
// Outputs: Percentage speed of motors

// I wrote this by hand before realizing that arduino has a PID libray. Decided to use that. 
// This is left as a reference or in case the library does not work in the future. 

uint16_t computePID(float desiredForce){
  
  // Read in time; 
  unsigned long now = micros(); // current time in micro seconds
  unsigned long timeChange = now - lastTime; // Compute time change

  if (firstTime == true) return 0; // Skip the first time

  if (timeChange < sampleTime) return false; // If trying to calculate more often than sample time, return false

  // Compute error
  float error = desiredForce - currentForce; 
  float dError = (error-oldError)/timeChange;
  IError = IError + error * timeChange; 

  // Compute PID Output
  float pidOut = Kp*error + Kd*dError + Ki*IError; 

  // Save some variables for next time
  lastTime = now; 
  oldError = error; 

  return pidOut; 
}
