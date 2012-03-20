//constant table size limit
#define MAXBLOCK 10
enum Kind { CONSTANT, VARIABLE, ARRAY, PROCEDURE, DEFINED }
enum mType { INTEGER, BOOLEAN, UNIVERSAL }

typedef struct {

	 //integer code for ID
	int id;
	
	//Kind is an enum type to represent objects Kind e.g. CONSTANT VARIABLE ARRAY PROCEDURE UNDEFINED
	Kind okind;
	
	//mType is an enum type to represent datatypes in PL e.g. Integer Boolean Universal
	mType otype;
	
	//Only for array types this entry will be filled. All others will have zero
	int size;
	
	//Only for constants
	int value;	
	
} TableEntry; //Extention of the token object?
