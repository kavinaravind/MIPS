#include "InstructionsParser.h"
#include "refSystemConstants.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RNUMRECORDS 15
#define INUMRECORDS 16
#define MNUMRECORDS 32

/**  
 * RFormat struct contains Function codes in Binary and the Function
 * Instructions
 */
static RFormat RFormatOpCodeInstructionsTable[RNUMRECORDS] = {
	{"000000","sll"},
	{"101010","slt"},
	{"000010","srl"},
	{"000011","sra"},
	{"001100","syscall"},
	{"100000","add"},
	{"100100","and"},
	{"100010","sub"},
	{"011010","div"},
	{"011000","mult"},
	{"010000","mfhi"},
	{"010010","mflo"},
	{"100111","nor"},
	{"100101","or"},
	{"100110","xor"}
};

/**  
 * IFormat struct contains Opcodes in Binary and the Opcode Instructions
 */
static IFormat IFormatOpCodeInstructionsTable[INUMRECORDS] = {
	{"001101","ori"},
	{"100000","lb"},
	{"001110","xori"},
	{"101000","sb"},
	{"000001","bltz"},
	{"000100","beq"},
	{"000101","bne"},
	{"000110","blez"},
	{"001000","addi"},
	{"001001","addiu"},
	{"001010","slti"},
	{"001100","andi"},
	{"001111","lui"},
	{"100011","lw"},
	{"101011","sw"},
	{"000010", "j"}
};

/**  
 * MIPSRegisterFieldInstructions struct contains Field instructions and
 * its Binary representation
 */
static MIPSRegisterFieldInstructions MIPSRegisterFieldInstructionsTable[32] = {
	{"00000","$zero", 0},
	{"00001","$at", 1},
	{"00010","$v0", 2},
	{"00011","$v1", 3},
	{"00100","$a0", 4},
	{"00101","$a1", 5},
	{"00110","$a2", 6},
	{"00111","$a3", 7},
	{"01000","$t0", 8},
	{"01001","$t1", 9},
	{"01010","$t2", 10},
	{"01011","$t3", 11},
	{"01100","$t4", 12},
	{"01101","$t5", 13},
	{"01110","$t6", 14},
	{"01111","$t7", 15},
	{"10000","$s0", 16},
	{"10001","$s1", 17},
	{"10010","$s2", 18},
	{"10011","$s3", 19},
	{"10100","$s4", 20},
	{"10101","$s5", 21},
	{"10110","$s6", 22},
	{"10111","$s7", 23},
	{"11000","$t8", 24},
	{"11001","$t9", 25},
	{"11010","$k0", 26},
	{"11011","$k1", 27},
	{"11100","$gp", 28},
	{"11101","$sp", 29},
	{"11110","$fp", 30},
	{"11111","$ra", 31}
};

/**  
 * Sets the MI char array to MInstruction in ParseResult struct
 */
void setMInstruction(ParseResult* pr, const char* const MI)
{
   char* tempMI; //temporary array of chars for MI
   tempMI = calloc(1 + MILength, sizeof(char)); // dynamically allocate memory
   strncpy(tempMI, MI, MILength); // saves the Opcode from the Instruction
   tempMI[MILength] = '\0'; // adds null character to end of char array
   strncpy(pr->MInstruction, tempMI, MILength);
   free(tempMI); //frees memory afer use
}

/**  
 * Sets the opCode in MI char array to Opcode in ParseResult struct
 */
void setOpCode(ParseResult* pr, const char* const MI)
{
   char* tempOpCode; //temporary array of chars for OpCode
   tempOpCode = (char*) calloc(1 + OPCODEFIELD_LENGTH, sizeof(char)); // dynamically allocate memory
   strncpy(tempOpCode, MI + OPCODE_OFFSET, OPCODEFIELD_LENGTH); // saves the Opcode from MI to tempOpCode
   tempOpCode[OPCODEFIELD_LENGTH] = '\0'; // adds null character to end of char array
   strncpy(pr->Opcode, tempOpCode, OPCODEFIELD_LENGTH); // copies tempOpCode to pr->Opcode
   free(tempOpCode); //frees memory afer use
}

