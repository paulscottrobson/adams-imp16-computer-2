case 0x000: /**** 0000 halt ****/
	HALT = 1;
	break;

case 0x001: /**** 0080 pushf ****/
	FLAGS = FLAGS & 0x1FFF;
	if (CY) FLAGS |= 0x2000;
	if (OV) FLAGS |= 0x4000;
	if (L) FLAGS |= 0x8000;
	PUSH(FLAGS);
	break;

case 0x002: /**** 0100 rti %7 ****/
	PC = PULL() + (IR & 0x7F);
	INTEN = 1;
	break;

case 0x004: /**** 0200 rts %7 ****/
	PC = PULL() + (IR & 0x7F);
	break;

case 0x005: /**** 0280 pullf ****/
	FLAGS = PULL();
	CY = (FLAGS >> 13) & 1;
	OV = (FLAGS >> 14) & 1;
	L = (FLAGS >> 15) & 1;
	break;

case 0x007: /**** 0380 jsri %7 ****/
	PUSH(PC);
	PC = IR | 0xFF80;
	break;

case 0x008: /**** 0400 rin %7 ****/
	MAR = (IR & 0x7F) + AC3;
	AC0 = READIO(MAR);
	break;

case 0x00c: /**** 0600 rout %7 ****/
	MAR = (IR & 0x7F) + AC3;
	WRITEIO(MAR,AC0);
	break;

case 0x010: /**** 0800 sflg f8 ****/
	MDR = (IR >> 8) & 7;
	SETFLAG(MDR);
	if (MDR == 1) INTEN = 1;
	if (MDR == 2) SEL = 1;
	break;

case 0x011: /**** 0880 pflg f8 ****/
	MDR = (IR >> 8) & 7;
	PULSEFLAG(MDR);
	if (MDR == 1) INTEN = 0;
	if (MDR == 2) SEL = 0;
	break;

case 0x012: /**** 0900 sflg inten ****/
	MDR = (IR >> 8) & 7;
	SETFLAG(MDR);
	if (MDR == 1) INTEN = 1;
	if (MDR == 2) SEL = 1;
	break;

case 0x013: /**** 0980 pflg inten ****/
	MDR = (IR >> 8) & 7;
	PULSEFLAG(MDR);
	if (MDR == 1) INTEN = 0;
	if (MDR == 2) SEL = 0;
	break;

case 0x014: /**** 0a00 sflg sel ****/
	MDR = (IR >> 8) & 7;
	SETFLAG(MDR);
	if (MDR == 1) INTEN = 1;
	if (MDR == 2) SEL = 1;
	break;

case 0x015: /**** 0a80 pflg sel ****/
	MDR = (IR >> 8) & 7;
	PULSEFLAG(MDR);
	if (MDR == 1) INTEN = 0;
	if (MDR == 2) SEL = 0;
	break;

case 0x016: /**** 0b00 sflg f11 ****/
	MDR = (IR >> 8) & 7;
	SETFLAG(MDR);
	if (MDR == 1) INTEN = 1;
	if (MDR == 2) SEL = 1;
	break;

case 0x017: /**** 0b80 pflg f11 ****/
	MDR = (IR >> 8) & 7;
	PULSEFLAG(MDR);
	if (MDR == 1) INTEN = 0;
	if (MDR == 2) SEL = 0;
	break;

case 0x018: /**** 0c00 sflg f12 ****/
	MDR = (IR >> 8) & 7;
	SETFLAG(MDR);
	if (MDR == 1) INTEN = 1;
	if (MDR == 2) SEL = 1;
	break;

case 0x019: /**** 0c80 pflg f12 ****/
	MDR = (IR >> 8) & 7;
	PULSEFLAG(MDR);
	if (MDR == 1) INTEN = 0;
	if (MDR == 2) SEL = 0;
	break;

case 0x01a: /**** 0d00 sflg f13 ****/
	MDR = (IR >> 8) & 7;
	SETFLAG(MDR);
	if (MDR == 1) INTEN = 1;
	if (MDR == 2) SEL = 1;
	break;

case 0x01b: /**** 0d80 pflg f13 ****/
	MDR = (IR >> 8) & 7;
	PULSEFLAG(MDR);
	if (MDR == 1) INTEN = 0;
	if (MDR == 2) SEL = 0;
	break;

case 0x01c: /**** 0e00 sflg f14 ****/
	MDR = (IR >> 8) & 7;
	SETFLAG(MDR);
	if (MDR == 1) INTEN = 1;
	if (MDR == 2) SEL = 1;
	break;

case 0x01d: /**** 0e80 pflg f14 ****/
	MDR = (IR >> 8) & 7;
	PULSEFLAG(MDR);
	if (MDR == 1) INTEN = 0;
	if (MDR == 2) SEL = 0;
	break;

case 0x01e: /**** 0f00 sflg f15 ****/
	MDR = (IR >> 8) & 7;
	SETFLAG(MDR);
	if (MDR == 1) INTEN = 1;
	if (MDR == 2) SEL = 1;
	break;

case 0x01f: /**** 0f80 pflg f15 ****/
	MDR = (IR >> 8) & 7;
	PULSEFLAG(MDR);
	if (MDR == 1) INTEN = 0;
	if (MDR == 2) SEL = 0;
	break;

case 0x022: /**** 1100 boc 1,%s ****/
	MAR = AC2+(IR&0x007F);
	BRANCH(AC0 == 0);
	break;

case 0x023: /**** 1180 boc 1,%s ****/
	MAR = AC2+(IR|0xFF00);
	BRANCH(AC0 == 0);
	break;

case 0x024: /**** 1200 boc 2,%s ****/
	MAR = AC2+(IR&0x007F);
	BRANCH((AC0 & 0x8000) == 0);
	break;

case 0x025: /**** 1280 boc 2,%s ****/
	MAR = AC2+(IR|0xFF00);
	BRANCH((AC0 & 0x8000) == 0);
	break;

case 0x026: /**** 1300 boc 3,%s ****/
	MAR = AC2+(IR&0x007F);
	BRANCH((AC0 & 0x0001) != 0);
	break;

case 0x027: /**** 1380 boc 3,%s ****/
	MAR = AC2+(IR|0xFF00);
	BRANCH((AC0 & 0x0001) != 0);
	break;

case 0x028: /**** 1400 boc 4,%s ****/
	MAR = AC2+(IR&0x007F);
	BRANCH((AC0 & 0x0002) != 0);
	break;

case 0x029: /**** 1480 boc 4,%s ****/
	MAR = AC2+(IR|0xFF00);
	BRANCH((AC0 & 0x0002) != 0);
	break;

case 0x02a: /**** 1500 boc 5,%s ****/
	MAR = AC2+(IR&0x007F);
	BRANCH(AC0 != 0);
	break;

case 0x02b: /**** 1580 boc 5,%s ****/
	MAR = AC2+(IR|0xFF00);
	BRANCH(AC0 != 0);
	break;

case 0x030: /**** 1800 boc 8,%s ****/
	MAR = AC2+(IR&0x007F);
	BRANCH((SPTR & 0xF0) != 0);
	break;

case 0x031: /**** 1880 boc 8,%s ****/
	MAR = AC2+(IR|0xFF00);
	BRANCH((SPTR & 0xF0) != 0);
	break;

case 0x032: /**** 1900 boc 9,%s ****/
	MAR = AC2+(IR&0x007F);
	BRANCH(INTEN != 0);
	break;

case 0x033: /**** 1980 boc 9,%s ****/
	MAR = AC2+(IR|0xFF00);
	BRANCH(INTEN != 0);
	break;

case 0x034: /**** 1a00 boc 10,%s ****/
	MAR = AC2+(IR&0x007F);
	BRANCH((SEL == 0) ? CY:OV );
	break;

case 0x035: /**** 1a80 boc 10,%s ****/
	MAR = AC2+(IR|0xFF00);
	BRANCH((SEL == 0) ? CY:OV );
	break;

case 0x036: /**** 1b00 boc 11,%s ****/
	MAR = AC2+(IR&0x007F);
	BRANCH((AC0 == 0) || ((AC0 & 0x8000) != 0));
	break;

