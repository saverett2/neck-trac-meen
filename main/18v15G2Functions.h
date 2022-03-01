//include I2C library for serial communication
#include <Wire.h>//TODO: maybe move this to display category


//required globals//TODO: probably make these static
uint16_t smcDeviceNumber;
uint16_t smcDeviceNumberL1 = 1;//Left means its left from standing at food of bed
uint16_t smcDeviceNumberR2 = 2;

double travelSpeed;



// This function disables safe start and is required for the motors to move. 
// Must be called when the controller restarts and after any error. 
void exitSafeStart(uint8_t smcDeviceNumber){
  Wire.beginTransmission(smcDeviceNumber);
  Wire.write(0x83);  // Exit safe start
  Wire.endTransmission();
}
 

// This function sets the speed of the motor. 
void setMotorSpeed(uint8_t smcDeviceNumber, int16_t percentSpeed)
{
  uint8_t cmd = 0x85;  // Motor forward
  if (percentSpeed < 0)
  {
    cmd = 0x86;  // Motor reverse
    percentSpeed = -percentSpeed;
  }
  Wire.beginTransmission(smcDeviceNumber);
  Wire.write(cmd);
  Wire.write(0);
  Wire.write(percentSpeed);
  Wire.endTransmission();
}
 

// This function reads how long it has been since the last time the motor was reset or powered up. 
uint16_t readUpTime(uint8_t smcDeviceNumber)
{
  Wire.beginTransmission(smcDeviceNumber);
  Wire.write(0xA1);  // Command: Get variable
  Wire.write(28);    // Variable ID: Up time (low)
  Wire.endTransmission();
  Wire.requestFrom(smcDeviceNumber, (uint8_t)2);
  uint16_t upTime = Wire.read();
  upTime |= Wire.read() << 8;
  return upTime;
}

// This function reads the value of the potentiometer from the selected arduino pin and converts it to position using calibrated values
double getPosition(uint16_t motorPotPin){
  double val = analogRead(motorPotPin); 
  double postn = 0.0069*val -0.1978; // Convert count to position, values determined by experiment using a reference voltage of 4.79 V
  return postn; 
}

// This function sets the speed of both motors. 
// Note device numbers must be named in main scrip.
void setSpeedBoth(int16_t spd){
  setMotorSpeed(smcDeviceNumberL1, spd); 
  setMotorSpeed(smcDeviceNumberR2, spd); 
}

// This function sets the maximum speed between 0 and 3200 that the motor can go forward or backward. 
// Note: This speed cannot be above the absolute maximum motor speed programmed through the motor driver through USB.
// Possible returns: 
// 0 - No problem setting the limit
// 1 - Unable to set forward limit because of hard motor limit settings
// 2 - Unable to set reverse limit because of hard motor limit settings
// 3 - Unable to set forward and reverse limit because of hard motor limit settings
int setMaxSpeed(uint8_t smcDeviceNumber, uint16_t maxSpeed){
  // convert max speed to two bytes
  byte firstByte = maxSpeed & 0x1F; 
  byte secondByte = maxSpeed >> 5 & 0x7F; 
  
  Wire.beginTransmission(smcDeviceNumber);
  Wire.write(0xA2); // Command Byte
  Wire.write(firstByte); 
  Wire.write(secondByte);
  int check = Wire.endTransmission(); 
  return check; 
}

// This function checks the current error of the motor. 
// See data sheet for error output meanings
int checkError(uint8_t smcDeviceNumber){
  Wire.beginTransmission(smcDeviceNumber); 
  Wire.write(0xA1); // Command byte
  Wire.write(0); // ID for error 
  Wire.endTransmission(); 
  Wire.requestFrom(smcDeviceNumber, (uint8_t)2);
  uint16_t error = Wire.read(); 
  error |= Wire.read() << 8; 
  return error; 
}

// This function stops the motor and will make controller susceptible to a safe-start violation if it is enabled 
void stopMotor(uint8_t smcDeviceNumber){
  Wire.beginTransmission(smcDeviceNumber); 
  Wire.write(0xE0); // Stop motor
  Wire.endTransmission(); 
}

// This function will stop both motors and make the controllers susceptible to safe-start violations if enabled.
void stopBoth(){
  stopMotor(smcDeviceNumberL1); 
  stopMotor(smcDeviceNumberR2); 
}
