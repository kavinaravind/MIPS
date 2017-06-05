#ifndef PARSERESULT_H
#define PARSERESULT_H
#include <inttypes.h>
#include <stdio.h>

/**  Represents the possible field values for a MIPS32 machine instruction.
 * 
 *   A ParseResult object is said to be proper iff:
 * 
 *     - Each of the char* members is either NULL or points to a zero-
 *       terminated C-string.
 *     - If MInstruction is not NULL, the contents of the array represent
 *       a MIPS32 machine instruction.
 *     - If MInstruction is not NULL, the other fields are set to properly
 *       represent the corrsponding fields of the MIPS32 machine instruction
 *       stored in MInstruction.
 *     - Each of the remaing (numeric) fields is either 0 or represents
 *       the value of the corresponding field of the MIPS32 instruction
 *       stored in MInstruction.
 */
struct _ParseResult {
   // Raw machine code portion
   // These are malloc'd zero-terminated C-strings
   char* MInstruction;      // the complete machine instruction
   char* Opcode;            // the extracted opcode field bits
   char* Funct;             // the extracted funct field bits
   
   // Interpreted asm code portion
   char*    Mnemonic;       // the symbolic name of the instruction
   uint8_t  rd;             // the three register fields, as small unsigned integers
   uint8_t  rs;
   uint8_t  rt;
   char*    rdName;         // the names of the registers, as C-strings
   char*    rsName;
   char*    rtName;
   uint8_t  shft;           // the shift field, as a small integer
   uint16_t unsignedImm;    // the immediate field, as an unsigned integer
   uint16_t unsignedTarget;    // the immediate field, as an unsigned integer
   int16_t  signedImm;      // the immediate field, as a signed integer
};
typedef struct _ParseResult ParseResult;

/**  Frees the dynamic content of a ParseResult object.
 * 
 *   Pre:  pPR points to a proper ParseResult object.
 *   Post: All of the dynamically-allocated arrays in *pPR have been
 *         deallocated.
 *         *pPR is proper.
 * 
 *   Comments:
 *     -  The function has no information about whether *pPR has been
 *        allocated dynamically, so it cannot risk attempting to 
 *        deallocate *pPR.
 *     -  The function is intended to provide the user with a simple
 *        way to free memory; the user may or may not reuse *pPR.  So,
 *        the function does set the pointers in *pPR to NULL.
 */
void clearResult(ParseResult* const pPR);

#endif
