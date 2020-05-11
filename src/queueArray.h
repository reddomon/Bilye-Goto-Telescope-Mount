//queueArray.h
//from appballard code

#ifndef _QUEUEARRAY_h
#define _QUEUEARRAY_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define BUFSIZE 12
#define MAX_CMD_SIZE 50

class QueueArray
{
protected:


public:
	void init();
	void push(const char* inStr);
	const char* pop();
	bool isEmpty();
	uint8_t getCommandCount();

private:
	char _buffer[BUFSIZE][MAX_CMD_SIZE];

	uint8_t command_queue_index_r = 0,
		command_queue_index_w = 0,
		command_count;
};

//extern QueueArray Queue;

#endif