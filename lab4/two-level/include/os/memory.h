#ifndef	_memory_h_
#define	_memory_h_

// Put all your #define's in memory_constants.h
#include "memory_constants.h"

#define FREEMAP_PAGENO_OFFSET_MASK (uint32)0x1f

extern int lastosaddress; // Defined in an assembly file

//--------------------------------------------------------
// Existing function prototypes:
//--------------------------------------------------------

int MemoryGetSize();
void MemoryModuleInit();
uint32 MemoryTranslateUserToSystem (PCB *pcb, uint32 addr);
int MemoryMoveBetweenSpaces (PCB *pcb, unsigned char *system, unsigned char *user, int n, int dir);
int MemoryCopySystemToUser (PCB *pcb, unsigned char *from, unsigned char *to, int n);
int MemoryCopyUserToSystem (PCB *pcb, unsigned char *from, unsigned char *to, int n);
int MemoryPageFaultHandler(PCB *pcb);

//---------------------------------------------------------
// Put your function prototypes here
//---------------------------------------------------------
int MemoryAllocPage ();
void MemoryFreePage(uint32);
uint32 MemorySetupPte (uint32);
uint32* getFreeL2pagetable(int);
void checkAndAllocateL2pagetable(PCB*, uint32);
void FreeL2PagetablesForProc(int);

typedef struct l2_pagetable {
  int pid;
  uint32 inuse;
  uint32 table[MEM_L2TABLE_SIZE];
} l2_pagetable;

#endif	// _memory_h_
