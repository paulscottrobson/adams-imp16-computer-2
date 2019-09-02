# *******************************************************************************************************************************************************
# *******************************************************************************************************************************************************
#
#																	IMP16C - Assembler
#
# *******************************************************************************************************************************************************
# *******************************************************************************************************************************************************

# *******************************************************************************************************************************************************
#
#													Exception subclass used for assembler errors.
#
# *******************************************************************************************************************************************************

class AssemblerException(Exception): 
	pass

# *******************************************************************************************************************************************************
#
#																Simple identifier store.
#
# *******************************************************************************************************************************************************

class IdentifierStore:
	def __init__(self):
		self.identifiers = {}

	def set(self,identifier,value):
		identifier = identifier.lower().strip()
		if identifier in self.identifiers:
			raise AssemblerException("Duplicate identifier "+identifier)
		self.identifiers[identifier] = value

	def get(self,identifier):
		identifier = identifier.lower().strip()
		if identifier not in self.identifiers:
			raise AssemblerException("Unknown identifier "+identifier)
		return self.identifiers[identifier]

# *******************************************************************************************************************************************************
#
#												Simple expression evaluator - left->right, + and - only.
#
# *******************************************************************************************************************************************************

class Expression:
	def __init__(self,identifierStore):
		self.identifierStore = identifierStore

	def term(self,term): 	
		result = 0
		try:
			if term[0] == '$':														# $<x> is hex
				result = int(term[1:].strip(),16)
			elif term[0] >= '0' and term[0] <= '9':									# decimal if begins with 0-9
				result = int(term)
			elif term[0] == "'" or term[0] == '"':
				result = ord(term[1].upper()) % 64 + 64
			else:
				result = self.identifierStore.get(term) 							# all others are identifiers.
		except ValueError, e: 														# map format errors onto assembler errors.
			raise AssemblerException("Bad number format "+term)

		return result

	def expression(self,expr):
		expr = expr.expandtabs(8).lower().replace(" ","")							# remove spaces, tabs, lower case
		expr = "+"+(expr.replace("+","%+").replace("-","%-"))						# make alternation +<term>%-<term> etc.
		if expr[:2] == "+%":
			expr = expr[2:]
		terms = expr.split("%")														# now a list of them
		result = 0 																	# start with 0
		for t in terms:																# apply terms L->R
			if t[0] == "+":															# yes I do know about operator precedence.
				result = result + self.term(t[1:])
			elif t[0] == "-":
				result = result - self.term(t[1:])
			else:
				raise AssemblerException("Syntax Error in expression")
		return result

# *******************************************************************************************************************************************************
#
#																Represents a single instruction
#
# *******************************************************************************************************************************************************

class Instruction:
	def __init__(self,line,lineNumber):
		self.lineNumber = lineNumber											# keep line no.
		self.codeAddress = -1 													# may not have code address/label
		self.label = ""
		line = line.strip().expandtabs().lower() 								# process line
		while line.find("  ") >= 0: 											# remove double spaces
			line = line.replace("  "," ")
		p = line.find(":")														# is there a colon (label marker)
		if p > 0:																# if yes, then strip off the label
			self.label = line[:p].strip()
			line = line[p+1:].strip()
		line += " "																# make sure ends in space
		p = line.find(" ")														# split into opcode/operand
		self.opcode = line[:p].strip()
		self.operand = line[p:].strip().replace("(",":").replace(")","")
		self.isIndirect = False 												# check indirect
		if self.operand.strip() == "":
			self.operand = []
		else:
			self.operand = self.operand.split(",")								# split operand around comma.
			if self.operand[-1][0] == "@":										# last operand can be indirect.
				self.isIndirect = True
				self.operand[-1] = self.operand[-1][1:]

	def getOpcode(self):
		return self.opcode

	def getOperand(self):
		return self.operand

	def getIndirect(self):
		return self.isIndirect

	def getCodeAddress(self):
		return self.codeAddress

	def assignLabelValue(self,value,identifierStore):
		if self.label != "":													# if we have a label, give it this value.
			identifierStore.set(self.label,value)

	def setCodeAddress(self,addr):	
		self.codeAddress = addr
		return self

	def __str__(self):
		return "{0:5} {1:04x} {2:20} {3:4} : {4}{5}".format(self.lineNumber,self.codeAddress & 0xFFFF, \
					self.label+":" if self.label != "" else "",self.opcode,"@" if self.isIndirect else "",self.operand)

