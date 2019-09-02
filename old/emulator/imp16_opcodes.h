case 0x004: // 0400 rin #7
    ac0 = readInput(ac3+(opcode & 0xFF));cycles -= 7;
    break;
case 0x006: // 0600 rout #7
    writeOutput(ac3+(opcode & 0xFF),ac0);cycles -= 7;
    break;
case 0x008: // 0800 #fflg #7
    setControlFlag(1);if (opcode & 0x80) setControlFlag(0);cycles -= 4;
    break;
case 0x009: // 0900 #fflg #7
    setControlFlag(1);if (opcode & 0x80) setControlFlag(0);cycles -= 4;
    break;
case 0x00a: // 0a00 #fflg #7
    setControlFlag(1);if (opcode & 0x80) setControlFlag(0);cycles -= 4;
    break;
case 0x00b: // 0b00 #fflg #7
    setControlFlag(1);if (opcode & 0x80) setControlFlag(0);cycles -= 4;
    break;
case 0x00c: // 0c00 #fflg #7
    setControlFlag(1);if (opcode & 0x80) setControlFlag(0);cycles -= 4;
    break;
case 0x00d: // 0d00 #fflg #7
    setControlFlag(1);if (opcode & 0x80) setControlFlag(0);cycles -= 4;
    break;
case 0x00e: // 0e00 #fflg #7
    setControlFlag(1);if (opcode & 0x80) setControlFlag(0);cycles -= 4;
    break;
case 0x00f: // 0f00 #fflg #7
    setControlFlag(1);if (opcode & 0x80) setControlFlag(0);cycles -= 4;
    break;
case 0x010: // 1000 boc 0,#r
    temp16 = SEXT((opcode & 0xFF));if (0) pc += temp16;cycles -= 4;
    break;
case 0x011: // 1100 boc 1,#r
    temp16 = SEXT((opcode & 0xFF));if (ac0 == 0) pc += temp16;cycles -= 4;
    break;
case 0x012: // 1200 boc 2,#r
    temp16 = SEXT((opcode & 0xFF));if (ac0 != 0 && (ac0 & 0x8000) == 0) pc += temp16;cycles -= 4;
    break;
case 0x013: // 1300 boc 3,#r
    temp16 = SEXT((opcode & 0xFF));if (ac0 & 1) pc += temp16;cycles -= 4;
    break;
case 0x014: // 1400 boc 4,#r
    temp16 = SEXT((opcode & 0xFF));if (ac0 & 2) pc += temp16;cycles -= 4;
    break;
case 0x015: // 1500 boc 5,#r
    temp16 = SEXT((opcode & 0xFF));if (ac0) pc += temp16;cycles -= 4;
    break;
case 0x016: // 1600 boc 6,#r
    temp16 = SEXT((opcode & 0xFF));if (0) pc += temp16;cycles -= 4;
    break;
case 0x017: // 1700 boc 7,#r
    temp16 = SEXT((opcode & 0xFF));if (0) pc += temp16;cycles -= 4;
    break;
case 0x018: // 1800 boc 8,#r
    temp16 = SEXT((opcode & 0xFF));if (sp >= 16) pc += temp16;cycles -= 4;
    break;
case 0x019: // 1900 boc 9,#r
    temp16 = SEXT((opcode & 0xFF));if (interruptEnabled) pc += temp16;cycles -= 4;
    break;
case 0x01a: // 1a00 boc a,#r
    temp16 = SEXT((opcode & 0xFF));if (carry != 0) pc += temp16;cycles -= 4;
    break;
case 0x01b: // 1b00 boc b,#r
    temp16 = SEXT((opcode & 0xFF));if (ac0 == 0 || (ac0 & 0x8000) != 0) pc += temp16;cycles -= 4;
    break;
case 0x01c: // 1c00 boc c,#r
    temp16 = SEXT((opcode & 0xFF));if (readConditionCodeLine(12)) pc += temp16;cycles -= 4;
    break;
case 0x01d: // 1d00 boc d,#r
    temp16 = SEXT((opcode & 0xFF));if (readConditionCodeLine(13)) pc += temp16;cycles -= 4;
    break;
case 0x01e: // 1e00 boc e,#r
    temp16 = SEXT((opcode & 0xFF));if (readConditionCodeLine(14)) pc += temp16;cycles -= 4;
    break;
case 0x01f: // 1f00 boc f,#r
    temp16 = SEXT((opcode & 0xFF));if (readConditionCodeLine(15)) pc += temp16;cycles -= 4;
    break;
case 0x020: // 2000 jmp #i
    eac = opcode & 0xFF;pc = eac;cycles -= 3;
    break;
case 0x021: // 2100 jmp #r
    eac = SEXT(opcode & 0xFF) + pc;pc = eac;cycles -= 3;
    break;
case 0x022: // 2200 jmp #8(2)
    eac = SEXT(opcode & 0xFF) + ac2;pc = eac;cycles -= 3;
    break;
