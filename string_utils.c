#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1;
#define FALSE 0;

unsigned char *get_nth_part(int n, unsigned char *string);

int wordcount(char *string)
{
	int words = 0;
	int in_space = TRUE;

	if(string == NULL) {
		return 0;
	}

	/* Move the pointer one step at the time until
	 * end of string is reached.
	 */
	for(;*string != '\0'; string++) {
		if(in_space && (!isspace(*string))) {
			in_space = FALSE;
			words++;
		} else if(!in_space && isspace(*string)) {
			in_space = TRUE;
		}
	}
	
	return words;
}


unsigned char *string2nick(unsigned char *str)
{
	unsigned char *result;

	result = get_nth_part(0,str);
	if(result == NULL) {
		fprintf(stderr, "Error in data format! (string2nick)\n");
	}

	return result;
}

unsigned char *string2name(unsigned char *str)
{
	unsigned char *result, *strpek;
	unsigned char *firstname, *lastname;

	firstname = get_nth_part(1, str);
	if(firstname == NULL) {
		fprintf(stderr, "Error in data format! (string2name #1)\n");
		return NULL;
	}

	lastname = get_nth_part(2, str);
	if(lastname == NULL) {
		fprintf(stderr, "Error in data format! (string2name #2)\n");
		free(firstname);
		return NULL;
	}

	/* Append first and second name in a string */
	result = malloc(strlen(firstname)+strlen(lastname)+2);
	assert(result);
	strcpy(result, firstname);
	strpek=result+strlen(firstname);
	strpek[0]=' ';
	strpek++;
	strcpy(strpek, lastname);
	
	free(firstname);
	free(lastname);

	return result;
}


unsigned char *string2email(unsigned char *str)
{
	unsigned char *result;

	result = get_nth_part(3,str);
	if(result == NULL) {
		fprintf(stderr, "Error in data format! (string2email)\n");
	}
	return result;
}

/* Returns a string containing the nth part of a string
 * of several parts divided by 0xFE. The first part is
 * n = 0.
 * Returns NULL if there is no nth part. Returns a 
 * pointer to a new string otherwise. Don't forget 
 * to free the new string (with free) when finished 
 * with it.
 */
unsigned char *get_nth_part(int n, unsigned char *string)
{
	int length;
	unsigned char *strpek;
	unsigned char *newstring;

	if(string == NULL) {
		return NULL;
	}

	while(n>0) {
		/* Move forward to the right part. */
		if(*string == '\0') return NULL;
		if(*string == 0xFE) n--;
		string++;
	}

	/* Find length of string including a null to end it. */
	length = 0;
	strpek = string;
	while((*strpek != 0xFE) && (*strpek != '\0')) {
		length++;
		strpek++;
	}
	
	newstring = malloc(length+1);
	assert(newstring);

	strncpy(newstring, string, length);
	newstring[length] = '\0';

	return newstring;
}