# *******************************************************************************************************************************************************
#
#														Base Class for Instruction Assembler Workers
#
# *******************************************************************************************************************************************************

class InstructionAssemblerBase:
	def __init__(self,baseOpcode):
		self.baseOpcode = baseOpcode												# save base opcode.

	def assemble(self,codeItem,evaluator):											# assemble one single code item.
		self.codeItem = codeItem 	
		self.evaluator = evaluator
		self.parameters = self.createParameters(codeItem) 							# convert parameters to values.
		opcode = self.construct(self.baseOpcode,self.parameters)					# construct an opcode.
		if self.codeItem.getIndirect():												# indirect supported ?
				opcode = self.adjustIndirect(opcode)
		return opcode

	def createParameters(self,codeItem):											# this builds the parameters.
		raise Exception("Abstract Method called")

	def construct(self,baseOpcode,parameterList):
		opcode = self.baseOpcode													# add the base and all the parameter values together.
		for p in parameterList:
			opcode += p
		return opcode

	def adjustIndirect(self,opcode):
		raise AssemblerException("Cannot use indirection on this opcode")

	def checkParameterCount(self,c):
		if len(self.codeItem.getOperand()) != c:
			raise AssemblerException("Wrong number of parameters")

	def checkRange(self,value,min,max):
		if value < min or value > max:
			raise AssemblerException("Value out of range")

	def evaluateParam(self,paramID):
		return self.evaluator.expression(self.codeItem.getOperand()[paramID-1])

	def relativeParam(self,paramID,codeAddress):
		address = self.evaluator.expression(self.codeItem.getOperand()[paramID-1])
		address = address - (codeAddress+1)
		if address < -128 or address > 127:
			raise AssemblerException("Address out of range")
		return address & 0xFF

	def addressParam(self,paramID,codeAddress):
		param = self.codeItem.getOperand()[paramID-1]
		result = 0
		if param.find(":") >= 0:
			param = param.split(":")
			index = self.evaluator.expression(param[1])
			if index != 2 and index != 3:
				raise AssemblerException("Bad Index Register")
			result = self.evaluator.expression(param[0])
			self.checkRange(result,-128,127)
			result = (result & 0xFF) + (index << 8)
		else:
			address = self.evaluator.expression(param)
			if address >= 0 and address < 256:
				result = 0x000 + address
			else:
				result = 0x100 + self.relativeParam(paramID,codeAddress)

		return result

# *******************************************************************************************************************************************************
#
#																		The actual workers
#
# *******************************************************************************************************************************************************

class WordInstruction(InstructionAssemblerBase):									# word instruction (psuedo-op)
	def createParameters(self,codeItem):	
		self.checkParameterCount(1)
		params = [self.evaluateParam(1) & 0xFFFF]
		self.checkRange(params[0],0,0xFFFF)
		return params

class SimpleInstruction(InstructionAssemblerBase):									# instructions with no parameters.
	def createParameters(self,codeItem):											# NOP, PUSHF, PULLF, HALT
		self.checkParameterCount(0)
		return []

class RegisterToRegisterInstruction(InstructionAssemblerBase):						# 2 operand register to register
	def createParameters(self,codeItem):											# RAdd, RXch, RCpy, RXor, RAnd
		self.checkParameterCount(2)
		params = [self.evaluateParam(1),self.evaluateParam(2)]
		self.checkRange(params[0],0,3)												# Validate
		self.checkRange(params[1],0,3)
		params[0] = params[0] << 8 												# shift into place
		params[1] = params[1] << 10
		return params

class SingleRegisterInstruction(InstructionAssemblerBase):							# 1 operand register
	def createParameters(self,codeItem):											# Push, Pull, Xchrs
		self.checkParameterCount(1)
		params = [self.evaluateParam(1)]
		self.checkRange(params[0],0,3)												# Validate
		params[0] = params[0] << 8	 												# shift into place
		return params

