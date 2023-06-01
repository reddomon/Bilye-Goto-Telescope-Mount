// Uimount class.h

#pragma once

#include "arduino.h"
#include "config.h"




#include "Wire.h"
#include "../nextion/Nextion.h"
//#include "TimeLib.h"



class UImount
{
 protected:


 public:
	void init();

	void printerHalted();
	void UIstart();
	void Finished();
	void Started();

	//datareceive secreen setters
	void dataReceivingStarted();
	void dataReceivingFinished();

	void setFan(char * inSpeed,int fan);

	void setFanStatus(int fan,bool status);

	void setWifiMode(char* wifimode);
	void setIpName(char* ip);
	void setStatusText(char * status);
	void setWifiMode(int mode);

	void setPage(int page);
	int getPage();
	void resetPageChanged();
	bool pageChanged();
	
	//page 2 observation
	void setObservationPageVariables(const char* name,
									const char* type,
									const char* info,
									const char* ra,
									 const char* dec,
									 const char* lha,
									 const char* gmt,
									 const char* lt,
									 const char*gmst,
									 const char* lmst,
									 const char* ldate,
									 const char* lat,
									 const char*lon,
									 const char *alt,
									 int trackingtype);//,const char* lha,const char* mountStatus,const char * gmt,
										//const char* lt,const char*gmst,const char* lmst);

					//objectLHA.setText(lha);
					//objectGMT.setText(gmt);
					//objectLT.setText(lt);
					//objectGMST.setText(gmst);
					//objectLMST.setText(lmst);

	void setPower(bool status);
	void setCaseLight(bool status);


	void setMessage(const char * inStr);


	//void setTime(int year, int month, int day, int hour, int minute);
	void setTime();

	void setFilelist(String [],String [],int);
	void setWifilist(String[],int);
	

private:
	char _x[7], _y[7], _z[7];
	char _et[4], _fan[4], _bt[4];
	uint8_t  _ba, _ea;
	int  _page = 0;
	bool _pageChanged;
	bool _caseLight, _power = 0;
	bool _isPrinting = false;
	bool _isHomed = false;
	bool _startup = false;



	//globals

	//data receiving?
	NexVariable datareceive= NexVariable(1,26,"datareceive");


	NexNumber ext1status= NexNumber(5,16,"working.ext1");
	NexNumber ext2status= NexNumber(5,27,"working.ext2");
	NexNumber bedstatus = NexNumber(5,28,"working.bed");
	NexNumber fan1status= NexNumber(5,29,"working.fan1");
	NexNumber fan2status= NexNumber(5,30,"working.fan2");


	NexPage pageError = NexPage(12,0,"error");
	NexPage pageMain =  NexPage(1,0,"mainpage");
	NexPage pageFinished = NexPage(15,0,"finished");
	NexPage pageDatareceive = NexPage(14,0,"datareceive");
	NexPage pageWorking = NexPage(5,0,"working");


	//page 0
	//page 1
    //ip and mode info (global can be read from any page)
	NexText tMode = NexText(1, 6, "mainpage.modetext");
	NexText tIp = NexText(1, 7, "mainpage.iptext");

	//page 2 observation
	NexText objectName = NexText(2,1,"name");
	NexText objectType = NexText(2,2,"type");
	NexText objectInfo = NexText(2,3,"info");
	NexText objectRA = NexText(2,4,"ra");
	NexText objectDEC = NexText(2,5,"dec");
	NexText objectLHA = NexText(2,6,"lha");

	NexText date = NexText(2,18,"date");

	NexText objectGMT = NexText(2,7,"gmt");
	NexText objectLT = NexText(2,8,"lt");
	NexText objectGMST = NexText(2,9,"gmst");
	NexText objectLMST = NexText(2,10,"lmst");

	NexText mountLat = NexText(2,11,"lat");
	NexText mountLon = NexText(2,12,"lon");
	NexText mountAlt = NexText(2,13,"alt");

	//status
	NexVariable tracktype = NexVariable(2,30,"track");
	//lat, lon alt,



	//page 3


	//page 4 catalogs
	NexText file1 = NexText(2, 2, "t1");
	NexText file2 = NexText(2, 3, "t2");
	NexText file3 = NexText(2, 4, "t3");
	NexText file4 = NexText(2, 5, "t4");
	NexText file5 = NexText(2, 6, "t5");
	NexText file6 = NexText(2, 7, "t6");
	NexText file7 = NexText(2, 8, "t7");
	NexText file8 = NexText(2, 9, "t8");
	NexText file9 = NexText(2, 10, "t9");
	
	NexVariable maxFilePageCount = NexVariable(2, 32, "h0");

	NexVariable shortfile1 = NexVariable(2, 18, "ts1");
	NexVariable shortfile2 = NexVariable(2, 19, "ts2");
	NexVariable shortfile3 = NexVariable(2, 20, "ts3");
	NexVariable shortfile4 = NexVariable(2, 21, "ts4");
	NexVariable shortfile5 = NexVariable(2, 22, "ts5");
	NexVariable shortfile6 = NexVariable(2, 23, "ts6");
	NexVariable shortfile7 = NexVariable(2, 24, "ts7");
	NexVariable shortfile8 = NexVariable(2, 25, "ts8");
	NexVariable shortfile9 = NexVariable(2, 26, "ts9");
	//page 5


	//page 6
	


	//page 7  wifi
	NexText wifiText0 = NexText(7, 17, "wifi0");
	NexText wifiText1 = NexText(7, 18, "wifi1");
	NexText wifiText2 = NexText(7, 19, "wifi2");
	NexText wifiText3 = NexText(7, 20, "wifi3");
	NexText wifiText4 = NexText(7, 21, "wifi4");
	NexText wifiText5 = NexText(7, 22, "wifi5");
	NexText wifiText6 = NexText(7, 23, "wifi6");
	NexVariable maxWifiPageCount = NexVariable(7, 3, "wifipagecount");
	//current status variable
	NexText statusText = NexText(7,24,"status");
	//wifi mode variable
	NexVariable wifimode = NexVariable(7, 2, "wifimode");

	//page 9 temperature
	//temperature settings
	NexNumber tExt1 = NexNumber(9,11,"tExt1");
	NexNumber tExt2 = NexNumber(9,14,"tExt2");
	NexNumber tBed = NexNumber(9,16,"tBed");
	NexNumber fanSpeed1 = NexNumber(9,18,"fanSpeed1");
	NexNumber fanSpeed2 = NexNumber(9,20,"fanSpeed2");

	//page 12
	//error page
	//
	//datareceive


	NexDSButton btPower = NexDSButton(2, 5, "MainMenu.btPower");
	NexDSButton btLight = NexDSButton(2, 6, "MainMenu.btLight");

	NexPicture pPower = NexPicture(1, 29, "MainPage.pPower");
	NexPicture pLight = NexPicture(1, 30, "MainPage.pLight");

	NexVariable vaPower = NexVariable(2, 8, "MainMenu.vaPower");
	NexVariable vaLight = NexVariable(2, 9, "MainMenu.vaLight");

	NexPicture pHome = NexPicture(1, 35, "MainPage.pHome");
	NexPicture pExtruding = NexPicture(1, 34, "MainPage.pExtruding");

	NexTimer tmSS = NexTimer(1, 37, "tmSS");
	NexVariable vaCounter = NexVariable(1, 38, "vaCounter");

	NexVariable tTime = NexVariable(1, 39, "MainPage.tTime");

	char ext[10];
	int _minutes;

};




