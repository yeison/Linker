/*
 *  utility.h
 *  linker
 *
 *  Created by Yeison Rodriguez on 7/19/10.
 *
 */
#include "main.h"

char loadModule();
char buildModuleName(char *moduleNamePointer);
void buildUseList(UseNode *useListArray[]);
char buildProgramText(ProgText *progTextArray[]);
void buildDefList(defNodePtr symbolTable[]);
struct definitionNode getDefinition();
defNodePtr dalloc(void);
defNode getDefNode(char nodeNumber, defNodePtr p);
//void toSymbolTable(defNodePtr p, char offSet);
void toSymbolTable(defNodePtr symbolNodePointer);
unsigned long hash(unsigned char *str);