case 0x037: /**** 1b80 boc 11,%s ****/
	MAR = AC2+(IR|0xFF00);
	BRANCH((AC0 == 0) || ((AC0 & 0x8000) != 0));
	break;

case 0x040: /**** 2000 jmp %8 ****/
	MAR = IR & 0xFF;
	PC = MAR;
	break;

case 0x041: /**** 2080 jmp %8 ****/
	MAR = IR & 0xFF;
	PC = MAR;
	break;

case 0x042: /**** 2100 jmp %s(pc) ****/
	MAR = PC+(IR&0x007F);
	PC = MAR;
	break;

case 0x043: /**** 2180 jmp %s(pc) ****/
	MAR = PC+(IR|0xFF00);
	PC = MAR;
	break;

case 0x044: /**** 2200 jmp %s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	PC = MAR;
	break;

case 0x045: /**** 2280 jmp %s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	PC = MAR;
	break;

case 0x046: /**** 2300 jmp %s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	PC = MAR;
	break;

case 0x047: /**** 2380 jmp %s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	PC = MAR;
	break;

case 0x048: /**** 2400 jmp @%8 ****/
	MAR = IR & 0xFF;
	READ();
	PC = MDR;
	break;

case 0x049: /**** 2480 jmp @%8 ****/
	MAR = IR & 0xFF;
	READ();
	PC = MDR;
	break;

case 0x04a: /**** 2500 jmp @%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	PC = MDR;
	break;

case 0x04b: /**** 2580 jmp @%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	PC = MDR;
	break;

case 0x04c: /**** 2600 jmp @%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	PC = MDR;
	break;

case 0x04d: /**** 2680 jmp @%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	PC = MDR;
	break;

case 0x04e: /**** 2700 jmp @%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	PC = MDR;
	break;

case 0x04f: /**** 2780 jmp @%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	PC = MDR;
	break;

case 0x050: /**** 2800 jsr %8 ****/
	MAR = IR & 0xFF;
	PUSH(PC);
	PC = MAR;
	break;

case 0x051: /**** 2880 jsr %8 ****/
	MAR = IR & 0xFF;
	PUSH(PC);
	PC = MAR;
	break;

case 0x052: /**** 2900 jsr %s(pc) ****/
	MAR = PC+(IR&0x007F);
	PUSH(PC);
	PC = MAR;
	break;

case 0x053: /**** 2980 jsr %s(pc) ****/
	MAR = PC+(IR|0xFF00);
	PUSH(PC);
	PC = MAR;
	break;

case 0x054: /**** 2a00 jsr %s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	PUSH(PC);
	PC = MAR;
	break;

case 0x055: /**** 2a80 jsr %s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	PUSH(PC);
	PC = MAR;
	break;

case 0x056: /**** 2b00 jsr %s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	PUSH(PC);
	PC = MAR;
	break;

case 0x057: /**** 2b80 jsr %s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	PUSH(PC);
	PC = MAR;
	break;

case 0x058: /**** 2c00 jsr @%8 ****/
	MAR = IR & 0xFF;
	READ();
	PUSH(PC);
	PC = MDR;
	break;

case 0x059: /**** 2c80 jsr @%8 ****/
	MAR = IR & 0xFF;
	READ();
	PUSH(PC);
	PC = MDR;
	break;

case 0x05a: /**** 2d00 jsr @%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	PUSH(PC);
	PC = MDR;
	break;

case 0x05b: /**** 2d80 jsr @%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	PUSH(PC);
	PC = MDR;
	break;

case 0x05c: /**** 2e00 jsr @%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	PUSH(PC);
	PC = MDR;
	break;

case 0x05d: /**** 2e80 jsr @%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	PUSH(PC);
	PC = MDR;
	break;

case 0x05e: /**** 2f00 jsr @%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	PUSH(PC);
	PC = MDR;
	break;

case 0x05f: /**** 2f80 jsr @%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	PUSH(PC);
	PC = MDR;
	break;

case 0x060: /**** 3000 radd 0,0 ****/
	ADD16(AC0,AC0,0);
	break;

case 0x061: /**** 3080 r%c 0,0 ****/
	RegisterOperation(IR & 3,&AC0,&AC0);
	break;

case 0x062: /**** 3100 radd 0,1 ****/
	ADD16(AC1,AC0,0);
	break;

case 0x063: /**** 3180 r%c 0,1 ****/
	RegisterOperation(IR & 3,&AC0,&AC1);
	break;

case 0x064: /**** 3200 radd 0,2 ****/
	ADD16(AC2,AC0,0);
	break;

case 0x065: /**** 3280 r%c 0,2 ****/
	RegisterOperation(IR & 3,&AC0,&AC2);
	break;

case 0x066: /**** 3300 radd 0,3 ****/
	ADD16(AC3,AC0,0);
	break;

case 0x067: /**** 3380 r%c 0,3 ****/
	RegisterOperation(IR & 3,&AC0,&AC3);
	break;

case 0x068: /**** 3400 radd 1,0 ****/
	ADD16(AC0,AC1,0);
	break;

case 0x069: /**** 3480 r%c 1,0 ****/
	RegisterOperation(IR & 3,&AC1,&AC0);
	break;

case 0x06a: /**** 3500 radd 1,1 ****/
	ADD16(AC1,AC1,0);
	break;

case 0x06b: /**** 3580 r%c 1,1 ****/
	RegisterOperation(IR & 3,&AC1,&AC1);
	break;

case 0x06c: /**** 3600 radd 1,2 ****/
	ADD16(AC2,AC1,0);
	break;

case 0x06d: /**** 3680 r%c 1,2 ****/
	RegisterOperation(IR & 3,&AC1,&AC2);
	break;

case 0x06e: /**** 3700 radd 1,3 ****/
	ADD16(AC3,AC1,0);
	break;

case 0x06f: /**** 3780 r%c 1,3 ****/
	RegisterOperation(IR & 3,&AC1,&AC3);
	break;

case 0x070: /**** 3800 radd 2,0 ****/
	ADD16(AC0,AC2,0);
	break;

case 0x071: /**** 3880 r%c 2,0 ****/
	RegisterOperation(IR & 3,&AC2,&AC0);
	break;

case 0x072: /**** 3900 radd 2,1 ****/
	ADD16(AC1,AC2,0);
	break;

case 0x073: /**** 3980 r%c 2,1 ****/
	RegisterOperation(IR & 3,&AC2,&AC1);
	break;

case 0x074: /**** 3a00 radd 2,2 ****/
	ADD16(AC2,AC2,0);
	break;

case 0x075: /**** 3a80 r%c 2,2 ****/
	RegisterOperation(IR & 3,&AC2,&AC2);
	break;

case 0x076: /**** 3b00 radd 2,3 ****/
	ADD16(AC3,AC2,0);
	break;

case 0x077: /**** 3b80 r%c 2,3 ****/
	RegisterOperation(IR & 3,&AC2,&AC3);
	break;

case 0x078: /**** 3c00 radd 3,0 ****/
	ADD16(AC0,AC3,0);
	break;

case 0x079: /**** 3c80 r%c 3,0 ****/
	RegisterOperation(IR & 3,&AC3,&AC0);
	break;

case 0x07a: /**** 3d00 radd 3,1 ****/
	ADD16(AC1,AC3,0);
	break;

case 0x07b: /**** 3d80 r%c 3,1 ****/
	RegisterOperation(IR & 3,&AC3,&AC1);
	break;

case 0x07c: /**** 3e00 radd 3,2 ****/
	ADD16(AC2,AC3,0);
	break;

case 0x07d: /**** 3e80 r%c 3,2 ****/
	RegisterOperation(IR & 3,&AC3,&AC2);
	break;

case 0x07e: /**** 3f00 radd 3,3 ****/
	ADD16(AC3,AC3,0);
	break;

case 0x07f: /**** 3f80 r%c 3,3 ****/
	RegisterOperation(IR & 3,&AC3,&AC3);
	break;

case 0x080: /**** 4000 push 0 ****/
	PUSH(AC0);
	break;

case 0x081: /**** 4080 push 0 ****/
	PUSH(AC0);
	break;

case 0x082: /**** 4100 push 1 ****/
	PUSH(AC1);
	break;