/**  
 * Sets the Function in MI char array to Funct in ParseResult struct
 */
void setFunct(ParseResult* pr, const char* const MI)
{
   char* tempFunct; //temporary array of chars for Funct
   tempFunct = (char*) calloc(1 + FUNCTFIELD_LENGTH, sizeof(char)); // dynamically allocate memory
   strncpy(tempFunct, MI + FUNCT_OFFSET, FUNCTFIELD_LENGTH); //saves the funct from MI to tempFunct
   tempFunct[FUNCTFIELD_LENGTH] = '\0'; // adds null character to end of char array
   strncpy(pr->Funct, tempFunct, FUNCTFIELD_LENGTH);// copies tempFunct to pr->Opcode
   free(tempFunct); //frees memory afer use
}

/**  
 * Sets the rs-code in MI char array to rsName in ParseResult struct 
 * and converts binary value to uint_8t for rs in ParseResult struct
 */
void setRSName(ParseResult* pr, const char* const MI)
{
	char* tempRS; //temporary array of chars for RS
	tempRS = (char*) calloc(1 + REGFIELD_LENGTH, sizeof(char)); // dynamically allocated memory
	strncpy(tempRS, MI + REG1_OFFSET, REGFIELD_LENGTH); // saves the Opcode from MI to tempRS
	tempRS[REGFIELD_LENGTH] = '\0'; // adds null character to end of char array
	
	for (int i = 0; i < 32; i++)
	{
		if (strcmp(MIPSRegisterFieldInstructionsTable[i].Value, tempRS) == 0)
		{
			pr->rsName = (char*) calloc(1 + strlen(MIPSRegisterFieldInstructionsTable[i].Instruction), sizeof(char)); // allocate memory
			strncpy(pr->rsName, MIPSRegisterFieldInstructionsTable[i].Instruction, strlen(MIPSRegisterFieldInstructionsTable[i].Instruction));
			// copies binary values to pr->rsName
			pr->rs = MIPSRegisterFieldInstructionsTable[i].IntegerValue; //adds the correct rs value integer to pr->rs
			free(tempRS); //frees memory after use
			return;
		}
	}
}

/**  
 * Sets the rt-code in MI char array to rtName in ParseResult struct 
 * and converts binary value to uint_8t for rt in ParseResult struct
 */
void setRTName(ParseResult* pr, const char* const MI)
{
	char* tempRT; //temporary array of chars for RS
	tempRT = (char*) calloc(1 + REGFIELD_LENGTH, sizeof(char)); // dynamically allocated memory
	strncpy(tempRT, MI + REG2_OFFSET, REGFIELD_LENGTH); // saves the Opcode from MI to tempRS
	tempRT[REGFIELD_LENGTH] = '\0'; // adds null character to end of char array
	
	for (int i = 0; i < 32; i++)
	{
		if (strcmp(MIPSRegisterFieldInstructionsTable[i].Value, tempRT) == 0) //compares tempRt to each value in MIPS Struct
		{
			pr->rtName = (char*) calloc(1 + strlen(MIPSRegisterFieldInstructionsTable[i].Instruction), sizeof(char));// allocate memory
			strncpy(pr->rtName, MIPSRegisterFieldInstructionsTable[i].Instruction, strlen(MIPSRegisterFieldInstructionsTable[i].Instruction));
			// copies binary values to pr->rtName
			pr->rt = MIPSRegisterFieldInstructionsTable[i].IntegerValue; //adds the correct rt value integer to pr->rt
			free(tempRT); //frees memory after use
			return;
		}
	}
}

/**  
 * Sets the rd-code in MI char array to rdName in ParseResult struct 
 * and converts binary value to uint_8t for rd in ParseResult struct
 */
