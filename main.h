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
#define MODULE_NAME 0
#define DEF_LIST 1
#define USE_LIST 2
#define PROGRAM_TEXT 3

void fileTokenizer(char *delimiter, FILE *file);
int getNextToken(FILE *file);
char packModule(int *stringPointer, char round);
char buildDefList(int *defListPointer, int defQuantity);
void buildUseList(int *useListPointer, int useQuantity);
void buildProgramText(int *progTextPointer, int instructionQuantity);

int *moduleName;
int *definitionList;