class JSRIInstruction(InstructionAssemblerBase):									# Special Instruction
	def createParameters(self,codeItem):											# JSRI only - parameter is $FF80-$FFFF
		self.checkParameterCount(1) 												# only uses lower 6 bits.
		params = [self.evaluateParam(1)]
		self.checkRange(params[0],0xFF80,0xFFFF)									
		params[0] = params[0] & 0x7F
		return params

class Default7BitInstruction(InstructionAssemblerBase):								# 7 Bits immediate opcode.
	def createParameters(self,codeItem):											# Rts, Rti, ROut, RIn
		params = [ 0 ]
		if len(self.codeItem.getOperand()) > 0:										# parameters were provided - for these it is optional
			params = [self.evaluateParam(1)]										# and defaults to zero.
			self.checkRange(params[0],0x00,0x7F)									# 7 bit integer parameter.
		return params

class FlagInstruction(InstructionAssemblerBase):									# 2 operand flag instruction
	def createParameters(self,codeItem):											# SFlg Pflg
		self.checkParameterCount(2)
		params = [self.evaluateParam(1),self.evaluateParam(2)]
		self.checkRange(params[0],0,7)												# Validate
		self.checkRange(params[1],-64,63)
		params[0] = params[0] << 8 													# shift into place
		params[1] = params[1] & 0x7F
		return params

class ImmediateInstruction(InstructionAssemblerBase):								# 8 Bits immediate opcode.
	def createParameters(self,codeItem):											# Li, Aisz, Cai
		self.checkParameterCount(2)
		params = [self.evaluateParam(1),self.evaluateParam(2)]
		self.checkRange(params[0],0,3)												# Validate
		self.checkRange(params[1],-128,127)
		params[0] = params[0] << 8 													# shift into place
		params[1] = params[1] & 0xFF
		return params

class RotateInstruction(InstructionAssemblerBase):									# 8 Bits immediate opcode which is a rotation

	def setRightRotation(self,isRight):
		self.isRightRotation = isRight

	def createParameters(self,codeItem):											# Shl, Shr, Rol, Ror
		self.checkParameterCount(2)
		params = [self.evaluateParam(1),self.evaluateParam(2)]
		self.checkRange(params[0],0,3)												# Validate
		self.checkRange(params[1],1,15)												# note, can be 0-127 but other values are pointless.
		if self.isRightRotation:													# left is +ve. Programmers manual is wrong :)
			params[1] = -params[1]
		params[0] = params[0] << 8 													# shift into place
		params[1] = params[1] & 0xFF
		return params

class BranchInstruction(InstructionAssemblerBase):									# Boc instruction
	def createParameters(self,codeItem):											# 2 parameters, constant and offset.
		self.checkParameterCount(2)
		params = [self.evaluateParam(1),self.relativeParam(2,codeItem.getCodeAddress())]
		self.checkRange(params[0],0,15)												# Validate
		params[0] = params[0] << 8 													# shift into place
		params[1] = params[1] & 0xFF
		return params

class AdjustSkipInstruction(InstructionAssemblerBase):								# Single address, no indirection.
	def createParameters(self,codeItem):											# dsz, isz
		self.checkParameterCount(1)													# one parameter, an address.
		params = [self.addressParam(1,codeItem.getCodeAddress())]	
		return params

class JumpInstruction(AdjustSkipInstruction):										# jsr and jmp are single address with indirection.
	def adjustIndirect(self,opcode):
		return opcode + 0x400

class RegisterReferenceInstruction(InstructionAssemblerBase):						# register and address
	def createParameters(self,codeItem):											# add, sub, skg, skne
		self.checkParameterCount(2)													
		params = [self.evaluateParam(1),self.addressParam(2,codeItem.getCodeAddress())]	
		self.checkRange(params[0],0,self.getRegisterRange())
		params[0] = params[0] << 10
		return params
	def getRegisterRange(self):
		return 3

class LimitedRegisterReferenceInstruction(RegisterReferenceInstruction):			# same as above, but only allows 0 & 1
	def getRegisterRange(self):														# and, or, skaz
		return 1