case 0x083: /**** 4180 push 1 ****/
	PUSH(AC1);
	break;

case 0x084: /**** 4200 push 2 ****/
	PUSH(AC2);
	break;

case 0x085: /**** 4280 push 2 ****/
	PUSH(AC2);
	break;

case 0x086: /**** 4300 push 3 ****/
	PUSH(AC3);
	break;

case 0x087: /**** 4380 push 3 ****/
	PUSH(AC3);
	break;

case 0x088: /**** 4400 pull 0 ****/
	AC0 = PULL();
	break;

case 0x089: /**** 4480 pull 0 ****/
	AC0 = PULL();
	break;

case 0x08a: /**** 4500 pull 1 ****/
	AC1 = PULL();
	break;

case 0x08b: /**** 4580 pull 1 ****/
	AC1 = PULL();
	break;

case 0x08c: /**** 4600 pull 2 ****/
	AC2 = PULL();
	break;

case 0x08d: /**** 4680 pull 2 ****/
	AC2 = PULL();
	break;

case 0x08e: /**** 4700 pull 3 ****/
	AC3 = PULL();
	break;

case 0x08f: /**** 4780 pull 3 ****/
	AC3 = PULL();
	break;

case 0x090: /**** 4800 aisz 0,%s ****/
	ADD16(AC0,(IR & 0xFF),0);
	if (AC0 == 0) PC++;
	break;

case 0x091: /**** 4880 aisz 0,%s ****/
	ADD16(AC0,(IR & 0xFF),0);
	if (AC0 == 0) PC++;
	break;

case 0x092: /**** 4900 aisz 1,%s ****/
	ADD16(AC1,(IR & 0xFF),0);
	if (AC1 == 0) PC++;
	break;

case 0x093: /**** 4980 aisz 1,%s ****/
	ADD16(AC1,(IR & 0xFF),0);
	if (AC1 == 0) PC++;
	break;

case 0x094: /**** 4a00 aisz 2,%s ****/
	ADD16(AC2,(IR & 0xFF),0);
	if (AC2 == 0) PC++;
	break;

case 0x095: /**** 4a80 aisz 2,%s ****/
	ADD16(AC2,(IR & 0xFF),0);
	if (AC2 == 0) PC++;
	break;

case 0x096: /**** 4b00 aisz 3,%s ****/
	ADD16(AC3,(IR & 0xFF),0);
	if (AC3 == 0) PC++;
	break;

case 0x097: /**** 4b80 aisz 3,%s ****/
	ADD16(AC3,(IR & 0xFF),0);
	if (AC3 == 0) PC++;
	break;

case 0x098: /**** 4c00 li 0,%s ****/
	AC0 = (IR & 0xFF);
	break;

case 0x099: /**** 4c80 li 0,%s ****/
	AC0 = (IR & 0xFF);
	break;

case 0x09a: /**** 4d00 li 1,%s ****/
	AC1 = (IR & 0xFF);
	break;

case 0x09b: /**** 4d80 li 1,%s ****/
	AC1 = (IR & 0xFF);
	break;

case 0x09c: /**** 4e00 li 2,%s ****/
	AC2 = (IR & 0xFF);
	break;

case 0x09d: /**** 4e80 li 2,%s ****/
	AC2 = (IR & 0xFF);
	break;

case 0x09e: /**** 4f00 li 3,%s ****/
	AC3 = (IR & 0xFF);
	break;

case 0x09f: /**** 4f80 li 3,%s ****/
	AC3 = (IR & 0xFF);
	break;

case 0x0a0: /**** 5000 cai 0,%s ****/
	AC0 = (AC0 ^ 0xFFFF)+((IR & 0xFF));
	break;

case 0x0a1: /**** 5080 cai 0,%s ****/
	AC0 = (AC0 ^ 0xFFFF)+((IR & 0xFF));
	break;

case 0x0a2: /**** 5100 cai 1,%s ****/
	AC1 = (AC1 ^ 0xFFFF)+((IR & 0xFF));
	break;

case 0x0a3: /**** 5180 cai 1,%s ****/
	AC1 = (AC1 ^ 0xFFFF)+((IR & 0xFF));
	break;

case 0x0a4: /**** 5200 cai 2,%s ****/
	AC2 = (AC2 ^ 0xFFFF)+((IR & 0xFF));
	break;

case 0x0a5: /**** 5280 cai 2,%s ****/
	AC2 = (AC2 ^ 0xFFFF)+((IR & 0xFF));
	break;

case 0x0a6: /**** 5300 cai 3,%s ****/
	AC3 = (AC3 ^ 0xFFFF)+((IR & 0xFF));
	break;

case 0x0a7: /**** 5380 cai 3,%s ****/
	AC3 = (AC3 ^ 0xFFFF)+((IR & 0xFF));
	break;

case 0x0a8: /**** 5400 xchrs 0 ****/
	MDR = AC0;
	AC0 = PULL();
	PUSH(MDR);
	break;

case 0x0a9: /**** 5480 xchrs 0 ****/
	MDR = AC0;
	AC0 = PULL();
	PUSH(MDR);
	break;

case 0x0aa: /**** 5500 xchrs 1 ****/
	MDR = AC1;
	AC1 = PULL();
	PUSH(MDR);
	break;

case 0x0ab: /**** 5580 xchrs 1 ****/
	MDR = AC1;
	AC1 = PULL();
	PUSH(MDR);
	break;

case 0x0ac: /**** 5600 xchrs 2 ****/
	MDR = AC2;
	AC2 = PULL();
	PUSH(MDR);
	break;

case 0x0ad: /**** 5680 xchrs 2 ****/
	MDR = AC2;
	AC2 = PULL();
	PUSH(MDR);
	break;

case 0x0ae: /**** 5700 xchrs 3 ****/
	MDR = AC3;
	AC3 = PULL();
	PUSH(MDR);
	break;

case 0x0af: /**** 5780 xchrs 3 ****/
	MDR = AC3;
	AC3 = PULL();
	PUSH(MDR);
	break;

case 0x0b0: /**** 5800 rol 0,%s ****/
	for (int i = 0;
	i < (IR & 0x1F);
	i++) AC&V = RotateLeft(AC&V,SEL);
	break;

case 0x0b1: /**** 5880 ror 0,%s ****/
	for (int i = (IR & 0x1F);
	i < 0x20;
	i++) AC&V = RotateRight(AC&V,SEL);
	break;

case 0x0b2: /**** 5900 rol 1,%s ****/
	for (int i = 0;
	i < (IR & 0x1F);
	i++) AC&V = RotateLeft(AC&V,SEL);
	break;

case 0x0b3: /**** 5980 ror 1,%s ****/
	for (int i = (IR & 0x1F);
	i < 0x20;
	i++) AC&V = RotateRight(AC&V,SEL);
	break;

case 0x0b4: /**** 5a00 rol 2,%s ****/
	for (int i = 0;
	i < (IR & 0x1F);
	i++) AC&V = RotateLeft(AC&V,SEL);
	break;

case 0x0b5: /**** 5a80 ror 2,%s ****/
	for (int i = (IR & 0x1F);
	i < 0x20;
	i++) AC&V = RotateRight(AC&V,SEL);
	break;

case 0x0b6: /**** 5b00 rol 3,%s ****/
	for (int i = 0;
	i < (IR & 0x1F);
	i++) AC&V = RotateLeft(AC&V,SEL);
	break;

case 0x0b7: /**** 5b80 ror 3,%s ****/
	for (int i = (IR & 0x1F);
	i < 0x20;
	i++) AC&V = RotateRight(AC&V,SEL);
	break;

case 0x0b8: /**** 5c00 shl 0,%s ****/
	for (int i = 0;
	i < (IR & 0x1F);
	i++) AC0 = ShiftLeft(AC0,SEL);
	break;

case 0x0b9: /**** 5c80 shr 0,%s ****/
	for (int i = (IR & 0x1F);
	i < 0x20;
	i++) AC0 = ShiftRight(AC0,SEL);
	break;

case 0x0ba: /**** 5d00 shl 1,%s ****/
	for (int i = 0;
	i < (IR & 0x1F);
	i++) AC1 = ShiftLeft(AC1,SEL);
	break;

