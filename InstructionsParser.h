#include "ParseResult.h"
#include <stdio.h>

/**  
 * RFormat struct contains Function codes in Binary and the Function
 * Instructions
 */
struct _RFormat {
	char* Function;
	char* Instruction;
};
typedef struct _RFormat RFormat;

/**  
 * IFormat struct contains Opcodes in Binary and the Opcode Instructions
 */
struct _IFormat {
	char* OpCode;
	char* Instruction;
};
typedef struct _IFormat IFormat;

/**  
 * MIPSRegisterFieldInstructions struct contains Field instructions and
 * its Binary representation
 */
struct _MIPSRegisterFieldInstructions {
	char* Value;
	char* Instruction;
	uint8_t IntegerValue;
};
typedef struct _MIPSRegisterFieldInstructions MIPSRegisterFieldInstructions;

/**  
 * Sets the MI char array to MInstruction in ParseResult struct
 */
void setMInstruction(ParseResult* pr, const char* const MI);

/**  
 * Sets the opCode in MI char array to Opcode in ParseResult struct
 */
void setOpCode(ParseResult* pr, const char* const MI);

/**  
 * Sets the Function in MI char array to Funct in ParseResult struct
 */
void setFunct(ParseResult* pr, const char* const MI);

/**  
 * Sets the rs-code in MI char array to rsName in ParseResult struct 
 * and converts binary value to uint_8t for rs in ParseResult struct
 */
void setRSName(ParseResult* pr, const char* const MI);

/**  
 * Sets the rt-code in MI char array to rtName in ParseResult struct 
 * and converts binary value to uint_8t for rt in ParseResult struct
 */
void setRTName(ParseResult* pr, const char* const MI);

/**  
 * Sets the rd-code in MI char array to rdName in ParseResult struct 
 * and converts binary value to uint_8t for rd in ParseResult struct
 */
void setRDName(ParseResult* pr, const char* const MI);

/**  
 * Sets the correct Mnemonic from either the Opcode for I-Type or the 
 * Funct for R-Type using the IFormat and RFormat structs
 */
void setMnemonic(ParseResult* pr, const char* const MI);

/**  
 * Sets the correct Shift value in MI char array to shft in ParseResult
 * struct
 */
void setShift(ParseResult* pr, const char* const MI);

/**  
 * Sets the correct unisigned value in MI char array from the Immediate
 * binary char array to unsignedImm in ParseResult
 */
void setUnsignedImm(ParseResult* pr, const char* const MI);

/**  
 * Sets the correct signed value in MI char array from the Immediate
 * binary char array to signedImm in ParseResult
 */
void setSignedImm(ParseResult* pr, const char* const MI);

/**  
 * Sets the correct unsigned value in MI char array of the 26 bit Target
 */
void setUnsignedTarget(ParseResult* pr, const char* const MI);

/**  
 * Calls all methods above and returns a pointer to ParseResult which 
 * has mInstruction, opcode, funct, rd,rs,rt, rdName, rsName, rtName, 
 * shft, signedImm, and unsignedImm values
 */
ParseResult* parser(ParseResult* pr, const char* const MI);