class MemoryReferenceInstruction(RegisterReferenceInstruction):						# same as above, but allows indirection
	def adjustIndirect(self,opcode):
		return opcode + 0x1000

class OpcodeHash:
	def __init__(self):
		self.opcodes = {}															# empty hash.

		h = { "ld":0x8000, "st":0xA000 }											# Memory Reference Instructions
		for mnemonic,base in h.iteritems():
			self.opcodes[mnemonic] = MemoryReferenceInstruction(base)

		h = { "add":0xC000, "sub":0xD000, "skg":0xE000,"skne":0xF000 }				# Register memory operations
		for mnemonic,base in h.iteritems():
			self.opcodes[mnemonic] = RegisterReferenceInstruction(base)

		h = { "and":0x6000, "or":0x6800,"skaz":0x7000 }								# Limited register/memory ops
		for mnemonic,base in h.iteritems():
			self.opcodes[mnemonic] = LimitedRegisterReferenceInstruction(base)

		h = { "isz":0x7800, "dsz":0x7C00 }											# Address only, no indirection
		for mnemonic,base in h.iteritems():
			self.opcodes[mnemonic] = AdjustSkipInstruction(base)

		h = { "jmp":0x2000, "jsr":0x2800 }											# Jump and Call
		for mnemonic,base in h.iteritems():
			self.opcodes[mnemonic] = JumpInstruction(base)

		h = { "boc":0x1000 }														# Conditional Branch
		for mnemonic,base in h.iteritems():
			self.opcodes[mnemonic] = BranchInstruction(base)

		h = { "rti":0x0100, "rts":0x0200, "rin":0x0400, "rout":0x0600 }				# 7 bit immediate optional parameter
		for mnemonic,base in h.iteritems():
			self.opcodes[mnemonic] = Default7BitInstruction(base)

		h = { "jsri": 0x0380 }														# jsr last 128 words
		for mnemonic,base in h.iteritems():
			self.opcodes[mnemonic] = JSRIInstruction(base)

		h = { "rol": 0x5800, "ror": 0x5800, "shl": 0x5C00, "shr":0x5C00 }			# Rotates.
		for mnemonic,base in h.iteritems():
			self.opcodes[mnemonic] = RotateInstruction(base)
			self.opcodes[mnemonic].setRightRotation(mnemonic[-1] == "r")

		h = { "push":0x4000, "pull":0x4400, "xchrs":0x5400 }						# only a single register
		for mnemonic,base in h.iteritems():
			self.opcodes[mnemonic] = SingleRegisterInstruction(base)

		h = { "li":	0x4C00 , "aisz":0x4800, "cai":0x5000 }							# 8 bit signed immediate
		for mnemonic,base in h.iteritems():
			self.opcodes[mnemonic] = ImmediateInstruction(base)

		h = { "radd": 0x3000,"rxch": 0x3080, "rcpy":0x3081, "rxor": 0x3082, "rand": 0x3083 }		# register to register
		for mnemonic,base in h.iteritems():
			self.opcodes[mnemonic] = RegisterToRegisterInstruction(base)

		h = { "halt": 0x0000, "nop": 0x2100, "pushf": 0x0080,"pullf": 0x0280, }		# no operands at all
		for mnemonic,base in h.iteritems():
			self.opcodes[mnemonic] = SimpleInstruction(base)

		h = { "sflg": 0x0800, "pflg": 0x0880 }										# Flag Instructions
		for mnemonic,base in h.iteritems():
			self.opcodes[mnemonic] = FlagInstruction(base)

		self.opcodes["word"] = WordInstruction(0x0000) 								# and the word

	def find(self,opcode):
		if opcode not in self.opcodes:
			raise AssemblerException("Unknown opcode "+opcode)
		return self.opcodes[opcode]

# *******************************************************************************************************************************************************
#
#											A very simple two pass assembler. All code kept in memory.
#
# *******************************************************************************************************************************************************

