#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1;
#define FALSE 0;

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


/* HACK!! */
unsigned char *string2nick(unsigned char *str)
{
	int i=0;
	unsigned char *result;

	while(str[i] != 0xFE) {
		i++;
	}

	result = malloc(i+1);
	assert(result);

	strncpy(result, str, i);
	result[i]='\0';

	return result;
}

unsigned char *string2name(unsigned char *str)
{
	int start;
	int i=0, j=0;
	unsigned char *result;

	while(str[i] != 0xFE) {
		i++;
	}
	start = i+1;
	i=0;
	while(str[start+i] != 0xFE) {
		i++;
	}
	j=i;
	i++;
	while(str[start+i] != 0xFE) {
		i++;
	}

	result = malloc(i+1);
	assert(result);

	strncpy(result, &str[start], i);
	result[i]='\0';
	result[j]=' ';

	return result;
}


unsigned char *string2email(unsigned char *str)
{
	int start;
	int i=0;
	unsigned char *result;

	while(str[i] != 0xFE) {
		i++;
	}
	start = i+1;
	i=0;
	while(str[start+i] != 0xFE) {
		i++;
	}
	start = start + i + 1;
	i=0;
	while(str[start+i] != 0xFE) {
		i++;
	}
	start = start + i + 1;
	i=0;
	while(str[start+i] != 0xFE) {
		i++;
	}

	result = malloc(i+1);
	assert(result);

	strncpy(result, &str[start], i);
	result[i]='\0';

	return result;
}

