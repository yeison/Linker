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
#include "uthash.h"

//The machine's Memory in Words starting at location 0.
#define MACHINE_SIZE 249
//Largest name for a module is 14 characters.
#define MODULE_NAME_SIZE 14
//Largest symbol is 8 chars.
#define SYMBOL_SIZE 8
//Per module, 5 symbols max.
#define MAX_SYMBOLS 5
//In total, 10 symbols max.
#define MAX_TOTAL_SYMBOLS 10
//Per module, 5 use-list entries max.
#define	MAX_USELIST 5
//7 modules maximum.
#define MAX_MODULES 7
//Per module, 11 instructions max.
#define MAX_INSTRUCTIONS 11

//This structure holds an instruction and its corresponding type.
struct ProgText {
	char type;
	int instruction;
};
typedef struct ProgText *ProgTextPtr;
typedef struct ProgText ProgText;

//This structure holds a use-list symbol, whether it has been used,
//and the external address of this use-list symbol.
struct UseNode {
	char *symbol;
	int externalAddress;
        char used;
};
typedef struct UseNode UseNode;

//Definition Nodes comprise the symbol table.
struct DefinitionNode{
	char *symbol;
	int relativeAddress;
	struct definitionNode *next;
	char memberOfModule;
        char used;
};
typedef struct DefinitionNode *DefNodePtr;
typedef struct DefinitionNode DefNode;

//Module struct carries the other structs.
struct Module{
	char moduleName[MODULE_NAME_SIZE];
	DefNode *definitionList[MAX_SYMBOLS];
	UseNode *useList[MAX_USELIST];
	ProgText *programText[MAX_INSTRUCTIONS];
	int offset;
        int size;
};
typedef struct Module Module;


FILE *inputFile; // The file.
