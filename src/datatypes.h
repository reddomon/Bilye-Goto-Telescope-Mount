#ifndef DATATYPES_H  //definition check
#define DATATYPES_H

#include <Arduino.h>

#define pi 3.141592653589793

//goto type
enum clockType{
    stopped,
    tracking,
    stellar,
    planetary,
    solar,
    lunar

     };

enum planets{
    Sun,
    Moon,
    Mercury,
    Venus,
    Mars,
    Jupiter,
    Saturn,
    Uranus,
    Neptune,
    Pluto,  //not defined
    Stellar = 20

};


//time data holder class
class TimeData{

public:

    //variables
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;
    //centisecond cannot be acquired from gps or rtc. So we have to do it in timer interrupts
    double centiseconds = 0;
    //rawtime is the decimal representation of the time
    double rawtime = 0;

};


//class types have to defined struct but we can change working style, because we may need some getters and setters
//machine status
class MountStatus{

private:

public:
    //gettable and settable
    int mountTrackingType = tracking; //enum
    int isTracking = 0;
    bool objectFound = false;

    //current coordinates
    double currentLat = 40.821654;  //longitude
    double currentLong = 29.950718; //latitude
    double currentAlt = 25; //meters
    //date settings
    int currentYear = 2000,currentMonth = 1,currentDay = 1;
    //time settings
    int currentHour = 0,currentMinute = 0,currentSecond = 0,currentCentiSecond = 0;
    int GMT=3;
    int summerTime = false;  //true or false

    TimeData GMST; //grw siderealtime
    TimeData LMST; //local siderealtime
    TimeData GT;  //grw time
    TimeData LT;  //localtime


public:
    String getGMTString();
    String getLTString();
    String getGMSTString();
    String getLMSTString();
    String getDATEString();
    String getLATString();
    String getLONString();
    String getALTString();




};



//observing object's basicproperties like ra dec
class ObservingObject{

private:
   

public:
    long rowid;
    int code = 0;
    String name = "";
    int type = 0;  //star, nebula, planetary nebula, planet, Sun vs
    double objRA = 0;
    double objDEC = 0;
    double LHA ;
    double LDEC;
    String sign = "";  //constallation  
    String catalog = "";  //messsier, ngc
    String information = ""; 
    
public:
    //not necessary right now.
    void parseObject();

    String getObjectRAString();
    String getObjectDECString();
    String getObjectLHAString();
    String getObjectLDECString();

};


class Point{

public:
    double x;
    double y;
};


class OrbitalElements{

public:

    double N;
    double i;
    double w;
    double a;
    double e;
    double M;
    double oblecl;

    double dx;
    double dy;

};

#endif