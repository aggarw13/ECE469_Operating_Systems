#include "ostraps.h"
#include "dlxos.h"
#include "process.h"
#include "synch.h"
#include "queue.h"
#include "mbox.h"

static mbox MailBox[MBOX_NUM_MBOXES];
static mbox_message Messg_Buffers[MBOX_NUM_BUFFERS];
static int used_buffers = 0;

int i,j;

//-------------------------------------------------------
//
// void MboxModuleInit();
//
// Initialize all mailboxes.  This process does not need
// to worry about synchronization as it is called at boot
// time.  Only initialize necessary items here: you can
// initialize others in MboxCreate.  In other words, 
// don't waste system resources like locks and semaphores
// on unused mailboxes.
//
//-------------------------------------------------------
void MboxModuleInit() {
	for(i = 0; i < MBOX_NUM_MBOXES; i++)
	{
		MailBox[i].inuse = 0;
	}

	for(i = 0; i < MBOX_NUM_BUFFERS; i++)
	{
		Messg_Buffers[i].size = 0;
	}
}

//-------------------------------------------------------
//
// mbox_t MboxCreate();
//
// Allocate an available mailbox structure for use. 
//
// Returns the mailbox handle on success
// Returns MBOX_FAIL on error.
//
//-------------------------------------------------------
mbox_t MboxCreate() {

	mbox_t mbox_no = MBOX_NUM_MBOXES;
	for(i = 0; i < MBOX_NUM_MBOXES;i++)
	{
		if(MailBox[i].inuse == false)
		{
			mbox_no = i;
			break;
		}
	}

	if(mbox_no == MBOX_NUM_MBOXES)
	{
		printf("MailBox cannot be assigned to the calling process : %d\n", GetCurrentPid());
		return MBOX_FAIL;
	}

	MailBox[mbox_no].inuse = true;

	if((MailBox[mbox_no].lock = LockCreate()) == SYNC_FAIL)
	{
		printf("MailBox cannot associate a lock with itself for calling process : %d\n", GetCurrentPid());
		return MBOX_FAIL;
	}

	if((MailBox[mbox_no].moreData = CondCreate(MailBox[mbox_no].lock)) == SYNC_FAIL)
	{
		printf("MailBox cannot associate a cond var for buffer emptiness calling process : %d\n", GetCurrentPid());
		return MBOX_FAIL;
	}

	if((MailBox[mbox_no].moreSpace = CondCreate(MailBox[mbox_no].lock)) == SYNC_FAIL)
	{
		printf("MailBox cannot associate a cond var for buffer saturation for calling process : %d\n", GetCurrentPid());
		return MBOX_FAIL;
	}

	if(AQueueInit(&MailBox[mbox_no].buffers) == QUEUE_FAIL)
	{
		printf("FATAL Error : Available mailbox : %d cannot have its buffer queue initialized for process : %d\n", mbox_no, GetCurrentPid());
		return MBOX_FAIL;
		//exitsim();
	}

	for(i = 0; i < PROCESS_MAX_PROCS; i++)
	{
		MailBox[mbox_no].procs_link[i] = 0;
	}

	//MailBox[mbox_no].tail = 0; 
	//MailBox[mbox_no].head = 1;
	MailBox[mbox_no].process_count = 0;


	//printf("Created mailbox with handle : %d\n", mbox_no);
	return mbox_no;
}

//-------------------------------------------------------
// 
// void MboxOpen(mbox_t);
//
// Open the mailbox for use by the current process.  Note
// that it is assumed that the internal lock/mutex handle 
// of the mailbox and the inuse flag will not be changed 
// during execution.  This allows us to get the a valid 
// lock handle without a need for synchronization.
//
// Returns MBOX_FAIL on failure.
// Returns MBOX_SUCCESS on success.
//
//-------------------------------------------------------
int MboxOpen(mbox_t handle) {
	int intrs;

	if(MailBox[handle].inuse == false)
	{
		printf("Currently passed mailbox handle : %d by calling process : %d is unallocated\n", handle ,GetCurrentPid());
		return MBOX_FAIL;
	}

	intrs = DisableIntrs ();

	MailBox[handle].process_count++;
	MailBox[handle].procs_link[GetCurrentPid()] = 1;
		

	RestoreIntrs (intrs);
	return MBOX_SUCCESS;
}

