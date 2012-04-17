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
void buildDefList(DefNodePtr symbolTable[], char moduleNumber);
struct DefinitionNode getDefinition();
DefNodePtr dalloc(void);
DefNode getDefNode(char nodeNumber, DefNodePtr p);
//void toSymbolTable(defNodePtr p, char offSet);
void toSymbolTable(DefNodePtr symbolNodePointer);
unsigned long hash(unsigned char *str);
