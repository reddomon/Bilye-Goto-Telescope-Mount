//sınıf halne getirilmeli
//database objesi üglobal olarak tanımlanırsa daha az memory tüketecektis
//TODO
#include "database.h"


extern ObservingObject observingobject;
extern std::vector<ObservingObject> objectList;

const char* data = "Callback function called";

//callback function for data parsing
static int callback(void *data, int argc, char **argv, char **azColName){
   
   ObservingObject listObject;
   
/*  int i;
   //Serial.printf("%s: ", (const char*)data);
   for (i = 0; i<argc; i++){
       //these are object properties retrieved from database
       //ROWID,name,ra,dec 
       Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
       //create objects from database
       
         
   }  
   Serial.printf("\n");
   */
    //assign datas to apropriate object properties
    //setter or getters not preferred because of function entrancy 
    listObject.rowid = std::atoi(argv[0]);
    listObject.name = argv[1];
    listObject.objRA= std::atof(argv[2]);
    listObject.objDEC = std::atof(argv[3]);
    //push into vector
    objectList.push_back(listObject);

    
   return 0;
}

//open database
int openDb(const char *filename, sqlite3 **db) {
   int rc = sqlite3_open(filename, db);
   if (rc) {
       Serial.printf("Can't open database: %s\n", sqlite3_errmsg(*db));
       return rc;
   } else {
       Serial.printf("Opened database successfully\n");
   }
   return rc;
}

//destruct database 
int closeDb(sqlite3 *db){

    return sqlite3_close(db);

}


char *zErrMsg = 0;

//execute db SENTENCE
//içsel kontol mevcut
int db_exec(sqlite3 *db, const char *sql) {
   Serial.println(sql);

   //clear object list if there is any info
   objectList.clear();

    //süre ölçümü için 
   long start = micros();
   int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if (rc != SQLITE_OK) {
       Serial.printf("SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
   } else {
       Serial.printf("Operation done successfully\n");
   }
   Serial.print(F("Time taken:"));
   Serial.println(micros()-start);
   for(int i = 0;i<objectList.size();i++){

       Serial.println(objectList.at(i).objRA,8);
       Serial.println(objectList.at(i).objDEC,8);
   }


   return rc;
}