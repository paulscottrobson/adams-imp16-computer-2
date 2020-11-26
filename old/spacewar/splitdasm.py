#
#			Splits SpaceWar.Dasm into a binary file and an assembly file.
#
# 	Q+D
#
def convertTab(s, tabstop = 4):
  result = str()
  for c in s:
    if c == '\t':
      result += ' ';
      while (len(result) % tabstop != 0):
        result += ' ';
    else:
      result += c    
  return result

binaryDump = [ 0 ] * 512

source = open("spacewar.dasm").readlines()
source = [s.rstrip() for s in source]

srcFile = open("spacewar.asm","w")
srcFile.write("; **** created from the disassembled code. DO NOT EDIT IT *****\n\n")
for s in source:
	if (s+" ")[0] == "0":
		dat = s[:9].split()
		binaryDump[int(dat[0],16)] = int(dat[1],16)
		s = s[9:]
	s = convertTab(s)
	if (s+" ")[0] != ";":
		s = s[8:]
	srcFile.write(s+"\n")
srcFile.close()

binFile = open("spacewar.bin","wb")
for i in range(0,512):
	binFile.write(bytes([binaryDump[i] >> 8]))
	binFile.write(bytes([binaryDump[i]%256]))
binFile.close()

open("spacewar.h","w").write("static WORDCONST _spaceWar[] = { " +  (",".join([ "0x{0:04x}".format(s) for s in binaryDump ])) + "};\n")

