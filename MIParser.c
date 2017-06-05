#include "MIParser.h"
#include "InstructionsParser.h"
#include <stdlib.h>


/**  Breaks up given MIPS32 machine instruction and displays information about
 *   all possible fields.
 * 
 *   Pre:  MI points to an array holding the bits (as chars) of an MI
 *         Log is open on an output file
 * 
 *   Returns:
 *         A pointer to a ParseResult object whose fields have been
 *         correctly initialized to correspond to the target of MI.
 */
ParseResult* parseMI(const char* const MI) 
{
   ParseResult* pr = (ParseResult*) calloc(1, sizeof(ParseResult));
   return parser(pr,MI);
}
