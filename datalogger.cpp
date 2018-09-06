#include "datalogger.h"
#include <SD.h>
#include <math.h>
#include <string.h>

// I'd like to gather some data while our code is being flight tested,
// looking for a text file formatted similar to this:

/*
    Time(sec)    Acceleration(ft/sec^2)    Velocity(ft/sec)    Altitude(ft)    Proj. Altitude(ft)    Raw Altitude(ft)    Raw Acceleration(ft/sec^2)
         2.20                    -90.94              574.47         3231.78               5280.00             3191.74                        -93.21
         2.25                    -89.25              565.39         3272.41               5279.00             3292.42                        -87.84
         2.30                    -87.94              560.42         3303.61               5281.00             3330.05                        -89.12
    ... and so on
*/

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
    else if (n < 0) return floor(log10(abs(n))) + 2;
    else return floor(log10(abs(n))) + 1;
}

// Load the data from the current loop in the main script
// into the array that holds the data. I guess we can just 
// do this directly without having two arrays

// Clear the data array so we can load the data for the next iteration
void pop_data() {
    memset(data_array, 0, sizeof(data_array));
}

// Print the data array to the text file
void print_data() {
    if (dataFile) {
        // Length of data array
        int arrLength = sizeof(data_array) / sizeof(data_array[0]);
        char data_buffer[256]; // load each formatted data point into this buffer, then write to the file

        for (int i = 0; i < arrLength; i++) {
            // The amount of indenting per column so data is indented correctly
            // Subtracting 3 here because we're printing the data to two decimal places
            // so we need space for the two decimal places and the period
            int indent = header_len(headers[i]) - data_len(data_array[i]) - 3;
            // Print data tab delimited, unless it's the last data point, then print a new line
            sprintf(data_buffer + strlen(data_buffer), "%*s%.2f\t", indent, "", data_array[i]);
        }
        dataFile.println(data_buffer);
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
            char header_buffer[164];
            for (int i = 0; i < num_data; i++) {
                sprintf(header_buffer + strlen(header_buffer), "%s\t", headers[i]);
            }
            dataFile.println(header_buffer);
        }
    }
}