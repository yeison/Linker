/*
 *  tokenizer.h
 *  linker
 *
 *  Created by Yeison Rodriguez on 7/13/10.
 *  Copyright 2010 New York University. All rights reserved.
 *
 */

#include <errno.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>
#include <stdio.h>

char getNextToken(char *buffer, FILE *file);