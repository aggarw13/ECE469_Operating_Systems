#ifndef __circular_buffer_h_
#define __circular_buffer_h_

#define BUFFER_SIZE 10
#define FILENAME_PROD "producer.dlx.obj"
#define FILENAME_CONS "consumer.dlx.obj"

typedef struct circ_buffer
{
	char mem_buffer[BUFFER_SIZE];
	int head;
	int tail;
};

#endif