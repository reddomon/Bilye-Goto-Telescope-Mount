//main file for Bilye Telescop Mount Controller
//Mayıs 2019 korona günlerinde
//May 2019 in covid-19 days

//this controller consists of 5 module
//main module, parser module, ui module, drive module, calculator module

#include <Arduino.h>
#include <vector> //for csv
#include <TinyGPS++.h>
#include <Ticker.h>
#include <SD.h>
#include <Update.h>
#include <time.h>
#include <sys/time.h>

#include "FS.h"
#include "SD.h"
#include <SPI.h>
//bluetooth
#include "BluetoothSerial.h"

//matrix calculations in alignment options
//not implemented yet
#include <Matrix.h>

//for getting mcu info like CPU frequency
#include "esp32-hal-cpu.h"  

#include "AccelStepper.h"
#include "queueArray.h"
#include "stars.h"
#include "algoritms.h"
#include "planets.h"
#include "wifi/wifiesp.h"
#include "wifi/web.h"
#include "database/database.h"


//for user interface 
#include "userinterfaceapi.h"

//date time class
#include "ESPDateTime.h"

//configurations
#include "config.h"

//#include "timerone/TimerOne.h"

//we have to declare global class for data acquisition
//current RA, current DEC etc
//it simplifies plugin development for "Bilye"



//mechanical error ignored for calculation like backlash etc
double RA_MAX_STEPS = RA_STEP*RA_DRIVER_USTEP*RA_GEAR_RATIO;
double DEC_MAX_STEPS = DEC_STEP*DEC_DRIVER_USTEP*DEC_GEAR_RATIO;

double RA_RESOLUTION = RA_REV/RA_MAX_STEPS;
double DEC_RESOLUTION  = DEC_REV/DEC_MAX_STEPS;

//current values 
double currentDECvalue=0,currentRAvalue=0;
double gotoDECvalue=0,gotoRAvalue=0;
double errorDECvalue=0,errorRAvalue=0;


//current values, info values
double currRA_H, currRA_M, currRA_S, currDEC_D, currDEC_M, currDEC_S; 

//timer clocks for biggest axis from rduionscope is not used
int Clock_Sidereal = 500000/(DEC_MAX_STEPS/RA_REV);
int Clock_Solar = 500000/(DEC_MAX_STEPS/(RA_REV-235.9095));
int Clock_Lunar = 500000/(DEC_MAX_STEPS/(RA_REV-2089.2292));

//mount status class
MountStatus mountstatus;

//observing objectproperties
ObservingObject observingobject;

//centisecond time

//bluetooth object
BluetoothSerial SerialBT;

//timer
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;




//AccelStepper Setup
//dont forget to call setEnable(PIN)
AccelStepper stepperRA(AccelStepper::DRIVER, RA_STEP_PIN, RA_DIR_PIN); // 1 = Easy Driver interface
AccelStepper stepperDEC(AccelStepper::DRIVER, DEC_STEP_PIN, DEC_DIR_PIN);
AccelStepper stepperFOCUS(AccelStepper::DRIVER, FOCUS_STEP_PIN, FOCUS_DIR_PIN);



//Stepper Travel Variables
long TravelRA, TravelDEC, TravelFOCUS;
int sofar = 0; 
int sofarBT = 0; 

int move_finished = 1;
long initial_homing_RA = -1;
long initial_homing_DEC = -1;
long initial_homing_FOCUS = -1;

//eksen degiskenleri
long axisRA,axisDEC,axisFOCUS;

int period = 250;
unsigned long time_now = 0;
int HMIsatus = 0;

//serial command buffer
QueueArray arrayBuffer;

//database
sqlite3 *starsDb;


char buffer[MAX_BUF]; // where we store the message until we get a newline
char buffer2[MAX_BUF]; // where we store the message until we get a newline
char bufferBT[MAX_BUF]; // where we store the message until we get a newline

//gps variable
// The TinyGPS++ object
TinyGPSPlus gps;
//calculations
Algorithms algorithms = Algorithms();


//stepper  runner
void runner();


