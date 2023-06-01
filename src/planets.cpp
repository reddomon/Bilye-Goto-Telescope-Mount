#include "planets.h"


/*
//obital elements of planets of the apes

//Mercury:
    N =  48.3313_deg + 3.24587E-5_deg   * d;    //(Long of asc. node)
    i =   7.0047_deg + 5.00E-8_deg      * d    //(Inclination)
    w =  29.1241_deg + 1.01444E-5_deg   * d    //(Argument of perihelion)
    a = 0.387098                              // (Semi-major axis)
    e = 0.205635     + 5.59E-10         * d    //(Eccentricity)
    M = 168.6562_deg + 4.0923344368_deg * d    //(Mean anonaly)
//Venus:
    N =  76.6799_deg + 2.46590E-5_deg   * d
    i =   3.3946_deg + 2.75E-8_deg      * d
    w =  54.8910_deg + 1.38374E-5_deg   * d
    a = 0.723330
    e = 0.006773     - 1.302E-9         * d
    M =  48.0052_deg + 1.6021302244_deg * d
//Mars:
    N =  49.5574_deg + 2.11081E-5_deg   * d
    i =   1.8497_deg - 1.78E-8_deg      * d
    w = 286.5016_deg + 2.92961E-5_deg   * d
    a = 1.523688
    e = 0.093405     + 2.516E-9         * d
    M =  18.6021_deg + 0.5240207766_deg * d
//Jupiter:
    N = 100.4542_deg + 2.76854E-5_deg   * d
    i =   1.3030_deg - 1.557E-7_deg     * d
    w = 273.8777_deg + 1.64505E-5_deg   * d
    a = 5.20256
    e = 0.048498     + 4.469E-9         * d
    M =  19.8950_deg + 0.0830853001_deg * d
//Saturn:
    N = 113.6634_deg + 2.38980E-5_deg   * d
    i =   2.4886_deg - 1.081E-7_deg     * d
    w = 339.3939_deg + 2.97661E-5_deg   * d
    a = 9.55475
    e = 0.055546     - 9.499E-9         * d
    M = 316.9670_deg + 0.0334442282_deg * d
//Uranus:
    N =  74.0005_deg + 1.3978E-5_deg    * d
    i =   0.7733_deg + 1.9E-8_deg       * d
    w =  96.6612_deg + 3.0565E-5_deg    * d
    a = 19.18171     - 1.55E-8          * d
    e = 0.047318     + 7.45E-9          * d
    M = 142.5905_deg + 0.011725806_deg  * d
//Neptune:
    N = 131.7806_deg + 3.0173E-5_deg    * d
    i =   1.7700_deg - 2.55E-7_deg      * d
    w = 272.8461_deg - 6.027E-6_deg     * d
    a = 30.05826     + 3.313E-8         * d
    e = 0.008606     + 2.15E-9          * d
    M = 260.2471_deg + 0.005995147_deg  * d
*/

