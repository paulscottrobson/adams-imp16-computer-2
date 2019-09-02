//*******************************************************************************************************
//*******************************************************************************************************
//
//      Name:       Core.H (IMP16C)
//      Purpose:    CPU Core Header
//      Author:     Paul Robson
//      Date:       8th February 2014
//
//*******************************************************************************************************
//*******************************************************************************************************

#ifndef _CORE_H
#define _CORE_H

#ifdef DEBUGGABLE

#define EMUNAME 			"IMP-16 Computer"

#define T_WIDTH 	(50)
#define T_HEIGHT 	(25)

typedef struct _CPUStatus {
	WORD16 ac0,ac1,ac2,ac3,flags,sel,interruptEnabled,cycles,*stack,stackPointer;
	WORD16 pc;
	int (*readCodeMemory)(int address);
	int (*readDataMemory)(int address);
} CPUSTATUS;

CPUSTATUS *CPUGetStatus(CPUSTATUS *s);
int CPUGetInstructionInfo(int address,char *buffer);
int CPUGetCodeAddressMask(void);
int CPUGetDataAddressMask(void);
void CPURenderDebug(SDL_Rect *rc,int codeAddress,int dataAddress);
DisplaySurface *CPUGetDisplaySurface(void);
void CPUInitialiseResources(void);
int CPUScanCodeToASCIICheck(int scanCode);
void CPUInitialise(int argc,char *argv[]);

#endif

#define RUN_FOREVER 	(0xAA) 				// Modes for running processor.
#define SINGLE_STEP 	(0x01)
#define RUN_FRAME 		(0x00)

void CPUReset(void);
BYTE8 CPUExecute(BYTE8 singleStep);

void CPUXSynchronise(WORD16 frequency);
void CPUXWriteScreen(BYTE8 x,BYTE8 y,BYTE8 ch);
#endif
