/*********************
 * nextionui.cpp *
 *********************/





#include "userinterfaceapi.h"
#include "queueArray.h"
#include "uimount.h"
#include "filelist.h"
#include <Wifi.h>
#include <Preferences.h>



extern UImount mountUi;
FileList fileList = FileList(); 
extern MountStatus mountstatus;
Preferences wifiCredentials;

char inputBuffer[32];
bool stringEnd = false;
bool stringStart = true;

QueueArray inputQueue;

#define LCD_UPDATE_INTERVAL  500

unsigned int ms = 0;



//page process
void ProcessPage(char * inputString, uint8_t receivedBytes)
{
	char subbuff[4];
	uint8_t strLength = receivedBytes - 2;

	memcpy(subbuff, &inputString[2], strLength);
	subbuff[strLength] = 0;

	mountUi.setPage(atoi(subbuff));



}

//serial data receive routine
//received by nexserial namely Serial2
void mySerialEvent()
{
  volatile char inputChar;
  volatile uint8_t stringLength = 0;
  stringStart = false;
  stringEnd = false;
  memset(inputBuffer, 0, 32);
  while (nexSerial.available())
  {
    inputChar = (char)nexSerial.read();
    //delayMicroseconds(200);
    if ((int)inputChar >= 32 && (int)inputChar < 255)
    {
      if (inputChar != '!')
      {
        stringEnd = false;
      }
      if (!stringEnd && inputChar != '!' && inputChar != '*')
      {
        inputBuffer[stringLength] = inputChar;
        stringLength++;
      }

      if (inputChar == '!' && !stringEnd)
      {
        stringEnd = true;
        inputBuffer[stringLength] = 0;

        if (inputBuffer[0] == 'P')
        {
          //page indicator processor
          ProcessPage(inputBuffer, stringLength);
		  Serial.println(inputBuffer);
          stringLength = 0;
        }
        else
        {
       
          //injectCommands_P(inputBuffer);
          inputQueue.push(inputBuffer);
		  
          stringLength = 0;
        }
      }
    }
  }
}



