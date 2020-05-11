#pragma once




//mechanical definitions
//mount specific definitions
//i will use the same specs for each axis
//for axis RA
#define RA_STEP 200         //motor step count
#define RA_DRIVER_USTEP 128 //driver micro step settings
#define RA_GEAR_RATIO 3   //gear ratio worm gear, planetary gear or harmonic drive
//for axis DEC
#define DEC_STEP 200         //motor step count
#define DEC_DRIVER_USTEP 128 //driver micro step settings
#define DEC_GEAR_RATIO 3   //gear ratio worm gear, planetary gear or harmonic drive

#define RA_REV  86164.0905 //23 hour 56 minutes 4.09053 seconds
#define DEC_REV 1296000  //360 degree 

//max step count for axis
//total second per revolution 86400 second
//resolution  RA 0.016875 second
//total degree per revolution 360 degree
//resolution  DEC 0.253125 arcsecond



// Define the Pins used for motor driver.
//spi drives not implemented yet
#define home_switch -1 // Pin 9 connected to Home Switch

#define RA_STEP_PIN 33
#define RA_DIR_PIN 32
#define RA_ENABLE_PIN 25

#define DEC_STEP_PIN 26
#define DEC_DIR_PIN 27
#define DEC_ENABLE_PIN 25

#define FOCUS_STEP_PIN 18
#define FOCUS_DIR_PIN 19
#define FOCUS_ENABLE_PIN 25

//autofocus settings
//TODO

//output pins
#define OUTPUT0 14
#define OUTPUT1 -1
#define OUTPUT2 -1
#define OUTPUT3 -1

//serialport line buffer size
#define MAX_BUF 50


#define SD_CS 15

//ble
#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"


#define RA_MAX_SPEED = 5000;     // Set Max Speed of Stepper (Faster for regular movements)
#define RA_MAX_ACCELERATION = 10000; // Set Acceleration of Stepper


#define DEC_MAX_SPEED = 5000;     // Set Max Speed of Stepper (Faster for regular movements)
#define DEC_MAX_ACCELERATION = 10000; // Set Acceleration of Stepper

  //FOCUS axis

#define FOCUS_MAX_SPEED = 5000;     // Set Max Speed of Stepper (Faster for regular movements)
#define FOCUS_MAX_ACCELERATION = 10000; // Set Acceleration of Stepper