class Assembler:
	def __init__(self,source):
		source = [self._stripComment(s) for s in source]						# remove comments, leading, trailing spaces keep intact
		source = [s.strip() for s in source]
		lineNumber = 1 															# line number
		self.code = []
		for s in source:														# work through source
			if s != "":															# if something there, then add the line.
				self.code.append(Instruction(s,lineNumber))
			lineNumber += 1
		self.identifierStore = IdentifierStore()								# create id store and evaluator
		self.evaluator = Expression(self.identifierStore)
		self.opcodeInfo = OpcodeHash()
		self.loadSpaceWar()

	def _stripComment(self,s):
		p = s.find(";")
		if p == 0:																# start of line
			s = ""
		if p > 0:																# middle of line
			s = s[0:p-1].strip()
		return s

	def pass1(self):
		origin = 0 																# org 0 is a default
		for c in self.code:														# work through code.
			if c.getOpcode() == "org": 											# if org <n> update origin pointer
				origin = self.evaluator.expression(c.getOperand()[0])
			else:																# assign the label a value if there is one
				c.assignLabelValue(origin,self.identifierStore)
				if c.getOpcode() != "":											# and if there is an opcode, set the instructions address
					c.setCodeAddress(origin)									
					origin += 1 												# and they all, on the IMP-16C are one instruction long.

																				# remove all pseudo operations and lines without code.
		self.code = [c for c in self.code if not(c.getOpcode() == "" or c.getOpcode() == "org")]
		return self

	def pass2(self):
		jssw =  [ 0x2402, 0x208d, 0x001e, 0x01bc, 0x017c, 0x0000, 0xff80, 0x0000, 0x010b, 0x00c9, 0x0103, 0x0112, 0x015f, 0x00b1, 0x0120, 0x0043, 0x0045, 0x006b, 0x003b, 0x006a, 0x0060, 0x0003, 0x00c2, 0x0010, 0x0104, 0x0001, 0x0001, 0x0010, 0x0000, 0x0000, 0x241f, 0x019a, 0x0000, 0x1000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1100, 0x0000, 0x0000, 0x0000, 0x7f00, 0x0000, 0x0000, 0x0000, 0xd500, 0x0000, 0x0000, 0x0000, 0x6a00, 0x0000, 0x0000, 0x0000, 0xa600, 0x0000, 0x0000, 0x0000, 0xf700, 0xf080, 0x0000, 0x0000, 0xff80, 0xef80, 0x0000, 0x0000, 0x9900, 0x1000, 0x0000, 0x0000, 0x0210, 0xf000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0020, 0x00c0, 0x0000, 0x0000, 0x0020, 0x0012, 0x0000, 0xf000, 0x0080, 0x1000, 0xff80, 0x0045, 0x0044, 0x0058, 0x0051, 0x0057, 0x0053, 0x005a, 0x0041, 0x0055, 0x0048, 0x004e, 0x0050, 0x0049, 0x004b, 0x004d, 0x004a, 0x007f, 0x0007, 0x0080, 0x014f, 0x0002, 0x0e80, 0x0ec0, 0x20f0, 0x0240, 0x2492, 0x0231, 0x2494, 0x02de, 0x0087, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xa46f, 0x2c0c, 0x1501, 0x0200, 0x8603, 0xa602, 0x8e00, 0x2c08, 0x48a0, 0x2c6e, 0x886f, 0x820e, 0x2c09, 0x0200, 0x0000, 0x886f, 0x820e, 0x2c18, 0x4c01, 0xa01d, 0x2c03, 0x4400, 0x2000, 0x0000, 0x4000, 0x8006, 0x3383, 0x0f83, 0x4400, 0x062a, 0x0200, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x78f3, 0x0100, 0x0100, 0x0000, 0x881b, 0x4a09, 0x24f7, 0x0150, 0x8c20, 0x800f, 0x2c09, 0x8c24, 0x8010, 0x2c09, 0x0200, 0x0000, 0x0f83, 0x062a, 0x0200, 0x4c60, 0x4f00, 0x2c09, 0xcd03, 0xfc07, 0x0200, 0x21fb, 0x0080, 0x0f83, 0x0405, 0x6101, 0x0200, 0x007f, 0x0000, 0x0000, 0xac1c, 0x4c30, 0x2c18, 0x8c1c, 0x8014, 0x2c09, 0x4c40, 0xe0f3, 0x2101, 0x21fd, 0x2c0a, 0x20f8, 0x0000, 0x0000, 0x8405, 0x4902, 0x2c0c, 0x1501, 0x0200, 0x7e03, 0x2101, 0x0200, 0x8e00, 0x2c08, 0x48a0, 0x2111, 0x8005, 0x48fb, 0x1203, 0x8012, 0x7a03, 0x2105, 0x48fc, 0x1202, 0x8013, 0x21fa, 0x8011, 0x2c09, 0x2100, 0x2100, 0x8417, 0xa602, 0x210d, 0x4d00, 0xa603, 0x4860, 0xf00f, 0x2c16, 0xf010, 0x2c16, 0x2103, 0x0000, 0x0000, 0x0000, 0x2c0b, 0x0200, 0xa81c, 0x8006, 0x701c, 0x0000, 0x0200, 0x20f4, 0xa81c, 0x4e48, 0x4a04, 0x8203, 0x1103, 0x7c1c, 0x21fb, 0x0200, 0xa600, 0xae01, 0x4d01, 0xa602, 0x4d0f, 0xa603, 0x0200, 0x3681, 0x5e02, 0x4a1c, 0x4c00, 0x4f00, 0xfe03, 0x0200, 0x7e02, 0x0200, 0x8e00, 0x8014, 0x2c09, 0x8600, 0xc601, 0x6406, 0xa600, 0x0200, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0xa020, 0xc08d, 0xa024, 0x2c0b, 0x841b, 0xa41c, 0x4e28, 0x4d01, 0x4f02, 0xa602, 0xae03, 0x4a04, 0xf908, 0x4f00, 0x4901, 0x7c1c, 0x21f8, 0xac23, 0xac27, 0xac1d, 0x0200, 0x004c, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0ec0, 0x4c00, 0xa0f3, 0x0900, 0xf0f3, 0x21fe, 0x2c04, 0x2c03, 0x841b, 0xa405, 0x2c0e, 0x7c05, 0x21fd, 0x4d01, 0x2c0d, 0x4d02, 0x2c0d, 0x7c19, 0x2101, 0x7819, 0x7c1a, 0x2101, 0x781a, 0x21ef, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xf06d, 0x2139, 0x210c, 0x4c00, 0x4d00, 0xa4f3, 0xf4f3, 0x2101, 0x21f7, 0x0f81, 0x1efb, 0x2100, 0x0400, 0x6088, 0x21f7, 0x2100, 0x112e, 0x8895, 0x4d10, 0xa41c, 0xf200, 0x2105, 0x4aff, 0x2100, 0x7c1c, 0x21fa, 0x2124, 0x7c1c, 0x2100, 0x881c, 0x5efd, 0x5e02, 0x4d07, 0x8c1c, 0x3783, 0xec8c, 0x2106, 0xfc15, 0x210a, 0x2100, 0x8770, 0xa621, 0x2114, 0x2100, 0x8770, 0xa623, 0x2100, 0xa622, 0x210e, 0x8e21, 0x8620, 0x210e, 0x8219, 0x48ff, 0x2108, 0x808a, 0xa219, 0x2c8b, 0x2104, 0x801d, 0x4d01, 0x3482, 0xa01d, 0x801d, 0x15c2, 0x0200, 0x3d00, 0x5efe, 0x21ef ] 
		for c in self.code:
			word = self.opcodeInfo.find(c.getOpcode()).assemble(c,self.evaluator)
			wsw = self.spaceWar[c.getCodeAddress()]
			if word != wsw:
				print("DASM:{0:04x} ASM:{1:04x}  {2}".format(wsw,word,c))
			wsj = jssw[c.getCodeAddress()]
			if word != wsj:
				print("JSON:{0:04x} ASM:{1:04x}  {2}".format(wsj,word,c))

	def codeDump(self):
		for c in self.code:
			print(c)

	def loadSpaceWar(self):
		self.spaceWar = [0]*512
		h = open("spacewar.bin","rb")
		for i in range(0,512):
			self.spaceWar[i] = ord(h.read(1)) * 256
			self.spaceWar[i] += ord(h.read(1)) 
		h.close()

src = open("spacewar.asm","r").readlines()
asm = Assembler(src)
asm.pass1()
asm.pass2()
print("Done !")