case 0x023: // 2300 jmp #8(3)
    eac = SEXT(opcode & 0xFF) + ac3;pc = eac;cycles -= 3;
    break;
case 0x024: // 2400 jmp @#i
    eac = opcode & 0xFF;pc = readMemory(eac);cycles -= 5;
    break;
case 0x025: // 2500 jmp @#r
    eac = SEXT(opcode & 0xFF) + pc;pc = readMemory(eac);cycles -= 5;
    break;
case 0x026: // 2600 jmp @#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;pc = readMemory(eac);cycles -= 5;
    break;
case 0x027: // 2700 jmp @#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;pc = readMemory(eac);cycles -= 5;
    break;
case 0x028: // 2800 jsr #i
    eac = opcode & 0xFF;PUSH(pc);pc = eac;cycles -= 4;
    break;
case 0x029: // 2900 jsr #r
    eac = SEXT(opcode & 0xFF) + pc;PUSH(pc);pc = eac;cycles -= 4;
    break;
case 0x02a: // 2a00 jsr #8(2)
    eac = SEXT(opcode & 0xFF) + ac2;PUSH(pc);pc = eac;cycles -= 4;
    break;
case 0x02b: // 2b00 jsr #8(3)
    eac = SEXT(opcode & 0xFF) + ac3;PUSH(pc);pc = eac;cycles -= 4;
    break;
case 0x02c: // 2c00 jsr @#i
    eac = opcode & 0xFF;PUSH(pc);pc = readMemory(eac);cycles -= 6;
    break;
case 0x02d: // 2d00 jsr @#r
    eac = SEXT(opcode & 0xFF) + pc;PUSH(pc);pc = readMemory(eac);cycles -= 6;
    break;
case 0x02e: // 2e00 jsr @#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;PUSH(pc);pc = readMemory(eac);cycles -= 6;
    break;
case 0x02f: // 2f00 jsr @#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;PUSH(pc);pc = readMemory(eac);cycles -= 6;
    break;
case 0x030: // 3000 #x 0,0
    registerInstruction(&ac0,&ac0,opcode & 0x83);cycles -= 6;
    break;
case 0x031: // 3100 #x 1,0
    registerInstruction(&ac0,&ac1,opcode & 0x83);cycles -= 6;
    break;
case 0x032: // 3200 #x 2,0
    registerInstruction(&ac0,&ac2,opcode & 0x83);cycles -= 6;
    break;
case 0x033: // 3300 #x 3,0
    registerInstruction(&ac0,&ac3,opcode & 0x83);cycles -= 6;
    break;
case 0x034: // 3400 #x 0,1
    registerInstruction(&ac1,&ac0,opcode & 0x83);cycles -= 6;
    break;
case 0x035: // 3500 #x 1,1
    registerInstruction(&ac1,&ac1,opcode & 0x83);cycles -= 6;
    break;
case 0x036: // 3600 #x 2,1
    registerInstruction(&ac1,&ac2,opcode & 0x83);cycles -= 6;
    break;
case 0x037: // 3700 #x 3,1
    registerInstruction(&ac1,&ac3,opcode & 0x83);cycles -= 6;
    break;
case 0x038: // 3800 #x 0,2
    registerInstruction(&ac2,&ac0,opcode & 0x83);cycles -= 6;
    break;
case 0x039: // 3900 #x 1,2
    registerInstruction(&ac2,&ac1,opcode & 0x83);cycles -= 6;
    break;
case 0x03a: // 3a00 #x 2,2
    registerInstruction(&ac2,&ac2,opcode & 0x83);cycles -= 6;
    break;
case 0x03b: // 3b00 #x 3,2
    registerInstruction(&ac2,&ac3,opcode & 0x83);cycles -= 6;
    break;
case 0x03c: // 3c00 #x 0,3
    registerInstruction(&ac3,&ac0,opcode & 0x83);cycles -= 6;
    break;
case 0x03d: // 3d00 #x 1,3
    registerInstruction(&ac3,&ac1,opcode & 0x83);cycles -= 6;
    break;
case 0x03e: // 3e00 #x 2,3
    registerInstruction(&ac3,&ac2,opcode & 0x83);cycles -= 6;
    break;
case 0x03f: // 3f00 #x 3,3
    registerInstruction(&ac3,&ac3,opcode & 0x83);cycles -= 6;
    break;
case 0x040: // 4000 push 0
    PUSH(ac0);cycles -= 3;
    break;
case 0x041: // 4100 push 1
    PUSH(ac1);cycles -= 3;
    break;
case 0x042: // 4200 push 2
    PUSH(ac2);cycles -= 3;
    break;
case 0x043: // 4300 push 3
    PUSH(ac3);cycles -= 3;
    break;
case 0x044: // 4400 pull 0
    ac0 = PULL();cycles -= 3;
    break;
case 0x045: // 4500 pull 1
    ac1 = PULL();cycles -= 3;
    break;
case 0x046: // 4600 pull 2
    ac2 = PULL();cycles -= 3;
    break;
