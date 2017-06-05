/*
 * Variables.c
 * 
 * Copyright 2015 Kavin Aravind <karavind@localhost.localdomain>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "Variables.h"
#define VariableTableSize 10

/**  
 * Variable struct contains Variable Name, Address, and Variable Type
 */
static Variable VariableTable[VariableTableSize] = {
	{"V01", 0x0002000, NULL},
	{"V02", 0x0002004, NULL},
	{"V03", 0x0002008, NULL},
	{"V04", 0x000200c, NULL},
	{"V05", 0x0002010, NULL},
	{"V06", 0x0002014, NULL},
	{"V07", 0x0002018, NULL},
	{"V08", 0x000201c, NULL},
	{"V09", 0x0002020, NULL},
	{"V10", 0x0002014, NULL}
};

/**  
 * Assigns the type of the Variable
 */
void assignTypeOfVariable(char* name, char* type)
{
	for (int i = 0; i < VariableTableSize; i++)
	{
		if (strncmp(VariableTable[i].VariableName, name, 3) == 0)
		{
			VariableTable[i].VariableType = type;
		}
	}
}

/**  
 * Assigns the name of the Variable
 */
Variable* assignNameOfVariable(uint16_t address)
{
	for (int i = 0; i < VariableTableSize; i++)
	{
		if (VariableTable[i].Address == address)
		{
			return &VariableTable[i];
		}
	}
	return NULL;
}

/**  
 * Translates the Data
 */
void translateData(FILE* input, FILE* output, char* const MI)
{
	bool exist = false;
	int32_t result = 0;
	int length = strlen(MI);
	for (int i = length - 1; i >= 0; i--)
	{
		if (MI[i] == '1')
		{
			result += (int32_t)pow(2, length - i - 1);
		}
	}
	for (int i = 0; i < VariableTableSize; i++)
	{
		if (VariableTable[i].VariableType != NULL)
		{
			fprintf(output, "%s:    %s  %u\n", VariableTable[i].VariableName, VariableTable[i].VariableType, result/2);
			exist = true;
			VariableTable[i].VariableType = NULL;
			i = VariableTableSize;
		}
	}
	if (!exist)
	{
		fprintf(output, "         %u\n", result/2);
	}
}