OrbitalElements  Planet::getOrbitalElementsOf(int planet,double julianDate){

    OrbitalElements orbitalelements;

    switch(planet){
        case Sun:
            //orbital elements for sun
            orbitalelements.w = normalizeDegree(282.9404 + 4.70935E-5 * julianDate); //longitude of perihelion
            orbitalelements.a = 1.000000 ;   //mean distance
            orbitalelements.e = 0.016709 - 1.151E-9*julianDate; //eccentricity
            orbitalelements.M = normalizeDegree(356.0470 + 0.9856002585*julianDate); //mean anomaly
            orbitalelements.N = 0;
            orbitalelements.i = 0;

            break;
        case Moon:
            //orbital elements for moon (luna)
            orbitalelements.N = normalizeDegree(125.1228 -0.0529538083 * julianDate);
            orbitalelements.i = 5.1454;
            orbitalelements.w = normalizeDegree(318.0634 + 0.1643573223 *julianDate);
            orbitalelements.a = 60.2666;
            orbitalelements.e = 0.054900;
            orbitalelements.M = normalizeDegree(115.3654 + 13.0649929509 * julianDate);
            break;
        case Mercury:
            orbitalelements.N = 48.3313 + 3.24587E-5   * julianDate;  //Long of asc. node
            orbitalelements.i = 7.0047+ 5.00E-8      * julianDate;    //Inclination
            orbitalelements.w = 29.1241 + 1.01444E-5   * julianDate;  //Argument of perihelion
            orbitalelements.a = 0.387098;                               // Semi-major axis
            orbitalelements.e = 0.205635     + 5.59E-10  * julianDate; //Eccentricity
            orbitalelements.M = 168.6562 + 4.0923344368 * julianDate;  //Mean anonaly
            break;
        case Venus:
            orbitalelements.N = 76.6799 + 2.46590E-5   * julianDate;
            orbitalelements.i = 3.3946 + 2.75E-8      * julianDate;
            orbitalelements.w = 54.8910 + 1.38374E-5   * julianDate; 
            orbitalelements.a = 0.723330;
            orbitalelements.e = 0.006773     - 1.302E-9   * julianDate; 
            orbitalelements.M = 48.0052 + 1.6021302244 * julianDate; 
            break;
        case Mars:
            orbitalelements.N = normalizeDegree(49.5574 + 2.11081E-5   * julianDate);
            orbitalelements.i =normalizeDegree( 1.8497 - 1.78E-8 * julianDate);
            orbitalelements.w =normalizeDegree( 286.5016 + 2.92961E-5* julianDate); 
            orbitalelements.a = 1.523688;
            orbitalelements.e = normalizeDegree(0.093405     + 2.516E-9  * julianDate); 
            orbitalelements.M = normalizeDegree(18.6021 + 0.5240207766 * julianDate);
            break;
        case Jupiter:
            orbitalelements.N = 100.4542 + 2.76854E-5   * julianDate;
            orbitalelements.i = 1.3030 - 1.557E-7 * julianDate;
            orbitalelements.w= 273.8777 + 1.64505E-5* julianDate; 
            orbitalelements.a = 5.20256;
            orbitalelements.e = 0.048498     + 4.469E-9 * julianDate; 
            orbitalelements.M = 19.8950 + 0.0830853001 * julianDate;
            break;
        case Saturn:
            orbitalelements.N = 113.6634 + 2.38980E-5   * julianDate;
            orbitalelements.i = 2.4886 - 1.081E-7* julianDate;
            orbitalelements.w= 339.3939 + 2.97661E-5 * julianDate; 
            orbitalelements.a = 9.55475;
            orbitalelements.e = 0.055546     - 9.499E-9  * julianDate; 
            orbitalelements.M = 316.9670 + 0.0334442282 * julianDate;
            break;
        case Uranus:
            orbitalelements.N = 74.0005 + 1.3978E-5  * julianDate;
            orbitalelements.i = 0.7733 + 1.9E-8 * julianDate;
            orbitalelements.w= 96.6612 + 3.0565E-5 * julianDate; 
            orbitalelements.a =19.18171     - 1.55E-8;
            orbitalelements.e = 0.047318     + 7.45E-9  * julianDate; 
            orbitalelements.M = 142.5905 + 0.011725806 * julianDate;
            break;
        case Neptune:   
            orbitalelements.N = 131.7806 + 3.0173E-5  * julianDate;
            orbitalelements.i = 1.7700 - 2.55E-7 * julianDate;
            orbitalelements.w= 272.8461 - 6.027E-6  * julianDate; 
            orbitalelements.a =30.05826     + 3.313E-8 ;
            orbitalelements.e = 0.008606     + 2.15E-9  * julianDate; 
            orbitalelements.M = 260.2471 + 0.005995147 * julianDate;
            break;
        case Pluto:
            break;
        
        default:
            orbitalelements.N = 0;
            orbitalelements.i = 0;
            orbitalelements.w = 0; 
            orbitalelements.a = 0;
            orbitalelements.e = 0; 
            orbitalelements.M = 0; 
            break;

    }

//perps
Point meanAnom = calculateMeanAnomaly(planet,julianDate);
orbitalelements.dx = meanAnom.x;
orbitalelements.dy = meanAnom.y;

orbitalelements.oblecl  =  23.4393 - 3.563E-7 * julianDate;
return orbitalelements;

};