//simple csv lib from https://github.com/gsb/CSV2Strings thanks to gsb
//Method: CSV String to Array [vector] of String Values.
std::vector<String> csv2list(String csv, char* seperator )
{
  std::vector<String> _list; // Create a new vector to work with.
  char tmp[csv.length()];    // Create char buffer for the csv char[].
  strcpy(tmp, csv.c_str());  //...copy the csv string into buffer.

  // Tokenize buffered char[] pushing each as Strings onto vector.
  char *ptr = strtok(tmp, seperator);
  while (ptr != NULL) {
    _list.push_back( String(ptr) );
    ptr = strtok(NULL, seperator);
  }

  return _list; // Return the working vector.
}

volatile int interruptCounter;
int totalInterruptCounter;


//timer interrupt
//timer kesmesi. bu kesme ana kesmedirve 10hz frekansına ayarlanmıştır.saniyede 10 defa yörünge hesaplaması yapılırbu sayede gezegen izlemi yumuşak birşekilde yapılabilir
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}

// i dont have any idea what isgoing on here just copy and modify some part of it
// gettin ms

int64_t xx_time_get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
  //return thousend's of miliseconds for tenth of a second
	return ( (tv.tv_usec / 100000LL));
}

void printLocalTime()
{
  
  Serial.println(DateTime.now());
  Serial.println(DateTime.getTime());
  Serial.println(DateTime.utcTime());
  Serial.println("--------------------");
  Serial.println(DateTime.toString());
  Serial.println(DateTime.toISOString());
  Serial.println(DateTime.toUTCString());
  Serial.println("--------------------");
   
}


//calculate RA steps error correction not required. previous error added next value so error is not getting bigger
double calculateRA_Steps(double currentRA, double gotoRA){

  return (gotoRA-currentRA)/RA_RESOLUTION;

}
//calculate DEC steps error correction not required. previous error added next value so error is not getting bigger
double calculateDEC_Steps(double currentDEC, double gotoDEC){

  return (gotoDEC-currentDEC)/DEC_RESOLUTION;

}

