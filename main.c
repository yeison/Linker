/* NOTE: You may want enable your text editor's word wrap functionality to read this source file.*/

#include "main.h"
#define DEFLIST_SIZE 32

FILE *inputFile; // The file
char *blankSpace = "[[:space:]]";
char symbolOffset = 0;
	
int main (int argc, const char *argv[]) {
	char i;
	module loaded;
	defNodePtr symbolTable[MAX_SYMBOLS];
	
	// If the user provides no argument, print the program's usage
	if (argc < 2) {
		printf("\tUsage: Lab1 <filename> \n\n");
		exit(0);
	}
	
	//Open a stream to the file provided as the first argument.
	inputFile = fopen(argv[1], "r");

	//Check to see if fopen() was successful
	if (inputFile == NULL) {
		//If not, then print this message to the standard error output stream.
		perror("Error: File not found. Make sure the file exists. If the file name contains spaces, then surround the whole name with quotes.\nError");
		//and exit with an error code of 1
		exit(1);
	}
	
	
	buildModuleName(loaded.moduleName);
	printf("%s\n", loaded.moduleName);
	
	loaded.definitionList = dalloc();
	loaded.definitionList = buildDefList(loaded.definitionList);
	printList(loaded.definitionList);
	printf("The relAddress: %d\n", (*symbolTable[0]).relativeAddress);	

	
	loaded.offset = 0; // Gets 0 until the useList works.
	
	
//	while (loaded.definitionList[i].symbol != NULL) {
//		symbolTable[i] = makeSymbolNode(getDefNode(i), 1);
//	}  // length(definitionList)
	
	exit(0);
}

char buildModuleName(char *moduleNamePointer){
	return getNextToken(blankSpace, moduleNamePointer, inputFile);
}

void buildUseList(int *useListPointer, int useQuantity){
	
}

void buildProgramText(int *progTextPointer, int instructionQuantity){

}

char getNextToken(char *delimiter, char *buffer, FILE *file){
	char tokenLength = 0;
	char newStatus = NULL;
	char oldStatus;
	char c;
	regex_t regularExpression; // A regular expression type (regex_t)	
	// Compile the regular expression.
	if(regcomp(&regularExpression, delimiter, REG_EXTENDED) != 0){
		perror("Check your regexp!");
		exit(1);
	}
	
	/* regexec() returns 0 if the string passed to it contains the sequence specified by the regular expression.  In this case the regular expression is any kind of white space, and the we are passing one character at a time to the regexec function.  Therefore, if the character passed is a white space, the if statement fails and the white space is ignored. */	
	while ((c = getc(file)) != EOF){ 
		//While the next character is not the end of the file.
		
		oldStatus = newStatus;
		//Save the old status; is the previous character a white space?
		
		newStatus = regexec(&regularExpression, &c, 0, NULL, REG_NOTBOL);
		newStatus = regexec(&regularExpression, &c, 0, NULL, REG_NOTBOL);
		//Get the new status; is the current character a white space?
		/* The reason I'm calling the regexec twice, is because there seems to be a bug. When I was testing my code with the Temperature module, the 9th call kept returning a zero, despite the fact that the 9th character is an 'r'.  Calling the function twice seems to circumvent the bug.*/
		
		/* If the current character and the previous character are both white spaces,
		 do nothing.  Continue to read the next character.*/
		if((oldStatus == 0) && (newStatus == 0)){
			continue;
		}
		/* If the new character and the old characters are both none white space, append the new character to the buffer.*/
		else if(newStatus) {
			buffer[tokenLength] = c;
			//strcat(buffer, &c);
			tokenLength++;
		}
		/*  Otherwise, if the current character is a white space but the previous one was not, finish */
		else {
			regfree(&regularExpression);
			buffer[tokenLength] = '\0';
			//strcat(buffer, "\0");
			//printf("Buffer: %s\n", buffer);
			return *buffer;
		}
	}
	
	// Release the memory used by the regular expression compile.
	perror("No token was found, the input file may not have the proper format.\n");
	exit(2);
}

defNodePtr buildDefList(defNodePtr symbolTable[]){
	char symbolListSize;
	(*defNodeP).next = dalloc();
	defNodePtr defHead = (*defNodeP).next;
	
	
	getNextToken(blankSpace, &symbolListSize, inputFile);
	char defQuantity = symbolListSize - '0';
	printf("defQuantity: %d\n", defQuantity);
	
	for (char i = 1; i <= defQuantity; i++) {
		//Place the next definition into the next node
		*(*defNodeP).next = getDefinition();
		//Move the pointer to the next node.
		defNodeP = (*defNodeP).next;
		toSymbolTable(defNodeP);
		//Alocate space for the next definition and referrence it with next
		(*defNodeP).next = dalloc();
	}
	(*defNodeP).next = NULL;
	
	//printf("defList: %c", defListPointer[1]);
	return defHead;
}

defNode getDefinition(){
	defNode temp;
	char *stringBuffer[SYMBOL_SIZE];
	
	getNextToken(blankSpace, stringBuffer, inputFile);
	strcpy(temp.symbol, stringBuffer);
	getNextToken(blankSpace, stringBuffer, inputFile);
	temp.relativeAddress = *stringBuffer - '0';
	
	//	printf("symbol: %s", temp.symbol);
	//	printf("rAddres: %s", temp.relativeAddress);
	
	return temp;
}

defNodePtr *dalloc(void){
	return (defNodePtr)malloc(sizeof(defNode));
}

void printList(defNodePtr p){
	int i = 0;
	while(p != NULL){
		printf("Node %d Symbol: %s\n", i, (*p).symbol);
		printf("Node %d rAddress: %d\n", i, (*p).relativeAddress);
		p = (*p).next;
		i++;
	}
}

// Get the definition node at the location indicated by nodeNumber, from the definition list who's head is pointed to by the defNodePtr p.
defNode getDefNode(char nodeNumber, defNodePtr p){
	for (int i = 0; i < nodeNumber; i++) {
		p = (*p).next;
	}
	return *p;
}

// Makes a symbol node, which contains the symbol and its absolute address.  The offset is the memory location where the module that this symbol belongs to begins.
//sNode makeSymbolNode(char *symbol, char relativeAddress){
//	sNode temp = *sNalloc();
//	strcpy(temp.symbol, symbol);
//	temp.address = relativeAddress;
//	return temp;
//}
//
//sNode *sNalloc(void){
//	return (sNode *)malloc(sizeof(sNode));
//}

void toSymbolTable(defNodePtr symbolNodePointer){
	symbolTable[symbolOffset] = symbolNodePointer;
	symbolOffset++;
}


// Places the symbolNode passed into the global symbolTable array.
//void toSymbolTable(defNodePtr p, char moduleOffset){
//	int i = 0;
//	while (p != NULL) {
//		symbolTable[symbolOffset + i] = makeSymbolNode(*p, moduleOffset);
//		p = (*p).next;
//		i++;
//	}
//	symbolOffset += i;
//}