


#include <WiFi.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include "wifiesp.h"
#include <Preferences.h>


AsyncWebServer server(80);
Preferences prefs;

/*
Preferences areas
NAME:wifi_credentials
String wifi_name
String wifi_password
integer wifi_mode
*/

#define WIFI_SSID_TVA "Bilye"
#define WIFI_PWD_TVA  "12345678"

#ifndef WIFI_HOSTNAME
  #define WIFI_HOSTNAME DEFAULT_WIFI_HOSTNAME
#endif

void wifi_init() {


  //SERIAL_ECHO_MSG("Starting WiFi...");

  prefs.begin("wifi",true);
  int wifiMode = prefs.getInt("wifi_mode",WIFI_MODE_AP);
  IPAddress IP;

  switch(wifiMode){

      case WIFI_MODE_AP:
          
          WiFi.mode(WIFI_AP);
          WiFi.softAP(prefs.getString("ssid",WIFI_SSID_TVA).c_str(), prefs.getString("pwd",WIFI_PWD_TVA).c_str());
          IP = WiFi.softAPIP();

          break;
      case WIFI_MODE_STA: //station mode

          WiFi.mode(WIFI_STA);
          //disconnect previous connected AP
          WiFi.disconnect();
          WiFi.begin(prefs.getString("ssidsta",WIFI_SSID_TVA).c_str(), prefs.getString("ssidpwd").c_str());


          //try connection 5 times in every 1000ms
          int connectTry = 0;
          while (!WiFi.isConnected()) {
            //SERIAL_ERROR_MSG("Unable to connect to WiFi");
            delay(1000);
            connectTry++;
            if(connectTry>5) break;
          }

          delay(10);

          if (!MDNS.begin(WIFI_HOSTNAME)) {
              //SERIAL_ERROR_MSG("Unable to start mDNS with hostname '" WIFI_HOSTNAME "', restarting.");
              delay(5000);
          }

          MDNS.addService("http", "tcp", 80);

          if(WiFi.isConnected()) ;//SERIAL_ECHOLNPAIR("Successfully connected to WiFi with SSID '" WIFI_SSID "', hostname: '" WIFI_HOSTNAME "', IP address: ", WiFi.localIP().toString().c_str());


          break;


  }

prefs.end();






  
}

