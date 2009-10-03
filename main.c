/* NOTE: You may want enable your text editor's word wrap functionality to read this source file.  Most major editors support this feature. */

#include "main.h"

static int *tokenbuf[MAXMEM];
static int *tbpointer = tokenbuf;
FILE *inputFile; // The file
	
int main (int argc, const char *argv[]) {
	char *fileName; // A pointer to the name of the file

	// If the user provides no argument, print the program's usage
	if (argc < 2) {
		printf("\tUsage: Lab1 <filename> \n\n");
		exit(0);
	}
	
	
//	printf(argv[1]);
//	printf("\n");
	
	//Open a stream to the file provided as the first argument.
	inputFile = fopen(argv[1], "r");

	//Check to see if fopen() was successful
	if (inputFile == NULL) {
		//If not, then print this message to the standard error output stream.
		perror("Error: File not found. Make sure the file exists. If the file name contains spaces, then surround the whole name with quotes.\nError");
		//and exit with an error code of 1
		exit(1);
	}
	
	/*  Use white spaces as delimiters to tokenize the contents of the file "inputFile".  The function fileTokenizer takes a regular expression as
		its input for delimiter. */
	fileTokenizer("[[:space:]]", inputFile);
	
    return 0;
}

static char round = MODULE_NAME;
// A regular expression type (regex_t) to match white spaces.
regex_t matchWhiteSpace;
	
void fileTokenizer(char *delimiter, FILE *file){
	int tokenLength = 0;
	char newStatus = NULL;
	char oldStatus;
	char c;
	
	
	// Compile the regular expression.
	if(regcomp(&matchWhiteSpace, delimiter, REG_EXTENDED) != 0){
		perror("Check your regexp!");
		exit(1);
	}
	
		/* regexec() returns 0 if the string passed to it contains the sequence
		 specified by the regular expression.  In this case the regular expression
		 is any kind of white space, and the we are passing one character at a time
		 to the regexec function.  Therefore, if the character passed is a white
		 space, the if statement fails and the white space is ignored. */	
	while ((c = getc(file)) != EOF){ 
		//While the next character is not the end of the file.
		
		oldStatus = newStatus; 
		//Save the old status; is the previous character a white space?
		
		newStatus = regexec(&matchWhiteSpace, &c, 0, NULL, REG_NOTBOL);
		//Get the new status; is the current character a white space?
		
		/* If the current character and the previous character are both white spaces,
		do nothing.  Continue to read the next character.*/
		if((oldStatus == 0) && (newStatus == 0))
			continue;
		/*  Otherwise, if the current character is a white space but the previous one was not, jump to the next sub-array. */
		else if(!newStatus){
			round = packModule(tbpointer, round);
			//printf(tbpointer);
			//printf("\n");
			tbpointer += tokenLength;
			tokenLength = 0; // Reset the token length
		}
		/* If the new character and the old characters are both none white space, append the new character to the current subarray, which is a string.*/
		else {
			strcat(tbpointer, &c);
			tokenLength++;
		}
	}
	printf(moduleName);
	printf("\n");
	printf(definitionList);
	printf("\n");
		
	// Release the memory used by the regular expression compile.
	regfree(&matchWhiteSpace);
}

int *moduleName;

char packModule(int *stringPointer, char round){	
	switch (round) {
		case MODULE_NAME:
			moduleName = stringPointer;
			return DEF_LIST;
		case DEF_LIST:
			return buildDefList(stringPointer, *stringPointer);
		case USE_LIST:
			buildUseList(stringPointer, *stringPointer);
			return PROGRAM_TEXT;
		case PROGRAM_TEXT:
			buildProgramText(stringPointer, *stringPointer);

		default:
			return(-1);
	}
}

//Where in memory does the definitionList start?
int *definitionList;
//Where in the definition list are we?  First value, second value, etc...

char buildDefList(int *defListPointer, int defQuantity){
	int nextToken;
	/*  If we are at the first value of the definition list, that value should indicate the number of definitions.  Store this value in defQuantity. In addition, this point is the beginning of the definitionList.  Save that as well. Increment the pointer to value 2. */
	definitionList = defListPointer;
	
	for (int i = 0; i < defQuantity; i++) {
		nextToken = getNextToken(inputFile);
		strcat(definitionList, &nextToken);
	}
	return USE_LIST;
}

void buildUseList(int *useListPointer, int useQuantity){
	
}

void buildProgramText(int *progTextPointer, int instructionQuantity){

}

int getNextToken(FILE *file){
	int *token;
	char c;
	char newStatus = NULL;
	char oldStatus;
	
	while ((c = getc(file)) != EOF){ 
		//While the next character is not the end of the file.
		
		//Save the old status; is the previous character a white space?		
		oldStatus = newStatus; 
		
		//Get the new status; is the current character a white space?
		newStatus = regexec(&matchWhiteSpace, &c, 0, NULL, REG_NOTBOL);
		
		
		/*  If the current character is a white space, do nothing.  Continue to read the next character. */
		if((oldStatus == 0) && (newStatus == 0))
			continue;
		/*  Otherwise, if the current character is a white space but the previous one was not, return the string. */
		else if(!newStatus){
			printf(token);
			return *token;
		}
		/* If the new character and the old characters are both none white space, append the new character to the current subarray, which is a string.*/
		else 
			strcat(token, &c);
	}
}