Point Planet::calculateMeanAnomaly(int planet,double julianDate){

        Point coords;

        double Mj = 19.8950 + 0.0830853001 * julianDate;  //Jupiters effects
        double Ms = 316.9670 + 0.0334442282 * julianDate; //saturns effects
        double Mu = 142.5905 + 0.011725806 * julianDate; //uranus's effects

        switch(planet){
            case Mercury:
                coords.x = 0;
                coords.y = 0;
                return coords;
                break;
            case Venus:
                coords.x = 0;
                coords.y = 0;
                return coords;
                break;
            case Mars:
                coords.x = 0;
                coords.y = 0;
                return coords;
                break;
            case Jupiter:{
            //perturbations
            double CorrectionJLon = -0.332 * sin(2*Mj - 5*Ms - 67.6*pi/180)
                 -0.056 * sin(2*Mj - 2*Ms + 21*pi/180) 
                 +0.042 * sin(3*Mj - 5*Ms + 21*pi/180)
                 -0.036 * sin(Mj - 2*Ms)
                 +0.022 * cos(Mj - Ms)
                 +0.023 * sin(2*Mj - 3*Ms + 52*pi/180)
                 -0.016 * sin(Mj - 5*Ms - 69*pi/180);

                 coords.x  = CorrectionJLon;
                 coords.y = 0;
                 return coords;
                break;}
            case Saturn:{
            //perturbations
                double CorrectionSLon =  +0.812 * sin(2*Mj - 5*Ms - 67.6*pi/180)
                -0.229 * cos(2*Mj - 4*Ms - 2*pi/180)
                +0.119 * sin(Mj - 2*Ms - 3*pi/180)
                +0.046 * sin(2*Mj - 6*Ms - 69*pi/180)
                +0.014 * sin(Mj - 3*Ms + 32*pi/180);
                double CorrectionSLat = -0.020 *cos(2*Mj - 4*Ms - 2*pi/180)
                    +0.018 * sin(2*Mj - 6*Ms - 49*pi/180);

                coords.x = CorrectionSLon ;
                coords.y = CorrectionSLat;
                return coords;
                break;}
            case Uranus:{
                double CorrectionULon =     +0.040 * sin(Ms - 2*Mu + 6)
                    +0.035 * sin(Ms - 3*Mu + 33)
                    -0.015 * sin(Mj - Mu + 20);
                coords.x = CorrectionULon ;
                coords.y = 0;
                
                break;}


        }

};

//MOON
Point Planet::calculateMoonCoords(OrbitalElements elements){
        //eccentricity
        double E = normalizeDegree(iterateAnomalyValue(elements.e,elements.M,3));

        //compute the Moon's distance and true anomaly
        double x = elements.a * ( cos(E*pi/180) - elements.e);
        double y = elements.a* sqrt(1-elements.e*elements.e)* sin(E*pi/180);

        //Serial.println(x,16);
        //Serial.println(y,16);

        double r = sqrt(x*x+y*y);
        double v = atan2(y,x);
        //convert to degrees
        v = v/pi*180;

        double xeclip = r*( cos(elements.N*pi/180) * cos((v+elements.w)*pi/180) - sin(elements.N*pi/180)*sin((v+elements.w)*pi/180)*cos(elements.i*pi/180));
        double yeclip = r*( sin(elements.N*pi/180) * cos((v+elements.w)*pi/180) + cos(elements.N*pi/180)*sin((v+elements.w)*pi/180)*cos(elements.i*pi/180));
        double zeclip = r*sin((v+elements.w)*pi/180)*sin(elements.i*pi/180);

        //double lon = atan2(yeclip,xeclip);
        //double lat = atan2(zeclip,sqrt(xeclip*xeclip + yeclip*yeclip ));
        //double r = sqrt( xeclip*xeclip + yeclip*yeclip + zeclip*zeclip );
        //Serial.println(xeclip,16);
        //Serial.println(yeclip,16);
        //Serial.println(zeclip,16);
        //Serial.println("-------------");

        double xequat = xeclip;
        double yequat = yeclip * cos(elements.oblecl*pi/180) - zeclip * sin(elements.oblecl*pi/180);
        double zequat = yeclip * sin(elements.oblecl*pi/180) + zeclip * cos(elements.oblecl*pi/180);

        //Serial.println(xequat,16);
        //Serial.println(yequat,16);
        //Serial.println(zequat,16);
        Point RADEC;
        RADEC.x   = atan2(yequat, xequat ); //degree to HA
        RADEC.y = atan2(zequat, sqrt( xequat*xequat + yequat*yequat) );

        return RADEC;
        

};