case 0x047: // 4700 pull 3
    ac3 = PULL();cycles -= 3;
    break;
case 0x048: // 4800 aisz 0,#8
    ac0 = ADD16(ac0,SEXT((opcode & 0xFF)));SKIP(ac0 == 0);cycles -= 4;
    break;
case 0x049: // 4900 aisz 1,#8
    ac1 = ADD16(ac1,SEXT((opcode & 0xFF)));SKIP(ac1 == 0);cycles -= 4;
    break;
case 0x04a: // 4a00 aisz 2,#8
    ac2 = ADD16(ac2,SEXT((opcode & 0xFF)));SKIP(ac2 == 0);cycles -= 4;
    break;
case 0x04b: // 4b00 aisz 3,#8
    ac3 = ADD16(ac3,SEXT((opcode & 0xFF)));SKIP(ac3 == 0);cycles -= 4;
    break;
case 0x04c: // 4c00 li 0,#8
    ac0 = SEXT((opcode & 0xFF));cycles -= 3;
    break;
case 0x04d: // 4d00 li 1,#8
    ac1 = SEXT((opcode & 0xFF));cycles -= 3;
    break;
case 0x04e: // 4e00 li 2,#8
    ac2 = SEXT((opcode & 0xFF));cycles -= 3;
    break;
case 0x04f: // 4f00 li 3,#8
    ac3 = SEXT((opcode & 0xFF));cycles -= 3;
    break;
case 0x050: // 5000 cai 0,#8
    ac0 = (ac0 ^ 0xFFFF) + SEXT((opcode & 0xFF));cycles -= 3;
    break;
case 0x051: // 5100 cai 1,#8
    ac1 = (ac1 ^ 0xFFFF) + SEXT((opcode & 0xFF));cycles -= 3;
    break;
case 0x052: // 5200 cai 2,#8
    ac2 = (ac2 ^ 0xFFFF) + SEXT((opcode & 0xFF));cycles -= 3;
    break;
case 0x053: // 5300 cai 3,#8
    ac3 = (ac3 ^ 0xFFFF) + SEXT((opcode & 0xFF));cycles -= 3;
    break;
case 0x054: // 5400 xchrs 0
    temp16 = PULL();PUSH(ac0);ac0 = temp16;cycles -= 5;
    break;
case 0x055: // 5500 xchrs 1
    temp16 = PULL();PUSH(ac1);ac1 = temp16;cycles -= 5;
    break;
case 0x056: // 5600 xchrs 2
    temp16 = PULL();PUSH(ac2);ac2 = temp16;cycles -= 5;
    break;
case 0x057: // 5700 xchrs 3
    temp16 = PULL();PUSH(ac3);ac3 = temp16;cycles -= 5;
    break;
case 0x058: // 5800 ro#d 0,#q
    rotate(&ac0,(opcode & 0xFF));cycles -= 4;
    break;
case 0x059: // 5900 ro#d 1,#q
    rotate(&ac1,(opcode & 0xFF));cycles -= 4;
    break;
case 0x05a: // 5a00 ro#d 2,#q
    rotate(&ac2,(opcode & 0xFF));cycles -= 4;
    break;
case 0x05b: // 5b00 ro#d 3,#q
    rotate(&ac3,(opcode & 0xFF));cycles -= 4;
    break;
case 0x05c: // 5c00 sh#d 0,#q
    shift(&ac0,(opcode & 0xFF));cycles -= 4;
    break;
case 0x05d: // 5d00 sh#d 1,#q
    shift(&ac1,(opcode & 0xFF));cycles -= 4;
    break;
case 0x05e: // 5e00 sh#d 2,#q
    shift(&ac2,(opcode & 0xFF));cycles -= 4;
    break;
case 0x05f: // 5f00 sh#d 3,#q
    shift(&ac3,(opcode & 0xFF));cycles -= 4;
    break;
case 0x060: // 6000 and 0,#i
    eac = opcode & 0xFF;ac0 &= readMemory(eac);cycles -= 5;
    break;
case 0x061: // 6100 and 0,#r
    eac = SEXT(opcode & 0xFF) + pc;ac0 &= readMemory(eac);cycles -= 5;
    break;
case 0x062: // 6200 and 0,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac0 &= readMemory(eac);cycles -= 5;
    break;
case 0x063: // 6300 and 0,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac0 &= readMemory(eac);cycles -= 5;
    break;
case 0x064: // 6400 and 1,#i
    eac = opcode & 0xFF;ac1 &= readMemory(eac);cycles -= 5;
    break;
case 0x065: // 6500 and 1,#r
    eac = SEXT(opcode & 0xFF) + pc;ac1 &= readMemory(eac);cycles -= 5;
    break;
case 0x066: // 6600 and 1,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac1 &= readMemory(eac);cycles -= 5;
    break;
case 0x067: // 6700 and 1,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac1 &= readMemory(eac);cycles -= 5;
    break;
case 0x068: // 6800 or 0,#i
    eac = opcode & 0xFF;ac0 |= readMemory(eac);cycles -= 5;
    break;
