//*******************************************************************************************************
//*******************************************************************************************************
//
//      Name:       Core_sdl.C (8008 Machine)
//      Purpose:    CPU Core (SDL Specific Code, some debugger code.)
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

static void replace(char *str,char *find,char *replace);

//*******************************************************************************************************
//									Get Instruction Mnemonic and Length
//*******************************************************************************************************

#include "imp16_mnemonic.h"

int CPUGetInstructionInfo(int address,char *buffer) {
	CPUSTATUS *s = CPUGetStatus(NULL);
	int opcode = s->readCodeMemory(address); 										// Get the opcode
	char *p = _mnemonics[opcode >> 8];												// Get the mnemonic
	if ((opcode & 0xFC00) == 0) p = _mnemonics[(opcode >> 7)|0x100];
	char subst[16];
	int operand = opcode & 0xFF;
	if (operand & 0x80) operand = operand-256;
	if (buffer != NULL) {
		strcpy(buffer,p);
		sprintf(subst,"%x",(address + 1 + operand) & 0xFFFF);
		replace(buffer,"#r",subst);
		sprintf(subst,"%x",operand);
		if (operand & 0x80) sprintf(subst,"-%x",abs(operand));
		replace(buffer,"#8",subst);
		sprintf(subst,"%x",abs(operand));
		replace(buffer,"#q",subst);
		sprintf(subst,"%x",opcode & 0xFF);
		replace(buffer,"#i",subst);
		sprintf(subst,"%s",(opcode & 0x80) ? "r":"l");
		replace(buffer,"#d",subst);
		sprintf(subst,"%s",(opcode & 0x80) ? "p":"s");
		replace(buffer,"#f",subst);
		sprintf(subst,"%x",opcode & 0x7F);
		replace(buffer,"#7",subst);

		int rcmd = opcode & 0x83;
		if (rcmd < 4) replace(buffer,"#x","radd");
		if (rcmd == 0x80) replace(buffer,"#x","rxch");
		if (rcmd == 0x81) replace(buffer,"#x","rcpy");
		if (rcmd == 0x82) replace(buffer,"#x","rxor");
		if (rcmd == 0x83) replace(buffer,"#x","rand");
	}
	return 1;
}

//*******************************************************************************************************
//											String Replace
//*******************************************************************************************************

static char rBuffer[64];

static void replace(char *str,char *find,char *replace) {
	char *p = strstr(str,find);
	if (p != NULL) {
		strcpy(rBuffer,str);
		strcpy(rBuffer+(p-str),replace);
		strcpy(rBuffer+(p-str)+strlen(replace),p+2);
		strcpy(str,rBuffer);
	}
}

//*******************************************************************************************************
//									Get code and data address masks
//*******************************************************************************************************

int CPUGetCodeAddressMask(void) { return 0xFFFF; }
int CPUGetDataAddressMask(void) { return 0xFFFF; }

//*******************************************************************************************************
//											CPU Debug Renderer
//*******************************************************************************************************

void CPURenderDebug(SDL_Rect *rcDisplay,int codeAddress,int dataAddress) {
	rcDisplay->x = 54;rcDisplay->y = 2;rcDisplay->w = 99-rcDisplay->x;rcDisplay->h = 48; 		// Set rectangle for screen in percentages.
	char *labels[] = { "AC0","AC1","AC2","AC3","LNK","CRY","OVF","SEL","IEN","FLG","PC","BP","CYC",NULL };
	CPUSTATUS *s = CPUGetStatus(NULL);
	DBGUtilsLabels(19,0,labels,2);
	int n = 0;
	int x = 23;
	SYSWriteHex(x,n++,s->ac0,3,4);
	SYSWriteHex(x,n++,s->ac1,3,4);
	SYSWriteHex(x,n++,s->ac2,3,4);
	SYSWriteHex(x,n++,s->ac3,3,4);
	SYSWriteHex(x,n++,(s->flags >> 15) & 1,3,1);
	SYSWriteHex(x,n++,(s->flags >> 13) & 1,3,1);
	SYSWriteHex(x,n++,(s->flags >> 14) & 1,3,1);
	SYSWriteHex(x,n++,s->sel,3,1);
	SYSWriteHex(x,n++,s->interruptEnabled,3,1);
	SYSWriteHex(x,n++,s->flags,3,4);
	SYSWriteHex(x,n++,s->pc,3,4);
	SYSWriteHex(x,n++,DBGGetCurrentBreakpoint(),3,4);
	SYSWriteHex(x,n++,s->cycles,3,4);
	DBGUtilsDisassemble(0,0,15,codeAddress,s->pc);
	DBGUtilsDisplay(6,17,24,dataAddress,8,4,FALSE,-1);
	SYSWriteString(0,17,"Stack",2);
	for (n = 0;n < 6;n++) {
		int ss = s->stackPointer - 1 - n;
		if (ss >= 0)
			SYSWriteHex(1,n+18,s->stack[ss],3,4);
	}
}

static DisplaySurface mainDisplay,fontDisplay;										// Surface for screen display.

//*******************************************************************************************************
//									Initialise resources for the display
//*******************************************************************************************************

#include "impfont.h"

void CPUInitialiseResources(void) {
	SYSCreateSurface(&mainDisplay,32*6,16*8);										// Display is 32 x 16 lots of 5 x 7 characters in an 6 x 8 box.
	SYSCreateSurface(&fontDisplay,32*5,4*7);
	Uint32 col = SDL_MapRGB(fontDisplay.surface->format,0,255,0);
	SDL_Rect rc;rc.w = rc.h = 1;
	for (int x = 0;x < 32*5;x++) {
		for (int y = 0;y < 4*7;y++) {
			BYTE8 ch = (x/5)+(y/7)*32;										
			BYTE8 data = _signeticsFont[ch*7+(y % 7)];
			data = data & (0x80 >> (x % 5));
			rc.x = x;rc.y = y;
			if (data) SDL_FillRect(fontDisplay.surface,&rc,col);
		}
	}
	//for (int x = 0;x < 32;x++) for (int y = 0;y < 32;y++) CPUXWriteScreen(x,y,x+y*4);
}

//*******************************************************************************************************
//										Get the Display Surface
//*******************************************************************************************************

DisplaySurface *CPUGetDisplaySurface(void) {
    return &mainDisplay;
}

void CPUXWriteScreen(BYTE8 x,BYTE8 y,BYTE8 ch) {
	if (x >= 32 || y >= 16) return;
	SDL_Rect rcSrc,rcTgt;
	ch = ch & 0x7F;
	rcSrc.x = ch % 32 * 5;rcSrc.y = ch / 32 * 7;rcSrc.w = 5;rcSrc.h = 7;
	rcTgt.x = x * 6;rcTgt.y = y * 8;rcTgt.w = 5;rcTgt.h = 7;
	SDL_BlitSurface(fontDisplay.surface,&rcSrc,mainDisplay.surface,&rcTgt);
}


//*******************************************************************************************************
//									Any special ASCII keyboard keys ?
//*******************************************************************************************************

int CPUScanCodeToASCIICheck(int scanCode) {
	return -1;
}

//*******************************************************************************************************
//								Synchronise with an external clock
//*******************************************************************************************************

void CPUXSynchronise(WORD16 frequency) {
	SYSSynchronise(frequency);
}