//received data processing
void processBuffer(const char* receivedString) {

	int receivedByte = strlen(receivedString);
	int strLength = 0;
	char subbuff[64] = { 0 };

	String tempo[9];
	String shortfile[9];
	String tempoW[6];

	//SERIAL_ECHO(receivedString);

	switch (receivedString[0]) {
	case 'E':
		strLength = receivedByte - 3;
		memcpy(subbuff, &receivedString[3], strLength);
		subbuff[strLength + 1] = '\0';
		//thermalManager.setTargetHotend(atoi(subbuff), ((int)receivedString[1]) - 1);
		//if(receivedString[1] == '0') ExtUI::setTargetTemp_celsius(atoi(subbuff),ExtUI::H0);
		//else if(receivedString[1]  == '1') ExtUI::setTargetTemp_celsius(atoi(subbuff),ExtUI::H1);
		//SERIAL_ECHO(subbuff);
		
		break;

	case 'A':
		strLength = receivedByte - 2;
		memcpy(subbuff, &receivedString[2], strLength);
		subbuff[strLength + 1] = '\0';
		//thermalManager.setTargetBed(atoi(subbuff));
		//nextPrinter.setBedTarget(subbuff);
		//ExtUI::setTargetTemp_celsius(atoi(subbuff),ExtUI::BED);
		//SERIAL_ECHO(subbuff);
		
		break;

	case 'F':
		strLength = receivedByte - 3;
		memcpy(subbuff, &receivedString[3], strLength);
		subbuff[strLength + 1] = '\0';
		//fanSpeeds[0] = map(atoi(subbuff), 0, 100, 0, 255); //(ceil(atoi(subbuff) * 2.54));

		//nextPrinter.setFan(subbuff,((int)receivedString[2]));
		if(((int)receivedString[1])  == '0') ;//setTargetFan_percent(atoi(subbuff),FAN0);
		else if(((int)receivedString[1])  == '1') ;//setTargetFan_percent(atoi(subbuff),FAN1);
		//SERIAL_ECHO(subbuff);
		break;

	case 'G':
		strLength = receivedByte - 2;
		memcpy(subbuff, &receivedString[2], strLength);
		subbuff[strLength] = '\0';
		
    	//inject command into buffer directly
		arrayBuffer.push(subbuff);


		break;
	case 'P':
		strLength = receivedByte - 2;
		memcpy(subbuff, &receivedString[2], strLength);
		subbuff[receivedByte - strLength] = '\0';
		mountUi.setPage(atoi(subbuff));
		break;

	case 'I': //Power status
		strLength = receivedByte - 2;
		memcpy(subbuff, &receivedString[2], strLength);
		subbuff[receivedByte - strLength] = '\0';
		//printer.setPower((bool)atoi(subbuff));
		break;

	case 'C': //Machine status
		strLength = receivedByte - 2;
		memcpy(subbuff, &receivedString[2], strLength);
		subbuff[receivedByte - strLength] = '\0';
		switch(atoi(subbuff)){

				case 0://stop print
					//ExtUI::stopPrint();
					//SERIAL_ECHO("STOP");
					//change page
					//nextPrinter.Finished();
					break;
				case 1://pause print
					//ExtUI::pausePrint();
					//SERIAL_ECHO("PAUSE");
					break;
				case 2://continue print
					////ExtUI::resumePrint();
					//SERIAL_ECHO("RESUME");
					break;
				default:
					break;

		}
		//printer.setCaseLight((bool)atoi(subbuff));
		break;

	case 'S': //Get SD card list
		{
		//refresh list
		//fileList.refresh();
		fileList.refresh();	
		//page number
		strLength = receivedByte - 2;
		memcpy(subbuff, &receivedString[2], strLength);
		subbuff[receivedByte - strLength] = '\0';

		int maxVal = 0;
		int listNumber = atoi(subbuff);

		//control file count for 9 slots
		if((listNumber+1)*9<=fileList.count()){
			maxVal = 9;	
		}
		else{
			maxVal = fileList.count()%9;
		}

		for(int i = 0;i<maxVal;i++){

			
			tempo[i] = fileList.seek(i+listNumber*9).c_str();
	
		}
		for(int i = 0;i<9-maxVal;i++){
			//delete txt name if not 9
			tempo[i+maxVal] = "";
			shortfile[i+maxVal] = "";
		}
		//list 9 files
		mountUi.setFilelist(tempo,shortfile,((fileList.count()-1)/9));
		break;
		}
	case 'W': //get wifi list
	{

		//change network mode
		WiFi.mode(WIFI_MODE_STA);
		//scan for networks
		int wifiCount = WiFi.scanNetworks();
		//page number
		strLength = receivedByte - 2;
		memcpy(subbuff, &receivedString[2], strLength);
		subbuff[receivedByte - strLength] = '\0';

		
		int maxValWifi = 0;
		int listNumberWifi = atoi(subbuff);

		//control file count for 6 slots
		if((listNumberWifi+1)*6<=wifiCount){
			maxValWifi = 6;	
		}
		else{
			maxValWifi = wifiCount%6;
		}

		for(int i = 0;i<maxValWifi;i++){

			
			tempoW[i] = WiFi.SSID(i);
	
		}
		for(int i = 0;i<6-maxValWifi;i++){
			//delete txt name if not 6
			tempoW[i+maxValWifi] = "";
		}
		//list 6 files
		mountUi.setWifilist(tempoW,((wifiCount-1)/6));
		break;
		}
	case 'L':
		{
		strLength = receivedByte - 2;
		memcpy(subbuff, &receivedString[2], strLength);
		subbuff[strLength] = '\0';
		const char s[2] = "/";
		int wMode;
		char* wPwd,*wSsid;

		char* token = strtok(subbuff, s);
   		
		wMode = atoi(token);
   		wSsid = strtok(NULL, s);
		wPwd = strtok(NULL, s);

		WiFi.mode(WIFI_MODE_AP);
		WiFi.softAP(wSsid,wPwd);

		//ap mode creating wifiAP and save credentials
		delay(20);
		wifiCredentials.begin("wifi",false);
		wifiCredentials.putInt("wifi_mode",2);
		wifiCredentials.putString("ssid",wSsid);
		wifiCredentials.putString("pwd",wPwd);
		wifiCredentials.end();
		break;
		}

	case 'M':
	{
		//sta mode. connecting another network and save credentials
		strLength = receivedByte - 2;
		memcpy(subbuff, &receivedString[2], strLength);
		subbuff[strLength] = '\0';
		const char s[2] = "/";
		int wMode;
		char* wPwd,*wSsid;

		char* token = strtok(subbuff, s);
   		
		wMode = atoi(token);
   		wSsid = strtok(NULL, s);
		wPwd = strtok(NULL, s);

		WiFi.mode(WIFI_MODE_STA);
        WiFi.begin(wSsid, wPwd);
        int connectTry = 0;
        while (!WiFi.isConnected()) {
        	//SERIAL_ERROR_MSG("Unable to connect to WiFi");
            delay(500);
            connectTry++;
            if(connectTry>5) break;
        }


		delay(20);	
		wifiCredentials.begin("wifi",false);
		wifiCredentials.putInt("wifi_mode",1);
		wifiCredentials.putString("ssidsta",wSsid);
		wifiCredentials.putString("ssidpwd",wPwd);
		wifiCredentials.end();
		break;
		}

	}
}








