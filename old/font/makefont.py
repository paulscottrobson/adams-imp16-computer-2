# Make font from font.txt

lines = open("font.txt").readlines()
lines = [l.strip() for l in lines if l.strip() != ""]
lines = [l for l in lines if l[0] != ';']
if len(lines) != 896:
	raise Exception("Wrong number of lines")
lines = [int(l.replace(".","0").replace("X","1"),2) * 8 for l in lines]
lines = ["0x{0:02x}".format(l) for l in lines]
code = "static BYTECONST _signeticsFont[] = { "+(",".join(lines))+ "};\n"
open("impfont.h","w").write(code)