//SUN's raw corrds
Point Planet::calculateSunCoords(OrbitalElements elements){

        //Sun's mean longitude
        double L = elements.w + elements.M;
        L = normalizeDegree(L);
        double E = elements.M + (180/pi) * elements.e * sin(elements.M*pi/180) * (1 + elements.e * cos(elements.M*pi/180));

        double x = cos(E*pi/180) - elements.e;
        double y = sin(E*pi/180) * sqrt(1 - elements.e*elements.e);

        double r = sqrt(x*x+y*y);
        double v = atan2(y,x);
        double lon =normalizeDegree( v*180/pi+elements.w);

        //eclipticrectangular corrds
        
        x = r*cos(lon*pi/180);
        y = r*sin(lon*pi/180);
        //Serial.println(x,16);
        //Serial.println(y,16);
        double z = 0;

        double xequat = x;
        double yequat = y *cos(23.4406*pi/180) - z*sin(23.4406*pi/180);
        double zequat = y *sin(23.4406*pi/180) + z*cos(23.4406*pi/180);
        //Serial.println(xequat,16);
        //Serial.println(yequat,16);
        //Serial.println(zequat,16);
        double ux = xequat;
        double uy = yequat;
        double uz = zequat;
        r = sqrt(ux*ux+uy*uy+uz*uz);
        Point RADEC;
        RADEC.x = atan2(uy,ux); //degree to HA
        RADEC.y = atan2(uz , sqrt(ux*ux+uy*uy) );

        return RADEC;
           
}

//SUN's eq coords. Required for offsetting planets orbits from heiocentric to geocentric
Point Planet::calculateSunEqCoords(OrbitalElements elements){

        //Sun's mean longitude
        double L = elements.w + elements.M;
        double E = elements.M + (180/pi) * elements.e * sin(elements.M*pi/180) * (1 + elements.e * cos(elements.M*pi/180));

        double x = cos(E*pi/180) - elements.e;
        double y = sin(E*pi/180) * sqrt(1 - elements.e*elements.e);

        double r = sqrt(x*x+y*y);
        double v = atan2(y,x);
        double lon =normalizeDegree( v*180/pi+elements.w);

        //ecliptic rectangular coordinates
        x = r*cos(lon*pi/180);
        y = r*sin(lon*pi/180);
        double z = 0;  //not used on equatorial corrdinates

        Point EQ;
        EQ.x = x;
        EQ.y = y;


        return EQ;
         

}

Point Planet::calculatePlanetCoords(OrbitalElements elements,Point sunEqPos){

                //eccentricity
        double E = normalizeDegree(iterateAnomalyValue(elements.e,elements.M,3));
        //Serial.println(E);
        //compute the Moon's distance and true anomaly
        double x = elements.a * ( cos(E*pi/180) - elements.e);
        double y = elements.a* sqrt(1-elements.e*elements.e)* sin(E*pi/180);
        //Serial.println(x);
        //Serial.println(y);
        double r = sqrt(x*x+y*y);
        double v = atan2(y,x);
        //convert to degrees
        v = v/pi*180;

        double xeclip = r*( cos(elements.N*pi/180) * cos((v+elements.w)*pi/180) - sin(elements.N*pi/180)*sin((v+elements.w)*pi/180)*cos(elements.i*pi/180));
        double yeclip = r*( sin(elements.N*pi/180) * cos((v+elements.w)*pi/180) + cos(elements.N*pi/180)*sin((v+elements.w)*pi/180)*cos(elements.i*pi/180));
        double zeclip = r*sin((v+elements.w)*pi/180)*sin(elements.i*pi/180);

        //Serial.println(xeclip);
        //Serial.println(yeclip);
        //Serial.println(zeclip);

        //double lon = atan2(yeclip,xeclip);
        //double lat = atan2(zeclip,sqrt(xeclip*xeclip + yeclip*yeclip ));
        //double r = sqrt( xeclip*xeclip + yeclip*yeclip + zeclip*zeclip );

        //offset heliocentric to geocentric coordinates
        xeclip = xeclip + sunEqPos.x;
        yeclip = yeclip + sunEqPos.y;

        //Serial.println(xeclip);
        //Serial.println(yeclip);

        double xequat = xeclip;
        double yequat = yeclip * cos(elements.oblecl*pi/180) - zeclip * sin(elements.oblecl*pi/180);
        double zequat = yeclip * sin(elements.oblecl*pi/180) + zeclip * cos(elements.oblecl*pi/180);
        Point RADEC;
        RADEC.x = atan2(yequat, xequat ); //degree to HA
        RADEC.y = atan2(zequat, sqrt( xequat*xequat + yequat*yequat) );
        
        RADEC.x = normalizeDegree(RADEC.x/pi*180);
        RADEC.x = RADEC.x*pi/180;
        return RADEC;


}