case 0x069: // 6900 or 0,#r
    eac = SEXT(opcode & 0xFF) + pc;ac0 |= readMemory(eac);cycles -= 5;
    break;
case 0x06a: // 6a00 or 0,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac0 |= readMemory(eac);cycles -= 5;
    break;
case 0x06b: // 6b00 or 0,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac0 |= readMemory(eac);cycles -= 5;
    break;
case 0x06c: // 6c00 or 1,#i
    eac = opcode & 0xFF;ac1 |= readMemory(eac);cycles -= 5;
    break;
case 0x06d: // 6d00 or 1,#r
    eac = SEXT(opcode & 0xFF) + pc;ac1 |= readMemory(eac);cycles -= 5;
    break;
case 0x06e: // 6e00 or 1,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac1 |= readMemory(eac);cycles -= 5;
    break;
case 0x06f: // 6f00 or 1,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac1 |= readMemory(eac);cycles -= 5;
    break;
case 0x070: // 7000 skaz 0,#i
    eac = opcode & 0xFF;SKIP((ac0 & readMemory(eac)) == 0);cycles -= 6;
    break;
case 0x071: // 7100 skaz 0,#r
    eac = SEXT(opcode & 0xFF) + pc;SKIP((ac0 & readMemory(eac)) == 0);cycles -= 6;
    break;
case 0x072: // 7200 skaz 0,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;SKIP((ac0 & readMemory(eac)) == 0);cycles -= 6;
    break;
case 0x073: // 7300 skaz 0,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;SKIP((ac0 & readMemory(eac)) == 0);cycles -= 6;
    break;
case 0x074: // 7400 skaz 1,#i
    eac = opcode & 0xFF;SKIP((ac1 & readMemory(eac)) == 0);cycles -= 6;
    break;
case 0x075: // 7500 skaz 1,#r
    eac = SEXT(opcode & 0xFF) + pc;SKIP((ac1 & readMemory(eac)) == 0);cycles -= 6;
    break;
case 0x076: // 7600 skaz 1,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;SKIP((ac1 & readMemory(eac)) == 0);cycles -= 6;
    break;
case 0x077: // 7700 skaz 1,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;SKIP((ac1 & readMemory(eac)) == 0);cycles -= 6;
    break;
case 0x078: // 7800 isz #i
    eac = opcode & 0xFF;temp16 = readMemory(eac)+1;writeMemory(eac,temp16);SKIP(temp16 == 0);cycles -= 7;
    break;
case 0x079: // 7900 isz #r
    eac = SEXT(opcode & 0xFF) + pc;temp16 = readMemory(eac)+1;writeMemory(eac,temp16);SKIP(temp16 == 0);cycles -= 7;
    break;
case 0x07a: // 7a00 isz #8(2)
    eac = SEXT(opcode & 0xFF) + ac2;temp16 = readMemory(eac)+1;writeMemory(eac,temp16);SKIP(temp16 == 0);cycles -= 7;
    break;
case 0x07b: // 7b00 isz #8(3)
    eac = SEXT(opcode & 0xFF) + ac3;temp16 = readMemory(eac)+1;writeMemory(eac,temp16);SKIP(temp16 == 0);cycles -= 7;
    break;
case 0x07c: // 7c00 dsz #i
    eac = opcode & 0xFF;temp16 = readMemory(eac)-1;writeMemory(eac,temp16);SKIP(temp16 == 0);cycles -= 7;
    break;
case 0x07d: // 7d00 dsz #r
    eac = SEXT(opcode & 0xFF) + pc;temp16 = readMemory(eac)-1;writeMemory(eac,temp16);SKIP(temp16 == 0);cycles -= 7;
    break;
case 0x07e: // 7e00 dsz #8(2)
    eac = SEXT(opcode & 0xFF) + ac2;temp16 = readMemory(eac)-1;writeMemory(eac,temp16);SKIP(temp16 == 0);cycles -= 7;
    break;
case 0x07f: // 7f00 dsz #8(3)
    eac = SEXT(opcode & 0xFF) + ac3;temp16 = readMemory(eac)-1;writeMemory(eac,temp16);SKIP(temp16 == 0);cycles -= 7;
    break;
case 0x080: // 8000 ld 0,#i
    eac = opcode & 0xFF;ac0 = readMemory(eac);cycles -= 5;
    break;
case 0x081: // 8100 ld 0,#r
    eac = SEXT(opcode & 0xFF) + pc;ac0 = readMemory(eac);cycles -= 5;
    break;
case 0x082: // 8200 ld 0,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac0 = readMemory(eac);cycles -= 5;
    break;
case 0x083: // 8300 ld 0,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac0 = readMemory(eac);cycles -= 5;
    break;
case 0x084: // 8400 ld 1,#i
    eac = opcode & 0xFF;ac1 = readMemory(eac);cycles -= 5;
    break;
