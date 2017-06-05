#include "ParseResult.h"
#include <stdlib.h>
	
/**  Frees the dynamic content of a ParseResult object.
 * 
 *   Pre:  pPR points to a proper ParseResult object.
 *   Post: All of the dynamically-allocated arrays in *pPR have been
 *         deallocated.
 * 
 *   Comments:
 *     -  The function has no information about whether *pPR has been
 *        allocated dynamically, so it cannot risk attempting to 
 *        deallocate *pPR.
 *     -  The function is intended to provide the user with a simple
 *        way to free memory; the user may or may not reuse *pPR.  So,
 *        the function does set the pointers in *pPR to NULL.
 */
void clearResult(ParseResult* const pPR) 
{
   free(pPR->MInstruction);
   free(pPR->Opcode);
   free(pPR->Funct);
   free(pPR->Mnemonic);
   free(pPR->rdName);
   free(pPR->rsName);
   free(pPR->rtName);
   
   pPR->MInstruction = NULL;
   pPR->Opcode = NULL;
   pPR->Funct = NULL;
   pPR->Mnemonic = NULL;
   pPR->rdName = NULL;
   pPR->rsName = NULL;
   pPR->rtName = NULL;
   
   pPR->rd = 0;
   pPR->rs = 0;
   pPR->rt = 0;
   pPR->shft = 0;
   pPR->unsignedImm = 0;
   pPR->signedImm = 0;
   pPR->unsignedTarget = 0; 
}