void setRDName(ParseResult* pr, const char* const MI)
{
	char* tempRD; //temporary array of chars for RD
	tempRD = (char*) calloc(1 + REGFIELD_LENGTH, sizeof(char)); // dynamically allocated memory
	strncpy(tempRD, MI + REG3_OFFSET, REGFIELD_LENGTH); // saves the Opcode from MI to tempRS
	tempRD[REGFIELD_LENGTH] = '\0'; // adds null character to end of char array		
	
	for (int i = 0; i < 32; i++)
	{
		if (strcmp(MIPSRegisterFieldInstructionsTable[i].Value, tempRD) == 0) //compares tempRD to each value in MIPS Struct
		{
			pr->rdName = (char*) calloc(1 + strlen(MIPSRegisterFieldInstructionsTable[i].Instruction), sizeof(char)); // allocate memory
			strncpy(pr->rdName, MIPSRegisterFieldInstructionsTable[i].Instruction, strlen(MIPSRegisterFieldInstructionsTable[i].Instruction));
			// copies binary values to pr->rdName
			pr->rd = MIPSRegisterFieldInstructionsTable[i].IntegerValue; //adds the correct rd value integer to pr->rd
			free(tempRD); //frees memory after use
			return;
		}
	}
}

/**  
 * Sets the correct Mnemonic from either the Opcode for I-Type or the 
 * Funct for R-Type using the IFormat and RFormat structs
 */
void setMnemonic(ParseResult* pr, const char* const MI)
{
	for (int i = 0; i < INUMRECORDS; i++) //Iterates through IFormat Struct
	{
		if (strcmp(IFormatOpCodeInstructionsTable[i].OpCode, pr->Opcode) == 0) //compares IFormat opcode values to the opcode from MI
		{
			pr->Mnemonic = (char*) calloc(1 + strlen(IFormatOpCodeInstructionsTable[i].Instruction), sizeof(char));// allocate memory
			strncpy(pr->Mnemonic, IFormatOpCodeInstructionsTable[i].Instruction, strlen(IFormatOpCodeInstructionsTable[i].Instruction));
			// copies binary values to pr->Mnemonic
			return;
		}
	}
	for (int i = 0; i < RNUMRECORDS; i++) //Iterates through RFormat Struct
	{
		if (strcmp(RFormatOpCodeInstructionsTable[i].Function, pr->Funct) == 0) //compares RFormat function values to the funct from MI
		{
			pr->Mnemonic = (char*) calloc(1 + strlen(RFormatOpCodeInstructionsTable[i].Instruction), sizeof(char));// allocate memory
			strncpy(pr->Mnemonic, RFormatOpCodeInstructionsTable[i].Instruction, strlen(RFormatOpCodeInstructionsTable[i].Instruction));
			// copies binary values to pr->Mnemonic
			return;
		}
	}
}

/**  
 * Simple power function that takes exponent and base and returns power
 */
int power(int base, int exponent)
{
	int solution = 1;
	int iterator = 1;
	while (iterator <= exponent)
	{
		solution = solution * base; //multiply base exponent times
		iterator++;
	}
	return solution;
}

/**  
 * Sets the correct Shift value in MI char array to shft in ParseResult
 * struct
 */
void setShift(ParseResult* pr, const char* const MI)
{
	char* tempShift; //temporary array of chars for RS
	tempShift = (char*) calloc(1 + SHFTFIELD_LENGTH, sizeof(char)); // dynamically allocated memory
	strncpy(tempShift, MI + SHMT_OFFSET, SHFTFIELD_LENGTH); // saves the Opcode from MI to tempShift
	tempShift[SHFTFIELD_LENGTH] = '\0'; // adds null character to end of char array
	uint8_t result = 0;
	for (int i = strlen(tempShift) - 1; i >= 0; i--)
	{
		if (tempShift[i] == '1')
		{
			result += (uint8_t)power(2, strlen(tempShift) - i - 1); 
			//if binary val is 1, power 2 with index of tempShift and add to result
		}
	}
	pr->shft = result;
	free(tempShift); //frees memory after use
}
/**  
 * Sets the correct unisigned value in MI char array from the Immediate
 * binary char array to unsignedImm in ParseResult
 */