case 0x085: // 8500 ld 1,#r
    eac = SEXT(opcode & 0xFF) + pc;ac1 = readMemory(eac);cycles -= 5;
    break;
case 0x086: // 8600 ld 1,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac1 = readMemory(eac);cycles -= 5;
    break;
case 0x087: // 8700 ld 1,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac1 = readMemory(eac);cycles -= 5;
    break;
case 0x088: // 8800 ld 2,#i
    eac = opcode & 0xFF;ac2 = readMemory(eac);cycles -= 5;
    break;
case 0x089: // 8900 ld 2,#r
    eac = SEXT(opcode & 0xFF) + pc;ac2 = readMemory(eac);cycles -= 5;
    break;
case 0x08a: // 8a00 ld 2,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac2 = readMemory(eac);cycles -= 5;
    break;
case 0x08b: // 8b00 ld 2,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac2 = readMemory(eac);cycles -= 5;
    break;
case 0x08c: // 8c00 ld 3,#i
    eac = opcode & 0xFF;ac3 = readMemory(eac);cycles -= 5;
    break;
case 0x08d: // 8d00 ld 3,#r
    eac = SEXT(opcode & 0xFF) + pc;ac3 = readMemory(eac);cycles -= 5;
    break;
case 0x08e: // 8e00 ld 3,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac3 = readMemory(eac);cycles -= 5;
    break;
case 0x08f: // 8f00 ld 3,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac3 = readMemory(eac);cycles -= 5;
    break;
case 0x090: // 9000 ld 0,@#i
    eac = opcode & 0xFF;ac0 = readMemory(readMemory(eac));cycles -= 6;
    break;
case 0x091: // 9100 ld 0,@#r
    eac = SEXT(opcode & 0xFF) + pc;ac0 = readMemory(readMemory(eac));cycles -= 6;
    break;
case 0x092: // 9200 ld 0,@#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac0 = readMemory(readMemory(eac));cycles -= 6;
    break;
case 0x093: // 9300 ld 0,@#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac0 = readMemory(readMemory(eac));cycles -= 6;
    break;
case 0x094: // 9400 ld 1,@#i
    eac = opcode & 0xFF;ac1 = readMemory(readMemory(eac));cycles -= 6;
    break;
case 0x095: // 9500 ld 1,@#r
    eac = SEXT(opcode & 0xFF) + pc;ac1 = readMemory(readMemory(eac));cycles -= 6;
    break;
case 0x096: // 9600 ld 1,@#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac1 = readMemory(readMemory(eac));cycles -= 6;
    break;
case 0x097: // 9700 ld 1,@#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac1 = readMemory(readMemory(eac));cycles -= 6;
    break;
case 0x098: // 9800 ld 2,@#i
    eac = opcode & 0xFF;ac2 = readMemory(readMemory(eac));cycles -= 6;
    break;
case 0x099: // 9900 ld 2,@#r
    eac = SEXT(opcode & 0xFF) + pc;ac2 = readMemory(readMemory(eac));cycles -= 6;
    break;
case 0x09a: // 9a00 ld 2,@#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac2 = readMemory(readMemory(eac));cycles -= 6;
    break;
case 0x09b: // 9b00 ld 2,@#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac2 = readMemory(readMemory(eac));cycles -= 6;
    break;
case 0x09c: // 9c00 ld 3,@#i
    eac = opcode & 0xFF;ac3 = readMemory(readMemory(eac));cycles -= 6;
    break;
case 0x09d: // 9d00 ld 3,@#r
    eac = SEXT(opcode & 0xFF) + pc;ac3 = readMemory(readMemory(eac));cycles -= 6;
    break;
case 0x09e: // 9e00 ld 3,@#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac3 = readMemory(readMemory(eac));cycles -= 6;
    break;
case 0x09f: // 9f00 ld 3,@#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac3 = readMemory(readMemory(eac));cycles -= 6;
    break;
case 0x0a0: // a000 st 0,#i
    eac = opcode & 0xFF;writeMemory(eac,ac0);cycles -= 5;
    break;
case 0x0a1: // a100 st 0,#r
    eac = SEXT(opcode & 0xFF) + pc;writeMemory(eac,ac0);cycles -= 5;
    break;
case 0x0a2: // a200 st 0,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;writeMemory(eac,ac0);cycles -= 5;
    break;
case 0x0a3: // a300 st 0,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;writeMemory(eac,ac0);cycles -= 5;
    break;
case 0x0a4: // a400 st 1,#i
    eac = opcode & 0xFF;writeMemory(eac,ac1);cycles -= 5;
    break;
case 0x0a5: // a500 st 1,#r
    eac = SEXT(opcode & 0xFF) + pc;writeMemory(eac,ac1);cycles -= 5;
    break;
case 0x0a6: // a600 st 1,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;writeMemory(eac,ac1);cycles -= 5;
    break;
case 0x0a7: // a700 st 1,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;writeMemory(eac,ac1);cycles -= 5;
    break;
