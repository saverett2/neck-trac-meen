void GUIPages(int pageNum) {


  // WELCOME TO NECK TRACTION
  if (pageNum == 1) {
    tft.fillScreen(RA8875_WHITE);
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
  }
  //
  if (pageNum == 2) {







  }







}
