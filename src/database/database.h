#pragma once

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "datatypes.h"

static int callback(void *data, int argc, char **argv, char **azColName);
int openDb(const char *filename, sqlite3 **db);
int closeDb(sqlite3 *db);
int db_exec(sqlite3 *db, const char *sql);



//database scheme consists of these data types row by row