case 0x0bb: /**** 5d80 shr 1,%s ****/
	for (int i = (IR & 0x1F);
	i < 0x20;
	i++) AC1 = ShiftRight(AC1,SEL);
	break;

case 0x0bc: /**** 5e00 shl 2,%s ****/
	for (int i = 0;
	i < (IR & 0x1F);
	i++) AC2 = ShiftLeft(AC2,SEL);
	break;

case 0x0bd: /**** 5e80 shr 2,%s ****/
	for (int i = (IR & 0x1F);
	i < 0x20;
	i++) AC2 = ShiftRight(AC2,SEL);
	break;

case 0x0be: /**** 5f00 shl 3,%s ****/
	for (int i = 0;
	i < (IR & 0x1F);
	i++) AC3 = ShiftLeft(AC3,SEL);
	break;

case 0x0bf: /**** 5f80 shr 3,%s ****/
	for (int i = (IR & 0x1F);
	i < 0x20;
	i++) AC3 = ShiftRight(AC3,SEL);
	break;

case 0x0c0: /**** 6000 and 0,%8 ****/
	MAR = IR & 0xFF;
	READ();
	AC0 &= MDR;
	break;

case 0x0c1: /**** 6080 and 0,%8 ****/
	MAR = IR & 0xFF;
	READ();
	AC0 &= MDR;
	break;

case 0x0c2: /**** 6100 and 0,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	AC0 &= MDR;
	break;

case 0x0c3: /**** 6180 and 0,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	AC0 &= MDR;
	break;

case 0x0c4: /**** 6200 and 0,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	AC0 &= MDR;
	break;

case 0x0c5: /**** 6280 and 0,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	AC0 &= MDR;
	break;

case 0x0c6: /**** 6300 and 0,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	AC0 &= MDR;
	break;

case 0x0c7: /**** 6380 and 0,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	AC0 &= MDR;
	break;

case 0x0c8: /**** 6400 and 1,%8 ****/
	MAR = IR & 0xFF;
	READ();
	AC1 &= MDR;
	break;

case 0x0c9: /**** 6480 and 1,%8 ****/
	MAR = IR & 0xFF;
	READ();
	AC1 &= MDR;
	break;

case 0x0ca: /**** 6500 and 1,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	AC1 &= MDR;
	break;

case 0x0cb: /**** 6580 and 1,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	AC1 &= MDR;
	break;

case 0x0cc: /**** 6600 and 1,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	AC1 &= MDR;
	break;

case 0x0cd: /**** 6680 and 1,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	AC1 &= MDR;
	break;

case 0x0ce: /**** 6700 and 1,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	AC1 &= MDR;
	break;

case 0x0cf: /**** 6780 and 1,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	AC1 &= MDR;
	break;

case 0x0d0: /**** 6800 and 0,%8 ****/
	MAR = IR & 0xFF;
	READ();
	AC0 |= MDR;
	break;

case 0x0d1: /**** 6880 and 0,%8 ****/
	MAR = IR & 0xFF;
	READ();
	AC0 |= MDR;
	break;

case 0x0d2: /**** 6900 and 0,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	AC0 |= MDR;
	break;

case 0x0d3: /**** 6980 and 0,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	AC0 |= MDR;
	break;

case 0x0d4: /**** 6a00 and 0,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	AC0 |= MDR;
	break;

case 0x0d5: /**** 6a80 and 0,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	AC0 |= MDR;
	break;

case 0x0d6: /**** 6b00 and 0,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	AC0 |= MDR;
	break;

case 0x0d7: /**** 6b80 and 0,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	AC0 |= MDR;
	break;

case 0x0d8: /**** 6c00 and 1,%8 ****/
	MAR = IR & 0xFF;
	READ();
	AC1 |= MDR;
	break;

case 0x0d9: /**** 6c80 and 1,%8 ****/
	MAR = IR & 0xFF;
	READ();
	AC1 |= MDR;
	break;

case 0x0da: /**** 6d00 and 1,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	AC1 |= MDR;
	break;

case 0x0db: /**** 6d80 and 1,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	AC1 |= MDR;
	break;

case 0x0dc: /**** 6e00 and 1,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	AC1 |= MDR;
	break;

case 0x0dd: /**** 6e80 and 1,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	AC1 |= MDR;
	break;

case 0x0de: /**** 6f00 and 1,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	AC1 |= MDR;
	break;

case 0x0df: /**** 6f80 and 1,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	AC1 |= MDR;
	break;

case 0x0e0: /**** 7000 skaz 0,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if ((AC0 & MDR) != 0) PC++;
	break;

case 0x0e1: /**** 7080 skaz 0,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if ((AC0 & MDR) != 0) PC++;
	break;

case 0x0e2: /**** 7100 skaz 0,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	if ((AC0 & MDR) != 0) PC++;
	break;

case 0x0e3: /**** 7180 skaz 0,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	if ((AC0 & MDR) != 0) PC++;
	break;

case 0x0e4: /**** 7200 skaz 0,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	if ((AC0 & MDR) != 0) PC++;
	break;

case 0x0e5: /**** 7280 skaz 0,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	if ((AC0 & MDR) != 0) PC++;
	break;

case 0x0e6: /**** 7300 skaz 0,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	if ((AC0 & MDR) != 0) PC++;
	break;

case 0x0e7: /**** 7380 skaz 0,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	if ((AC0 & MDR) != 0) PC++;
	break;

case 0x0e8: /**** 7400 skaz 1,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if ((AC1 & MDR) != 0) PC++;
	break;

case 0x0e9: /**** 7480 skaz 1,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if ((AC1 & MDR) != 0) PC++;
	break;

case 0x0ea: /**** 7500 skaz 1,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	if ((AC1 & MDR) != 0) PC++;
	break;

case 0x0eb: /**** 7580 skaz 1,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	if ((AC1 & MDR) != 0) PC++;
	break;

case 0x0ec: /**** 7600 skaz 1,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	if ((AC1 & MDR) != 0) PC++;
	break;

case 0x0ed: /**** 7680 skaz 1,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	if ((AC1 & MDR) != 0) PC++;
	break;

case 0x0ee: /**** 7700 skaz 1,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	if ((AC1 & MDR) != 0) PC++;
	break;

case 0x0ef: /**** 7780 skaz 1,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	if ((AC1 & MDR) != 0) PC++;
	break;

case 0x0f0: /**** 7800 isz %8 ****/
	MAR = IR & 0xFF;
	READ();
	MDR++;
	WRITE();
	if (MDR == 0) PC++;
	break;

case 0x0f1: /**** 7880 isz %8 ****/
	MAR = IR & 0xFF;
	READ();
	MDR++;
	WRITE();
	if (MDR == 0) PC++;
	break;

case 0x0f2: /**** 7900 isz %s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	MDR++;
	WRITE();
	if (MDR == 0) PC++;
	break;

case 0x0f3: /**** 7980 isz %s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	MDR++;
	WRITE();
	if (MDR == 0) PC++;
	break;

case 0x0f4: /**** 7a00 isz %s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	MDR++;
	WRITE();
	if (MDR == 0) PC++;
	break;

case 0x0f5: /**** 7a80 isz %s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	MDR++;
	WRITE();
	if (MDR == 0) PC++;
	break;

case 0x0f6: /**** 7b00 isz %s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	MDR++;
	WRITE();
	if (MDR == 0) PC++;
	break;

case 0x0f7: /**** 7b80 isz %s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	MDR++;
	WRITE();
	if (MDR == 0) PC++;
	break;

case 0x0f8: /**** 7c00 dsz %8 ****/
	MAR = IR & 0xFF;
	READ();
	MDR--;
	WRITE();
	if (MDR == 0) PC++;
	break;

case 0x0f9: /**** 7c80 dsz %8 ****/
	MAR = IR & 0xFF;
	READ();
	MDR--;
	WRITE();
	if (MDR == 0) PC++;
	break;

