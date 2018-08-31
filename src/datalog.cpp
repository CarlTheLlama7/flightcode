#include "datalog.h"
#include <SD.h>

// I'd like to gather some data while our code is being flight tested,
// looking for a text file formatted similar to this:

/*

    Time    Acceleration    Velocity    Altitude    Proj. Altitude    Raw Altitude    Raw Acceleration
    2.20          -90.94      574.47     3231.78           5280.00         3191.74              -93.21
    2.25          -89.25      565.39     3272.41           5279.00         3292.42              -87.84
    2.30          -87.94      560.42     3303.61           5281.00         3330.05              -89.12

    ... and so on

*/


// Ignore this for now, work in progress
void DataLog::initialize_file() {
    if (!SD.begin(chipSelect)) {
        Serial.println("SD card failed.");
    } else {
        File dataFile = SD.open(fileName, FILE_WRITE);
        if (dataFile) {
            // If datafile opened fine print a header

//             dataFile.close();
        }
    }
}