//string integer number parser
//for example if we feed "G" and "G25\0" via serial, fnc parses "25" as ant return value
//for example if we feed "X" and "G25 X14\0" via serial, fnc parses "14" as ant return value
int parseNumberInt(char karakter, char *Line)
{

  int donguPointer = 0;
  char *floatLinePointer = 0;
  char floatLine[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; //Sayılar en fazla 9 karakter uzunluğunda olabilir. Daha fazlası için float tipinden vazgeçilmeli
  //HID_write("getinteger\r\n",64);

  if (( floatLinePointer = strchr(Line, karakter)) )

  {
    floatLinePointer++;
    do
    {

      //32 = SPC  bşluk karakteri ile eşdeğerdir. Boşluk karakteri bulana kadar okuma yap.
      floatLine[donguPointer] = *floatLinePointer; //tersten okunması gereken bir değerdir.
      donguPointer++;
      floatLinePointer++;
      //for end
      if (donguPointer > 8)
      {
        return 10000;
      }
      if (*floatLinePointer == 32)  //terminator space
        break;
      if (*floatLinePointer == '\n') //terminator
        break;
      if (*floatLinePointer == '\0') //terminator
        break;
    } while (1);

    return atoi(floatLine);
  } //if end
  else
  {

    return 32000;
  }
} //function end

//girdi olarak txt ve harf alır çıktı olarak ise harften sonra boşluğa kadar ki kısmı int olarak geri döner.
double parseNumber(char karakter, char *Line)
{

  int donguPointer = 0;
  char *floatLinePointer = 0;
  char floatLine[33] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //Sayılar en fazla 9 karakter uzunluğunda olabilir. Daha fazlası için float tipinden vazgeçilmeli
  if ( (floatLinePointer = strchr(Line, karakter)) )
  {
    //UART1_write_text(floatLinePointer);
    floatLinePointer++;
    do
    {
      //32 = SPC  bşluk karakteri ile eşdeğerdir. Boşluk karakteri yana newline bulana kadar okuma yap.
      floatLine[donguPointer] = *floatLinePointer;
      donguPointer++;
      floatLinePointer++;
      if (donguPointer > 32)
      {
        return 100000000.0;
      }
      if (*floatLinePointer == 32) //terminator space
        break;
      if (*floatLinePointer == '\n') //terminator
        break;
      if (*floatLinePointer == '\0') //terminator
        break;

    } //for end
    while (1);
    return atof(floatLine);
  } //if end
  else
  {
    return 100000000.0;
  } //10 metreye kadar kod yollama olduğunda
}



//gcode parser. implemented for csv parser
void processCommand(char *line)
{
  int cmd = parseNumberInt('G', line);
  //echo command back to serial
  Serial.print("Echo: ");
  Serial.print(line);
  switch (cmd)
  {
  case 0: //free movement
    if(parseNumber('R',line) < 100000000.0){
        //stepperRA.moveTo(parseNumber('R',line));
        observingobject.objRA = parseNumber('R',line);
        mountstatus.mountTrackingType = tracking;
    }  //x te ne kadar gideceği
    if(parseNumber('D',line) < 100000000.0){
        //stepperDEC.moveTo(parseNumber('D',line));
        observingobject.objDEC = parseNumber('D',line);
        mountstatus.mountTrackingType = tracking;
    }  //x te ne kadar gideceği
    if(parseNumber('F',line) < 100000000.0){
        stepperFOCUS.moveTo(parseNumber('F',line));
    }  //x te ne kadar gideceği
    break;

  case 2: //relative movement
      if(parseNumber('R',line) < 100000000.0){
        //stepperRA.moveTo(parseNumber('R',line));
        observingobject.objRA += parseNumber('R',line);
        mountstatus.mountTrackingType = tracking;
      }  //x te ne kadar gideceği
      if(parseNumber('D',line) < 100000000.0){
        //stepperDEC.moveTo(parseNumber('D',line));
        observingobject.objDEC += parseNumber('D',line);
        mountstatus.mountTrackingType = tracking;
      }  //x te ne kadar gideceği
      if(parseNumber('F',line) < 100000000.0){
        double numps = parseNumber('F',line);
        stepperFOCUS.moveTo(parseNumber('F',line));
      }  //x te ne kadar gideceği
       break;
  case 10:
    //return info via serial0
    Serial.print("RAstep   :");
    Serial.println(String(stepperRA.currentPosition()).c_str());
    Serial.print("RA       :");
    Serial.println(String(currentRAvalue=(double)stepperRA.currentPosition()*RA_RESOLUTION,8).c_str());
    Serial.print("DECstep  :");
    Serial.println(String(stepperDEC.currentPosition()).c_str());
    Serial.print("DEC      :");
    Serial.println(String(currentDECvalue=(double)stepperDEC.currentPosition()*DEC_RESOLUTION,8).c_str());
    Serial.print("FOCUSstep:");
    Serial.println(String(stepperFOCUS.currentPosition()).c_str());
    Serial.print("RAres    :");
    Serial.println(String(RA_RESOLUTION,8).c_str());
    Serial.print("DECres   :");
    Serial.println(String(DEC_RESOLUTION,8).c_str());
    Serial.print("GotoDec  :");
    Serial.println(String(gotoDECvalue,8).c_str());
    Serial.print("GotoRa   :");
    Serial.println(String(gotoRAvalue,8).c_str());
    break;
  case 28:

    break;
  case 29:

    break;
  case 30: 
    stepperRA.moveTo((long)calculateRA_Steps(0,1.57));
    stepperDEC.moveTo((long)calculateDEC_Steps(0,2));
    break;
  case 100: 
    {

    //select observing object from internal list named stars.h file
    std::vector<String> mainlist = csv2list(Stars[parseNumberInt('S', line)],";");
    //split list according to m and h letters. example "19h6m" splitted 19 and 6
    std::vector<String> splittedlist = csv2list(mainlist.at(2),"hm");
    //Serial.println(std::atof(sfr2.at(0).c_str())+std::atof(sfr2.at(1).c_str())/60,8);

    observingobject.sign = mainlist.at(0);
    observingobject.name = mainlist.at(1);
    //delimeter h and m
    observingobject.objRA = std::atof(splittedlist.at(0).c_str()) + std::atof(splittedlist.at(1).c_str())/60;
    observingobject.objDEC = std::atof(mainlist.at(3).c_str());


    break;
    }
case 101: 
    {
    //goto observing object
    gotoRAvalue = observingobject.objRA;
    gotoDECvalue = observingobject.objDEC;
    observingobject.code = Stellar;
    mountstatus.mountTrackingType = tracking;
    break;
    }
  //-----------------------------
  case 200:
    observingobject.code = parseNumberInt('S', line);

    break;
 case 201: 

 case 300:{
      String devDv = "SELECT ROWID,name,ra,dec FROM dso WHERE ROWID>" +String(parseNumberInt('S', line))+ " LIMIT 10";  
      db_exec(starsDb,devDv.c_str());  

      break;}
  default:
      Serial.println("Echo: Unsupported Code Format"); //errous code
    break;
  }
}



void setMotors(){

  //Timer1.stop();
  //RA axis
  stepperRA.enableOutputs();
  stepperRA.setCurrentPosition(0);
  stepperRA.setMaxSpeed(5000);     // Set Max Speed of Stepper (Faster for regular movements)
  stepperRA.setAcceleration(10000.0); // Set Acceleration of Stepper

  //DEC axis
  stepperDEC.enableOutputs();
  stepperDEC.setCurrentPosition(0);
  stepperDEC.setMaxSpeed(5000);     // Set Max Speed of Stepper (Faster for regular movements)
  stepperDEC.setAcceleration(10000.0); // Set Acceleration of Stepper

  //FOCUS axis
  stepperFOCUS.enableOutputs();
  stepperFOCUS.setCurrentPosition(0);
  stepperFOCUS.setMaxSpeed(5000);     // Set Max Speed of Stepper (Faster for regular movements)
  stepperFOCUS.setAcceleration(10000.0); // Set Acceleration of Stepper

  //Timer1.start();
}
//gather data from gps if valid
void gatherGPSData()
{
  
  if (gps.location.isValid())
  {
    mountstatus.currentLat=gps.location.lat();
    mountstatus.currentLong=gps.location.lng();
    mountstatus.currentAlt=gps.altitude.meters();
  }
  else
  {
    //Serial.print(F("INVALID"));
  }
  if (gps.date.isValid())
  {
    mountstatus.currentMonth = gps.date.month();
    mountstatus.currentDay = gps.date.day();
    mountstatus.currentYear = gps.date.year();
    
  }
  else
  {
    //Serial.print(F("INVALID"));
  }

  if (gps.time.isValid())
  {
    mountstatus.currentHour = gps.time.hour();
    mountstatus.currentMinute = gps.time.minute();
    mountstatus.currentSecond = gps.time.second();
    mountstatus.currentCentiSecond = gps.time.centisecond();

    mountstatus.GT.rawtime = (double)mountstatus.currentHour + (double)mountstatus.currentMinute/60 +(double)mountstatus.currentSecond/3600;

    //set current time via gps rtc
    DateTime.setTime(gps.time.value()/100);

    //set timezone
    DateTime.setTimeZone(mountstatus.GMT);

    //get localtime parts
    DateTimeParts DTP = DateTime.getParts();
    //set localtime
    mountstatus.LT.hour = 12;
    mountstatus.LT.minute = DTP.getMinutes();
    mountstatus.LT.second = DTP.getSeconds();

    
  }
  else
  {
    //Serial.println(F("INVALID"));
  }

}
  //setup function
void setup()
{
  //delay(5000);
  Serial.begin(115200); //konsol için gerekli pin ataması yapılmalı
  Serial1.begin(9600,134217756U,22,21); //Gps için gerekli  pin ataması yapılmalı
  Serial2.begin(57600); //hmı için gerekli  pin ataması yapılmalı
  
  //cpu frequency
  Serial.println(getCpuFrequencyMhz());

  //anahtar çıkışlarının durumunu ayarla
  //pinMode(RA_MIN_PIN, INPUT_PULLUP);
  //pinMode(DEC_MIN_PIN, INPUT_PULLUP);
  //pinMode(FOCUS_MIN_PIN, INPUT_PULLUP);

  //çıkışları ayarla
  pinMode(OUTPUT0,OUTPUT);
  pinMode(OUTPUT1,OUTPUT);
  pinMode(OUTPUT2,OUTPUT);
  pinMode(OUTPUT3,OUTPUT);

  //cikslari kapat
  digitalWrite(OUTPUT0,0);
  digitalWrite(OUTPUT1,0);
  digitalWrite(OUTPUT2,0);
  digitalWrite(OUTPUT3,0);

  stepperRA.setEnablePin(RA_ENABLE_PIN);
  stepperDEC.setEnablePin(RA_ENABLE_PIN);
  stepperFOCUS.setEnablePin(RA_ENABLE_PIN);

  //PL mode settings
  stepperRA.setPinsInverted(true,true,true);
  stepperDEC.setPinsInverted(false,true,true);
  stepperFOCUS.setPinsInverted(false,true,true);

  //circular buffer init
  arrayBuffer.init();

  //Timer1.initialize(10000);                  // Initialize timer 1
  //Timer1.stop();
  //Timer1.attachInterrupt(runner); 
  //delay(2); // Wait for driver wakeup

  time_now = millis();

  //timer setup
  //APB frequency max 80 mhz for peripheral data gathering clock
  //motor driving interrupt
  //configured by Algorithms::setTrackType() function
  //because solar tracking, planetary tracking and sidereal tracking different from each other
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  //10 hz interrupt
  timerAlarmWrite(timer, 100000, true);
  timerAlarmEnable(timer);


  //Timer1.stop();

  //setup motor settings
  setMotors();
  delay(200);
  //Timer1.start();
  
  //sdcard
  // Initialize SD card
  if(!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return;
  }
  else Serial.println("Card Mount Successed");

  //wifi init
  wifi_init();
  //web init
  web_init();

  //bluetooth
  //bleSetup();
  //SerialBT.begin("BilyeMountController");



  //reading csv file
  //std::vector<String> sfr = csv2list(Stars[1]);

  //database
  sqlite3_initialize();
  if (openDb("/sd/stars.db", &starsDb)){

      Serial.println("Echo: Database open error");
  }


   

  mountstatus.mountTrackingType = stopped;
  //ui on startup
  onStartup();

}
//ten'th of a second
int currentCentiSecond = 0;

void loop()
{
  
  // listen for serial commands
  while (Serial.available() > 0)
  {                         // if something is available
    char c = Serial.read(); // get it
    Serial.print(c);        // repeat it back so I know you got the message
    if (sofar < MAX_BUF - 1)
      buffer[sofar++] = c; // store it
    if ((c == '\n') || (c == '\r')|| ( c==255))
    {
      // entire message received
      buffer[sofar] = 0; // end the buffer so string functions work right
      //Serial.println(buffer[0]);  // echo a return character for humans
      //processCommand(buffer); // do something with the command
      arrayBuffer.push(buffer);
      sofar = 0; 
    }
  }
  /* //define in nextion_ui.cpp
  //listen for serial 2 commands .. Nextion screen
  while (Serial2.available() > 0)
  {                         // if something is available
    char c = Serial2.read(); // get it
    //Serial2.print(c);        // repeat it back so I know you got the message
    if (sofar < MAX_BUF - 1)
      buffer2[sofar++] = c; // store it
    if ((c == '\n') || (c == '\r') || ( c==255))
    {
      // entire message received
      buffer2[sofar] = 0; // end the buffer so string functions work right
      //Serial.println(buffer[0]);  // echo a return character for humans
      //processCommand(buffer); // do something with the command
      //arrayBuffer.push(buffer);
      
      sofar = 0; 
    }
  }
  */

  //GPS data acquisition
    while (Serial1.available() > 0) // if something is available
  {     
        //read and encode serial GPS data
        //baudrate 115200 you have to set baudrate for getting corect results                 
        if (gps.encode(Serial1.read())) 
        { gatherGPSData();
          currentCentiSecond = 0;
        };
        
        

  }

  //bluetooth data acquisition
  while(SerialBT.available()){

    char c = SerialBT.read(); // get it
    if (sofarBT < MAX_BUF - 1)
      bufferBT[sofarBT++] = c; // store it
    if ((c == '\n') || (c == '\r')|| ( c==255))
    {
      // entire message received
      bufferBT[sofarBT] = 0; // end the buffer so string functions work right
      //push into prog buffer
      arrayBuffer.push(bufferBT);
      sofarBT = 0; 
    }

     // Serial.print(String((char)SerialBT.read()).c_str());


   }

//belirlenen zamanda bir veri durumu değiştir
      while(millis() >time_now + period){
        time_now = millis();
        //gidilecek adım var mı bak
        if(stepperRA.distanceToGo() || stepperDEC.distanceToGo() || stepperFOCUS.distanceToGo()){
            //eğer takip açıksa sürekli kontrol halinde olacak
  
        }
        //yoksa komuta uygun hale getir
        else{

        }


    }

//&& !stepperRA.distanceToGo() &&  !stepperDEC.distanceToGo() &&  !stepperFOCUS.distanceToGo()
if(!arrayBuffer.isEmpty() ){
  //eksen hareketleri bittğinde yeni bir komut çek
  processCommand((char*)arrayBuffer.pop());

}
else{ //buffer is empty then fill it

  //arrayBuffer.push("G0 X2000 Y2000\n");
  //arrayBuffer.push("G0 X4000 Y0\n");
  //arrayBuffer.push("G0 X2000 Y2000\n");
  //arrayBuffer.push("G0 X0 Y0\n");

}

    //sidereal,lunar or solar clock refresh interrupt
    //Burada RA ve DEC değerleri hesaplanır ve step motorların bu koordinatlara doğru hareket etmesi sağlanır
    //10 hz interrupt
if (interruptCounter > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
    digitalWrite(OUTPUT0,!digitalRead(OUTPUT0));
    
    //gebze lng 29.43068056
    //calculate gmst
     mountstatus.GMST = algorithms.calculateGMST(mountstatus.currentYear,
                                              mountstatus.currentMonth,
                                              mountstatus.currentDay,
                                              mountstatus.currentHour,
                                              mountstatus.currentMinute,
                                              mountstatus.currentSecond,
                                              mountstatus.currentCentiSecond,
                                              mountstatus.summerTime,
                                              mountstatus.GMT);
    //gps longitude
    //calculate lmst
    mountstatus.LMST = algorithms.calculateLMST(mountstatus.currentLong,mountstatus.GMST.rawtime);
    double Julian = algorithms.calculateJD(mountstatus.currentYear,
                                              mountstatus.currentMonth,
                                              mountstatus.currentDay,
                                              mountstatus.currentHour,
                                              mountstatus.currentMinute,
                                              mountstatus.currentSecond,
                                              mountstatus.currentCentiSecond,
                                              mountstatus.summerTime,
                                              mountstatus.GMT);



    //is mount tracking then calculate the required RA and Dec 
    if(mountstatus.mountTrackingType)
    {
      //stepperDEC.moveTo(bla);
      //stepperRA.moveTo(bla);
      //totalInterruptCounter++;
      //Serial.print("An interrupt as occurred. Total number: ");
      //Serial.println(totalInterruptCounter);
      //Serial.println(String(currentRAvalue,6).c_str());
      //Serial.println(String(currentDECvalue,6).c_str());

      



      //LHA and Local Dec calculations
      Planet planet;
      //double localDec;
      //double localHourAngle;
      Point poi;
      switch(observingobject.code){

        case Sun:{
          poi = planet.calculateSunCoords(planet.getOrbitalElementsOf(Sun,Julian));
          observingobject.LHA = algorithms.calculateLocalHA(mountstatus.LMST.rawtime,poi.x/pi*12);
          observingobject.LDEC = algorithms.calculateLocalDEC(mountstatus.currentLat,poi.y/pi*180);
          observingobject.objRA = poi.x/pi*12;
          observingobject.objDEC = poi.y/pi*180;
          observingobject.name = "SUN";
          break;}
        case Moon:{
          poi = planet.calculateMoonCoords(planet.getOrbitalElementsOf(Moon,Julian));
          observingobject.LHA = algorithms.calculateLocalHA(mountstatus.LMST.rawtime,poi.x/pi*12);
          observingobject.LDEC = algorithms.calculateLocalDEC(mountstatus.currentLat,poi.y/pi*180);
          observingobject.objRA = poi.x/pi*12;
          observingobject.objDEC = poi.y/pi*180;
          observingobject.name = "Moon";
          break;}
        case Mercury:
          poi = planet.calculatePlanetCoords( planet.getOrbitalElementsOf(Mercury,Julian), planet.calculateSunEqCoords( planet.getOrbitalElementsOf(Sun,Julian) ) );
          observingobject.LHA = algorithms.calculateLocalHA(mountstatus.LMST.rawtime,poi.x/pi*12);
          observingobject.LDEC = algorithms.calculateLocalDEC(mountstatus.currentLat,poi.y/pi*180);
          observingobject.objRA = poi.x/pi*12;
          observingobject.objDEC = poi.y/pi*180;
          observingobject.name = "Mercury";
          break;
        case Venus:
          poi = planet.calculatePlanetCoords( planet.getOrbitalElementsOf(Venus,Julian), planet.calculateSunEqCoords( planet.getOrbitalElementsOf(Sun,Julian) ) );
          observingobject.LHA = algorithms.calculateLocalHA(mountstatus.LMST.rawtime,poi.x/pi*12);
          observingobject.LDEC = algorithms.calculateLocalDEC(mountstatus.currentLat,poi.y/pi*180);
          observingobject.objRA = poi.x/pi*12;
          observingobject.objDEC = poi.y/pi*180;
          observingobject.name = "Venus";
          break;
        case Mars:
          poi = planet.calculatePlanetCoords( planet.getOrbitalElementsOf(Mars,Julian), planet.calculateSunEqCoords( planet.getOrbitalElementsOf(Sun,Julian) ) );
          observingobject.LHA = algorithms.calculateLocalHA(mountstatus.LMST.rawtime,poi.x/pi*12);
          observingobject.LDEC = algorithms.calculateLocalDEC(mountstatus.currentLat,poi.y/pi*180);
          observingobject.objRA = poi.x/pi*12;
          observingobject.objDEC = poi.y/pi*180;
          observingobject.name = "Mars";
          break;
        case Jupiter:
          poi = planet.calculatePlanetCoords( planet.getOrbitalElementsOf(Jupiter,Julian), planet.calculateSunEqCoords( planet.getOrbitalElementsOf(Sun,Julian) ) );
          observingobject.LHA = algorithms.calculateLocalHA(mountstatus.LMST.rawtime,poi.x/pi*12);
          observingobject.LDEC = algorithms.calculateLocalDEC(mountstatus.currentLat,poi.y/pi*180);
          observingobject.objRA = poi.x/pi*12;
          observingobject.objDEC = poi.y/pi*180;
          observingobject.name = "Jupiter";
          break;
        case Saturn:
          poi = planet.calculatePlanetCoords( planet.getOrbitalElementsOf(Saturn,Julian), planet.calculateSunEqCoords( planet.getOrbitalElementsOf(Sun,Julian) ) );
          observingobject.LHA = algorithms.calculateLocalHA(mountstatus.LMST.rawtime,poi.x/pi*12);
          observingobject.LDEC = algorithms.calculateLocalDEC(mountstatus.currentLat,poi.y/pi*180);
          observingobject.objRA = poi.x/pi*12;
          observingobject.objDEC = poi.y/pi*180;
          observingobject.name = "Saturn";
          break;
        case Uranus:
          poi = planet.calculatePlanetCoords( planet.getOrbitalElementsOf(Uranus,Julian), planet.calculateSunEqCoords( planet.getOrbitalElementsOf(Sun,Julian) ) );
          observingobject.LHA = algorithms.calculateLocalHA(mountstatus.LMST.rawtime,poi.x/pi*12);
          observingobject.LDEC = algorithms.calculateLocalDEC(mountstatus.currentLat,poi.y/pi*180);
          observingobject.objRA = poi.x/pi*12;
          observingobject.objDEC = poi.y/pi*180;
          observingobject.name = "Uranus";
          break;
        case Neptune:
          poi = planet.calculatePlanetCoords( planet.getOrbitalElementsOf(Neptune,Julian), planet.calculateSunEqCoords( planet.getOrbitalElementsOf(Sun,Julian) ) );
          observingobject.LHA = algorithms.calculateLocalHA(mountstatus.LMST.rawtime,poi.x/pi*12);
          observingobject.LDEC = algorithms.calculateLocalDEC(mountstatus.currentLat,poi.y/pi*180);
          observingobject.objRA = poi.x/pi*12;
          observingobject.objDEC = poi.y/pi*180;
          observingobject.name = "Neptune";
          break;
        case Pluto:
          //not implemented yet
          break;
        default: //all other situations including deep sky objects ansd stars
          observingobject.LHA = algorithms.calculateLocalHA(mountstatus.LMST.rawtime,observingobject.objRA);
          observingobject.LDEC = algorithms.calculateLocalDEC(mountstatus.currentLat,observingobject.objDEC);
          
          break;

      }
      //now LHA and DEC calculated 


      //belirlenen zamanda bir veri durumu değiştir
      long int now = millis();

      //Point pp = planet.calculatePlanetCoords( planet.getOrbitalElementsOf(Venus,Julian), planet.calculateSunEqCoords( planet.getOrbitalElementsOf(Sun,Julian) ) );
      //double neptuneLHA = algorithms.calculateLocalHA(lmst.rawtime,pp.x/pi*12);
      /*
      Serial.print("RA:");
      Serial.println(neptuneLHA,8);
      Serial.print("DEC:");
      Serial.println(pp.y/pi*180,8);
      Serial.println("----------------------");
      Serial.print(lmst.hour);Serial.print(":");
      Serial.print(lmst.minute);Serial.print(":");
      Serial.println(lmst.second);
      Serial.print(GMST.hour);Serial.print(":");
      Serial.print(GMST.minute);Serial.print(":");
      Serial.println(GMST.second);
      Serial.println("----------------------");
      Serial.print(mountstatus.currentYear);
      Serial.print(mountstatus.currentHour);Serial.print(":");
      Serial.print(mountstatus.currentMinute);Serial.print(":");
      Serial.print(mountstatus.currentSecond);Serial.print(":");
      Serial.println(mountstatus.currentCentiSecond);
      Serial.println(mountstatus.currentLat,8);
      Serial.println(mountstatus.currentLong,8);
      */
      long int then = millis();
      //Serial.print(then-now);
      //printLocalTime();
      //getting centisecond of internal rtc
      int x = xx_time_get_time();
      mountstatus.currentCentiSecond = x;
      //Serial.println(String(x).c_str()); 

      /*
      Serial.println(gps.location.lng(),8);
      Serial.print("LST: ");
      Serial.print(lmst.hour); Serial.print(":");
      Serial.print(lmst.minute);Serial.print(":");
      Serial.println(lmst.second);
      */
      //6.767647222
      //HA of akyıldız (whitestar)

      //double localHourAngle = algorithms.calculateLocalHA(lmst.rawtime,gotoRAvalue);
      //latitude object dec
      //-16.74616667
      //localdec required for altZ mounts only. for equatorial mounts dec value is same as the observing object
      //double localDec = algorithms.calculateLocalDEC(mountstatus.currentLat,gotoDECvalue);

      //Axial mode
      //stepperRA.moveTo((long)calculateRA_Steps(0,localHourAngle*3600));
      //stepperDEC.moveTo((long)calculateDEC_Steps(0,localDec*3600));

      //PL Mode for Bilye Hardware
      Point DD = algorithms.calculateParallelMotion((long)calculateRA_Steps(0,observingobject.LHA*3600),
                                                    (long)calculateDEC_Steps(0,observingobject.LDEC*3600));

      stepperRA.moveTo(DD.x);
      stepperDEC.moveTo(DD.y);



    }
      //arrayBuffer.push(injection.c_str());
    /*
    Serial.print("LHA: ");
    Serial.print((int)(localHourAngle));Serial.print(":");
    Serial.print((int) fmod(localHourAngle*60,60));Serial.print(":");
    Serial.println(fmod(localHourAngle*3600,60));

    Serial.print("LDEC: ");
    Serial.print((int)(localDec));Serial.print(":");
    Serial.print((int) abs(fmod(localDec*60,60)));Serial.print(":");
    Serial.println(abs(fmod(localDec*3600,60)));

     */

  }
  


  //step motorları koştur
  //motor driving 
  //we have to do it in timer interrupt
  runner();

  //ui driving
  //user interface mainloop 
  onIdle();

}

void runner(){

    //error control have to be done
    stepperRA.run();
    stepperDEC.run();
    stepperFOCUS.run();

} 