case 0x0fa: /**** 7d00 dsz %s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	MDR--;
	WRITE();
	if (MDR == 0) PC++;
	break;

case 0x0fb: /**** 7d80 dsz %s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	MDR--;
	WRITE();
	if (MDR == 0) PC++;
	break;

case 0x0fc: /**** 7e00 dsz %s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	MDR--;
	WRITE();
	if (MDR == 0) PC++;
	break;

case 0x0fd: /**** 7e80 dsz %s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	MDR--;
	WRITE();
	if (MDR == 0) PC++;
	break;

case 0x0fe: /**** 7f00 dsz %s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	MDR--;
	WRITE();
	if (MDR == 0) PC++;
	break;

case 0x0ff: /**** 7f80 dsz %s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	MDR--;
	WRITE();
	if (MDR == 0) PC++;
	break;

case 0x100: /**** 8000 ld 0,%8 ****/
	MAR = IR & 0xFF;
	READ();
	AC0 = MDR;
	break;

case 0x101: /**** 8080 ld 0,%8 ****/
	MAR = IR & 0xFF;
	READ();
	AC0 = MDR;
	break;

case 0x102: /**** 8100 ld 0,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	AC0 = MDR;
	break;

case 0x103: /**** 8180 ld 0,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	AC0 = MDR;
	break;

case 0x104: /**** 8200 ld 0,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	AC0 = MDR;
	break;

case 0x105: /**** 8280 ld 0,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	AC0 = MDR;
	break;

case 0x106: /**** 8300 ld 0,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	AC0 = MDR;
	break;

case 0x107: /**** 8380 ld 0,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	AC0 = MDR;
	break;

case 0x108: /**** 8400 ld 1,%8 ****/
	MAR = IR & 0xFF;
	READ();
	AC1 = MDR;
	break;

case 0x109: /**** 8480 ld 1,%8 ****/
	MAR = IR & 0xFF;
	READ();
	AC1 = MDR;
	break;

case 0x10a: /**** 8500 ld 1,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	AC1 = MDR;
	break;

case 0x10b: /**** 8580 ld 1,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	AC1 = MDR;
	break;

case 0x10c: /**** 8600 ld 1,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	AC1 = MDR;
	break;

case 0x10d: /**** 8680 ld 1,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	AC1 = MDR;
	break;

case 0x10e: /**** 8700 ld 1,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	AC1 = MDR;
	break;

case 0x10f: /**** 8780 ld 1,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	AC1 = MDR;
	break;

case 0x110: /**** 8800 ld 2,%8 ****/
	MAR = IR & 0xFF;
	READ();
	AC2 = MDR;
	break;

case 0x111: /**** 8880 ld 2,%8 ****/
	MAR = IR & 0xFF;
	READ();
	AC2 = MDR;
	break;

case 0x112: /**** 8900 ld 2,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	AC2 = MDR;
	break;

case 0x113: /**** 8980 ld 2,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	AC2 = MDR;
	break;

case 0x114: /**** 8a00 ld 2,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	AC2 = MDR;
	break;

case 0x115: /**** 8a80 ld 2,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	AC2 = MDR;
	break;

case 0x116: /**** 8b00 ld 2,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	AC2 = MDR;
	break;

case 0x117: /**** 8b80 ld 2,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	AC2 = MDR;
	break;

case 0x118: /**** 8c00 ld 3,%8 ****/
	MAR = IR & 0xFF;
	READ();
	AC3 = MDR;
	break;

case 0x119: /**** 8c80 ld 3,%8 ****/
	MAR = IR & 0xFF;
	READ();
	AC3 = MDR;
	break;

case 0x11a: /**** 8d00 ld 3,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	AC3 = MDR;
	break;

case 0x11b: /**** 8d80 ld 3,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	AC3 = MDR;
	break;

case 0x11c: /**** 8e00 ld 3,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	AC3 = MDR;
	break;

case 0x11d: /**** 8e80 ld 3,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	AC3 = MDR;
	break;

case 0x11e: /**** 8f00 ld 3,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	AC3 = MDR;
	break;

case 0x11f: /**** 8f80 ld 3,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	AC3 = MDR;
	break;

case 0x120: /**** 9000 ld 0,@%8 ****/
	MAR = IR & 0xFF;
	READ();
	MAR = MDR;
	READ();
	AC0 = MDR;
	break;

case 0x121: /**** 9080 ld 0,@%8 ****/
	MAR = IR & 0xFF;
	READ();
	MAR = MDR;
	READ();
	AC0 = MDR;
	break;

case 0x122: /**** 9100 ld 0,@%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	MAR = MDR;
	READ();
	AC0 = MDR;
	break;

case 0x123: /**** 9180 ld 0,@%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	MAR = MDR;
	READ();
	AC0 = MDR;
	break;

case 0x124: /**** 9200 ld 0,@%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	MAR = MDR;
	READ();
	AC0 = MDR;
	break;

case 0x125: /**** 9280 ld 0,@%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	MAR = MDR;
	READ();
	AC0 = MDR;
	break;

case 0x126: /**** 9300 ld 0,@%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	MAR = MDR;
	READ();
	AC0 = MDR;
	break;

case 0x127: /**** 9380 ld 0,@%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	MAR = MDR;
	READ();
	AC0 = MDR;
	break;

case 0x128: /**** 9400 ld 1,@%8 ****/
	MAR = IR & 0xFF;
	READ();
	MAR = MDR;
	READ();
	AC1 = MDR;
	break;

case 0x129: /**** 9480 ld 1,@%8 ****/
	MAR = IR & 0xFF;
	READ();
	MAR = MDR;
	READ();
	AC1 = MDR;
	break;

case 0x12a: /**** 9500 ld 1,@%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	MAR = MDR;
	READ();
	AC1 = MDR;
	break;

case 0x12b: /**** 9580 ld 1,@%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	MAR = MDR;
	READ();
	AC1 = MDR;
	break;

case 0x12c: /**** 9600 ld 1,@%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	MAR = MDR;
	READ();
	AC1 = MDR;
	break;

case 0x12d: /**** 9680 ld 1,@%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	MAR = MDR;
	READ();
	AC1 = MDR;
	break;

case 0x12e: /**** 9700 ld 1,@%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	MAR = MDR;
	READ();
	AC1 = MDR;
	break;

case 0x12f: /**** 9780 ld 1,@%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	MAR = MDR;
	READ();
	AC1 = MDR;
	break;

case 0x130: /**** 9800 ld 2,@%8 ****/
	MAR = IR & 0xFF;
	READ();
	MAR = MDR;
	READ();
	AC2 = MDR;
	break;

case 0x131: /**** 9880 ld 2,@%8 ****/
	MAR = IR & 0xFF;
	READ();
	MAR = MDR;
	READ();
	AC2 = MDR;
	break;

case 0x132: /**** 9900 ld 2,@%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	MAR = MDR;
	READ();
	AC2 = MDR;
	break;

case 0x133: /**** 9980 ld 2,@%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	MAR = MDR;
	READ();
	AC2 = MDR;
	break;

case 0x134: /**** 9a00 ld 2,@%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	MAR = MDR;
	READ();
	AC2 = MDR;
	break;

case 0x135: /**** 9a80 ld 2,@%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	MAR = MDR;
	READ();
	AC2 = MDR;
	break;

case 0x136: /**** 9b00 ld 2,@%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	MAR = MDR;
	READ();
	AC2 = MDR;
	break;

case 0x137: /**** 9b80 ld 2,@%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	MAR = MDR;
	READ();
	AC2 = MDR;
	break;

case 0x138: /**** 9c00 ld 3,@%8 ****/
	MAR = IR & 0xFF;
	READ();
	MAR = MDR;
	READ();
	AC3 = MDR;
	break;

case 0x139: /**** 9c80 ld 3,@%8 ****/
	MAR = IR & 0xFF;
	READ();
	MAR = MDR;
	READ();
	AC3 = MDR;
	break;

case 0x13a: /**** 9d00 ld 3,@%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	MAR = MDR;
	READ();
	AC3 = MDR;
	break;

case 0x13b: /**** 9d80 ld 3,@%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	MAR = MDR;
	READ();
	AC3 = MDR;
	break;

case 0x13c: /**** 9e00 ld 3,@%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	MAR = MDR;
	READ();
	AC3 = MDR;
	break;

case 0x13d: /**** 9e80 ld 3,@%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	MAR = MDR;
	READ();
	AC3 = MDR;
	break;

case 0x13e: /**** 9f00 ld 3,@%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	MAR = MDR;
	READ();
	AC3 = MDR;
	break;

case 0x13f: /**** 9f80 ld 3,@%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	MAR = MDR;
	READ();
	AC3 = MDR;
	break;

case 0x140: /**** a000 st 0,%8 ****/
	MAR = IR & 0xFF;
	MDR = AC0;
	WRITE();
	break;

