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

#define MAXMEM 500
#define DEF_LIST 1
#define USE_LIST 2
#define PROGRAM_TEXT 3
#define MODULE_NAME_SIZE 32
#define SYMBOL_SIZE 8
#define MAX_SYMBOLS 4

struct module{
	char moduleName[MODULE_NAME_SIZE];
	struct definitionNode *definitionList;
	char *useList;
	char *programText;
	char offset;
};

struct definitionNode{
	char *symbol[SYMBOL_SIZE];
	char relativeAddress;
	struct definitionNode *next;
	char memberOfModule;
};

struct sNode{
	char *symbol[SYMBOL_SIZE];
	short address;
	char memberOfModule;
};

typedef struct module module;
typedef struct sNode sNode;
typedef struct definitionNode *defNodePtr;
typedef struct definitionNode defNode;

char getNextToken(char *delimeter, char *buffer, FILE *file);
char loadModule();
char buildModuleName(char *moduleNamePointer);
void buildUseList(int *useListPointer, int useQuantity);
void buildProgramText(int *progTextPointer, int instructionQuantity);
defNodePtr buildDefList(defNodePtr defNodeP);
struct definitionNode getDefinition();
defNodePtr *dalloc(void);
sNode addSymbol(defNode d, char offSet);
defNode getDefNode(char nodeNumber, defNodePtr p);
sNode makeSymbolNode(char *symbol, char relativeAddress);
//void toSymbolTable(defNodePtr p, char offSet);
void toSymbolTable(defNodePtr symbolNodePointer);
sNode *sNalloc(void);