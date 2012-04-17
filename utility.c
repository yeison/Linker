/*
 *  utility.c
 *  linker
 *
 *  Created by Yeison Rodriguez on 7/19/10.
 *  Copyright 2010 New York University. All rights reserved.
 *
 */

#include "utility.h"
#include "tokenizer.h"

char buildModuleName(char moduleNameArray[]){
	return getNextToken(moduleNameArray, inputFile);
}

void buildUseList(UseNode *useListArray[]){
	//Allocate 1 byte for size(count) of useList
	char *useBuffer = malloc(sizeof(char)); 
	//Get the size of the useList
	getNextToken(useBuffer, inputFile);
	//Convert the ascii to decimal
	char useCount = atoi(useBuffer);
        //Print error if use-list is too large
        if(useCount > MAX_USELIST){
            perror("Use-list exceeds MAX_USELIST value.");
            useCount = MAX_USELIST;
        }
	//Save the decimal value into the 0th array location
	useListArray[0] = useCount; 
        //Allocate space for the first UseNode
	UseNode *use = malloc(sizeof(UseNode)); 
	
	for (char i = 1; i <= useCount; i++) {
		//Allocate space for the buffer (4 bytes on 32 bit cpu)
		useBuffer = malloc(sizeof(int)); 

		//Store the symbol in the buffer
		getNextToken( useBuffer, inputFile); 

		//Point use.symbol to allocated space for symbol
		(*use).symbol = useBuffer; 

		//Initialize to 0 for testing later.
		(*use).externalAddress = 0; 

		//Point array pointer to allocated space for UseNode
		useListArray[i] = use; 

		//Allocate space for next UseNode in linked list.
		use = malloc(sizeof(UseNode)); 
	}
}

char buildProgramText(ProgText *progTextArray[]){
	char *progTextBuffer = malloc(sizeof(int));
	getNextToken(progTextBuffer, inputFile);
	char progTextCount = atoi(progTextBuffer);
        char errorTokens = 0;
        if(progTextCount > MAX_INSTRUCTIONS){
            perror("MAX_INSTRUCTIONS per module exceeded. Using MAX_INSTRUCTIONS as size.");
            errorTokens = progTextCount - MAX_INSTRUCTIONS;
            progTextCount = MAX_INSTRUCTIONS;
        }
	progTextArray[0] = progTextCount;
	ProgText *textNode = malloc(sizeof(ProgText));
	
	for (char i = 1; i <= progTextCount; i++) {
		getNextToken(progTextBuffer, inputFile);
		(*textNode).type = *progTextBuffer;
		getNextToken(progTextBuffer, inputFile);
		(*textNode).instruction = atoi(progTextBuffer);
		progTextArray[i] = textNode;
		textNode = malloc(sizeof(ProgText));
	}

        while(errorTokens != 0){
            getNextToken(progTextBuffer, inputFile);
            getNextToken(progTextBuffer, inputFile);
        }
	return progTextCount;
}

void buildDefList(DefNodePtr symbolTable[], char moduleNumber){
	char symbolListSize;
	DefNodePtr defHead = dalloc();
	DefNodePtr defNodeP = defHead;
	DefNodePtr defNodeP2;
	getNextToken(&symbolListSize, inputFile);
	char defQuantity = symbolListSize - '0';
        char errorTokens = 0;
        if(defQuantity > MAX_SYMBOLS){
            perror("MAX_SYMBOLS per module exceeded. Using MAX_SYMBOLS as size.");
            errorTokens = defQuantity - MAX_SYMBOLS;
            defQuantity = MAX_SYMBOLS;
        }
	symbolTable[0] = defQuantity;
	
	for (char i = 1; i <= defQuantity; i++) {
		//Place the next definition into the next node
		*defNodeP = getDefinition(*defNodeP);
                (*defNodeP).memberOfModule = moduleNumber;
		symbolTable[i] = defNodeP;
		//Alocate space for the next definition and reference it with next
		(*defNodeP).next = dalloc();
		defNodeP2 = defNodeP;
		//Move the pointer to the next node.
		defNodeP = (*defNodeP).next;
	}

        //These tokens will not be read into the program.
        while(errorTokens != 0){
            DefNode errTokDN;
            getDefinition(errTokDN);
            errorTokens--;
        }
 
	if (defQuantity)
		(*defNodeP2).next = NULL;	
	//printf("defList: %c", defListPointer[1]);
}

DefNode getDefinition(DefNode temp){
	char *stringBuffer = malloc(sizeof(int));
	temp.symbol = malloc(sizeof(int));
	
	getNextToken(stringBuffer, inputFile);
	strcpy(temp.symbol, stringBuffer);
	getNextToken(stringBuffer, inputFile);
	temp.relativeAddress = atoi(stringBuffer);
	
	//	printf("symbol: %s", temp.symbol);
	//	printf("rAddres: %s", temp.relativeAddress);
	
	return temp;
}

DefNodePtr dalloc(void){
	return (DefNodePtr)malloc(sizeof(DefNode));
}

void printList(DefNodePtr p){
	int i = 0;
	while(p != NULL){
		printf("Node %d Symbol: %s\n", i, (*p).symbol);
		printf("Node %d rAddress: %d\n", i, (*p).relativeAddress);
		p = (*p).next;
		i++;
	}
}

/*Get the definition node at the location indicated by nodeNumber, from the 
 definition list who's head is pointed to by the defNodePtr p. */
DefNode getDefNode(char nodeNumber, DefNodePtr p){
	for (int i = 0; i < nodeNumber; i++) {
		p = (*p).next;
	}
	return *p;
}

//Hash function from http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(unsigned char *str) {
    unsigned long hash = 5;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

