/*
 * Disassembler.c for MIPS Disassembler
 * 
 * Requires: MIParser.h
 * 
 * Process:
 * 
 * 		disassem <input file> <output file>
 * 
 * 		param 1:  The input file will contain a valid MIPS32 machine 
 * 				  code program, including both a .text segment and a 
 *				  .data segment, written in binary text format, with 
 * 			      one word per line.
 * 
 * 		param 2:  Name of the file to hold results from disassembler
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ParseResult.h"     // for reporting parse results
#include "MIParser.h"        // for student's solution
#include "Variables.h"
#include "Labels.h"

#define MILength  32
#define InstructionCount 50
#define VariableCount 25
#define LabelsTableLength 100

static bool readMI(FILE* fp, char* const MI);
static void translateToAssembly(FILE* file, ParseResult* pr, Labels labelsTable[], int labelsIndex, int lineNum);

int main(int argc, char **argv)
{
	FILE* input = fopen(argv[1], "r"); //open input file
	FILE* output  = fopen(argv[2], "w"); //open results file

	if (input == NULL) 
	{
		printf("Unable to find file %s... exiting.\n", argv[1]);
		return 1;
	}
	
	fprintf(output, ".text\n");
	
	char MInstruction[MILength + 1]; // holds current Mips Instruction
	ParseResult* Instructions[InstructionCount]; // holds ParseResult pointers
	
	int lineIndex = 0; //index iterator count
	while (readMI(input, MInstruction)) //process if instructions exist
	{
		Instructions[lineIndex] = parseMI(MInstruction);
		lineIndex++;
   	}
   
   	Labels LabelsTable[LabelsTableLength]; //Labels Struct
	bool exist = false; //check if Label exists in struct
	int labelsIndex = 0; //index counter for Labels Struct
   	for (int i = 0; i < lineIndex; i++)
   	{
		exist = false;
		if (strcmp("beq", Instructions[i]->Mnemonic) == 0 || 
			strcmp("bne", Instructions[i]->Mnemonic) == 0 ||
			strcmp("blez", Instructions[i]->Mnemonic) == 0 || 
			strcmp("bltz", Instructions[i]->Mnemonic) == 0)
		{
			for (int j = 0; j < labelsIndex; j++) 
			{
				//check if Label Address is already in struct
				if (LabelsTable[j].Address == Instructions[i]->signedImm + i + 1)
				{
					exist = true;
					j = labelsIndex;
				}
			}
			if (!exist) //create Label and add to struct
			{
				LabelsTable[labelsIndex].Exists = true;
				LabelsTable[labelsIndex].Index = i;
				LabelsTable[labelsIndex].Offset = Instructions[i]->signedImm;
				LabelsTable[labelsIndex].Address =  i + Instructions[i]->signedImm + 1;
				labelsIndex++;
			}
		}
		else if (strcmp("j", Instructions[i]->Mnemonic) == 0)
		{
			for (int j = 0; j < labelsIndex; j++)
			{
				//check if Label Address is already in struct
				if (LabelsTable[j].Address == Instructions[i]->unsignedTarget)
				{
					exist = true;
					j = labelsIndex;
				}
			}
			if (!exist) //create Label and add to struct
			{
				LabelsTable[labelsIndex].Exists = true;
				LabelsTable[labelsIndex].Index = i;
				LabelsTable[labelsIndex].Offset = Instructions[i]->unsignedTarget;
				LabelsTable[labelsIndex].Address =  Instructions[i]->unsignedTarget;
				labelsIndex++;
			}
		}
	}
	for (int i = 0; i < lineIndex; i++) // Clean up dynamic allocations and print structure
	{
		if (i == 0)
		{
			fprintf(output, "main:");
		}
		exist = false;
		for (int j = 0; j < labelsIndex;j++)
		{
			if (LabelsTable[j].Exists)
			{
				if (LabelsTable[j].Address == i)
				{
					fprintf(output, "L0%d:", j + 1); //Add Label
					translateToAssembly(output, Instructions[i], LabelsTable, labelsIndex, i);
					exist = true;
					j = labelsIndex;
				}
			}
		}		
		if (!exist) //Label not needed
		{
			translateToAssembly(output, Instructions[i], LabelsTable, labelsIndex, i);
		}
		clearResult(Instructions[i]); //clean up dynamic allocations
		free(Instructions[i]);
	}

	fprintf(output, "\n.data\n");
	fgetc(input); //gets to data segment
	
	while (readMI(input, MInstruction)) //process if data exist
	{
		translateData(input, output, MInstruction); //print data
	}

	fclose(input); //close input file
	fclose(output); //close output file
   
	return 0;
}

/**  Attempts to read s 32-bit machine instruction (in text binary format) from the
 *   specified input stream.
 * 
 *   Pre:  *fp is an open input file containing MIPS32 machine instructions
 *         *MI is a char array of dimension 33
 *   Post: *MI contains 32 bits of a MIPS machine instruction, or an input failure
 *            has been detected
 *   Returns:  false if input failure occurs; true otherwise
 */
