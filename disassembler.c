//TODO 0xaX, 0xbX, 0xcX, 0xdX, 0xeX, 0xfX series

#include<stdio.h>
#include<stdlib.h>
int disassemble(unsigned char *buffer, int pc); //disassembler decl
int main(int argc, char **argv){
	FILE *f = fopen(argv[1],"rb");
	if (f == NULL){
		printf("Error: could not open %s\n", argv[1]);
		exit(1);
	}
	//Read code into into buffer
	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f); //grabbing total file size
	fseek(f, 0L, SEEK_SET);
	unsigned char *buffer = malloc(fsize);
	fread(buffer, fsize, 1, f);
	fclose(f);
	//Perform Disassembly
	int pc = 0;
	while(pc < fsize)
	{
		pc += disassemble(buffer, pc);
	}

	return 0;
}

int disassemble(unsigned char *buffer, int pc){
	//Performs actual disassembly of 8080 Machine Code
	//Params: 
	//	unsigned char *buffer - Buffer containing the machine code
	//	int pc - Program Counter
	//
	//Returns:
	//	int - New Program Counter Location
	
	unsigned char *opcode = &buffer[pc];
	int opbytes = 1;
	printf("0x%04x ",pc);
	switch(*opcode)
	{
		case 0x00:
			printf("NOP");
			break;
		case 0x01:
			printf("LXI\tB, #$0x%02x%02x", opcode[2], opcode[1]);
			opbytes = 3;
			break;
		case 0x02:
			printf("STAX\tB");
			break;
		case 0x03:
			printf("INX\tB");
			break;
		case 0x04:
			printf("INR\tB");
			break;
		case 0x05:
			printf("DCR\tB");
			break;
		case 0x06:
			printf("MVI\tB, #$0x%02x", opcode[1]);
			opbytes = 2;
			break;
		case 0x07:
			printf("RLC");
			break;
		case 0x09:
			printf("DAD\tB");
			break;
		case 0x0a:
			printf("LDAX\tB");
			break;
		case 0x0b:
			printf("DCX\tB");
			break;
		case 0x0c:
			printf("INR\tC");
			break;
		case 0x0d:
			printf("DCR\tC");
			break;
		case 0x0e:
			printf("MVI\tC, #$0x%02x", opcode[1]);
			opbytes = 2;
			break;
		case 0x0f:
			printf("RRC");
			break;
		case 0x10:
			break;
		case 0x11:
			printf("LXI\tD, #$0x%02x%02x", opcode[2], opcode[1]);
			opbytes = 3;
			break;
		case 0x12:
			printf("STAX\tD");
			break;
		case 0x13:
			printf("INX\tD");
			break;
		case 0x14:
			printf("INR\tD");
			break;
		case 0x15:
			printf("DCR\tD");
			break;
		case 0x16:
			printf("MVI\tD, #$0x%02x", opcode[1]);
			opbytes = 2;
			break;
		case 0x17:
			printf("RAL");
			break;
		case 0x18:
			break;
		case 0x19:
			printf("DAD\tD");
			break;
		case 0x1a:
			printf("LDAX\tD");
			break;
		case 0x1b:
			printf("DCX\tD");
			break;
		case 0x1c:
			printf("INR\tE");
			break;
		case 0x1d:
			printf("DCR\tE");
			break;
		case 0x1e:
			printf("MVI\tE, #$0x%02x", opcode[1]);
			opcode = 2;
			break;
		case 0x1f:
			printf("RAR");
			break;
		case 0x20:
			break;
		case 0x21:
			printf("LXI\tH, #$0x%02x%02x", opcode[2], opcode[1]);
			opbytes = 3;
			break;
		case 0x22:
			printf("SHLD\t$%02x%02x", opcode[2], opcode[1]);
			opbytes = 3;
			break;
		case 0x23:
			printf("INX\tH");
			break;
		case 0x24:
			printf("INR\tH");
			break;
		case 0x25:
			printf("DCR\tH");
			break;
		case 0x26:
			printf("MVI\tH, #$0x%02x", opcode[1]);
			opbytes = 2;
			break;
		case 0x27:
			printf("DAA");
			break;
		case 0x28:
			break;
		case 0x29:
			printf("DAD\tH");
			break;
		case 0x2a:
			printf("LHLD\t$%02x%02x", opcode[2], opcode[1]);
			opbytes = 3;
			break;
		case 0x2b:
			printf("DCX\tH");
			break;
		case 0x2c:
			printf("INR\tL");
			break;
		case 0x2d:
			printf("DCR\tL");
			break;
		case 0x2e:
			printf("MVI\tL, #$0x%02x", opcode[1]);
			opbytes = 2;
			break;
		case 0x2f:
			printf("CMA");
			break;
		case 0x30:
			break;
		case 0x31:
			printf("LXI\tSP, #$0x%02x%02x", opcode[2], opcode[1]);
			opbytes = 3;
			break;
		case 0x32:
			printf("STA\t$%02x%02x", opcode[2], opcode[1]);
			opbytes = 3;
			break;	
		case 0x33:
			printf("INX\tSP");
			break;
		case 0x34:
			printf("INR\tM");
			break;
		case 0x35:
			printf("DCR\tM");
			break;
		case 0x36:
			printf("MVI\tM, #$0x%02x", opcode[1]);
			opbytes = 2;
			break;
		case 0x37:
			printf("STC");
			break;
		case 0x38:
			break;
		case 0x39:
			printf("DAD\tSP");
			break;
		case 0x3a:
			printf("LDA\t$%02x%02x", opcode[2], opcode[1]);
			opbytes = 3;
			break;
		case 0x3b:
			printf("DCX\tSP");
			break;
		case 0x3c:
			printf("INR\tA");
			break;
		case 0x3d:
			printf("DCR\tA");
			break;
		case 0x3e:
			printf("MVI\tA, #$0x%02x", opcode[1]);
			opbytes = 2;
			break;
		case 0x3f:
			printf("CMC");
			break;
		case 0x40:
			printf("MOV\tB,B");
			break;
		case 0x41:
			printf("MOV\tB,C");
			break;
		case 0x42:
			printf("MOV\tB,D");
			break;
		case 0x43:
			printf("MOV\tB,E");
			break;
		case 0x44:
			printf("MOV\tB,H");
			break;
		case 0x45:
			printf("MOV\tB,L");
			break;
		case 0x46:
			printf("MOV\tB,M");
			break;
		case 0x47:
			printf("MOV\tB,A");
			break;
		case 0x48:
			printf("MOV\tC,B");
			break;
		case 0x49:
			printf("MOV\tC,C");
			break;
		case 0x4a:
			printf("MOV\tC,D");
			break;
		case 0x4b:
			printf("MOV\tC,E");
			break;
		case 0x4c:
			printf("MOV\tC,F");
			break;
		case 0x4d:
			printf("MOV\tC,L");
			break;
		case 0x4e:
			printf("MOV\tC,M");
			break;
		case 0x4f:
			printf("MOV\tC,A");
			break;
		case 0x50:
			printf("MOV\tD,B");
			break;
		case 0x51:
			printf("MOV\tD,C");
			break;
		case 0x52:
			printf("MOV\tD,D");
			break;
		case 0x53:
			printf("MOV\tD,E");
			break;
		case 0x54:
			printf("MOV\tD,H");
			break;
		case 0x55:
			printf("MOV\tD,L");
			break;
		case 0x56:
			printf("MOV\tD,M");
			break;
		case 0x57:
			printf("MOV\tD,A");
			break;
		case 0x58:
			printf("MOV\tE,B");
			break;
		case 0x59:
			printf("MOV\tE,C");
			break;
		case 0x5a:
			printf("MOV\tE,D");
			break;
		case 0x5b:
			printf("MOV\tE,E");
			break;
		case 0x5c:
			printf("MOV\tE,H");
			break;
		case 0x5d:
			printf("MOV\tE,L");
			break;
		case 0x5e:
			printf("MOV\tE,M");
			break;
		case 0x5f:
			printf("MOV\tE,A");
			break;
		case 0x60:
			printf("MOV\tH,B");
			break;
		case 0x61:
			printf("MOV\tH,C");
			break;
		case 0x62:
			printf("MOV\tH,D");
			break;
		case 0x63:
			printf("MOV\tH.E");
			break;
		case 0x64:
			printf("MOV\tH,H");
			break;
		case 0x65:
			printf("MOV\tH,L");
			break;
		case 0x66:
			printf("MOV\tH,M");
			break;
		case 0x67:
			printf("MOV\tH,A");
			break;
		case 0x68:
			printf("MOV\tL,B");
			break;
		case 0x69:
			printf("MOV\tL,C");
			break;
		case 0x6a:
			printf("MOV\tL,D");
			break;
		case 0x6b:
			printf("MOV\tL,E");
			break;
		case 0x6c:
			printf("MOV\tL,H");
			break;
		case 0x6d:
			printf("MOV\tL,L");
				break;
		case 0x6e:
			printf("MOV\tL,M");
			break;
		case 0x6f:
			printf("MOV\tL,A");
			break;
		case 0x70:
			printf("MOV\tM,B");
			break;
		case 0x71:
			printf("MOV\tM,C");
			break;
		case 0x72:
			printf("MOV\tM,D");
			break;
		case 0x73:
			printf("MOV\tM.E");
			break;
		case 0x74:
			printf("MOV\tM,H");
			break;
		case 0x75:
			printf("MOV\tM,L");
			break;
		case 0x76:
			printf("HLT");
			break;
		case 0x77:
			printf("MOV\tM,A");
			break;
		case 0x78:
			printf("MOV\tA,B");
			break;
		case 0x79:
			printf("MOV\tA,C");
			break;
		case 0x7a:
			printf("MOV\tA,D");
			break;
		case 0x7b:
			printf("MOV\tA,E");
			break;
		case 0x7c:
			printf("MOV\tA,H");
			break;
		case 0x7d:
			printf("MOV\tA,L");
			break;
		case 0x7e:
			printf("MOV\tA,M");
			break;
		case 0x7f:
			printf("MOV\tA,A");
			break;
		case 0x80:
			printf("ADD\tB");
			break;
		case 0x81:
			printf("ADD\tC");
			break;
		case 0x82:
			printf("ADD\tD");
			break;
		case 0x83:
			printf("ADD\tE");
			break;
		case 0x84:
			printf("ADD\tH");
			break;
		case 0x85:
			printf("ADD\tL");
			break;
		case 0x86:
			printf("ADD\tM");
			break;
		case 0x87:
			printf("ADD\tA");
			break;
		case 0x88:
			printf("ADC\tB");
			break;
		case 0x89:
			printf("ADC\tC");
			break;
		case 0x8a:
			printf("ADC\tD");
			break;
		case 0x8b:
			printf("ADC\tE");
			break;
		case 0x8c:
			printf("ADC\tH");
			break;
		case 0x8d:
			printf("ADC\tL");
			break;
		case 0x8e:
			printf("ADC\tM");
			break;
		case 0x8f:
			printf("ADC\tA");
			break;
		case 0x90:
			printf("SUB\tB");
			break;
		case 0x91:
			printf("SUB\tC");
			break;
		case 0x92:
			printf("SUB\tD");
			break;
		case 0x93:
			printf("SUB\tE");
			break;
		case 0x94:
			printf("SUB\tH");
			break;
		case 0x95:
			printf("SUB\tL");
			break;
		case 0x96:
			printf("SUB\tM");
			break;
		case 0x97:
			printf("SUB\tA");
			break;
		case 0x98:
			printf("SBB\tB");
			break;
		case 0x99:
			printf("SBB\tC");
			break;
		case 0x9a:
			printf("SBB\tD");
			break;
		case 0x9b:
			printf("SBB\tE");
			break;
		case 0x9c:
			printf("SBB\tH");
			break;
		case 0x9d:
			printf("SBB\tL");
			break;
		case 0x9e:
			printf("SBB\tM");
			break;
		case 0x9f:
			printf("SBB\tA");
			break;
		


		default: //Unknown instruction handling
			printf("Unknown Instruction: 0x%02x", *opcode);
			break;
	}
	printf("\n");
	return opbytes;
}

