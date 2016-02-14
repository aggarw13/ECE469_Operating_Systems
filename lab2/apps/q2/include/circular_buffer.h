#ifndef __USERPROG__
#define __USERPROG__

#define FILENAME_PROD "producer.dlx.obj"
#define FILENAME_CONS "consumer.dlx.obj"

typedef struct circ_buffer
{
	char mem_buffer[BUFFERSIZE];
	int head;
	int tail;
}circ_buffer;

#endif