case 0x0a8: // a800 st 2,#i
    eac = opcode & 0xFF;writeMemory(eac,ac2);cycles -= 5;
    break;
case 0x0a9: // a900 st 2,#r
    eac = SEXT(opcode & 0xFF) + pc;writeMemory(eac,ac2);cycles -= 5;
    break;
case 0x0aa: // aa00 st 2,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;writeMemory(eac,ac2);cycles -= 5;
    break;
case 0x0ab: // ab00 st 2,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;writeMemory(eac,ac2);cycles -= 5;
    break;
case 0x0ac: // ac00 st 3,#i
    eac = opcode & 0xFF;writeMemory(eac,ac3);cycles -= 5;
    break;
case 0x0ad: // ad00 st 3,#r
    eac = SEXT(opcode & 0xFF) + pc;writeMemory(eac,ac3);cycles -= 5;
    break;
case 0x0ae: // ae00 st 3,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;writeMemory(eac,ac3);cycles -= 5;
    break;
case 0x0af: // af00 st 3,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;writeMemory(eac,ac3);cycles -= 5;
    break;
case 0x0b0: // b000 st 0,@#i
    eac = opcode & 0xFF;writeMemory(readMemory(eac),ac0);cycles -= 8;
    break;
case 0x0b1: // b100 st 0,@#r
    eac = SEXT(opcode & 0xFF) + pc;writeMemory(readMemory(eac),ac0);cycles -= 8;
    break;
case 0x0b2: // b200 st 0,@#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;writeMemory(readMemory(eac),ac0);cycles -= 8;
    break;
case 0x0b3: // b300 st 0,@#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;writeMemory(readMemory(eac),ac0);cycles -= 8;
    break;
case 0x0b4: // b400 st 1,@#i
    eac = opcode & 0xFF;writeMemory(readMemory(eac),ac1);cycles -= 8;
    break;
case 0x0b5: // b500 st 1,@#r
    eac = SEXT(opcode & 0xFF) + pc;writeMemory(readMemory(eac),ac1);cycles -= 8;
    break;
case 0x0b6: // b600 st 1,@#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;writeMemory(readMemory(eac),ac1);cycles -= 8;
    break;
case 0x0b7: // b700 st 1,@#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;writeMemory(readMemory(eac),ac1);cycles -= 8;
    break;
case 0x0b8: // b800 st 2,@#i
    eac = opcode & 0xFF;writeMemory(readMemory(eac),ac2);cycles -= 8;
    break;
case 0x0b9: // b900 st 2,@#r
    eac = SEXT(opcode & 0xFF) + pc;writeMemory(readMemory(eac),ac2);cycles -= 8;
    break;
case 0x0ba: // ba00 st 2,@#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;writeMemory(readMemory(eac),ac2);cycles -= 8;
    break;
case 0x0bb: // bb00 st 2,@#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;writeMemory(readMemory(eac),ac2);cycles -= 8;
    break;
case 0x0bc: // bc00 st 3,@#i
    eac = opcode & 0xFF;writeMemory(readMemory(eac),ac3);cycles -= 8;
    break;
case 0x0bd: // bd00 st 3,@#r
    eac = SEXT(opcode & 0xFF) + pc;writeMemory(readMemory(eac),ac3);cycles -= 8;
    break;
case 0x0be: // be00 st 3,@#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;writeMemory(readMemory(eac),ac3);cycles -= 8;
    break;
case 0x0bf: // bf00 st 3,@#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;writeMemory(readMemory(eac),ac3);cycles -= 8;
    break;
case 0x0c0: // c000 add 0,#i
    eac = opcode & 0xFF;ac0 = ADD16(ac0,readMemory(eac));cycles -= 5;
    break;
case 0x0c1: // c100 add 0,#r
    eac = SEXT(opcode & 0xFF) + pc;ac0 = ADD16(ac0,readMemory(eac));cycles -= 5;
    break;
case 0x0c2: // c200 add 0,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac0 = ADD16(ac0,readMemory(eac));cycles -= 5;
    break;
case 0x0c3: // c300 add 0,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac0 = ADD16(ac0,readMemory(eac));cycles -= 5;
    break;
case 0x0c4: // c400 add 1,#i
    eac = opcode & 0xFF;ac1 = ADD16(ac1,readMemory(eac));cycles -= 5;
    break;
case 0x0c5: // c500 add 1,#r
    eac = SEXT(opcode & 0xFF) + pc;ac1 = ADD16(ac1,readMemory(eac));cycles -= 5;
    break;
case 0x0c6: // c600 add 1,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac1 = ADD16(ac1,readMemory(eac));cycles -= 5;
    break;
case 0x0c7: // c700 add 1,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac1 = ADD16(ac1,readMemory(eac));cycles -= 5;
    break;
case 0x0c8: // c800 add 2,#i
    eac = opcode & 0xFF;ac2 = ADD16(ac2,readMemory(eac));cycles -= 5;
    break;
