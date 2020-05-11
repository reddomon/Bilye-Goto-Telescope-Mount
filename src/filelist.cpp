#include   "filelist.h"


//init sd and get file store list in a vector
FileList::FileList(){

	if(!SD.begin(SD_CS)) {
    	Serial.println("Card Mount Failed");
    	return;
  	}
  	else Serial.println("Card Mount Success");
  	root = SD.open("/");
  	while (true) {

    	File entry =  root.openNextFile();
    	if (! entry) {
      	// no more files
      	break;
    	}
    	if (entry.isDirectory()) {
      		
      		
    	} else {
      	// files have sizes, directories do not
		  	list.push_back(String(entry.name()));
		  
    		}
    		entry.close();
  	}
	
}
//file count from vector
uint16_t   FileList::count(){

	return list.capacity();
    
}
//return file name
String FileList::seek(const uint16_t num){

		return list.at(num);
}

//refresh file list from sdcard
void FileList::refresh(){

		if(!SD.begin(SD_CS)) {
    	Serial.println("Card Mount Failed");
    	return;
  	}
  	else Serial.println("Card Mount Success");
  	root = SD.open("/");
  	while (true) {

    	File entry =  root.openNextFile();
    	if (! entry) {
      	// no more files
      	break;
    	}
    	if (entry.isDirectory()) {
      		
      		
    	} else {
      	// files have sizes, directories do not
		  	list.push_back(String(entry.name()));
		  
    		}
    		entry.close();
  	}
}