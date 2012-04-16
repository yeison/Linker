#include "tokenizer.h"
#include "utility.h"

int main (int argc, const char *argv[]) {
    module loaded;
    int offset = 0;
    char errorString [1000];

	/*SymbolTable below is a permanent structure to hold symbols and their 
	absolute addresses.*/
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
		perror("Error: File not found. Make sure the file exists.");
		//and exit with an error code of 1
		exit(1);
	}

        /*  -------- Pass 1 -------  */
	int moduleNumber = 0;
	printf("\n%-4s%-15s%-10s\t%s\n", "#", "Module", "Offset", "Use-List");
	//The while-loop below iterates over each module.
	while(buildModuleName(loaded.moduleName)) {
	        loaded.offset = offset;
		printf("%-4i%-15s%-10i\t", moduleNumber + 1, loaded.moduleName, loaded.offset);
	
		buildDefList(loaded.definitionList);
		char symbolsInModule = (char)loaded.definitionList[0];
		/*The for-loop below iterates through the presently loaded 
		 definitionList in order to migrate all of the definitionNodes to the 
		 symbol table.  The currently loaded module will be overwritten at the 
		 end of the outer for-loop.*/
		for (char i = 0; i < symbolsInModule; i++) {			
			/*In the symbolTable, relativeAddress becomes absolute address. The 
			 variable name remains relativeAddress.*/
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
//
		// Build the use list, and then print it
		int i = 0;
		while(i < loaded.useList[0]){
			printf(" %s ", *(loaded.useList[i + 1]));
			i++;
		}
		printf("\n");
		
	
		/*After the program text is read, we can return the size of the module.  
		 This is saved in loaded.offset for the next module to know its starting 
		 address.*/
		offset += buildProgramText(loaded.programText);
                loaded.size = offset;
		moduleTable[moduleNumber] = loaded;
		moduleNumber++;
	}
	
	//fclose(inputFile);
        /* ----- Creat from Symbol Table a Linked List ---- */

        for(int i = 0; i < symbolOffset - 1 ; i++) {
            defNodePtr sym_i = symbolTable[i];

            (*sym_i).next = symbolTable[i+1];
        }

        /* ----- Print Symbol Table ----- */
	
	printf("\n%s\n","Symbol Table");

        defNodePtr sym_i = symbolTable[0];
//!
        while (sym_i != NULL ){
            if ( findRemoveDuplicateDefinition(sym_i) ) {
                printf("%s = %d, Error: This symbol is multiply defined; first value used.\n", (*sym_i).symbol, (*sym_i).relativeAddress);
                
               
            } else {
                printf("%s = %d\n", (*sym_i).symbol, (*sym_i).relativeAddress);
            }

            sym_i = (*sym_i).next;
            
        }


	printf("\n\n");
	
        /*  --------- Pass 2 --------- */
        int memoryMapCounter  = 0;
        printf("Memory Map\n");
        for (int i = 0; i < moduleNumber; i++) {

            char useListSize = (char)moduleTable[i].useList[0];
            for ( int j = 1; j <= useListSize; j++ ) {
                UseNode *useNodePtr = moduleTable[i].useList[j];
                //A Hash Table would be nicer here.  We can address that later.
                for (int k = 0; k < symbolOffset; k++) {
                    defNodePtr sym = symbolTable[k];

                    if(!strcmp((*useNodePtr).symbol, (*sym).symbol)) {
                        (*useNodePtr).externalAddress = (*sym).relativeAddress;
                    }
                }
            }
                
            char programSize = (char)moduleTable[i].programText[0];
            for (int j = 1; j <= programSize; j++) {
                ProgTextPtr progTextPtr = moduleTable[i].programText[j];
                ProgText progText = *progTextPtr;

                char type = progText.type;
                int instruction = progText.instruction;
                char instructionStr[5];
                UseNode *useNodePtr;
                UseNode useNode;
                int externalAddress;
                int instructionSuffix;
                int instructionPrefix;
                int new_instruction = instruction;
                char *addressError = NULL;

                sprintf(instructionStr, "%d", instruction);
                instructionSuffix = atoi(&instructionStr[1]);
                //ASCII conversion
                instructionPrefix = instructionStr[0] - 48;

                switch(type) {

                    case 'I':
                        new_instruction = instruction;
                        break;

                    case 'A':
                        if(instructionSuffix > MACHINE_SIZE){
                            new_instruction = instructionPrefix*1000 + 0; 
                            addressError = "Error: Absolute address exceeds machine size; zero used.";
                        } else {
                            new_instruction = instruction;
                        }
                        break;

                    case 'R':
                        if(instructionSuffix > moduleTable[i].size){
                            new_instruction = instructionPrefix*1000 + 0; 
                            addressError = "Error: Relative address exceeds module size; zero used.";
                        } else {
                            new_instruction = instruction + moduleTable[i].offset;
                        }
                        break;

                    case 'E':
                        if(instructionSuffix < useListSize){
                            useNodePtr = moduleTable[i].useList[instructionSuffix + 1];
                            useNode = *useNodePtr;
                    //printf("%s ~^~  %i ~~~ \n", useNode.symbol, useNode.externalAddress);
                            externalAddress = useNode.externalAddress;
                            new_instruction = instructionPrefix*1000 + externalAddress;
                        } else {
                            addressError = "Error: External address exceeds length of use list; treated as immediate.";
                        }
                        break;
                }

                if(addressError == NULL){
                    printf("%i: %c %i -> %i \n", memoryMapCounter, type, instruction, new_instruction );
                } else {
                    printf("%i: %c %i -> %i; %s\n", memoryMapCounter, type, instruction, new_instruction, addressError );
                }
                memoryMapCounter++;


                //printf("%i %s\n", uN.externalAddress, uN.symbol);

            }
            printf("\n");
        }

	exit(0);
}

/* Pass header of Symbol Table linked-list, remove dup and return 1. */
int findRemoveDuplicateDefinition (defNodePtr defPtr) {
    int  returnValue = 0;
    defNode    definition;
    defNode    nextDef;
    
    definition = *defPtr;

    while (definition.next != NULL) {
        defNodePtr nextDefPtr = definition.next;
        nextDef = *nextDefPtr;

        if (!strcmp(definition.symbol, nextDef.symbol)) {
            (*defPtr).next = nextDef.next;
            definition = nextDef;
            returnValue = 1;
        } else {
            definition = nextDef;
        }
        
    }

    return returnValue;
}



