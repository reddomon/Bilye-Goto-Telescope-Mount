#include <datatypes.h>


String MountStatus::getGMSTString(){

        String time="";
        time+=String(GMST.hour);
        time+=":";
        time+=String(GMST.minute);
        time+=":";
        time+=String(GMST.second);

        return time;


    };

String MountStatus::getLMSTString(){

        String time="";
        time+=String(LMST.hour);
        time+=":";
        time+=String(LMST.minute);
        time+=":";
        time+=String(LMST.second);

        return time;

    };

String MountStatus::getGMTString(){
        String time="";
        time+=String(currentHour);
        time+=":";
        time+=String(currentMinute);
        time+=":";
        time+=String(currentSecond);

        return time;

    };

String MountStatus::getLTString(){
        String time="";
        time+=String(LT.hour);
        time+=":";
        time+=String(LT.minute);
        time+=":";
        time+=String(LT.second);

        return time;


    };
String MountStatus::getDATEString(){

        String time="";
        time+=String(LT.year);
        time+="/";
        time+=String(LT.month);
        time+="/";
        time+=String(LT.day);

        return time;

    };

String MountStatus::getLATString(){

        String loc="";
        loc+=String(currentLat,9);
        return loc;

    };
String MountStatus::getLONString(){

        String loc="";
        loc+=String(currentLong,9);
        return loc;

    };
String MountStatus::getALTString(){

        String loc="";
        loc+=String(currentAlt);
        loc+=" meters";
        return loc;

    };

    String ObservingObject::getObjectRAString(){
            String ra;

            int raH = (int)objRA;
            int raM = int((objRA-raH)*60);
            int raS = int((((objRA-raH)*60 -raM))*60);
            ra+=String(raH)+"h"+String(raM)+"m"+String(raS)+"s";
            return ra;


    };

    String ObservingObject::getObjectDECString(){
            String dec;
            int decH = (int)objDEC;
            int decM = int((objDEC-decH)*60);
            int decS = int((((objDEC-decH)*60-decM))*60);
            dec+=String(decH)+"d"+String(decM)+"m"+String(decS)+"s";
            return dec;

    };

    String ObservingObject::getObjectLHAString(){
            String dec;
            int decH = (int)LHA;
            int decM = int((LHA-decH)*60);
            int decS = int(((LHA-decH)*60-decM)*60);
            dec+=String(decH)+"h"+String(decM)+"m"+String(decS)+"s";
            return dec;

    };

    String ObservingObject::getObjectLDECString(){

            String dec;
            int decH = (int)LDEC;
            int decM = int((LDEC-decH)*60);
            int decS = int(((LDEC-decH)*60-decM)*60);
            dec+=String(decH)+"d"+String(decM)+"m"+String(decS)+"s";
            return dec;

    };

