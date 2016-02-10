#ifndef __USERPROG__
#define __USERPROG__

#define BUFFER_SIZE 10
#define FILENAME_PROD "producer.dlx.obj"
#define FILENAME_CONS "consumer.dlx.obj"

typedef struct circ_buffer
{
	char mem_buffer[BUFFER_SIZE];
	uint8 head;
	uint8 tail;
}


#endif