void setUnsignedImm(ParseResult* pr, const char* const MI)
{
	char* tempUImmediate; //temporary array of chars for RS
	tempUImmediate = (char*) calloc(1 + IMMFIELD_LENGTH, sizeof(char)); // dynamically allocated memory
	strncpy(tempUImmediate, MI + IMM_OFFSET, IMMFIELD_LENGTH); // saves the Opcode from MI to tempShift
	tempUImmediate[IMMFIELD_LENGTH] = '\0'; // adds null character to end of char array
	uint16_t result = 0;
	for (int i = strlen(tempUImmediate) - 1; i >= 0; i--)
	{
		if (tempUImmediate[i] == '1')
		{
			result += (uint16_t)power(2, strlen(tempUImmediate) - i - 1);
			//if binary val is 1, power 2 with index of tempShift and add to result
		}
	}
	pr->unsignedImm = result;
	free(tempUImmediate); //frees memory after use
}

/**  
 * Sets the correct signed value in MI char array from the Immediate
 * binary char array to signedImm in ParseResult
 */
void setSignedImm(ParseResult* pr, const char* const MI)
{
	char* tempSImmediate; //temporary array of chars for RS
	tempSImmediate = (char*) calloc(1 + IMMFIELD_LENGTH, sizeof(char)); // dynamically allocated memory
	strncpy(tempSImmediate, MI + IMM_OFFSET, IMMFIELD_LENGTH); // saves the Opcode from MI to tempShift
	tempSImmediate[IMMFIELD_LENGTH] = '\0'; // adds null character to end of char array
	int16_t result = 0;
	for (int i = strlen(tempSImmediate) - 1; i >= 0; i--)
	{
		if (tempSImmediate[i] == '1')
		{
			result += (int16_t)power(2, strlen(tempSImmediate) - i - 1);
			//if binary val is 1, power 2 with index of tempShift and add to result
		}
	}
	pr->signedImm = result;
	free(tempSImmediate); //frees memory after use
}

/**  
 * Sets the correct unsigned value in MI char array of the 26 bit Target
 */
void setUnsignedTarget(ParseResult* pr, const char* const MI)
{
	char* tempTarget; //temporary array of chars for RS
	tempTarget = (char*) calloc(1 + TARGET_LENGTH, sizeof(char)); // dynamically allocated memory
	strncpy(tempTarget, MI + TARGET_OFFSET, TARGET_LENGTH); // saves the Opcode from MI to tempShift
	tempTarget[TARGET_LENGTH] = '\0'; // adds null character to end of char array
	uint16_t result = 0;
	for (int i = strlen(tempTarget) - 1; i >= 0; i--)
	{
		if (tempTarget[i] == '1')
		{
			result += (uint16_t)power(2, strlen(tempTarget) - i - 1);
			//if binary val is 1, power 2 with index of tempShift and add to result
		}
	}
	pr->unsignedTarget = result;
	free(tempTarget); //frees memory after use
}


/**  
 * Calls all methods above and returns a pointer to ParseResult which 
 * has mInstruction, opcode, funct, rd,rs,rt, rdName, rsName, rtName, 
 * shft, signedImm, and unsignedImm values
 */
ParseResult* parser(ParseResult* pr, const char* const MI)
{
	pr->MInstruction = (char*) calloc(1 + MILength, sizeof(char));
	pr->Opcode = (char*) calloc(1 + OPCODEFIELD_LENGTH, sizeof(char));
	pr->Funct = (char*) calloc(1 + FUNCTFIELD_LENGTH, sizeof(char));
	
	setMInstruction(pr,MI);
	setOpCode(pr,MI);
	setFunct(pr, MI);
	
	setRSName(pr,MI);
	setRTName(pr,MI);
	setRDName(pr,MI);
	
	setMnemonic(pr,MI);
	
	setShift(pr, MI);
	setUnsignedImm(pr, MI);
	setSignedImm(pr, MI);
	setUnsignedTarget(pr, MI);

	return pr;
}

