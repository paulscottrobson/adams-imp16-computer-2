//*******************************************************************************************************
//*******************************************************************************************************
//
//      Name:       Core.C (IMP16C)
//      Purpose:    CPU Core
//      Author:     Paul Robson
//      Date:       14th February 2014
//
//*******************************************************************************************************
//*******************************************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include "general.h"
#include "system.h"
#include "debugger.h"
#include "core.h"

#define RAMSIZE 	(3072)															// RAM available

static WORD16 ramMemory[RAMSIZE];													// We give it 3k words of RAM.
static BYTE8  screenMemory[32*16]; 													// Screen Memory.
static BYTE8  isVSyncInterruptEnabled = 0; 											// Non Zero if VSI running.
static BYTE8  selectDeviceFlag7 = 0;												// 1-keyboard 3-crt
static BYTE8  keyboardInputLatch;

#define CYCLESPERSECOND 	(10000000/14)											// Cycle time is 1.4us
#define FRAMERATE 			(60L) 													// Frame rate 
#define CYCLESPERFRAME 		((WORD16)(CYCLESPERSECOND/FRAMERATE))					// CPU Cycles per frame.
#define INSTRUCTIONSPERSEC 	(CYCLESPERSECOND/4L) 									// Instructions per second, roughly.
#define INSTRPEREMUFRAME 	(INSTRUCTIONSPERSEC/30) 								// About 30 refreshes a second.

#define readMemory(a) 		  ((a < RAMSIZE) ? ramMemory[a] : 0x2100)				// Memory Read/Write
#define writeMemory(a,d) 	  { if (a < RAMSIZE) ramMemory[a] = (d); }

static void writeOutput(WORD16 addressRegister,WORD16 data);
static WORD16 readInput(WORD16 addressRegister);
static void writeControlFlag(WORD16 address,WORD16 flag,BYTE8 level,WORD16 addressRegister);
static BYTE8 readConditionCodeLine(BYTE8 line);

static void loadSpaceWar(void);

#include "imp16_code.h"																// This is the system generated code 

//*******************************************************************************************************
//										Program Loading etc.
//*******************************************************************************************************

void CPUInitialise(int argc,char *argv[]) {
	if (argc == 2) {
		FILE *f = fopen(argv[1],"rb");
		if (f == NULL) exit(fprintf(stderr,"Can't open binary file %s\n",argv[1]));
		WORD16 addr = 0;
		while (!feof(f)) {
			WORD16 w = fgetc(f);
			writeMemory(addr,(w << 8) | fgetc(f));
			addr++;
		}
		fread(ramMemory,1,RAMSIZE,f);
		fclose(f);
	}
}

//*******************************************************************************************************
//											Reset the Processor
//*******************************************************************************************************

void CPUReset(void) {
	resetIMP16(); 																	// Resets the CPU
	isVSyncInterruptEnabled = 0;													// VSync Interrupt off by default.
	loadSpaceWar();
}

//*******************************************************************************************************
//								Execute one or more instructions on the TMS1100
//*******************************************************************************************************


BYTE8 CPUExecute(BYTE8 singleStep) {
	WORD16 count = (singleStep != 0) ? 1 : INSTRPEREMUFRAME;						// How many to do ?
	WORD16 breakPoint = DBGGetCurrentBreakpoint() & 0xFFFF;							// Get the current breakpoint.
	WORD16 cOpcode;
	BYTE8 runForever = (singleStep == RUN_FOREVER);									// Run forever flag.
	do {
		opcode = readMemory(pc);pc++;
		if ((opcode & 0xFC00) == 0) cOpcode = 0x100 + (opcode >> 7);
		else cOpcode = opcode >> 8;
		switch(cOpcode) {															// Execute operation code
			#include "imp16_opcodes.h"
		}

		if ((cycles & 0x8000) != 0) { 												// Cycle time out
			cycles += CYCLESPERFRAME;												// Reset it.
			CPUXSynchronise(FRAMERATE);												// Synchronise it to real speed.
			if (isVSyncInterruptEnabled) interruptIMP16();							// Cause interrupt if interrupt turned on.
		}
	}
	while (runForever || (--count > 0 && pc != breakPoint));						// Execute until breakpoint or tState counter high or once in S/S mode
																					// maybe run for ever in which case short circuit will not do check.

	return (pc == breakPoint);														// Return true if at breakpoint.
}

//*******************************************************************************************************
//											Read Input
//*******************************************************************************************************

static WORD16 readInput(WORD16 addressRegister) {
	WORD16 retVal = 0;
	if ((addressRegister & 0x7F) == 0x05 && selectDeviceFlag7 == 3) {
		WORD16 addr = addressRegister >> 7;
		retVal = screenMemory[addr];
	}
	if (selectDeviceFlag7 == 1) {
		retVal = keyboardInputLatch;
	}
	return retVal;
}

//*******************************************************************************************************
//										Write to output
//*******************************************************************************************************

static void writeOutput(WORD16 addressRegister,WORD16 data) {
	if ((addressRegister & 0x7F) == 0x2A && selectDeviceFlag7 == 3) {
		WORD16 addr = addressRegister >> 7;
		BYTE8 d8 = data & 0xFF;
		if (screenMemory[addr] != d8) {
			screenMemory[addr] = d8;
			CPUXWriteScreen(addr/16,addr%16,d8 & 0x7F);
		}
	}
}

//*******************************************************************************************************
//									Read the Condition Code Line
//*******************************************************************************************************

static BYTE8 readConditionCodeLine(BYTE8 line) {
	BYTE8 retVal = 0;
	if (selectDeviceFlag7 == 1 && line == 14) {
		retVal = (SYSIsKeyAvailable() == 0);
		if (retVal == 0) keyboardInputLatch = SYSReadKeyboard();
	}
	return retVal;
}

//*******************************************************************************************************
//										Write Control Flag
//*******************************************************************************************************

static void writeControlFlag(WORD16 address,WORD16 flag,BYTE8 level,WORD16 addressRegister) {
	if (flag == 6 && level != 0) {													// Turn VSYNC Interrupt generator on/off.
		isVSyncInterruptEnabled = (addressRegister & 0x40) ? 1 : 0;					// If zero probably also clears an interrupt latch.
	}
	if (flag == 7) {																// Pulse flag 7 selects device 
		selectDeviceFlag7 = (addressRegister & 0x7F);
	}

}

//*******************************************************************************************************
//								Load Scott's Space War into RAM Memory.
//*******************************************************************************************************

#include "spacewar.h"

static void loadSpaceWar(void) {
	WORD16 i;
	pc = 0;																			// Fix to run from $0000
	for (i = 0;i < 0x200;i++) ramMemory[i] = _spaceWar[i];							// Copy SpaceWar code into RAM.	
}

#ifdef DEBUGGABLE

//*******************************************************************************************************
//									Return the Status of the Processor
//*******************************************************************************************************

static CPUSTATUS stat;																// Used for status when NULL passed.

int _readMemory(int addr) { return readMemory(addr); }								// required because the functions are int/int.

CPUSTATUS *CPUGetStatus(CPUSTATUS *st) {
	if (st == NULL) st = &stat;
	st->ac0 = ac0;st->ac1 = ac1;st->ac2 = ac2;st->ac3 = ac3;
	st->flags = CONSTRUCTFLAGREGISTER();st->sel = sel;st->interruptEnabled = interruptEnabled;
	st->cycles = cycles;st->stack = stack;st->stackPointer = sp;
	st->pc = pc;
	st->readCodeMemory = _readMemory;
	st->readDataMemory = _readMemory;
	return st;
}

#endif