case 0x141: /**** a080 st 0,%8 ****/
	MAR = IR & 0xFF;
	MDR = AC0;
	WRITE();
	break;

case 0x142: /**** a100 st 0,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	MDR = AC0;
	WRITE();
	break;

case 0x143: /**** a180 st 0,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	MDR = AC0;
	WRITE();
	break;

case 0x144: /**** a200 st 0,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	MDR = AC0;
	WRITE();
	break;

case 0x145: /**** a280 st 0,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	MDR = AC0;
	WRITE();
	break;

case 0x146: /**** a300 st 0,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	MDR = AC0;
	WRITE();
	break;

case 0x147: /**** a380 st 0,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	MDR = AC0;
	WRITE();
	break;

case 0x148: /**** a400 st 1,%8 ****/
	MAR = IR & 0xFF;
	MDR = AC1;
	WRITE();
	break;

case 0x149: /**** a480 st 1,%8 ****/
	MAR = IR & 0xFF;
	MDR = AC1;
	WRITE();
	break;

case 0x14a: /**** a500 st 1,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	MDR = AC1;
	WRITE();
	break;

case 0x14b: /**** a580 st 1,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	MDR = AC1;
	WRITE();
	break;

case 0x14c: /**** a600 st 1,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	MDR = AC1;
	WRITE();
	break;

case 0x14d: /**** a680 st 1,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	MDR = AC1;
	WRITE();
	break;

case 0x14e: /**** a700 st 1,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	MDR = AC1;
	WRITE();
	break;

case 0x14f: /**** a780 st 1,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	MDR = AC1;
	WRITE();
	break;

case 0x150: /**** a800 st 2,%8 ****/
	MAR = IR & 0xFF;
	MDR = AC2;
	WRITE();
	break;

case 0x151: /**** a880 st 2,%8 ****/
	MAR = IR & 0xFF;
	MDR = AC2;
	WRITE();
	break;

case 0x152: /**** a900 st 2,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	MDR = AC2;
	WRITE();
	break;

case 0x153: /**** a980 st 2,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	MDR = AC2;
	WRITE();
	break;

case 0x154: /**** aa00 st 2,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	MDR = AC2;
	WRITE();
	break;

case 0x155: /**** aa80 st 2,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	MDR = AC2;
	WRITE();
	break;

case 0x156: /**** ab00 st 2,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	MDR = AC2;
	WRITE();
	break;

case 0x157: /**** ab80 st 2,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	MDR = AC2;
	WRITE();
	break;

case 0x158: /**** ac00 st 3,%8 ****/
	MAR = IR & 0xFF;
	MDR = AC3;
	WRITE();
	break;

case 0x159: /**** ac80 st 3,%8 ****/
	MAR = IR & 0xFF;
	MDR = AC3;
	WRITE();
	break;

case 0x15a: /**** ad00 st 3,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	MDR = AC3;
	WRITE();
	break;

case 0x15b: /**** ad80 st 3,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	MDR = AC3;
	WRITE();
	break;

case 0x15c: /**** ae00 st 3,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	MDR = AC3;
	WRITE();
	break;

case 0x15d: /**** ae80 st 3,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	MDR = AC3;
	WRITE();
	break;

case 0x15e: /**** af00 st 3,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	MDR = AC3;
	WRITE();
	break;

case 0x15f: /**** af80 st 3,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	MDR = AC3;
	WRITE();
	break;

case 0x160: /**** b000 st 0,@%8 ****/
	MAR = IR & 0xFF;
	READ();
	MAR = MDR;
	MDR = AC0;
	WRITE();
	break;

case 0x161: /**** b080 st 0,@%8 ****/
	MAR = IR & 0xFF;
	READ();
	MAR = MDR;
	MDR = AC0;
	WRITE();
	break;

case 0x162: /**** b100 st 0,@%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	MAR = MDR;
	MDR = AC0;
	WRITE();
	break;

case 0x163: /**** b180 st 0,@%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	MAR = MDR;
	MDR = AC0;
	WRITE();
	break;

case 0x164: /**** b200 st 0,@%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	MAR = MDR;
	MDR = AC0;
	WRITE();
	break;

case 0x165: /**** b280 st 0,@%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	MAR = MDR;
	MDR = AC0;
	WRITE();
	break;

case 0x166: /**** b300 st 0,@%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	MAR = MDR;
	MDR = AC0;
	WRITE();
	break;

case 0x167: /**** b380 st 0,@%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	MAR = MDR;
	MDR = AC0;
	WRITE();
	break;

case 0x168: /**** b400 st 1,@%8 ****/
	MAR = IR & 0xFF;
	READ();
	MAR = MDR;
	MDR = AC1;
	WRITE();
	break;

case 0x169: /**** b480 st 1,@%8 ****/
	MAR = IR & 0xFF;
	READ();
	MAR = MDR;
	MDR = AC1;
	WRITE();
	break;

case 0x16a: /**** b500 st 1,@%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	MAR = MDR;
	MDR = AC1;
	WRITE();
	break;

case 0x16b: /**** b580 st 1,@%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	MAR = MDR;
	MDR = AC1;
	WRITE();
	break;

case 0x16c: /**** b600 st 1,@%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	MAR = MDR;
	MDR = AC1;
	WRITE();
	break;

case 0x16d: /**** b680 st 1,@%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	MAR = MDR;
	MDR = AC1;
	WRITE();
	break;

case 0x16e: /**** b700 st 1,@%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	MAR = MDR;
	MDR = AC1;
	WRITE();
	break;

case 0x16f: /**** b780 st 1,@%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	MAR = MDR;
	MDR = AC1;
	WRITE();
	break;

case 0x170: /**** b800 st 2,@%8 ****/
	MAR = IR & 0xFF;
	READ();
	MAR = MDR;
	MDR = AC2;
	WRITE();
	break;

case 0x171: /**** b880 st 2,@%8 ****/
	MAR = IR & 0xFF;
	READ();
	MAR = MDR;
	MDR = AC2;
	WRITE();
	break;

case 0x172: /**** b900 st 2,@%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	MAR = MDR;
	MDR = AC2;
	WRITE();
	break;

case 0x173: /**** b980 st 2,@%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	MAR = MDR;
	MDR = AC2;
	WRITE();
	break;

case 0x174: /**** ba00 st 2,@%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	MAR = MDR;
	MDR = AC2;
	WRITE();
	break;

case 0x175: /**** ba80 st 2,@%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	MAR = MDR;
	MDR = AC2;
	WRITE();
	break;

case 0x176: /**** bb00 st 2,@%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	MAR = MDR;
	MDR = AC2;
	WRITE();
	break;

case 0x177: /**** bb80 st 2,@%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	MAR = MDR;
	MDR = AC2;
	WRITE();
	break;

case 0x178: /**** bc00 st 3,@%8 ****/
	MAR = IR & 0xFF;
	READ();
	MAR = MDR;
	MDR = AC3;
	WRITE();
	break;

case 0x179: /**** bc80 st 3,@%8 ****/
	MAR = IR & 0xFF;
	READ();
	MAR = MDR;
	MDR = AC3;
	WRITE();
	break;

case 0x17a: /**** bd00 st 3,@%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	MAR = MDR;
	MDR = AC3;
	WRITE();
	break;

case 0x17b: /**** bd80 st 3,@%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	MAR = MDR;
	MDR = AC3;
	WRITE();
	break;

case 0x17c: /**** be00 st 3,@%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	MAR = MDR;
	MDR = AC3;
	WRITE();
	break;

case 0x17d: /**** be80 st 3,@%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	MAR = MDR;
	MDR = AC3;
	WRITE();
	break;

case 0x17e: /**** bf00 st 3,@%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	MAR = MDR;
	MDR = AC3;
	WRITE();
	break;

case 0x17f: /**** bf80 st 3,@%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	MAR = MDR;
	MDR = AC3;
	WRITE();
	break;

case 0x180: /**** c000 add 0,%8 ****/
	MAR = IR & 0xFF;
	READ();
	ADD16(AC0,MDR,0);
	break;