//-------------------------------------------------------
//
// int MboxClose(mbox_t);
//
// Close the mailbox for use to the current process.
// If the number of processes using the given mailbox
// is zero, then disable the mailbox structure and
// return it to the set of available mboxes.
//
// Returns MBOX_FAIL on failure.
// Returns MBOX_SUCCESS on success.
//
//-------------------------------------------------------
int MboxClose(mbox_t handle) {

	int intrs;

	if(MailBox[handle].inuse == 0)
	{
		printf("Currently passed mailbox handle : %d by calling process : %d is unallocated\n", handle, GetCurrentPid());
		return MBOX_FAIL;
	}

	intrs = DisableIntrs();

	MailBox[handle].procs_link[GetCurrentPid()] = 0;
	MailBox[handle].process_count--;
	if(MailBox[handle].process_count == false)
		MailBox[handle].inuse = 0;

	RestoreIntrs(intrs);

  return MBOX_SUCCESS;
}

//-------------------------------------------------------
//
// int MboxSend(mbox_t handle,int length, void* message);
//
// Send a message (pointed to by "message") of length
// "length" bytes to the specified mailbox.  Messages of
// length 0 are allowed.  The call 
// blocks when there is not enough space in the mailbox.
// Messages cannot be longer than MBOX_MAX_MESSAGE_LENGTH.
// Note that the calling process must have opened the 
// mailbox via MboxOpen.
//
// Returns MBOX_FAIL on failure.
// Returns MBOX_SUCCESS on success.
//
//-------------------------------------------------------
int MboxSend(mbox_t handle, int length, void* message) {

	int intrs;
	int wasEmpty = 0;
	Link * mbuffer_link;

	if(MailBox[handle].inuse == 0)
	{
		printf("Currently passed mailbox handle : %d by calling process : %d is unallocated\n", handle, GetCurrentPid());
		return MBOX_FAIL;
	}

	if(MailBox[handle].procs_link[GetCurrentPid()] == false)
	{
		printf("Mailbox : %d not already opened by calling proces :%d\n", handle, GetCurrentPid());
		return MBOX_FAIL;
	}

	intrs = DisableIntrs();

	LockHandleAcquire(MailBox[handle].lock);

	while(AQueueLength(&MailBox[handle].buffers) == MBOX_MAX_BUFFERS_PER_MBOX || used_buffers == MBOX_NUM_BUFFERS)
	{
		CondHandleWait(MailBox[handle].moreSpace);
	}

	//printf("Buffer size of queue before inserting messager by : %d is : %d\n", GetCurrentPid(), AQueueLength(&MailBox[handle].buffers));

	if(AQueueLength(&MailBox[handle].buffers) == 0)
		wasEmpty = 1;

	if(AQueueLength(&MailBox[handle].buffers) != MBOX_MAX_BUFFERS_PER_MBOX)
	{
		if(length > MBOX_MAX_MESSAGE_LENGTH)
		{
			printf("Messge passed by user process : %d larger than accepted message length (Messge length sent - %d)", GetCurrentPid(), length);
			return MBOX_FAIL;
		}
	
		bcopy(message, Messg_Buffers[used_buffers++].message, 8); 
		
		Messg_Buffers[used_buffers - 1].size = length;

		//printf("Original Message : %s Copied  : %s in : %d\n", (char *)message, (char *)(Messg_Buffers[used_buffers - 1].message), handle);

		if((mbuffer_link = AQueueAllocLink(&Messg_Buffers[used_buffers - 1])) == QUEUE_FAIL)
		{
			printf("FATAL Error : Link object could not be created for message buffer : %d in process : %d",used_buffers - 1, GetCurrentPid());
			exitsim();
		}

		if(AQueueInsertLast(&MailBox[handle].buffers, mbuffer_link) != QUEUE_SUCCESS)
		{
			printf("FATAL Error : Link object could not be created for message buffer : %d in process : %d",used_buffers - 1, GetCurrentPid());
			exitsim();	
		}
	}

	//printf("Message inserted by process : %d using buffer : %d with current count : %d\n", GetCurrentPid(), buffer_no, AQueueLength(&MailBox[handle].buffers));

	//if(wasEmpty)
	CondHandleSignal(MailBox[handle].moreData);

	LockHandleRelease(MailBox[handle].lock);

	RestoreIntrs(intrs);

  return MBOX_SUCCESS;
}