case 0x0c9: // c900 add 2,#r
    eac = SEXT(opcode & 0xFF) + pc;ac2 = ADD16(ac2,readMemory(eac));cycles -= 5;
    break;
case 0x0ca: // ca00 add 2,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac2 = ADD16(ac2,readMemory(eac));cycles -= 5;
    break;
case 0x0cb: // cb00 add 2,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac2 = ADD16(ac2,readMemory(eac));cycles -= 5;
    break;
case 0x0cc: // cc00 add 3,#i
    eac = opcode & 0xFF;ac3 = ADD16(ac3,readMemory(eac));cycles -= 5;
    break;
case 0x0cd: // cd00 add 3,#r
    eac = SEXT(opcode & 0xFF) + pc;ac3 = ADD16(ac3,readMemory(eac));cycles -= 5;
    break;
case 0x0ce: // ce00 add 3,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac3 = ADD16(ac3,readMemory(eac));cycles -= 5;
    break;
case 0x0cf: // cf00 add 3,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac3 = ADD16(ac3,readMemory(eac));cycles -= 5;
    break;
case 0x0d0: // d000 sub 0,#i
    eac = opcode & 0xFF;ac0 = SUB16(ac0,readMemory(eac));cycles -= 5;
    break;
case 0x0d1: // d100 sub 0,#r
    eac = SEXT(opcode & 0xFF) + pc;ac0 = SUB16(ac0,readMemory(eac));cycles -= 5;
    break;
case 0x0d2: // d200 sub 0,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac0 = SUB16(ac0,readMemory(eac));cycles -= 5;
    break;
case 0x0d3: // d300 sub 0,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac0 = SUB16(ac0,readMemory(eac));cycles -= 5;
    break;
case 0x0d4: // d400 sub 1,#i
    eac = opcode & 0xFF;ac1 = SUB16(ac1,readMemory(eac));cycles -= 5;
    break;
case 0x0d5: // d500 sub 1,#r
    eac = SEXT(opcode & 0xFF) + pc;ac1 = SUB16(ac1,readMemory(eac));cycles -= 5;
    break;
case 0x0d6: // d600 sub 1,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac1 = SUB16(ac1,readMemory(eac));cycles -= 5;
    break;
case 0x0d7: // d700 sub 1,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac1 = SUB16(ac1,readMemory(eac));cycles -= 5;
    break;
case 0x0d8: // d800 sub 2,#i
    eac = opcode & 0xFF;ac2 = SUB16(ac2,readMemory(eac));cycles -= 5;
    break;
case 0x0d9: // d900 sub 2,#r
    eac = SEXT(opcode & 0xFF) + pc;ac2 = SUB16(ac2,readMemory(eac));cycles -= 5;
    break;
case 0x0da: // da00 sub 2,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac2 = SUB16(ac2,readMemory(eac));cycles -= 5;
    break;
case 0x0db: // db00 sub 2,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac2 = SUB16(ac2,readMemory(eac));cycles -= 5;
    break;
case 0x0dc: // dc00 sub 3,#i
    eac = opcode & 0xFF;ac3 = SUB16(ac3,readMemory(eac));cycles -= 5;
    break;
case 0x0dd: // dd00 sub 3,#r
    eac = SEXT(opcode & 0xFF) + pc;ac3 = SUB16(ac3,readMemory(eac));cycles -= 5;
    break;
case 0x0de: // de00 sub 3,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;ac3 = SUB16(ac3,readMemory(eac));cycles -= 5;
    break;
case 0x0df: // df00 sub 3,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;ac3 = SUB16(ac3,readMemory(eac));cycles -= 5;
    break;
case 0x0e0: // e000 skg 0,#i
    eac = opcode & 0xFF;i1 = ac0;i2 = readMemory(eac);SKIP(i1 > i2);cycles -= 8;
    break;
case 0x0e1: // e100 skg 0,#r
    eac = SEXT(opcode & 0xFF) + pc;i1 = ac0;i2 = readMemory(eac);SKIP(i1 > i2);cycles -= 8;
    break;
case 0x0e2: // e200 skg 0,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;i1 = ac0;i2 = readMemory(eac);SKIP(i1 > i2);cycles -= 8;
    break;
case 0x0e3: // e300 skg 0,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;i1 = ac0;i2 = readMemory(eac);SKIP(i1 > i2);cycles -= 8;
    break;
case 0x0e4: // e400 skg 1,#i
    eac = opcode & 0xFF;i1 = ac1;i2 = readMemory(eac);SKIP(i1 > i2);cycles -= 8;
    break;
case 0x0e5: // e500 skg 1,#r
    eac = SEXT(opcode & 0xFF) + pc;i1 = ac1;i2 = readMemory(eac);SKIP(i1 > i2);cycles -= 8;
    break;
case 0x0e6: // e600 skg 1,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;i1 = ac1;i2 = readMemory(eac);SKIP(i1 > i2);cycles -= 8;
    break;
case 0x0e7: // e700 skg 1,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;i1 = ac1;i2 = readMemory(eac);SKIP(i1 > i2);cycles -= 8;
    break;
