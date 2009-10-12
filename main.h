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

typedef struct definitionNode *defNodePtr;
typedef struct definitionNode defNode;

char getNextToken(char *delimeter, char *buffer, FILE *file);
char loadModule();
char buildModuleName(char *moduleNamePointer);
defNodePtr buildDefList(defNodePtr defNodeP);
struct definitionNode getDefinition();
void buildUseList(int *useListPointer, int useQuantity);
void buildProgramText(int *progTextPointer, int instructionQuantity);
defNodePtr *dalloc(void);