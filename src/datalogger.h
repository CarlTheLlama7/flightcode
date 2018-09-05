// datalog.h

#ifndef _DATALOG_h
#define _DATALOG_h


const int num_data = 6; // How many data points we have (i.e. time, acceleration, etc.)

// Thinking we can add the data to this array while the loop is running
// and just clear it out after all the data is written
double data_array[num_data];

const int chipSelect = 10;  // Don't know what it does exactly, gotta look at the docs
const char * fileName = "data.txt";  // Name of file

// All the names of the data points, which will work as column names
const char * headers[] = {"Time", "Acceleration", "Velocity", "Proj. Altitude", 
                          "Raw Altitude", "Raw Acceleration"};


// Set up text file with headers and what not.
void initialize_file();

// Get the length of the header for each column, so we can format the data
int header_len(const char * header);

// Number of whole digits for each data point 
int data_len(double data);

// Load the current iteration's data into an array to print
// void push_data(double dataArray[num_data]);

// Clear the current iteration's data so we can load the next iteration
void pop_data();

// pretty print text to file
void print_data();

#endif

