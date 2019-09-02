#
#			Process imp16.def
#
import re

#	%E 		Code for Effective Address Calculation, same as %A code to put in 'eac' 
#	%T 		Test implied by condition number (bits 8-11)

def process(s):
	n = s.find("//")
	if n >= 0:
		s = s[:n]
	return s

def substitute(s,offset):
	s = s.replace("%8","(opcode & 0xFF)")
	eac = [ "#I","#R","#8(2)","#8(3)"]
	s = s.replace("%A",eac[offset % 4])
	s = s.replace("%C","{0:x}".format(offset % 16))
	s = s.replace("%R",str(offset/4%4))
	s = s.replace("%S",str(offset%4))

	eac[0] = "eac = opcode & 0xFF"
	eac[1] = "eac = SEXT(opcode & 0xFF) + pc"
	eac[2] = "eac = SEXT(opcode & 0xFF) + ac2"
	eac[3] = "eac = SEXT(opcode & 0xFF) + ac3"

	tests = [ "" ] * 16
	tests[0] = "0"
	tests[1] = "ac0 == 0"
	tests[2] = "ac0 != 0 && (ac0 & 0x8000) == 0"
	tests[3] = "ac0 & 1"
	tests[4] = "ac0 & 2"
	tests[5] = "ac0"
	tests[6] = "0"
	tests[7] = "0"
	tests[8] = "sp >= 16"
	tests[9] = "interruptEnabled"
	tests[10] = "carry != 0"
	tests[11] = "ac0 == 0 || (ac0 & 0x8000) != 0"
	for i in range(12,16):
		tests[i] = "readConditionCodeLine({0})".format(i)


	s = s.replace("%E",eac[offset % 4])
	s = s.replace("%T",tests[offset % 16])
	return s

lines = open("imp16.def","r").readlines()
lines = [process(s).replace("\t"," ").strip() for s in lines]
lines = [s for s in lines if s != ""]
open("imp16_code.h","w").write("\n".join([s[1:] for s in lines if s[0] == ':']))

mneList = [ "" ] * 0x108
codeList = [ "" ] * 0x108

for line in [s for s in lines if s[0] != ':']:
	rexpr = '^([0-9A-F\-]+)\s+(\d+)\s+\"([0-9A-Z\s\#\%\,\@]+)\"\s+(.*)$'
	match = re.match(rexpr,line)
	if match is None :
		raise Exception("Syntax : "+line)
	groups = match.groups()

	rng = (groups[0] if groups[0].find("-") >= 0 else groups[0]+"-"+groups[0]).split("-")
	start = int(rng[0],16)
	end = int(rng[1],16)
	cycles = int(groups[1],16)
	mnemonic = groups[2].strip()
	code = (groups[3] if groups[3][-1] == ';' else groups[3] + ";").strip()
	code = code + "cycles -= {0};".format(cycles)

	for opcode in range(start,end+1):
		if mneList[opcode] != "":
			raise Exception("Opcode {0:x} duplicated {1}".format(opcode,line))
		mneList[opcode] = substitute(mnemonic,opcode-start)
		codeList[opcode] = substitute(code,opcode-start)

for i in range(0,0x108):
	if mneList[i] == "":
		mneList[i] = "word {0:02x}#I".format(i)
		if i >= 0x100:
			mneList[i] = "word {0:04x}".format((i % 256) << 7)
	mneList[i] = ('"'+mneList[i]+'"').lower()

open("imp16_mnemonic.h","w").write("static char *_mnemonics[] = {" + (",".join(mneList)) + "};\n")

h = open("imp16_opcodes.h","w")
for i in range(0,0x108):
	if codeList[i] != "":
		opcode = i << 8
		if i >= 0x100:
			opcode = (i - 0x100) << 7
		h.write("case 0x{0:03x}: // {1:04x} {2}\n".format(i,opcode,mneList[i][1:-1]))
		h.write("    {0}\n".format(codeList[i]))
		h.write("    break;\n")