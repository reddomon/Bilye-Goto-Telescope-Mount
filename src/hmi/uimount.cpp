

#include "uimount.h"


void UImount::init()
{

}
//pages
void UImount::printerHalted()
{
	pageError.show();
}
void UImount::UIstart()
{
	pageMain.show();
}

void UImount::Finished(){

	pageFinished.show();
	
}

void UImount::Started(){

	pageWorking.show();
}

//data receiving started
void UImount::dataReceivingStarted(){

	//pageDatareceive.show();
}
//data receiving finished
void UImount::dataReceivingFinished(){

	//pageMain.show();
}





void UImount::setFan(char *inSpeed, int fan)
{

	switch (fan)
	{
		{
		case 0:
				//tFan1.setText(inSpeed);
			break;
		case 1:
				//tFan2.setText(inSpeed);
		default:
			break;
		}
	}
}

void UImount::setPage(int page)
{
	if (page != _page)
		_pageChanged = true;
	_page = page;
}

int UImount::getPage()
{
	return _page;
}

void UImount::resetPageChanged()
{
	_pageChanged = false;
}

bool UImount::pageChanged()
{
	return _pageChanged;
}

void UImount::setPower(bool status)
{
	if (_power != status || _pageChanged)
	{
		_power = status;

		vaPower.setValue(_power ? 1 : 0);
		if (_page == 2)
			btPower.setValue(_power ? 1 : 0);
		else if (_page == 1)
		{
			if (status)
				pPower.setPic(19);
			else
				pPower.setPic(20);
		}
	}
}

void UImount::setCaseLight(bool status)
{
	if (_caseLight != status || _pageChanged )
	{
		_caseLight = status;

		vaLight.setValue(_caseLight ? 1 : 0);
		if (_page == 2)
			btLight.setValue(_caseLight ? 1 : 0);
		else if (_page == 1)
		{
			//uint32_t val = atoi(subbuff);
			if (status == 1)
				pLight.setPic(18);
			else
				pLight.setPic(20);
		}
	}
}




//set message
void UImount::setMessage(const char *inStr)
{
	
}


//void PrinterClass::setTime(int year, int month, int day, int hour, int minute)
void UImount::setTime()
{
}
//sdcard
void UImount::setFilelist(String str[], String shortfile[], int pageNumber)
{
	//long file names
	file1.setText(str[0].c_str());
	file2.setText(str[1].c_str());
	file3.setText(str[2].c_str());
	file4.setText(str[3].c_str());
	file5.setText(str[4].c_str());
	file6.setText(str[5].c_str());
	file7.setText(str[6].c_str());
	file8.setText(str[7].c_str());
	file9.setText(str[8].c_str());
	//short file names
	shortfile1.setText(shortfile[0].c_str());
	shortfile2.setText(shortfile[1].c_str());
	shortfile3.setText(shortfile[2].c_str());
	shortfile4.setText(shortfile[3].c_str());
	shortfile5.setText(shortfile[4].c_str());
	shortfile6.setText(shortfile[5].c_str());
	shortfile7.setText(shortfile[6].c_str());
	shortfile8.setText(shortfile[7].c_str());
	shortfile9.setText(shortfile[8].c_str());

	maxFilePageCount.setValue(pageNumber);
}


void UImount::setWifilist(String str[], int pageNumber)
{
	//wifi names
	wifiText0.setText(str[0].c_str());
	wifiText1.setText(str[1].c_str());
	wifiText2.setText(str[2].c_str());
	wifiText3.setText(str[3].c_str());
	wifiText4.setText(str[4].c_str());
	wifiText5.setText(str[5].c_str());

	maxWifiPageCount.setValue(pageNumber);
}



//Fan status
void UImount::setFanStatus(int fan,bool status){
switch(fan){

	case 0:
		fan1status.setValue(status);
		break;
	case 1:
		fan2status.setValue(status);
		break;
	default:
		break;
	}

}

//set wifiname
void UImount::setWifiMode(char* wifimode){
		
		tMode.setText(wifimode);
}
//set ip name
void UImount::setIpName(char* ip){

	tIp.setText(ip);
}
//set status text
void UImount::setStatusText(char *status){

	statusText.setText(status);
}
//set wifi mode
void UImount::setWifiMode(int mode){

	wifimode.setValue(mode);

}
void UImount::setObservationPageVariables(const char* name,
										const char* type,
										const char* info,
										const char* ra,
										 const char* dec,
										 const char* lha,
										 //const char* mountStatus,
										 const char * gmt,
										const char* lt,
										const char*gmst,
										const char* lmst,
										const char* ldate,
										const char* lat,
									 	const char*lon,
									 	const char *alt,
										 int trackingtype){
											
					objectName.setText(name);
					objectInfo.setText(info);
					objectRA.setText(ra);
					objectDEC.setText(dec);
					objectLHA.setText(lha);
					objectGMT.setText(gmt);
					objectLT.setText(lt);
					objectGMST.setText(gmst);
					objectLMST.setText(lmst);
					date.setText(ldate);
					mountLat.setText(lat);
					mountLon.setText(lon);
					mountAlt.setText(alt);
					tracktype.setValue(trackingtype);
					
					



}

UImount mountUi;


