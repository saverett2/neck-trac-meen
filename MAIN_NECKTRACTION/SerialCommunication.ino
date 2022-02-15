

void RecvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
    
    while (Serial.available() > 0 && newOrders == false) {
        rc = Serial.read();
        //Serial.println(rc); 
        if (recvInProgress == true) {
            if (rc != endMarker) {
                message[ndx] = rc;
                ndx++;
            }
            else {
                message[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newOrders = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}


void ParseData() {      // split the data into its parts

    char* strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(message,",");      // get the first part - the string
    strcpy(command, strtokIndx); // copy it to command
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    inputForce = (int) atof(strtokIndx);     // convert this part to uint8_t

    strtokIndx = strtok(NULL, ",");
    travelSpeed = (int) atof(strtokIndx);     // convert this part to a signed int
}