case 0x181: /**** c080 add 0,%8 ****/
	MAR = IR & 0xFF;
	READ();
	ADD16(AC0,MDR,0);
	break;

case 0x182: /**** c100 add 0,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	ADD16(AC0,MDR,0);
	break;

case 0x183: /**** c180 add 0,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	ADD16(AC0,MDR,0);
	break;

case 0x184: /**** c200 add 0,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	ADD16(AC0,MDR,0);
	break;

case 0x185: /**** c280 add 0,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	ADD16(AC0,MDR,0);
	break;

case 0x186: /**** c300 add 0,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	ADD16(AC0,MDR,0);
	break;

case 0x187: /**** c380 add 0,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	ADD16(AC0,MDR,0);
	break;

case 0x188: /**** c400 add 1,%8 ****/
	MAR = IR & 0xFF;
	READ();
	ADD16(AC1,MDR,0);
	break;

case 0x189: /**** c480 add 1,%8 ****/
	MAR = IR & 0xFF;
	READ();
	ADD16(AC1,MDR,0);
	break;

case 0x18a: /**** c500 add 1,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	ADD16(AC1,MDR,0);
	break;

case 0x18b: /**** c580 add 1,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	ADD16(AC1,MDR,0);
	break;

case 0x18c: /**** c600 add 1,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	ADD16(AC1,MDR,0);
	break;

case 0x18d: /**** c680 add 1,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	ADD16(AC1,MDR,0);
	break;

case 0x18e: /**** c700 add 1,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	ADD16(AC1,MDR,0);
	break;

case 0x18f: /**** c780 add 1,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	ADD16(AC1,MDR,0);
	break;

case 0x190: /**** c800 add 2,%8 ****/
	MAR = IR & 0xFF;
	READ();
	ADD16(AC2,MDR,0);
	break;

case 0x191: /**** c880 add 2,%8 ****/
	MAR = IR & 0xFF;
	READ();
	ADD16(AC2,MDR,0);
	break;

case 0x192: /**** c900 add 2,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	ADD16(AC2,MDR,0);
	break;

case 0x193: /**** c980 add 2,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	ADD16(AC2,MDR,0);
	break;

case 0x194: /**** ca00 add 2,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	ADD16(AC2,MDR,0);
	break;

case 0x195: /**** ca80 add 2,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	ADD16(AC2,MDR,0);
	break;

case 0x196: /**** cb00 add 2,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	ADD16(AC2,MDR,0);
	break;

case 0x197: /**** cb80 add 2,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	ADD16(AC2,MDR,0);
	break;

case 0x198: /**** cc00 add 3,%8 ****/
	MAR = IR & 0xFF;
	READ();
	ADD16(AC3,MDR,0);
	break;

case 0x199: /**** cc80 add 3,%8 ****/
	MAR = IR & 0xFF;
	READ();
	ADD16(AC3,MDR,0);
	break;

case 0x19a: /**** cd00 add 3,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	ADD16(AC3,MDR,0);
	break;

case 0x19b: /**** cd80 add 3,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	ADD16(AC3,MDR,0);
	break;

case 0x19c: /**** ce00 add 3,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	ADD16(AC3,MDR,0);
	break;

case 0x19d: /**** ce80 add 3,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	ADD16(AC3,MDR,0);
	break;

case 0x19e: /**** cf00 add 3,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	ADD16(AC3,MDR,0);
	break;

case 0x19f: /**** cf80 add 3,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	ADD16(AC3,MDR,0);
	break;

case 0x1a0: /**** d000 sub 0,%8 ****/
	MAR = IR & 0xFF;
	READ();
	ADD16(AC0,MDR^0xFFFF,1);
	break;

case 0x1a1: /**** d080 sub 0,%8 ****/
	MAR = IR & 0xFF;
	READ();
	ADD16(AC0,MDR^0xFFFF,1);
	break;

case 0x1a2: /**** d100 sub 0,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	ADD16(AC0,MDR^0xFFFF,1);
	break;

case 0x1a3: /**** d180 sub 0,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	ADD16(AC0,MDR^0xFFFF,1);
	break;

case 0x1a4: /**** d200 sub 0,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	ADD16(AC0,MDR^0xFFFF,1);
	break;

case 0x1a5: /**** d280 sub 0,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	ADD16(AC0,MDR^0xFFFF,1);
	break;

case 0x1a6: /**** d300 sub 0,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	ADD16(AC0,MDR^0xFFFF,1);
	break;

case 0x1a7: /**** d380 sub 0,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	ADD16(AC0,MDR^0xFFFF,1);
	break;

case 0x1a8: /**** d400 sub 1,%8 ****/
	MAR = IR & 0xFF;
	READ();
	ADD16(AC1,MDR^0xFFFF,1);
	break;

case 0x1a9: /**** d480 sub 1,%8 ****/
	MAR = IR & 0xFF;
	READ();
	ADD16(AC1,MDR^0xFFFF,1);
	break;

case 0x1aa: /**** d500 sub 1,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	ADD16(AC1,MDR^0xFFFF,1);
	break;

case 0x1ab: /**** d580 sub 1,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	ADD16(AC1,MDR^0xFFFF,1);
	break;

case 0x1ac: /**** d600 sub 1,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	ADD16(AC1,MDR^0xFFFF,1);
	break;

case 0x1ad: /**** d680 sub 1,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	ADD16(AC1,MDR^0xFFFF,1);
	break;

case 0x1ae: /**** d700 sub 1,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	ADD16(AC1,MDR^0xFFFF,1);
	break;

case 0x1af: /**** d780 sub 1,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	ADD16(AC1,MDR^0xFFFF,1);
	break;

case 0x1b0: /**** d800 sub 2,%8 ****/
	MAR = IR & 0xFF;
	READ();
	ADD16(AC2,MDR^0xFFFF,1);
	break;

case 0x1b1: /**** d880 sub 2,%8 ****/
	MAR = IR & 0xFF;
	READ();
	ADD16(AC2,MDR^0xFFFF,1);
	break;

case 0x1b2: /**** d900 sub 2,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	ADD16(AC2,MDR^0xFFFF,1);
	break;

case 0x1b3: /**** d980 sub 2,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	ADD16(AC2,MDR^0xFFFF,1);
	break;

case 0x1b4: /**** da00 sub 2,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	ADD16(AC2,MDR^0xFFFF,1);
	break;

case 0x1b5: /**** da80 sub 2,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	ADD16(AC2,MDR^0xFFFF,1);
	break;

case 0x1b6: /**** db00 sub 2,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	ADD16(AC2,MDR^0xFFFF,1);
	break;

case 0x1b7: /**** db80 sub 2,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	ADD16(AC2,MDR^0xFFFF,1);
	break;

case 0x1b8: /**** dc00 sub 3,%8 ****/
	MAR = IR & 0xFF;
	READ();
	ADD16(AC3,MDR^0xFFFF,1);
	break;

case 0x1b9: /**** dc80 sub 3,%8 ****/
	MAR = IR & 0xFF;
	READ();
	ADD16(AC3,MDR^0xFFFF,1);
	break;

