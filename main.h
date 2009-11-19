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
#define MODULE_NAME_SIZE 14
#define SYMBOL_SIZE 8
#define MAX_SYMBOLS 5
#define MAX_TOTAL_SYMBOLS 9
#define	MAX_USELIST 4
#define MAX_MODULES 7
#define MAX_INSTRUCTIONS 11

struct ProgText {
	char type;
	int instruction;
};

typedef struct ProgText ProgText;

struct UseNode {
	char *symbol;
	char externalAddress;
};

typedef struct UseNode UseNode;

struct definitionNode{
	char *symbol;
	char relativeAddress;
	struct definitionNode *next;
	char memberOfModule;
};

typedef struct definitionNode *defNodePtr;
typedef struct definitionNode defNode;

struct module{
	char moduleName[MODULE_NAME_SIZE];
	defNodePtr definitionList[MAX_SYMBOLS];
	UseNode *useList[MAX_USELIST];
	ProgText *programText[MAX_INSTRUCTIONS];
	int offset;
};

typedef struct module module;

char getNextToken(char *delimeter, char *buffer, FILE *file);
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