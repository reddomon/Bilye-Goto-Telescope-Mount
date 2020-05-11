


#include <SPIFFS.h>
#include "wifiesp.h"
#include "SD.h"



//root file handler
File rootF;
unsigned char bufferT[40000];
int signer = 0;
int counterSD = 0;



AsyncEventSource events("/events"); // event source (Server-Sent events)

void onNotFound(AsyncWebServerRequest *request){
  request->send(404);
}

void web_init() {

//these settings compatible with marlin
//these settings have to be used because upload file sd libary not based on Marlin's

  if(!SD.begin(5,SPI,16000000U,"/sd",10)){
        Serial.println("Card Mount Failed");
        return;
    }
  pinMode(LED_BUILTIN, OUTPUT);


  //upload file handler
  //sd card write routine have to check

  server.on( "/api/upload", HTTP_POST, []( AsyncWebServerRequest * request )
  {



    if ( request->authenticate( "admin", "admin" ) )
    {
      request->send( 200 );
    }
    else
    {
      request->requestAuthentication();
    }
  },
  []( AsyncWebServerRequest * request, String filename, size_t index, uint8_t *data, size_t len, bool final )
  {
    static bool   _authenticated;
    static time_t startTimer;



    if ( !index )
    {
      _authenticated = false;
      if ( request->authenticate("admin", "admin") )
      {
        startTimer = millis();
        //flushableSerial.printf( "UPLOAD: Started to receive '%s'.\n", filename.c_str() );
        digitalWrite(LED_BUILTIN, 1);
        String fileName= "/"+ filename;

        //screen set to data receive
        //ExtUI::onDataReceiveStarted();

        //allocate memory
        //bufferT= (unsigned char*)malloc(21000);
        
        //open file on write mode
        //card.openFile((char*) fileName.c_str(), false);
         rootF = SD.open(fileName, FILE_WRITE);
         //clean buffer
        // for(int j=0;j<13000;j++) {
         //  bufferT[j] = 0;
        //}
        _authenticated = true;
      }
      else
      {
        Serial.println( "Unauthorized access." );
        return request->send( 401, "text/plain", "Not logged in." );
      }      
    }

    if ( _authenticated )
    {
      //data receive indicator
      digitalWrite(LED_BUILTIN, 1);
      //flushableSerial.printf( "%i bytes received.\n", index );
      //flushableSerial.printf( "%i bytes received.\n", len );
      //Store or do something with the data...
      //write 1 chunk
      //card.write(data,len);
      //rootF.write(data,len);

        //buffer receiving data   
        for(int i = 0;i<len;i++,signer++){
            bufferT[signer]= data[i];
            
        }
        counterSD++;
        if(counterSD==27){
          
          
          rootF.write(bufferT,signer);
          //card.write(bufferT,signer);
          //flushableSerial.printf( "%i bytes written.\n", signer );
          counterSD = 0;
          signer = 0;
          //rootF.flush();
        }
        //data receive indicator
        digitalWrite(LED_BUILTIN, 0);

        //write remaining data
        if(final){

            rootF.write(bufferT,signer);
            counterSD = 0;
            signer = 0;
        }

    }
    
    if ( final && _authenticated )
    {
      digitalWrite(LED_BUILTIN, 0);
      //flushableSerial.printf( "UPLOAD: Done. Received %.2f kBytes in %.2fs which is %i kB/s.\n", index / 1024.0, ( millis() - startTimer ) / 1000.0, index / ( millis() - startTimer ) );

      //screen set from data receive to mainpage
      //data recceive finished signal
      //ExtUI::onDataReceiveFinished();

      //flushableSerial.printf(String(ESP.getFreeHeap()).c_str());
      //close  file object
      rootF.close();
      //free(bufferT);
      //card.closefile();
    }
  });

//upload page handler JQUERY retrieve from internet. in AP mode doesnt work
//TODO: gzipped embedded jquery
  server.on("/uploadpage", HTTP_GET, [](AsyncWebServerRequest *request){
  String uppage = 
"<!doctype HTML>\n"
"<!-- https://stackoverflow.com/questions/15410265/file-upload-progress-bar-with-jquery -->\n"
"<html lang=\"en\">\n"
"<head>\n"
"<title>TEST UPLOAD</title>\n"
"<meta charset=\"utf-8\">\n"
"<link rel=\"icon\" href=\"data:;base64,iVBORw0KGgo=\">  <!--prevent favicon requests-->\n"
"<script src=\"https://code.jquery.com/jquery-3.2.1.js\"></script>\n"
"<style>\n"
"button{\n"
"  margin:10px;\n"
"}\n"
"#uploadForm {\n"
"    width: 350px;\n"
"    margin: 0 auto;\n"
"    background-color: beige;\n"
"    padding: 10px;\n"
"    text-align:center;\n"
"}\n"
"#uploadForm #fileSelection{\n"
"    width: 100%;\n"
"    height: 50px;\n"
"    border: solid 1px lightgrey;\n"
"}\n"
"#uploadProgressBar{\n"
"    margin: 10px;\n"
"}\n"
"#uploadFileButton{\n"
"    width: 100px;\n"
"    height: 30px;\n"
"    margin: 10px;\n"
"}\n"
"</style>\n"
"</head>\n"
"<body>\n"
"  <form id=\"uploadForm\" enctype=\"multipart/form-data\" method=\"post\" action=\"api/upload\">\n"
"  <input id=\"fileSelection\" name=\"file\" type=\"file\" />\n"
"  <progress id=\"uploadProgressBar\" value=\"0\" max=\"0\"></progress>\n"
"  <p id=\"status\"></p>\n"
"  <input id=\"uploadFileButton\" type=\"submit\" value=\"Upload\" disabled=\"disabled\"/>\n"
"  </form>\n"
"<script type=\"text/javascript\">\n"
"const pickFile_Message = \"Click or drop a file in the box\";\n"
"$(\"#status\").html( pickFile_Message );\n"
"function uploadProgressHandler(event)\n"
"{\n"
"  //$(\"#loaded_n_total\").html(\"Uploaded \"+event.loaded+\" bytes of \"+event.total);\n"
"  var percent = (event.loaded / event.total) * 100;\n"
"  var progress = Math.round(percent);\n"
"  $(\"#status\").html(progress +\"% uploaded... please wait\");\n"
"  $( '#uploadProgressBar' ).attr({\n"
"    value: event.loaded,\n"
"    max: event.total\n"
"  });\n"
"}\n"
"function loadHandler(event)\n"
"{\n"
"  if ( event.target.responseText )\n"
"  {\n"
"    $(\"#status\").html( event.target.responseText );\n"
"  }\n"
"  else\n"
"  {\n"
"    $(\"#status\").html( pickFile_Message );\n"
"  }\n"
"  $('#uploadFileButton').val(\"Upload\");\n"
"  $( '#uploadProgressBar' ).attr({\n"
"    value: 0,\n"
"    max: 0\n"
"  });\n"
"}\n"
"function errorHandler(event){\n"
"  $(\"#status\").html(\"Upload failed\");\n"
"}\n"
"function abortHandler(event){\n"
"  $(\"#status\").html(\"Upload aborted\");\n"
"}\n"
"$('#fileSelection').on( 'change', function(){\n"
"  if ( $('#fileSelection').val() != \"\" ) {\n"
"    $('#uploadFileButton').val(\"Upload\").prop( 'disabled', false );\n"
"  }\n"
"  else\n"
"  {\n"
"    $('#uploadFileButton').val(\"Upload\").prop( 'disabled', true );\n"
"  }\n"
"});\n"
"$(\"#uploadFileButton\").click(function(event)\n"
"{\n"
"  event.preventDefault();\n"
"  if ( $( '#fileSelection' ).val() == \"\" ) { return; }\n"
"  //TODO: first do a post without data to see if we are logged in\n"
"  $.post( '/api/upload', function() {\n"
"    //alert( \"success\" );\n"
"  })\n"
"    .done(function() {\n"
"      $('#uploadFileButton').val(\"Uploading...\").prop( 'disabled', true );\n"
"      var file = $(\"#fileSelection\")[0].files[0];\n"
"      $(\"#fileSelection\").val(\"\");\n"
"      var formData = new FormData();\n"
"      formData.append(\"file1\", file);      \n"
"      $.ajax({\n"
"        url: '/api/upload',\n"
"        method: 'POST',\n"
"        type: 'POST',\n"
"        data: formData,\n"
"        contentType: false,\n"
"        processData: false,\n"
"        xhr: function()\n"
"        {\n"
"          var xhr = new window.XMLHttpRequest();\n"
"          xhr.upload.addEventListener(\"progress\",\n"
"                                      uploadProgressHandler,\n"
"                                      false\n"
"                                     );\n"
"          xhr.addEventListener(\"load\", loadHandler, false);\n"
"          xhr.addEventListener(\"error\", errorHandler, false);\n"
"          xhr.addEventListener(\"abort\", abortHandler, false);\n"
"          return xhr;\n"
"        }\n"
"      });\n"
"    })\n"
"    .fail(function() {\n"
"      $('#status').html( \"Not logged in.\" );\n"
"    });\n"
"});\n"
"</script>\n"
"</body>";
  request->send(200, "text/html", uppage);
  uppage = String();
});

  server.begin();
}