case 0x1ba: /**** dd00 sub 3,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	ADD16(AC3,MDR^0xFFFF,1);
	break;

case 0x1bb: /**** dd80 sub 3,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	ADD16(AC3,MDR^0xFFFF,1);
	break;

case 0x1bc: /**** de00 sub 3,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	ADD16(AC3,MDR^0xFFFF,1);
	break;

case 0x1bd: /**** de80 sub 3,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	ADD16(AC3,MDR^0xFFFF,1);
	break;

case 0x1be: /**** df00 sub 3,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	ADD16(AC3,MDR^0xFFFF,1);
	break;

case 0x1bf: /**** df80 sub 3,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	ADD16(AC3,MDR^0xFFFF,1);
	break;

case 0x1c0: /**** e000 skg 0,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if (((UINT16)AC0) > ((UINT16)MDR)) PC++;
	break;

case 0x1c1: /**** e080 skg 0,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if (((UINT16)AC0) > ((UINT16)MDR)) PC++;
	break;

case 0x1c2: /**** e100 skg 0,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	if (((UINT16)AC0) > ((UINT16)MDR)) PC++;
	break;

case 0x1c3: /**** e180 skg 0,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	if (((UINT16)AC0) > ((UINT16)MDR)) PC++;
	break;

case 0x1c4: /**** e200 skg 0,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	if (((UINT16)AC0) > ((UINT16)MDR)) PC++;
	break;

case 0x1c5: /**** e280 skg 0,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	if (((UINT16)AC0) > ((UINT16)MDR)) PC++;
	break;

case 0x1c6: /**** e300 skg 0,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	if (((UINT16)AC0) > ((UINT16)MDR)) PC++;
	break;

case 0x1c7: /**** e380 skg 0,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	if (((UINT16)AC0) > ((UINT16)MDR)) PC++;
	break;

case 0x1c8: /**** e400 skg 1,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if (((UINT16)AC1) > ((UINT16)MDR)) PC++;
	break;

case 0x1c9: /**** e480 skg 1,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if (((UINT16)AC1) > ((UINT16)MDR)) PC++;
	break;

case 0x1ca: /**** e500 skg 1,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	if (((UINT16)AC1) > ((UINT16)MDR)) PC++;
	break;

case 0x1cb: /**** e580 skg 1,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	if (((UINT16)AC1) > ((UINT16)MDR)) PC++;
	break;

case 0x1cc: /**** e600 skg 1,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	if (((UINT16)AC1) > ((UINT16)MDR)) PC++;
	break;

case 0x1cd: /**** e680 skg 1,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	if (((UINT16)AC1) > ((UINT16)MDR)) PC++;
	break;

case 0x1ce: /**** e700 skg 1,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	if (((UINT16)AC1) > ((UINT16)MDR)) PC++;
	break;

case 0x1cf: /**** e780 skg 1,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	if (((UINT16)AC1) > ((UINT16)MDR)) PC++;
	break;

case 0x1d0: /**** e800 skg 2,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if (((UINT16)AC2) > ((UINT16)MDR)) PC++;
	break;

case 0x1d1: /**** e880 skg 2,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if (((UINT16)AC2) > ((UINT16)MDR)) PC++;
	break;

case 0x1d2: /**** e900 skg 2,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	if (((UINT16)AC2) > ((UINT16)MDR)) PC++;
	break;

case 0x1d3: /**** e980 skg 2,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	if (((UINT16)AC2) > ((UINT16)MDR)) PC++;
	break;

case 0x1d4: /**** ea00 skg 2,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	if (((UINT16)AC2) > ((UINT16)MDR)) PC++;
	break;

case 0x1d5: /**** ea80 skg 2,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	if (((UINT16)AC2) > ((UINT16)MDR)) PC++;
	break;

case 0x1d6: /**** eb00 skg 2,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	if (((UINT16)AC2) > ((UINT16)MDR)) PC++;
	break;

case 0x1d7: /**** eb80 skg 2,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	if (((UINT16)AC2) > ((UINT16)MDR)) PC++;
	break;

case 0x1d8: /**** ec00 skg 3,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if (((UINT16)AC3) > ((UINT16)MDR)) PC++;
	break;

case 0x1d9: /**** ec80 skg 3,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if (((UINT16)AC3) > ((UINT16)MDR)) PC++;
	break;

case 0x1da: /**** ed00 skg 3,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	if (((UINT16)AC3) > ((UINT16)MDR)) PC++;
	break;

case 0x1db: /**** ed80 skg 3,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	if (((UINT16)AC3) > ((UINT16)MDR)) PC++;
	break;

case 0x1dc: /**** ee00 skg 3,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	if (((UINT16)AC3) > ((UINT16)MDR)) PC++;
	break;

case 0x1dd: /**** ee80 skg 3,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	if (((UINT16)AC3) > ((UINT16)MDR)) PC++;
	break;

case 0x1de: /**** ef00 skg 3,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	if (((UINT16)AC3) > ((UINT16)MDR)) PC++;
	break;

case 0x1df: /**** ef80 skg 3,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	if (((UINT16)AC3) > ((UINT16)MDR)) PC++;
	break;

case 0x1e0: /**** f000 skne 0,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if (AC0 != MDR) PC++;
	break;

case 0x1e1: /**** f080 skne 0,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if (AC0 != MDR) PC++;
	break;

case 0x1e2: /**** f100 skne 0,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	if (AC0 != MDR) PC++;
	break;

case 0x1e3: /**** f180 skne 0,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	if (AC0 != MDR) PC++;
	break;

case 0x1e4: /**** f200 skne 0,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	if (AC0 != MDR) PC++;
	break;

case 0x1e5: /**** f280 skne 0,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	if (AC0 != MDR) PC++;
	break;

case 0x1e6: /**** f300 skne 0,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	if (AC0 != MDR) PC++;
	break;

case 0x1e7: /**** f380 skne 0,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	if (AC0 != MDR) PC++;
	break;

case 0x1e8: /**** f400 skne 1,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if (AC1 != MDR) PC++;
	break;

case 0x1e9: /**** f480 skne 1,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if (AC1 != MDR) PC++;
	break;

case 0x1ea: /**** f500 skne 1,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	if (AC1 != MDR) PC++;
	break;

case 0x1eb: /**** f580 skne 1,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	if (AC1 != MDR) PC++;
	break;

case 0x1ec: /**** f600 skne 1,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	if (AC1 != MDR) PC++;
	break;

case 0x1ed: /**** f680 skne 1,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	if (AC1 != MDR) PC++;
	break;

case 0x1ee: /**** f700 skne 1,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	if (AC1 != MDR) PC++;
	break;

case 0x1ef: /**** f780 skne 1,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	if (AC1 != MDR) PC++;
	break;

case 0x1f0: /**** f800 skne 2,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if (AC2 != MDR) PC++;
	break;

case 0x1f1: /**** f880 skne 2,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if (AC2 != MDR) PC++;
	break;

case 0x1f2: /**** f900 skne 2,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	if (AC2 != MDR) PC++;
	break;

case 0x1f3: /**** f980 skne 2,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	if (AC2 != MDR) PC++;
	break;

case 0x1f4: /**** fa00 skne 2,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	if (AC2 != MDR) PC++;
	break;

case 0x1f5: /**** fa80 skne 2,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	if (AC2 != MDR) PC++;
	break;

case 0x1f6: /**** fb00 skne 2,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	if (AC2 != MDR) PC++;
	break;

case 0x1f7: /**** fb80 skne 2,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	if (AC2 != MDR) PC++;
	break;

case 0x1f8: /**** fc00 skne 3,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if (AC3 != MDR) PC++;
	break;

case 0x1f9: /**** fc80 skne 3,%8 ****/
	MAR = IR & 0xFF;
	READ();
	if (AC3 != MDR) PC++;
	break;

case 0x1fa: /**** fd00 skne 3,%s(pc) ****/
	MAR = PC+(IR&0x007F);
	READ();
	if (AC3 != MDR) PC++;
	break;

case 0x1fb: /**** fd80 skne 3,%s(pc) ****/
	MAR = PC+(IR|0xFF00);
	READ();
	if (AC3 != MDR) PC++;
	break;

case 0x1fc: /**** fe00 skne 3,%s(ac2) ****/
	MAR = AC2+(IR&0x007F);
	READ();
	if (AC3 != MDR) PC++;
	break;

case 0x1fd: /**** fe80 skne 3,%s(ac2) ****/
	MAR = AC2+(IR|0xFF00);
	READ();
	if (AC3 != MDR) PC++;
	break;

case 0x1fe: /**** ff00 skne 3,%s(ac3) ****/
	MAR = AC3+(IR&0x007F);
	READ();
	if (AC3 != MDR) PC++;
	break;

case 0x1ff: /**** ff80 skne 3,%s(ac3) ****/
	MAR = AC3+(IR|0xFF00);
	READ();
	if (AC3 != MDR) PC++;
	break;

