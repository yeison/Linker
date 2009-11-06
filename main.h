/*
 *  main.h
 *  Lab1
 *
 *  Created by Yeison Rodriguez on 9/28/09.
 *  Copyright 2009 New York University. All rights reserved.
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>

#define MAXMEM 500
#define DEF_LIST 1
#define USE_LIST 2
#define PROGRAM_TEXT 3
#define MODULE_NAME_SIZE 32
#define SYMBOL_SIZE 8
#define MAX_SYMBOLS 4
#define	MAX_USELIST 3
#define MAX_INSTRUCTIONS 10

struct module{
	char moduleName[MODULE_NAME_SIZE];
	struct definitionNode *definitionList;
	char *useList[MAX_USELIST];
	char *programText[MAX_INSTRUCTIONS];
	char offset;
};

struct definitionNode{
	char *symbol[SYMBOL_SIZE];
	char relativeAddress;
	struct definitionNode *next;
	char memberOfModule;
};

struct ProgText {
	char type;
	int instruction;
};


typedef struct module module;
typedef struct definitionNode *defNodePtr;
typedef struct definitionNode defNode;
typedef struct ProgText ProgText;

char getNextToken(char *delimeter, char *buffer, FILE *file);
char loadModule();
char buildModuleName(char *moduleNamePointer);
void buildUseList(char *useListArray[]);
void buildProgramText(ProgText *progTextArray[]);
defNodePtr buildDefList(defNodePtr symbolTable[]);
struct definitionNode getDefinition();
defNodePtr *dalloc(void);
defNode getDefNode(char nodeNumber, defNodePtr p);
//void toSymbolTable(defNodePtr p, char offSet);
void toSymbolTable(defNodePtr symbolNodePointer);