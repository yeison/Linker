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
		moduleTable[moduleNumber] = loaded;
		moduleNumber++;
	}
	
	//fclose(inputFile);
        /* ----- Convert Symbol Table to Linked List ---- */

        for(int i = 0; i < symbolOffset - 1 ; i++) {
            defNodePtr sym_i = symbolTable[i];

            (*sym_i).next = symbolTable[i+1];
        }

        /* ----- Print Symbol Table ----- */
	
	printf("\n%s\n","Symbol Table");
        for(int i = 0; i < symbolOffset; i++){
            if( symbolOffset == 0) 
                break;

            defNodePtr sym_i = symbolTable[i];

            if( symbolOffset == 1 ){
                printf("%s = %d \n", (*sym_i).symbol, (*sym_i).relativeAddress);
                continue;
            }
                
            int j = i + 1;
            while(j < symbolOffset){

                defNodePtr sym_j = symbolTable[j];
                if ( !strcmp((*sym_i).symbol, (*sym_j).symbol) ) {
                    printf("%s = %d, Error: This symbol is multiply defined; first value used.\n", (*sym_i).symbol, (*sym_i).relativeAddress);
                    symbolTable[j] = sym_i;
                    break;
                } 
                j++;
            }
            printf("%s = %d \n", (*sym_i).symbol, (*sym_i).relativeAddress);
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
                        if((*useNodePtr).externalAddress != 0){
                            char error[100]; 
                            sprintf(error, "Symbol already defined\n");
                            strcat(errorString, error);
                            continue;
                        } else {
                            (*useNodePtr).externalAddress = (*sym).relativeAddress;
                        }
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
                char externalSuffix;
                char externalPrefix;
                int new_instruction = instruction;

                switch(type) {

                    case 'I':
                        new_instruction = instruction;
                        break;

                    case 'A':
                        new_instruction = instruction;
                        break;

                    case 'R':
                        new_instruction = instruction + moduleTable[i].offset;
                        break;

                    case 'E':
                        sprintf(instructionStr, "%d", instruction);
                        externalSuffix = atoi(&instructionStr[1]);
                        //ASCII conversion
                        externalPrefix = instructionStr[0] - 48;
                        useNodePtr = moduleTable[i].useList[externalSuffix + 1];
                        useNode = *useNodePtr;
                    //printf("%s ~^~  %i ~~~ \n", useNode.symbol, useNode.externalAddress);
                        externalAddress = useNode.externalAddress;
                        new_instruction = externalPrefix*1000 + externalAddress;
                        break;
                }

                printf("%i: %c %i -> %i \n", memoryMapCounter, type, instruction, new_instruction );
                memoryMapCounter++;


                //printf("%i %s\n", uN.externalAddress, uN.symbol);

            }
            printf("\n");
        }

	exit(0);
}

int findDuplicateDefinition (defNodePtr defPtr) {
    defNode    definition;
    defNode    nextDef;
    
    definition = *defPtr;

    printf("^~~%i\n", (*defPtr).next);
    while ((*defPtr).next != NULL) {
        defPtr = definition.next;
        nextDef = *defPtr;

        printf("%s ~~ %s\n", definition.symbol, nextDef.symbol);
        if (!strcmp(definition.symbol, nextDef.symbol)) {
            return 1;
        }
        
        definition = nextDef;
    }

    return 0;
}