void onStartup()
{
  /* Initialize the display module here. The following
     * routines are available for access to the GPIO pins:
     *
     *   SET_OUTPUT(pin)
     *   SET_INPUT_PULLUP(pin)
     *   SET_INPUT(pin)
     *   WRITE(pin,value)
     *   READ(pin)
     */

  //char* speed= "125";
  //nextPrinter.setFan(speed);
  	//attachInterrupt(16,mySerialEvent,RISING);
	mountUi.UIstart();




}
void onIdle()
{

	mySerialEvent(); //general read from serial from display, put commands on queue

	if (!inputQueue.isEmpty()) //process any actual commands from the display
	{
		processBuffer(inputQueue.pop()); //call function to process command from the display
	}

	

	
	//problemli alan
	if ( millis() >ms + LCD_UPDATE_INTERVAL) { //time to send update to display



		//global variables


		switch(mountUi.getPage()){
			case 0: //logopage
				break;
			case 1: //mainpage
					//WIFI_MODE_NULL = 0,  /**< null mode */
    				//WIFI_MODE_STA,       /**< WiFi station mode */
    				//WIFI_MODE_AP,        /**< WiFi soft-AP mode */
    				//WIFI_MODE_APSTA,     /**< WiFi station + soft-AP mode */
    				//WIFI_MODE_MAX  
				switch(WiFi.getMode()){
					case WIFI_MODE_NULL:
						mountUi.setWifiMode("Null");
						break;
					case WIFI_MODE_STA:
						mountUi.setWifiMode("STA");
						break;
					case WIFI_MODE_AP:
						mountUi.setWifiMode("AP");
						break;
					case WIFI_MODE_APSTA:
						mountUi.setWifiMode("AP/STA");
						break;
					case WIFI_MODE_MAX:
						mountUi.setWifiMode("MAX");
						break;


				}	

				switch (WiFi.getMode())
				{
				case WIFI_MODE_STA: //station mode
				{
					String address= "http://"+WiFi.localIP().toString()+"/api/upload";
					mountUi.setIpName((char*)address.c_str());
					break;
				}
				
				case WIFI_MODE_AP:  //Access point Mode
				{
					String aPaddress ="http://"+WiFi.softAPIP().toString()+"/api/upload";
					mountUi.setIpName((char*)aPaddress.c_str());	
					break;
				}
				}
				break;
			case 2: //observation
				mountUi.setObservationPageVariables(observingobject.name.c_str(),
													String(observingobject.type,15).c_str(),
													observingobject.information.c_str(),
													observingobject.getObjectRAString().c_str(),
													observingobject.getObjectDECString().c_str(),
													observingobject.getObjectLHAString().c_str(),
													mountstatus.getGMTString().c_str(),
													mountstatus.getLTString().c_str(),
													mountstatus.getGMSTString().c_str(),
													mountstatus.getLMSTString().c_str()
													
													
													);

				break;
			case 3: //alignment
				break;
			case 4: //catalogs
				
				break;
			case 5: //settings
			
				//nextPrinter.setXPos((char*)String(getAxisPosition_mm(X)).c_str());
				//nextPrinter.setYPos((char*)String(getAxisPosition_mm(Y)).c_str());
				//nextPrinter.setZPos((char*)String(getAxisPosition_mm(Z)).c_str());
				//nextPrinter.setBedActual(getActualTemp_celsius(BED));
				//nextPrinter.setBedTarget((char*)String(getTargetTemp_celsius(BED)).c_str());
				//nextPrinter.setExtruderActual(getActualTemp_celsius(H0),0);
				//nextPrinter.setExtruderTarget((char*)String(getTargetTemp_celsius(H0)).c_str(),0);
				//nextPrinter.setExtruderActual(getActualTemp_celsius(H1),1);
				//nextPrinter.setExtruderTarget((char*)String(getTargetTemp_celsius(H1)).c_str(),1);
				//nextPrinter.setTPercentage(String(getProgress_percent()).c_str());
				//nextPrinter.setFan((char*)String(getActualFan_percent(ExtUI::FAN0)).c_str(),0);
				//nextPrinter.setFan((char*)String(getActualFan_percent(ExtUI::FAN1)).c_str(),1);
				//nextPrinter.setTElapsed((char*)String(getProgress_seconds_elapsed()).c_str());
				break;
			case 6: //about

				
				break;
			case 7:   //wifi settings
				mountUi.setWifiMode(WiFi.getMode());

				//check ap mode on
				if(WiFi.getMode()==WIFI_MODE_AP){
						mountUi.setStatusText("Access Point Mode is on.");

				}
				
				else if(WiFi.getMode()==WIFI_MODE_APSTA){
						mountUi.setStatusText("AP/STA Mode is on.");

				}
				//sta mode on
				else if(WiFi.getMode()==WIFI_MODE_STA){
					String goo=String("Connected to "+WiFi.SSID());
				
					if(WiFi.isConnected()) mountUi.setStatusText((char*)goo.c_str());
					else  mountUi.setStatusText("Disconnected.");

					
				}
				break;

			case 9:  //simple move
				//nextPrinter.setBedActual(getActualTemp_celsius(BED));
				//nextPrinter.setBedTarget((char*)String(getTargetTemp_celsius(BED)).c_str());
				//nextPrinter.setExtruderActual(getActualTemp_celsius(E0),0);
				//nextPrinter.setExtruderActual(getActualTemp_celsius(E1),1);
				//nextPrinter.setExtruderActual(getActualTemp_celsius(H0),0);
				//nextPrinter.setExtruderTarget((char*)String(getTargetTemp_celsius(H0)).c_str(),0);
				//nextPrinter.setExtruderActual(getActualTemp_celsius(H1),1);
				//nextPrinter.setExtruderTarget((char*)String(getTargetTemp_celsius(H1)).c_str(),1);
				break;
			case 12:
				break;
			default:
				break;





		}

		//printer.setExtruderActual((uint8_t)thermalManager.degHotend(1), 1);
		//printer.setExtruderTarget(itostr3left(thermalManager.degTargetHotend(1)), 1);

		//printer.setBedActual((uint8_t)thermalManager.degBed());
		//printer.setBedTarget(itoa(thermalManager.degTargetBed(), temp, 10));



		//printer.setFan(itoa(map(fanSpeeds[0], 0, 255, 0, 100), temp, 10));

		//printer.setPower(digitalRead(PS_ON_PIN));
		//printer.setCaseLight(digitalRead(CASE_LIGHT_PIN));

		//printer.setIsPrinting(planner.movesplanned());
		//printer.setIsHomed(axis_homed[X_AXIS] && axis_homed[Y_AXIS] && axis_homed[Z_AXIS] ? true : false);

		//printer.resetPageChanged();

		//printer.setTime(year(), month(), day(), hour(), minute());
		//printer.setTime();
		ms = millis();
	}

}

