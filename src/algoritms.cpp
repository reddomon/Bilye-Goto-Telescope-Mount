//for details please refer to : Astronomical Algorithms by Jean Meeus


#include "algoritms.h"
#include <math.h>


//parameters
//longitude
//latitude 
//altitude
//localtime
//summertime
//RA and DEC of an object
//solar angle calculations
//https://en.wikipedia.org/w/index.php?title=Sunrise_equation&oldid=932873734

//http://www.stjarnhimlen.se/comp/tutorial.html

//local time to siderealtime documents
//https://www.quora.com/How-do-I-convert-local-time-to-sidereal-time

    //constructor
    Algorithms::Algorithms(){

            //default constructor
            //do nothing
            //

    }


    //Local hour angle. Parameters: selected object's right ascension and local sidereal time
    double Algorithms::calculateLocalHA(double localSiderealTime,double onjectRA){
        
        double localHourDecimal = localSiderealTime - onjectRA;

        double clippedHourAngle = int(localHourDecimal);
        double clippedHourAngleMinute= ( localHourDecimal-clippedHourAngle)*60; 

        //hour angle corrections
        if (clippedHourAngleMinute < 0){
            clippedHourAngle -= 1;
            clippedHourAngleMinute +=60;
        }
        if (clippedHourAngle < 0){
            clippedHourAngle +=24;
            }
        //return decimal value of Local Hour Angle
        return clippedHourAngle+clippedHourAngleMinute/60;

    }
    //Calculate selected object DEC , parameters observer latitude and object declenation
    double Algorithms::calculateLocalDEC(double latitude,double objectDEC){
        //that simple
        return objectDEC - latitude;

    }
    //calculate GHA greenwitch hour angle
    double Algorithms::calculateGHA(double onjectRA,double greenwichSiderealTime){

        double greenwichHourDecimal = greenwichSiderealTime - onjectRA;

        double clippedHourAngle = int(greenwichHourDecimal);
        double clippedHourAngleMinute= ( greenwichHourDecimal-clippedHourAngle)*60; 

        //hour angle corrections
        if (clippedHourAngleMinute < 0){
            clippedHourAngle -= 1;
            clippedHourAngleMinute +=60;
        }
        if (clippedHourAngle < 0){
            clippedHourAngle +=24;
            }
        //return decimal value of Local Hour Angle
        return clippedHourAngle+clippedHourAngleMinute/60;
    }

    //calculate GMST greenwitch mean sidereal time
    TimeData Algorithms::calculateGMST(int year,int month,int day,int hour,int minute,int seconds,int centiseconds,bool summerTime, int GMT){

                //double LOT = 29.43055556  //gebze 29 25 50.45

        if (summerTime == true) hour -= 1;

        if (month < 3){
            month = month + 12;
            year = year - 1;
        };

        //total days
        double totalHourDays = hour + GMT + minute / 60.00 + ((double)seconds + (double)centiseconds/10 ) / 3600.00 ;
        double totalYearDays = (int)(365.25 * (year + 4716));
        double totalMontDays = (int)(30.6001 * (month + 1));

        double CurrentJDN = totalYearDays + totalMontDays + day - 1537.5 + (totalHourDays - 3) / 24;
        //double current_day = CurrentJDN - 2451543.5;
        double MJD = CurrentJDN - 2400000.5;
        int MJD0 = (int)(MJD);
        double ut = (MJD - MJD0) * 24.0;
        double t_eph = (MJD0 - 51544.5) / 36525.0;
        double GMST = 6.697374558 + 1.0027379093 * ut + (8640184.812866 + (0.093104 - 0.0000062 * t_eph) * t_eph) * t_eph / 3600.0;

        while (GMST>=24) GMST-=24;
        
        //return local sidereal time
        TimeData GMSTData;
        //usually rawtime is used. 
        GMSTData.rawtime = GMST;
        //Cropped values for representation only
        GMSTData.hour  = (int)(GMST);
        GMSTData.minute =(int) fmod(GMST*60,60);
        GMSTData.second = (int)fmod(GMST*3600,60);
        GMSTData.centiseconds = fmod(GMST*360000,60);
        
        return GMSTData;
    }

    //calculate current julian day
    double Algorithms::calculateJD(int year,int month,int day,int hour,int minute,int seconds,int centiseconds,bool summerTime, int GMT){

                //double LOT = 29.43055556  //gebze 29 25 50.45

        if (summerTime == true) hour -= 1;

        if (month < 3){
            month = month + 12;
            year = year - 1;
        };

        //total days
        double totalHourDays = hour + GMT + minute / 60.00 + ((double)seconds + (double)centiseconds/10 )/ 3600.00;
        double totalYearDays = (int)(365.25 * (year + 4716));
        double totalMontDays = (int)(30.6001 * (month + 1));

        double CurrentJDN = totalYearDays + totalMontDays + day - 1537.5 + (totalHourDays - 3) / 24;
        return  CurrentJDN - 2451543.5;

    }

    //calculate LMST local mean sidereal time
    TimeData Algorithms::calculateLMST(double longitude,double rawGMST){

        double LST = rawGMST + longitude / 15.0;

        while (LST>=24) LST-=24;
        
        //return local sidereal time
        TimeData LSTData;
        //usually rawtime is used. 
        LSTData.rawtime = LST;
        //Cropped values for representation only
        LSTData.hour  = (int)(LST);
        LSTData.minute =(int) fmod(LST*60,60);
        LSTData.second = (int)fmod(LST*3600,60);
        LSTData.centiseconds = fmod(LST*360000,60);

        
        return LSTData;
    }

        Point Algorithms::calculateParallelMotion(double X, double Y){

                    Point DD;
                    DD.x = X + Y;
                    //may be  DB = Y - X motion reversable
                    DD.y = X - Y;
                    return DD;
    }


//degree normalize between 0 -360 degree
double normalizeDegree(double degree){
    while (degree >= 360) degree -= 360;
    while (degree < 0) degree += 360;
    return degree;
}

//eccentricity iteration
double iterateAnomalyValue(double e ,double M, int iterateDepth){
    double E0 = M+ 180/pi*e*sin(M*pi/180)*(1 + e*cos(M*pi/180));

    for (int i = 0;i<iterateDepth;i++){
        double E1 = E0 -(E0-180/pi*e*sin(E0*pi/180)-M)/(1-e*cos(E0*pi/180));
        E0 = E1;
    }
    return E0;
}