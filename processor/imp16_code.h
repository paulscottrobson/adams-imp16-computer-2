WORD16  AC0,AC1,AC2,AC3;
WORD16  FLAGS;
BYTE8   L,OV,CY;
BYTE8   INTEN;
BYTE8   SEL;
BYTE8   HALT;
WORD16  PC;
WORD16  STACK[16];
BYTE8   SPTR;
WORD16  MAR,MDR;
WORD16  IR;
LONG32  temp32;
#define ADD16(a,d,n)  temp32 = (a)+(d)+(n);_CARRY();_OVERFLOW();a = temp32;
#define _CARRY()   CY = ((temp32 >> 16) & 1)
#define _OVERFLOW()  OV = (((a^d) & 0x8000) == 0 && ((a^temp32) & 0x8000) != 0)
#define PUSH(n)  STACK[(SPTR++) & 0x0F] = (n)
#define PULL()  STACK[(--SPTR) & 0x0F]
#define BRANCH(t) if (t) PC = MAR;
WORD16 RotateLeft(WORD16 n,BYTE8 SEL) {
 if (SEL == 0) {
  n = (n << 1) | (n >> 15);
 } else {
  BYTE8 newL = (n & 0x8000) ? 1 : 0;
  n = (n << 1) | (L ? 1 : 0);
  L = newL;
 }
 return 0;
}
WORD16 RotateRight(WORD16 n,BYTE8 SEL) {
 if (SEL == 0) {
  n = (n >> 1) | (n << 15);
 } else {
  BYTE8 newL = (n & 1);
  n = (n >> 1) | (L ? 0x8000 : 0x0000);
  L = newL;
 }
 return 0;
}
WORD16 ShiftLeft(WORD16 n,BYTE8 SEL) {
 if (SEL == 0) {
  n = n >> 1;
 } else {
  L = (n & 0x8000) ? 1 : 0;n = n >> 1;
 }
 return n;
}
WORD16 ShiftRight(WORD16 n,BYTE8 SEL) {
 if (SEL == 0) {
  n = (n >> 1) & 0x7FFF;
 } else {
  n = ((n >> 1) & 0x7FFF) | (L ? 0x8000:0x0000);L = 0;
 }
 return 0;
}
void RegisterOperation(BYTE8 op,WORD16 *psr,WORD16 *pdr) {
 switch (op) {
  case 0: MDR = *psr;*psr = *pdr;*pdr = MDR;break;
  case 1: *pdr = *psr;break;
  case 2: *pdr ^= *psr;break;
  case 3: *pdr &= *psr;break;
 }
}
void IMP16Reset(void) {
 AC0=AC1=AC2=AC3=0;
 HALT=SEL=FLAGS=L=OV=CY=INTEN=0;
 for (int i = 0;i < 16;i++) STACK[i]=0;
 SPTR=0;
 PC = 0xFFFE;
}
void IMP16Interrupt(void) {
 if (INTEN != 0) {
  PUSH(PC);
  PC = 1;
  INTEN = 0;
 }
}