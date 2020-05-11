#ifndef PLANETS_H  //definition check
#define PLANETS_H

#include <Arduino.h>
#include "datatypes.h"
#include "algoritms.h"




class Planet{

private:
   //better assumption for bigger planets, Ju, Sa and Ur
   Point calculateMeanAnomaly(int planetNumber,double julianDate);


public:
   //stored elements of planets
   OrbitalElements  getOrbitalElementsOf(int planetNumber,double julianDate);
   //calculate ra and dec x is ra and y is decl
   Point calculateMoonCoords(OrbitalElements elements);
   Point calculateSunCoords(OrbitalElements elements);
   //required for coordinate ofsetting
   Point calculateSunEqCoords(OrbitalElements elements);
   Point calculatePlanetCoords(OrbitalElements elements,Point sunPos);

   //helper functions

};



#endif