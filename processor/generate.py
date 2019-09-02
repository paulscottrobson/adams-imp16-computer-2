# *********************************************************************************
# *********************************************************************************
#
#				Generate source code from CPU Definition File
#
# *********************************************************************************
# *********************************************************************************

import re,os,sys

def effectiveAddress(bits):
	if ((bits >> 1) == 0):
		return "MAR = IR & 0xFF;"
	disp = "IR|0xFF00" if (bits & 1) else "IR&0x007F"
	regName = "PC" if bits < 4 else "AC"+str(bits >> 1)
	return "MAR = {0}+({1});".format(regName,disp)

def process(s,opcode):
	s = s.replace("$EAC",effectiveAddress(opcode & 0x7))
	s = s.replace("$EAR",effectiveAddress((opcode & 0x1)|4))
	s = s.replace("$V",str((opcode >> 1) & 3))
	s = s.replace("$R",str((opcode >> 3) & 3))
	s = s.replace("$T",str((opcode >> 3) & 1))
	s = s.replace("$IMM","(IR & 0xFF)")
	s = s.replace("$F","F8,INTEN,SEL,F11,F12,F13,F14,F15".split(",")[(opcode >> 1) & 7])
	s = s.replace("$A",["%8","%S(PC)","%S(AC2)","%S(AC3)"][(opcode >> 1) & 3])
	return s
#
#				Load, remove comments, remove blank lines.
#
src = [x.replace("\t"," ") for x in open("imp16.def").readlines()]
src = [x if x.find("//") < 0 else x[:x.find("//")] for x in src]
src = [x.rstrip() for x in src if x.rstrip() != ""]
#
#				Generate code file
#
h = open("imp16_code.h","w")
h.write("\n".join([x[1:] for x in src if x.startswith(":")]))
h.close()
#
#				Remove code and process into lines.
#
src = [x for x in src if not x.startswith(":")]
src = [x.strip()+";" if x.startswith(" ") else "##"+x.strip()+" " for x in src]
#
#				Join and split.
#
src = ("".join(src)).replace(";;",";").split("##")
#
#				Divide up.
#
instructions = {}
for s in [x for x in src if x != ""]:
	m = re.match('^([01\\-]+)\\s*\\"(.*?)\\"\\s*(.*)$',s)
	assert m is not None,s
	assert len(m.group(1)) == 9,s
	pattern = "^"+m.group(1).replace("-",".")+"$"
	assert pattern not in instructions,s
	instructions[pattern] = [ m.group(2).strip(),m.group(3)+"break;"]
iKeys = [x for x in instructions.keys()]	
#
#				Generate every opcode.
#
mnemonics = [ "nop" ] * 512
h = open("imp16_switch.h","w")
for opcode in range(0,512):
	cmd = None
	binVer = "{0:09b}".format(opcode)
	for k in iKeys:
		if re.match(k,binVer) is not None:
			assert cmd is None,"Duplicate opcode for "+binVer+" "+str(opcode)
			cmd = instructions[k]
	if cmd is not None:
		mnemonics[opcode] = process(cmd[0],opcode).lower()
		assert mnemonics[opcode].find("$") < 0,mnemonics[opcode]
		h.write("case 0x{0:03x}: /**** {1:04x} {2} ****/\n".format(opcode,opcode << 7,mnemonics[opcode]))
		cmds = process(cmd[1].replace(";;",";"),opcode).split(";")
		for c in [x for x in cmds if x != ""]:
			assert (c.find("$") < 0) ,c
			h.write("\t{0};\n".format(c))
		h.write("\n")
h.close()		
#
#			Output Mnemonics
#
h = open("imp16_mnemonics.h","w")
m = ",".join(['"'+x+'"' for x in mnemonics])
h.write("static const char *_mnemonics[] = {{ {0} }};\n".format(m))
h.close()
for opcode in range(0,512):
	if mnemonics[opcode] == "nop":
		print("Not used {0:04x}".format(opcode << 7))