//-------------------------------------------------------
//
// int MboxRecv(mbox_t handle, int maxlength, void* message);
//
// Receive a message from the specified mailbox.  The call 
// blocks when there is no message in the buffer.  Maxlength
// should indicate the maximum number of bytes that can be
// copied from the buffer into the address of "message".  
// An error occurs if the message is larger than maxlength.
// Note that the calling process must have opened the mailbox 
// via MboxOpen.
//   
// Returns MBOX_FAIL on failure.
// Returns number of bytes written into message on success.
//
//-------------------------------------------------------
int MboxRecv(mbox_t handle, int maxlength, void* message) {
  	
	int intrs;
	int wasFull = 0, buffersSaturated = 0;
	mbox_message * user_mesg;
	Link *l;
	
	if(MailBox[handle].inuse == false)
	{
		printf("Currently passed mailbox handle : %d by calling process : %d is unallocated\n", handle, GetCurrentPid());
		return MBOX_FAIL;
	}

	if(MailBox[handle].procs_link[GetCurrentPid()] == false)
	{
		printf("Mailbox : %d not already opened by calling proces :%d\n", handle, GetCurrentPid());
		return MBOX_FAIL;
	}

	intrs = DisableIntrs();

	LockHandleAcquire(MailBox[handle].lock);

	while(AQueueLength(&MailBox[handle].buffers) == 0)
	{
		//printf("Waiting for mailbox to have more messages\n");
		CondHandleWait(MailBox[handle].moreData);
	}

	//printf("Received mailbox lock by calling process : %d with mailbox buffer count : %d\n", GetCurrentPid(), AQueueLength(&MailBox[handle].buffers));

	if(AQueueLength(&MailBox[handle].buffers) == MBOX_MAX_BUFFERS_PER_MBOX)
		wasFull = 1;

	if(AQueueLength(&MailBox[handle].buffers) < MBOX_MAX_BUFFERS_PER_MBOX && used_buffers == MBOX_NUM_BUFFERS)
		buffersSaturated = 1;

	//printf("Obtained message in receiver : %d from buffer index : %d\n", user_mesg->size, (user_mesg - Messg_Buffers));

	if((user_mesg = (mbox_message *)AQueueObject(MailBox[handle].buffers.first)) == NULL)
	{
		printf("Undefined/unallocated Link pointer obtained from mailbox queue with handle :%d in process : %d\n", handle, GetCurrentPid());
		//printf("MailBox buffer size : %d\n", AQueueLength(&MailBox[handle].buffers));
		return MBOX_FAIL;
	}
		
	if(maxlength < user_mesg->size)
	{
		printf("Message : %s requested by user process : %d larger than acceptable message length : %d (Messge length received : %d)\n", user_mesg->message, GetCurrentPid(), maxlength, user_mesg->size);
		return MBOX_FAIL;
	}

	bcopy(user_mesg->message, message, user_mesg->size); 

	l = MailBox[handle].buffers.first;
	MailBox[handle].buffers.first = AQueueNext(MailBox[handle].buffers.first);
	if(AQueueRemove(&l) == QUEUE_FAIL)
	{
		printf("FATAL Error : Message object Link for buffer : %d received by process : %d could not be removed from queue of mailbox handle : %d",used_buffers, GetCurrentPid(), handle);
		exitsim();
	}

	used_buffers--;

	//if(wasFull || buffersSaturated)
	CondHandleSignal(MailBox[handle].moreSpace);

	LockHandleRelease(MailBox[handle].lock);

	RestoreIntrs(intrs);

  return user_mesg;
}


//--------------------------------------------------------------------------------
// 
// int MboxCloseAllByPid(int pid);
//
// Scans through all mailboxes and removes this pid from their "open procs" list.
// If this was the only open process, then it makes the mailbox available.  Call
// this function in ProcessFreeResources in process.c.
//
// Returns MBOX_FAIL on failure.
// Returns MBOX_SUCCESS on success.
//
//--------------------------------------------------------------------------------
int MboxCloseAllByPid(int pid) {
  
  for(i = 0; i < MBOX_NUM_MBOXES; i++)
  {
  		if(MailBox[i].inuse && MailBox[i].procs_link[pid] == true)
  		{
  			MailBox[i].procs_link[pid] = false;

  			if(MailBox[i].process_count == false)
				MailBox[i].inuse = 0;
   		}
   }

  return MBOX_SUCCESS;
}
