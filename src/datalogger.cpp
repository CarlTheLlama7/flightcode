#include "datalogger.h"
#include <SD.h>
#include <math.h>
#include <string.h>

// I'd like to gather some data while our code is being flight tested,
// looking for a text file formatted similar to this:

/*

    Time    Acceleration    Velocity    Altitude    Proj. Altitude    Raw Altitude    Raw Acceleration
    2.20          -90.94      574.47     3231.78           5280.00         3191.74              -93.21
    2.25          -89.25      565.39     3272.41           5279.00         3292.42              -87.84
    2.30          -87.94      560.42     3303.61           5281.00         3330.05              -89.12

    ... and so on

*/

// 99% sure this is guaranteed to not compile but I think y'all will see what I'm going for
// I went ahead and pushed it to the repo so we can all take a look. I forgot fprintf doesn't work 
// with SD library I think

File dataFile;

// Return the number of characters in each header name
int header_len(const char * header) {
    return strlen(header);
}

// Return the number of whole digits for each data point
// Thank you, Stack Overflow
int data_len(double data) {
    int n = (int) data;
    if (n == 0) return 1;
    return floor(log10(abs(n))) + 1;
}

// Load the data from the current loop in the main script
// into the array that holds the data. I guess we can just 
// do this directly without having two arrays

// void push_data(double dataArray[num_data]) {
//     memcpy(data, dataArray, sizeof(data));
// }

// Clear the data array so we can load the data for the next iteration
void pop_data() {
    memset(data_array, 0, sizeof(data_array));
}

// Print the data array to the text file
void print_data() {
    if (dataFile) {
        // Length of data array
        int arrLength = sizeof(data_array) / sizeof(data_array[0]);

        for (int i = 0; i < arrLength; i++) {
            // The amount of indenting per column so data is indented correctly
            // Subtracting 2 here because we're printing the data to two decimal places.
            int indent = header_len(headers[i]) - data_len(data_array[i]) - 2;

            // Print data tab delimited, unless it's the last data point, then print a new line
            if (i == num_data - 1) fprintf(dataFile, "%*s%.2f\n", indent, "", data_array[i]);
            else fprintf(dataFile, "%*s%.2f\t", indent, "", data_array[i]);
        }
    }
}

// Open the file up and print the header
void initialize_file() {
    if (!SD.begin(chipSelect)) {
        Serial.println("SD card failed.");
    } else {
        File dataFile = SD.open(fileName, FILE_WRITE);
        // printing the data file
        if (dataFile) {
            for (int i = 0; i < num_data; i++) {
                if (i == 6) fprintf(dataFile, "%s\n", headers[i]); // print new line if at end of array
                else fprintf(dataFile, "%s\t", headers[i]); // else print tabs
            }
        }
    }
}
