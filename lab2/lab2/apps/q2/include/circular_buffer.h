#ifndef __circular_buffer_h_
#define __circular_buffer_h_

#define FILENAME_PROD "producer.dlx.obj"
#define FILENAME_CONS "consumer.dlx.obj"

typedef struct circ_buffer
{
	char mem_buffer[BUFFERSIZE];
	int head;
	int tail;
};

#endif