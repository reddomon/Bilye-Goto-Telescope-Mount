#include "queueArray.h"

void QueueArray::init()
{


}

void QueueArray::push(const char * inStr)
{

	//not needed for debug purpose
	//memset((void*)&_buffer[command_queue_index_w][0], 0, sizeof(_buffer[command_queue_index_w]));
	strcpy(_buffer[command_queue_index_w], inStr);
	command_queue_index_w = (command_queue_index_w + 1) % BUFSIZE;
	if (command_count<BUFSIZE)
		command_count++;
}

const char * QueueArray::pop()
{
	const char * returnString = _buffer[command_queue_index_r];
	command_queue_index_r = (command_queue_index_r + 1) % BUFSIZE;
	command_count--;
	return returnString;
}

bool QueueArray::isEmpty()
{
	return command_count > 0 ? false : true;
}

uint8_t QueueArray::getCommandCount()
{
	return command_count;
}


//QueueArray Queue;
