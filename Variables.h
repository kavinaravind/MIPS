#include <stdlib.h>
#include <inttypes.h>

/**  
 * Variable struct contains the Variable Name, Address, and the Variable
 * Type.
 */
struct _Variable {
	char* VariableName;
	uint16_t Address;
	char* VariableType;
};

typedef struct _Variable Variable;

Variable* assignNameOfVariable(uint16_t address);

void assignTypeOfVariable(char* name, char* type);

void translateData(FILE* input, FILE* output, char* const MI);
