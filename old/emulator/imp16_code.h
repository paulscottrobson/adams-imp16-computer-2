static WORD16   ac0,ac1,ac2,ac3;
static WORD16   pc;
static WORD16   stack[16];
static BYTE8   sp;
static BYTE8   carry;
static BYTE8   link;
static BYTE8   overflow;
static WORD16   flags0_12;
static BYTE8   interruptEnabled;
static BYTE8   sel;
static WORD16   cycles;
static WORD16   opcode;
static WORD16   eac;
static WORD16   temp16;
static INT16   i1,i2;
#define SKIP(test) if (test) pc++
#define PUSH(n) { if (sp < 16) stack[sp] = (n); sp++; }
#define PULL()  ( (sp < 17) ? stack[--sp] : (--sp,0) )
#define CONSTRUCTFLAGREGISTER() (flags0_12 | (carry << 13) | (overflow << 14) | (link << 15))
#define SEXT(n)  ( ((n) & 0x80) ? ((n) | 0xFF00) : (n) )
static void resetIMP16(void) {
 BYTE8 i;
 pc = 0xFFFE;
 ac0 = ac1 = ac2 = ac3 = sp = carry = link = overflow = flags0_12 = interruptEnabled = sel = 0;
 for (i = 0;i < 16;i++) stack[i] = 0;
 cycles = 0x100;
}
static void interruptIMP16(void) {
 if (interruptEnabled) {
  interruptEnabled = 0;PUSH(pc);pc = 1;
 }
}
#define ADD16(a,b)  addFunction(a,b)
#define SUB16(a,b)  addFunction(a,((b) ^ 0xFFFF)+1)
static WORD16 addFunction(WORD16 w1,WORD16 w2) {
 WORD16 result = w1 + w2;
 carry = ((((LONG32)w1)+((LONG32)w2)) >> 16) & 1;
 overflow = 0;
 if (((w1 ^ w2) & 0x8000) == 0) {
  if ((result ^ w2) & 0x8000) overflow = 1;
 }
 return result;
}
static void shift(WORD16 *a,WORD16 qty) {
 BYTE8 count = qty & 0xF;
 if (qty & 0x80) count = (256-qty) & 0xF;
 while (count != 0) {
  count--;
  cycles -= 3;
  if (qty & 0x80) {
   if (sel != 0) link = (*a) & 1;
   *a = (*a) >> 1;
  } else {
   if (sel != 0) link = ((*a) & 0x8000) ? 1 : 0;
   *a = (*a) << 1;
  }
 }
}
static void rotate(WORD16 *a,WORD16 qty) {
 BYTE8 rotSize = 16 + sel;
 BYTE8 count = qty % rotSize;

 if (qty & 0x80) {
  count = rotSize - ((256 - qty)  % rotSize);
 }
 while (count != 0) {
  count--;
  cycles -= 3;
  temp16 = *a;
  if (sel == 0) {
   *a = (temp16 << 1) | (temp16 >> 15);
  } else {
   *a = (temp16 << 1) | link;
   link = (temp16 >> 15) & 1;
  }
 }
}
static void registerInstruction(WORD16 *source,WORD16 *destination,BYTE8 function) {

 switch(function) {
  case 0x00:
  case 0x01:
  case 0x02:
  case 0x03:
   *destination = ADD16(*destination,*source);
   break;
  case 0x80:
   temp16 = *source;*source = *destination;*destination = temp16;
   break;
  case 0x81:
   *destination = *source;
   break;
  case 0x82:
   *destination = (*source) ^ (*destination);
   break;
  case 0x83:
   *destination = (*source) & (*destination);
   break;
 }
}
static void expandFlagRegister(WORD16 v) {
 flags0_12 = v & 0x1FFF;
 carry = (v >> 13) & 1;overflow = (v >> 14) & 1;link = (v >> 15) & 1;
}
static void setControlFlag(BYTE8 state) {
 switch ((opcode >> 8) & 7) {
  case 1:  interruptEnabled = state;break;
  case 2:  sel = state;break;
  default: writeControlFlag(SEXT(opcode & 0x7F),(opcode >> 8) & 7,state,SEXT(opcode & 0xFF));
 }
}