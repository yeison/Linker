#include "tokenizer.h"
#include "utility.h"

int main (int argc, const char *argv[]) {
	module loaded;
        int offset = 0;

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

	int moduleNumber = 0;
	printf("\n%-4s%-15s%-10s\t%s\n", "#", "Module", "Offset", "Use-List");
	//The while-loop below iterates over each module.
	while(buildModuleName(loaded.moduleName)) {
	        loaded.offset = offset;
		printf("%-4i%-15s%-10i\t", moduleNumber, loaded.moduleName, loaded.offset);
	
		buildDefList(loaded.definitionList);
		char symbolsInModule = (char)loaded.definitionList[0];
		/*The for-loop below iterates through the presently loaded 
		 definitionList in order to migrate all of the definitionNodes to the 
		 symbol table.  The currently loaded module will be overwritten at the 
		 end of the outside for-loop.*/
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
	
	printf("\n%s\n","Symbol Table");
	for(int i = 0; i < symbolOffset; i++){
		defNodePtr sym = symbolTable[i];
		printf("\n%s = %d", (*sym).symbol, (*sym).relativeAddress);
	}
	printf("\n\n");
	
        for (int i = 0; i < moduleNumber; i++) {
                
            char programSize = (char)moduleTable[i].programText[0];
            for (int j = 1; j <= programSize; j++) {
                ProgTextPtr progTextPtr = moduleTable[i].programText[j];
                ProgText progText = *progTextPtr;

                char type = progText.type;
                int instruction = progText.instruction;
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
                        new_instruction = instruction;
                        break;
                }

                printf("%c %i -> %i\n", type, instruction, new_instruction);

            }
            printf("\n");
        }

	exit(0);
}

void resolve(ProgText progText) {


}