case 0x0e8: // e800 skg 2,#i
    eac = opcode & 0xFF;i1 = ac2;i2 = readMemory(eac);SKIP(i1 > i2);cycles -= 8;
    break;
case 0x0e9: // e900 skg 2,#r
    eac = SEXT(opcode & 0xFF) + pc;i1 = ac2;i2 = readMemory(eac);SKIP(i1 > i2);cycles -= 8;
    break;
case 0x0ea: // ea00 skg 2,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;i1 = ac2;i2 = readMemory(eac);SKIP(i1 > i2);cycles -= 8;
    break;
case 0x0eb: // eb00 skg 2,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;i1 = ac2;i2 = readMemory(eac);SKIP(i1 > i2);cycles -= 8;
    break;
case 0x0ec: // ec00 skg 3,#i
    eac = opcode & 0xFF;i1 = ac3;i2 = readMemory(eac);SKIP(i1 > i2);cycles -= 8;
    break;
case 0x0ed: // ed00 skg 3,#r
    eac = SEXT(opcode & 0xFF) + pc;i1 = ac3;i2 = readMemory(eac);SKIP(i1 > i2);cycles -= 8;
    break;
case 0x0ee: // ee00 skg 3,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;i1 = ac3;i2 = readMemory(eac);SKIP(i1 > i2);cycles -= 8;
    break;
case 0x0ef: // ef00 skg 3,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;i1 = ac3;i2 = readMemory(eac);SKIP(i1 > i2);cycles -= 8;
    break;
case 0x0f0: // f000 skne 0,#i
    eac = opcode & 0xFF;SKIP(ac0 != readMemory(eac));cycles -= 8;
    break;
case 0x0f1: // f100 skne 0,#r
    eac = SEXT(opcode & 0xFF) + pc;SKIP(ac0 != readMemory(eac));cycles -= 8;
    break;
case 0x0f2: // f200 skne 0,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;SKIP(ac0 != readMemory(eac));cycles -= 8;
    break;
case 0x0f3: // f300 skne 0,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;SKIP(ac0 != readMemory(eac));cycles -= 8;
    break;
case 0x0f4: // f400 skne 1,#i
    eac = opcode & 0xFF;SKIP(ac1 != readMemory(eac));cycles -= 8;
    break;
case 0x0f5: // f500 skne 1,#r
    eac = SEXT(opcode & 0xFF) + pc;SKIP(ac1 != readMemory(eac));cycles -= 8;
    break;
case 0x0f6: // f600 skne 1,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;SKIP(ac1 != readMemory(eac));cycles -= 8;
    break;
case 0x0f7: // f700 skne 1,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;SKIP(ac1 != readMemory(eac));cycles -= 8;
    break;
case 0x0f8: // f800 skne 2,#i
    eac = opcode & 0xFF;SKIP(ac2 != readMemory(eac));cycles -= 8;
    break;
case 0x0f9: // f900 skne 2,#r
    eac = SEXT(opcode & 0xFF) + pc;SKIP(ac2 != readMemory(eac));cycles -= 8;
    break;
case 0x0fa: // fa00 skne 2,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;SKIP(ac2 != readMemory(eac));cycles -= 8;
    break;
case 0x0fb: // fb00 skne 2,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;SKIP(ac2 != readMemory(eac));cycles -= 8;
    break;
case 0x0fc: // fc00 skne 3,#i
    eac = opcode & 0xFF;SKIP(ac3 != readMemory(eac));cycles -= 8;
    break;
case 0x0fd: // fd00 skne 3,#r
    eac = SEXT(opcode & 0xFF) + pc;SKIP(ac3 != readMemory(eac));cycles -= 8;
    break;
case 0x0fe: // fe00 skne 3,#8(2)
    eac = SEXT(opcode & 0xFF) + ac2;SKIP(ac3 != readMemory(eac));cycles -= 8;
    break;
case 0x0ff: // ff00 skne 3,#8(3)
    eac = SEXT(opcode & 0xFF) + ac3;SKIP(ac3 != readMemory(eac));cycles -= 8;
    break;
case 0x100: // 0000 halt
    pc--;cycles -= 0;
    break;
case 0x101: // 0080 pushf
    PUSH(CONSTRUCTFLAGREGISTER());cycles -= 4;
    break;
case 0x102: // 0100 rti #7
    interruptEnabled = 1;pc = PULL() + ((opcode & 0xFF));cycles -= 5;
    break;
case 0x103: // 0180 frame
    cycles = 0xFFFF;cycles -= 0;
    break;
case 0x104: // 0200 rts #7
    pc = PULL() + ((opcode & 0xFF));cycles -= 4;
    break;
case 0x105: // 0280 pullf
    expandFlagRegister(PULL());cycles -= 5;
    break;
case 0x107: // 0380 jsri ff#i
    PUSH(pc);pc = 0xFF00|((opcode & 0xFF));cycles -= 4;
    break;
