// Include force sensor library
#include <HX711.h>

// Load Cell Pins
const int loadcellDOUT = 11; // Load Cell
const int loadcellSCK = 12; // Load Cell

//TODO: maybe move this stuff to PID or comebine the to header files
double inputForce = 0;
double desiredForce = 0;
double currentForce = 0; // Current reading from force sensor (lb)

//initialize load cell (force sensor)
HX711 loadcell;

float calibrationFactor = 23550.f; // Found experimentally with Merryweather's load cell


//*********************************Functions*********************************//

//reads the force and displays to a position
//TODO: maype make this into two separate functions, one to read, one to print
void meausureCurrentForce() {
  if (loadcell.is_ready()) {//TODO: maybe don't do this everytime, maybe just do it in a setup function
    byte times = 5;//think this is neccessaryt to read the current force //TODO: check if it is
    currentForce = loadcell.get_units();
  }
}
