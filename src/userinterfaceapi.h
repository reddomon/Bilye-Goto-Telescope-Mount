#pragma once

#include <datatypes.h>
#include <queueArray.h>

//gettin mount general status
extern MountStatus mountstatus;
extern ObservingObject observingobject;
extern QueueArray arrayBuffer;

void onIdle();
void onStartup();
void onMountKilled(PGM_P const msg);

void onMediaInserted();
void onMediaError();
void onMediaRemoved();
void onPlayTone(const uint16_t frequency, const uint16_t duration);
void onUserConfirmRequired(const char *const msg);
void onStatusChanged(const char *const msg);
void onFactoryReset();

void onStoreSettings(char *buff);
void onLoadSettings(const char *buff);
void onConfigurationStoreWritten(bool success);
void onConfigurationStoreRead(bool success);

void onDataReceiveStarted();
void onDataReceiveFinished();

