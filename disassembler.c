//TODO 0x4X, 0x5X, 0x6X, 0x7X, 0x8X, 0x9X, 0xaX, 0xbX, 0xcX, 0xdX, 0xeX, 0xfX series

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
			printf("SHLD\tadr");
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
			printf("LHLD\tadr");
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
			printf("STA\tadr");
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
			printf("LDA\tadr");
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
		


		default: //Unknown instruction handling
			printf("Unknown Instruction: 0x%02x", *opcode);
			break;
	}
	printf("\n");
	return opbytes;
}