void onMountKilled(PGM_P const msg) {


		//system must be resetted
		mountUi.printerHalted();


}
void onMediaInserted(){

	//init sd card again
};
void onMediaError(){};
void onMediaRemoved(){};
void onPlayTone(const uint16_t frequency, const uint16_t duration) {}
void onUserConfirmRequired(const char *const msg) {}
void onStatusChanged(const char *const msg) {}
void onFactoryReset() {}

void onStoreSettings(char *buff)
{
  // This is called when saving to EEPROM (i.e. M500). If the ExtUI needs
  // permanent data to be stored, it can write up to eeprom_data_size bytes
  // into buff.

  // Example:
  //  static_assert(sizeof(myDataStruct) <= ExtUI::eeprom_data_size);
  //  memcpy(buff, &myDataStruct, sizeof(myDataStruct));
}

void onLoadSettings(const char *buff)
{
  // This is called while loading settings from EEPROM. If the ExtUI
  // needs to retrieve data, it should copy up to eeprom_data_size bytes
  // from buff

  // Example:
  //  static_assert(sizeof(myDataStruct) <= ExtUI::eeprom_data_size);
  //  memcpy(&myDataStruct, buff, sizeof(myDataStruct));
}

void onConfigurationStoreWritten(bool success)
{
  // This is called after the entire EEPROM has been written,
  // whether successful or not.
}

void onConfigurationStoreRead(bool success)
{
  // This is called after the entire EEPROM has been read,
  // whether successful or not.
}


void onDataReceiveStarted(){

mountUi.dataReceivingStarted();
//flushableSerial.println("data receiving started");

}

void onDataReceiveFinished(){
mountUi.dataReceivingFinished();
//flushableSerial.println("data receiving finished");

}



