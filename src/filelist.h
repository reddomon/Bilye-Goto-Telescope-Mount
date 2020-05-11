#pragma once


#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include <SPI.h>
#include "config.h"
  
  
  class FileList {
    private:
    uint16_t num_files;
	  File root;
	  std::vector<String> list;

    public:
      FileList();
      void refresh();
      String seek(const uint16_t num);

      const char *filename();
      bool isDir();

      void changeDir(const char * const dirname);
      void upDir();
      bool isAtRootDir();
      uint16_t    count();
  };

