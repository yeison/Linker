/* NOTE: You may want enable your text editor's word wrap functionality to read this source file.*/

#include "main.h"
#include "tokenizer.h"

FILE *inputFile; // The file.
char *blankSpace = "[[:space:]]"; // Regular expression for blank spaces.
	
int main (int argc, const char *argv[]) {
	module loaded;
	loaded.offset = 0;
	//SymbolTable below is a permanent structure to hold symbols and their absolute addresses.
	defNode *symbolTable[MAX_TOTAL_SYMBOLS];
	module moduleTable[MAX_MODULES];
	char symbolOffset = 0;
	
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

	int moduleNumber = 0;
	printf("Offsets");
	//The while-loop below iterates over each module.
	while(buildModuleName(loaded.moduleName)) {
		printf("\n");
		printf("Module %i - %i - %s", moduleNumber, loaded.offset, loaded.moduleName);
		//
	
	
		buildDefList(loaded.definitionList);
		char symbolsInModule = (char)loaded.definitionList[0];
		//The for-loop below iterates through the presently loaded definitionList in order to migrate all of the definitionNodes to the symbol table.  The currently loaded module will be overwritten at the end of the outside for-loop.
		for (char i = 0; i < symbolsInModule; i++) {
			//In the symbolTable, relativeAddress becomes absolute address. The variable name remains relativeAddress.
			(*loaded.definitionList[i + 1]).relativeAddress += loaded.offset;
			symbolTable[i+symbolOffset] = loaded.definitionList[i + 1];
		}
		symbolOffset += symbolsInModule;
		
		buildUseList(loaded.useList);
		
//		int j = 0;
//		while(j < loaded.definitionList[0]){
//			printf("%s ", *(loaded.definitionList[j + 1]));
//			j++;
//		}

		// Build the use list, and then print it
//		int i = 0;
//		while(i < loaded.useList[0]){
//			printf("%s ", *(loaded.useList[i + 1]));
//			i++;
//		}
//		printf("\n");
		
	
		//After the program text is read, we can return the size of the module.  This is saved in loaded.offset for the next module to know its starting address.
		loaded.offset += buildProgramText(loaded.programText);
		
		moduleTable[moduleNumber] = loaded;
		moduleNumber++;
	}
	
	//fclose(inputFile);
	
	printf("\n\nSymbol Table");
	for(int i = 0; i < symbolOffset; i++){
		defNode *sym = symbolTable[i];
		printf("\n%s = %d", *sym, (*sym).relativeAddress);
	}
	
	for (int i = 0; i <= moduleNumber; i++) {
		moduleTable[i].useList;
	}
	
	exit(0);
}

char buildModuleName(char moduleNameArray[]){
	return getNextToken(blankSpace, moduleNameArray, inputFile);
}

void buildUseList(UseNode *useListArray[]){
	char *useBuffer = malloc(sizeof(char)); //Allocate 1 byte for size of useList
	getNextToken(blankSpace, useBuffer, inputFile);//Get the size of the useList
	char useCount = atoi(useBuffer);//Convert the ascii to decimal
	useListArray[0] = useCount; //Save the decimal value into the 0th array location
	UseNode *use = malloc(sizeof(UseNode)); //Allocate space for the first UseNode
	
	for (char i = 1; i <= useCount; i++) {
		useBuffer = malloc(sizeof(int)); //Allocate space for the buffer (4 bytes, 32 bit cpu)
		getNextToken(blankSpace, useBuffer, inputFile); //Store the symbol in the buffer
		(*use).symbol = useBuffer; //Point use.symbol to allocated space for symbol
		(*use).externalAddress = 0; //Initialize to 0 for testing later.
		useListArray[i] = use; //Point array pointer to allocated space for UseNode
		use = malloc(sizeof(UseNode)); //Allocate space for next UseNode
	}
}

char buildProgramText(ProgText *progTextArray[]){
	char *progTextBuffer = malloc(sizeof(int));
	getNextToken(blankSpace, progTextBuffer, inputFile);
	char progTextCount = atoi(progTextBuffer);
	progTextArray[0] = progTextCount;
	ProgText *textNode = malloc(sizeof(ProgText));

	for (char i = 1; i <= progTextCount; i++) {
		getNextToken(blankSpace, progTextBuffer, inputFile);
		(*textNode).type = *progTextBuffer;
		getNextToken(blankSpace, progTextBuffer, inputFile);
		(*textNode).instruction = atoi(progTextBuffer);
		progTextArray[i] = textNode;
		textNode = malloc(sizeof(ProgText));
	}
	return progTextCount;
}

void buildDefList(defNodePtr symbolTable[]){
	char symbolListSize;
	defNodePtr defHead = dalloc();
	defNodePtr defNodeP = defHead;
	defNodePtr defNodeP2;
	
	getNextToken(blankSpace, &symbolListSize, inputFile);
	char defQuantity = symbolListSize - '0';
	symbolTable[0] = defQuantity;
	
	for (char i = 1; i <= defQuantity; i++) {
		//Place the next definition into the next node
		*defNodeP = getDefinition(*defNodeP);
		symbolTable[i] = defNodeP;
		//Alocate space for the next definition and referrence it with next
		(*defNodeP).next = dalloc();
		defNodeP2 = defNodeP;
		//Move the pointer to the next node.
		defNodeP = (*defNodeP).next;
	}
	if (defQuantity)
		(*defNodeP2).next = NULL;	
	//printf("defList: %c", defListPointer[1]);
}

defNode getDefinition(defNode temp){
	char *stringBuffer = malloc(sizeof(int));
	temp.symbol = malloc(sizeof(int));
	
	getNextToken(blankSpace, stringBuffer, inputFile);
	strcpy(temp.symbol, stringBuffer);
	getNextToken(blankSpace, stringBuffer, inputFile);
	temp.relativeAddress = atoi(stringBuffer);
	
	//	printf("symbol: %s", temp.symbol);
	//	printf("rAddres: %s", temp.relativeAddress);
	
	return temp;
}

defNodePtr dalloc(void){
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