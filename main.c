#include "tokenizer.h"
#include "utility.h"

int main (int argc, const char *argv[]) {
    Module loaded;
    int offset = 0;

    /*SymbolTable below is a permanent structure to hold symbols and their 
    absolute addresses.*/
    DefNode *symbolTable[MAX_TOTAL_SYMBOLS];
    Module moduleTable[MAX_MODULES];
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
	
        buildDefList(loaded.definitionList, moduleNumber);
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

    if (symbolOffset > MAX_TOTAL_SYMBOLS)
        perror("MAX_TOTAL_SYMBOLS value exceeded.\n");
	
    fclose(inputFile);

    /* ----- Create from Symbol Table a Linked List ---- */

    for(int i = 0; i < symbolOffset - 1 ; i++) {
        DefNodePtr sym_i = symbolTable[i];
            (*sym_i).next = symbolTable[i+1];
    }

    /* ----- Print Symbol Table ----- */
	
    printf("\n%s\n","Symbol Table");

    //The loop below creates a linked list from the symbolTable.
    //Multiply-defined symbols are ommitted from the linked list.
    DefNodePtr sym_i = symbolTable[0];
    while (sym_i != NULL ){
        if ( findRemoveDuplicateDefinition(sym_i) ) {
            printf("%s = %d, Error: This symbol is multiply defined; first value used.\n", \
                     (*sym_i).symbol, (*sym_i).relativeAddress);
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

        /* Go through this module's use-list. Compare the use-list
           symbol to symbol table.  Determine external address. 
           Set the used flag in symbol table to indicate symbol was used.*/
        char useListSize = (char)moduleTable[i].useList[0];
        for ( int j = 1; j <= useListSize; j++ ) {
            UseNode *useNodePtr = moduleTable[i].useList[j];
            //A Hash Table would be nicer here.  We can address that later.
            for (int k = 0; k < symbolOffset; k++) {
                DefNodePtr sym = symbolTable[k];
                   
                if(!strcmp((*useNodePtr).symbol, (*sym).symbol)) {
 
                    if((*useNodePtr).externalAddress != 0){
                        //Symbol already defined, continue.
                        continue;
                    } else {
                        //Save a ptr to this matching useNode.  We can check later if
                        //this symbol definition was used or not by E type reference.
                        (*useNodePtr).externalAddress = (*sym).relativeAddress;
                        (*sym).used = 1;
                    }
                }
            }
        }

 
        /* Now traverse the Instructions for this module and carry out the 
           appropriate instruction based on cases below. */
                
        //Instruction quantity is stored in the first index.
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
            int new_instruction = instruction;
            char *addressError = NULL;

            sprintf(instructionStr, "%d", instruction);
            int instructionSuffix = atoi(&instructionStr[1]);
            //ASCII conversion
            int instructionPrefix = instructionStr[0] - 48;

            switch(type) {

            //Immediate instruction
            case 'I':
                new_instruction = instruction;
                break;

            //Absolute instruction
            case 'A':
                if(instructionSuffix > MACHINE_SIZE){
                    new_instruction = instructionPrefix*1000 + 0; 
                    addressError = "Error: Absolute address exceeds machine size; zero used.";
                } else {
                    new_instruction = instruction;
                }
                break;

            //Relative instruction
            case 'R':
                if(instructionSuffix > moduleTable[i].size){
                    new_instruction = instructionPrefix*1000 + 0; 
                    addressError = "Error: Relative address exceeds module size; zero used.";
                } else {
                    new_instruction = instruction + moduleTable[i].offset;
                }
                break;

            //External instruction
            case 'E':
                if(instructionSuffix < useListSize){
                    useNodePtr = moduleTable[i].useList[instructionSuffix + 1];
                    useNode = *useNodePtr;
                    externalAddress = useNode.externalAddress;

                    if(externalAddress == 0){
                        addressError = "Error: Symbol used but not defined; zero used.";
                    }
                        
                    (*useNodePtr).used = 1;
                    new_instruction = instructionPrefix*1000 + externalAddress;

                } else {
                    addressError =  "Error: External address exceeds length of use list; treated as immediate.";
                }
                break;
            }

            if(addressError == NULL){
                printf("%i: %c %i -> %i \n", memoryMapCounter, type, instruction,\
                       new_instruction );
            } else {
                printf("%i: %c %i -> %i; %s\n", memoryMapCounter, type, instruction,\
                       new_instruction, addressError );
            }
            memoryMapCounter++;
        }

        /* Traverse use-list for this module to check if symbol was utilized.*/
        for(int j = 1; j <= useListSize; j++){
            UseNode *useNodePtr = moduleTable[i].useList[j];
            if(!(*useNodePtr).used){
                char warning[100];
                sprintf(warning, "Warning: In module %i (%s) %s appeared in the use-list but was not actually used.\n", i+1, moduleTable[i].moduleName, (*useNodePtr).symbol );
                strcat(warningString, warning);
            }
        }

        printf("\n");
    }

    /* Traverse symbol table to check if definition was utilized.*/
    DefNodePtr sym_j = symbolTable[0];
    while (sym_j != NULL ){

        if(!(*sym_j).used){
            char mom = (*sym_j).memberOfModule;
            printf("Warning: %s was defined in module %i (%s) but never used.\n",\
                   (*sym_j).symbol, mom + 1, moduleTable[mom].moduleName);
        }

        sym_j = (*sym_j).next;
    }
 
    /* Print Warning strings that were not printed in-line.*/
    if(warningString != NULL)
        printf("%s", warningString);
    if(errorString != NULL)
        printf("%s", errorString);

    exit(0);
}

/* Pass header of Symbol Table linked-list. Function removes duplicates and returns 1
   if duplicates are found. */
int findRemoveDuplicateDefinition (DefNodePtr defPtr) {
    int  returnValue = 0;
    DefNode    definition;
    DefNode    nextDef;
    
    definition = *defPtr;

    while (definition.next != NULL) {
        DefNodePtr nextDefPtr = definition.next;
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