static bool readMI(FILE* file, char* const MI) 
{   
   // Attempt to read MI from input stream
	bool success = fgets(MI, MILength + 1, file) != NULL;	
	if (!success)
	{
		return false; // fgets() returns NULL if it detects an error
	}  
	if (strlen(MI) == MILength) // Check whether a full MI was read:
	{
		success = true;
		fgetc(file); // If successful, get newline following MI
	}
	else
	{
		success = false;
	}
	return success;
}

/**  
 * translates 32 bit into Assembly
 */
static void translateToAssembly(FILE* file, ParseResult* pr, Labels labelsTable[], int labelsIndex, int lineNum)
{
	if (strcmp("add", pr->Mnemonic) == 0 || 
		strcmp("and", pr->Mnemonic) == 0 ||
		strcmp("nor", pr->Mnemonic) == 0 ||
		strcmp("or", pr->Mnemonic) == 0  ||
		strcmp("slt", pr->Mnemonic) == 0 ||
		strcmp("sub", pr->Mnemonic) == 0 ||
		strcmp("xor", pr->Mnemonic) == 0)
	{
		fprintf(file, "        %s    %s, %s, %s\n", pr->Mnemonic, pr->rdName, pr->rsName, pr->rtName);
	}
	if (strcmp("addi", pr->Mnemonic) == 0 ||
		strcmp("addiu", pr->Mnemonic) == 0 ||
		strcmp("andi", pr->Mnemonic) == 0 ||
		strcmp("ori", pr->Mnemonic) == 0  ||
		strcmp("slti", pr->Mnemonic) == 0 ||
		strcmp("xori", pr->Mnemonic) == 0)	
	{
		fprintf(file, "        %s   %s, %s, %d\n", pr->Mnemonic, pr->rtName, pr->rsName, pr->signedImm);
	}
	if (strcmp("beq", pr->Mnemonic) == 0 || 
		strcmp("bne", pr->Mnemonic) == 0)
	{
		for (int i = 0; i < labelsIndex; i++)
		{
			if (labelsTable[i].Address == (pr->signedImm + lineNum + 1)) //print correct label
			{
				fprintf(file, "        %s    %s, %s, L0%d\n", pr->Mnemonic, pr->rsName, pr->rtName, i + 1);
				i = LabelsTableLength;
			}
		}
	}
	if (strcmp("blez", pr->Mnemonic) == 0 || 
		strcmp("bltz", pr->Mnemonic) == 0)
	{
		for (int i = 0; i < labelsIndex; i++)
		{
			if (labelsTable[i].Address == (pr->signedImm + lineNum + 1)) // print correct label
			{
				fprintf(file, "        %s    %s, L0%d\n", pr->Mnemonic, pr->rsName, i + 1);
				i = LabelsTableLength;
			}
		}
	}
	if (strcmp("j", pr->Mnemonic) == 0)
	{
		for (int i = 0; i < labelsIndex; i++)
		{
			if (labelsTable[i].Exists)
			{
				if (labelsTable[i].Address == pr->unsignedTarget) // print correct label
				{
					fprintf(file, "        %s   L0%d\n", pr->Mnemonic, i + 1);
					i = LabelsTableLength;
				}
			}
		}
	}
	if (strcmp("div", pr->Mnemonic) == 0 || 
		strcmp("mult", pr->Mnemonic) == 0)
	{
		fprintf(file, "        %s     %s, %s\n", pr->Mnemonic, pr->rsName, pr->rtName);
	}
	if (strcmp("lw", pr->Mnemonic) == 0 || 
		strcmp("sw", pr->Mnemonic) == 0 ||
		strcmp("lb", pr->Mnemonic) == 0 ||
		strcmp("sb", pr->Mnemonic) == 0)
	{
		if (pr->unsignedImm >= 8192) //Create var
		{
			Variable* var = assignNameOfVariable(pr->signedImm); //assign name
			fprintf(file, "        %s     %s, %s\n", pr->Mnemonic, pr->rtName, var->VariableName);
			if (strcmp("lw", pr->Mnemonic) == 0 || 
				strcmp("sw", pr->Mnemonic) == 0)
			{
				assignTypeOfVariable(var->VariableName, ".word"); //assign word type
			}
			else
			{
				assignTypeOfVariable(var->VariableName, ".byte"); //assign byte type
			}
		}
		else
		{
			fprintf(file, "        %s     %s, %d(%s)\n", pr->Mnemonic, pr->rtName, pr->signedImm, pr->rsName);
		}
	}
	if (strcmp("lui", pr->Mnemonic) == 0)
	{
		fprintf(file, "        %s   %s, %d\n", pr->Mnemonic, pr->rtName, pr->signedImm);
	}
	if (strcmp("mfhi", pr->Mnemonic) == 0 || strcmp("mflo", pr->Mnemonic) == 0)
	{
		fprintf(file, "        %s   %s\n", pr->Mnemonic, pr->rdName);
	}
	if (strcmp("sll", pr->Mnemonic) == 0 ||
		strcmp("sra", pr->Mnemonic) == 0 ||
		strcmp("srl", pr->Mnemonic) == 0)
	{
		fprintf(file, "        %s   %s, %s, %d\n", pr->Mnemonic, pr->rdName, pr->rtName, pr->shft);
	}
	if (strcmp("syscall", pr->Mnemonic) == 0)
	{
		fprintf(file, "        %s\n", pr->Mnemonic);
	}
}
