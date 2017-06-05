#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

/**  
 * Variable struct contains the Variable Name, Address, and the Variable
 * Type.
 */
struct _Labels {
	bool Exists;
	int16_t Address;
	int16_t Offset;
	int16_t Index;
};

typedef struct _Labels Labels;
