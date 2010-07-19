/*
 *  tokenizer.c
 *  linker
 *
 *  Created by Yeison Rodriguez on 7/13/10.
 *  Copyright 2010 New York University. All rights reserved.
 *
 */

#include "tokenizer.h"

char getNextToken(char *buffer, FILE *file){
	char *blankSpace = "[[:space:]]"; // Regular expression for blank spaces.
	char tokenLength = 0;
	char newStatus = NULL;
	char oldStatus;
	char c;
	regex_t regularExpression; // A regular expression type (regex_t)	
	// Compile the regular expression.
	if(regcomp(&regularExpression, blankSpace, REG_EXTENDED) != 0){
		perror("Check your regexp!");
		exit(1);
	}
	
	/* regexec() returns 0 if the string passed to it contains the sequence specified by the regular expression.  In this case the regular expression is any kind of white space, and the we are passing one character at a time to the regexec function.  Therefore, if the character passed is a white space, the if statement fails and the white space is ignored. */	
	while ((c = getc(file)) != EOF){ 
		//While the next character is not the end of the file.
		
		oldStatus = newStatus;
		//Save the old status; is the previous character a white space?
		
		newStatus = regexec(&regularExpression, &c, 0, NULL, REG_NOTBOL);
		newStatus = regexec(&regularExpression, &c, 0, NULL, REG_NOTBOL);
		//Get the new status; is the current character a white space?
		/* The reason I'm calling the regexec twice, is because there seems to be a bug. When I was testing my code with the Temperature module, the 9th call kept returning a zero, despite the fact that the 9th character is an 'r'.  Calling the function twice seems to circumvent the bug.*/
		
		/* If the current character and the previous character are both white spaces,
		 do nothing.  Continue to read the next character.*/
		if((oldStatus == 0) && (newStatus == 0)){
			continue;
		}
		/* If the new character and the old characters are both none white space, append the new character to the buffer.*/
		else if(newStatus) {
			buffer[tokenLength] = c;
			//strcat(buffer, &c);
			tokenLength++;
		}
		/*  Otherwise, if the current character is a white space but the previous one was not, finish */
		else {
			regfree(&regularExpression);
			buffer[tokenLength] = '\0';
			//strcat(buffer, "\0");
			//printf("Buffer: %s\n", buffer);
			return *buffer;
		}
	}
	
	// Release the memory used by the regular expression compile.
	return 0;
}