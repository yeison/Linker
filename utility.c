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
	//Allocate 1 byte for size of useList
	char *useBuffer = malloc(sizeof(char)); 
	//Get the size of the useList
	getNextToken(useBuffer, inputFile);
	//Convert the ascii to decimal
	char useCount = atoi(useBuffer);
	//Save the decimal value into the 0th array location
	useListArray[0] = useCount; 
	UseNode *use = malloc(sizeof(UseNode)); //Allocate space for the first UseNode
	
	for (char i = 1; i <= useCount; i++) {
		useBuffer = malloc(sizeof(int)); 
		//Allocate space for the buffer (4 bytes, 32 bit cpu)
		getNextToken( useBuffer, inputFile); 
		//Store the symbol in the buffer
		(*use).symbol = useBuffer; 
		//Point use.symbol to allocated space for symbol
		(*use).externalAddress = 0; 
		//Initialize to 0 for testing later.
		useListArray[i] = use; 
		//Point array pointer to allocated space for UseNode
		use = malloc(sizeof(UseNode)); 
		//Allocate space for next UseNode
	}
}

char buildProgramText(ProgText *progTextArray[]){
	char *progTextBuffer = malloc(sizeof(int));
	getNextToken(progTextBuffer, inputFile);
	char progTextCount = atoi(progTextBuffer);
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
	return progTextCount;
}

void buildDefList(defNodePtr symbolTable[]){
	char symbolListSize;
	defNodePtr defHead = dalloc();
	defNodePtr defNodeP = defHead;
	defNodePtr defNodeP2;
	
	getNextToken(&symbolListSize, inputFile);
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

definitionNode getDefinition(definitionNode temp){
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

defNodePtr dalloc(void){
	return (defNodePtr)malloc(sizeof(definitionNode));
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

/*Get the definition node at the location indicated by nodeNumber, from the 
 definition list who's head is pointed to by the defNodePtr p. */
definitionNode getDefNode(char nodeNumber, defNodePtr p){
	for (int i = 0; i < nodeNumber; i++) {
		p = (*p).next;
	}
	return *p;
}
