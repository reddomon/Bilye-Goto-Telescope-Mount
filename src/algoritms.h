
#ifndef ALGORITMS_H  //definition check
#define ALGORITMS_H

#include "datatypes.h"

//parameters
//longitude latitude altitude
//localtime
//summertime
//RA and DEC of an object
class Algorithms{
private:




public:
    //constr
    Algorithms();

    //basic calculations for time
    double calculateLocalHA(double localSiderealTime,double onjectRA);
    double calculateLocalDEC(double latitude,double objectDEC);
    double calculateGHA(double onjectRA,double greenwichSiderealTime);
    TimeData calculateGMST(int year,int month,int day,int hour,int minute,int seconds,int centiseconds,bool summerTime, int GMT);
    TimeData calculateLMST(double lon,double rawGMST);
    //calculate current julian day
    double calculateJD(int year,int month,int day,int hour,int minute,int seconds,int centiseconds,bool summerTime, int GMT);
    //getters
    bool isObjectVisible(double localHA, double localDEC,double altitude);

    //planetspositions
    //planet.h


    Point calculateParallelMotion(double X, double Y);
};

//helpers
double normalizeDegree(double degree);
double iterateAnomalyValue(double e ,double M, int iterateDepth);

#endif