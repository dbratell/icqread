#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "ICQread.h"

#define DEBUG 1

int count = 0;

void readstart(FILE *datafil, int version, struct startfields *sf)
{
	fread(&(sf->uin), 4, 1, datafil);
	count +=4;
	fread(&(sf->length), 2, 1, datafil);
	count +=2;

	if(sf->length>0) {
		sf->string = malloc(sf->length);
		assert(sf->string);
		fread(sf->string, sf->length, 1, datafil);
		count +=sf->length;
	} else {
		sf->string = 0;
	}
}

void readend(FILE *datafil, int version, struct endfields *se)
{
	fread(&(se->junk1), 4, 1, datafil);
	count +=4;

	if(version >= INTRO_V96) {
		fread(&(se->junk2), 1, 1, datafil);
		count +=1;
		fread(&(se->junk3), 4, 1, datafil);
		count +=4;
	} else {
		se->junk2 = 0;
		se->junk3 = 0;
	}
}

/* Handle messages of type 1 (read ICQread.h for more
 * information.
 */
void handle_x01(FILE *datafil, int version)
{
	struct startfields sf;
	struct endfields se;
/*	__int32 uin;
	__int16 length; */

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2;
/*  __int32 junk4, junk6; */
	__int16 junk3;
/*	__int8 junk5; */

/*	char *data_string; 
	fread(&uin, 4, 1, datafil);
	count +=4;
	fread(&length, 2, 1, datafil);
	count +=2;

	data_string = malloc(length);
	assert(data_string);
	fread(data_string, length, 1, datafil);
	count +=length;

  */
	readstart(datafil, version, &sf);

	fread(&junk1, 4, 1, datafil);
	count +=4;
	fread(&junk2, 4, 1, datafil);
	count +=4;
	fread(&junk3, 2, 1, datafil);
	count +=2;

	fread(&date,4,1,datafil);
	count +=4;
	newtime = localtime(&date);

	readend(datafil, version, &se);
/*	fread(&junk4, 4, 1, datafil);
	count +=4;

	if(version >= INTRO_V96) {
		fread(&junk5, 1, 1, datafil);
		count +=1;
		fread(&junk6, 4, 1, datafil);
		count +=4;
	}
*/


	printf("Message X01 (Incoming (outgoing?) message)\nuin: %d,  length = %d\nText: '%s'\n", 
		sf.uin, sf.length,sf.string);

	printf("junk1: %x\n", junk1);
	printf("junk2: %x\n", junk2);
	printf("junk3: %x\n", junk3);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid: %x>", date);
	}
	printf("junk4: %x\n", se.junk1);

	if(version >= INTRO_V96) {
		printf("junk5: %x\n", se.junk2);
		printf("junk6: %x\n", se.junk3);
	}

	printf("\n");

	free(sf.string);

	return;
}




/* Handle messages of type 2 (read ICQread.h for more
 * information.
 */
void handle_x02(FILE *datafil)
{
	struct startfields sf;
	struct endfields se;
	__int32 uin;
	__int16 length1, length2;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2, junk4;
	__int16 junk3;

	char *data_string1, *data_string2;

	

	fread(&uin, 4, 1, datafil);
	count +=4;
	fread(&length1, 2, 1, datafil);
	count +=2;

	data_string1 = malloc(length1);
	assert(data_string1);
	fread(data_string1, length1, 1, datafil);
	count +=length1;

	fread(&junk1, 4, 1, datafil);
	count +=4;
	fread(&junk2, 4, 1, datafil);
	count +=4;
	fread(&junk3, 2, 1, datafil);
	count +=2;

	fread(&date,4,1,datafil);
	count +=4;
	newtime = localtime(&date);

	fread(&length2, 2, 1, datafil);
	count +=2;

	data_string2 = malloc(length2);
	assert(data_string2);
	fread(data_string2, length2, 1, datafil);
	count +=length2;

	fread(&junk4, 4, 1, datafil);
	count +=4;

	printf("Message X02 (Outgoing (Incoming?) chat request)\nuin: %d,  length = %d\nReason given: %s\n", 
		uin, length1,data_string1);

	printf("junk1: %x\n", junk1);
	printf("junk2: %x\n", junk2);
	printf("junk3: %x\n", junk3);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid: %x>", date);
	}
	printf("Other people?\nLength = %d\nString: '%s'\n", 
		length2,data_string2);

	printf("junk4: %x\n", junk4);
	printf("\n");

	free(data_string1);
	free(data_string2);

	return;
}



/* Handle messages of type 3 (read ICQread.h for more
 * information.
 */
void handle_x03(FILE *datafil)
{
	struct startfields sf;
	struct endfields se;
	__int32 uin;
	__int16 length1, length2, length3;

	__int32 date;
	struct tm *newtime;

	__int32 filelength; /* (?) */
	__int32 junk1, junk2, junk4;
	__int16 junk3;

	char *data_string1, *data_string2, *data_string3;
	fread(&uin, 4, 1, datafil);
	count +=4;
	fread(&length1, 2, 1, datafil);
	count +=2;

	data_string1 = malloc(length1);
	assert(data_string1);
	fread(data_string1, length1, 1, datafil);
	count +=length1;

	fread(&junk1, 4, 1, datafil);
	count +=4;
	fread(&junk2, 4, 1, datafil);
	count +=4;
	fread(&junk3, 2, 1, datafil);
	count +=2;

	fread(&date,4,1,datafil);
	count +=4;
	newtime = localtime(&date);

	fread(&length2, 2, 1, datafil);
	count +=2;

	data_string2 = malloc(length2);
	assert(data_string2);
	fread(data_string2, length2, 1, datafil);
	count +=length2;

	fread(&filelength, 4, 1, datafil);
	count +=4;

	newtime = localtime(&date);

	fread(&length3, 2, 1, datafil);
	count +=2;

	if(length3 != 0) {
		data_string3 = malloc(length3);
		assert(data_string3);
		fread(data_string3, length3, 1, datafil);
		count +=length3;
	} else {
		data_string3 = NULL;
	}

	fread(&junk4, 4, 1, datafil);
	count +=4;

	printf("Message X03 (Outgoing (Incoming?) file)\nuin: %d,  length = %d\nDescription: %s\n", 
		uin, length1,data_string1);

	printf("junk1: %x\n", junk1);
	printf("junk2: %x\n", junk2);
	printf("junk3: %x\n", junk3);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid: %x>", date);
	}
	printf("File:\nLength_of_name = %d\nName: '%s'\nSize (?): %d\n", 
		length2,data_string2, filelength);

	if(length3 != 0) {
		printf("Response?:\nLength: %d\nString:'%s'\n", 
			length3,data_string3);
	} else {
		printf("Empty response(?).\n");
	}
	printf("junk4: %x\n", junk4);
	printf("\n");

	free(data_string1);
	free(data_string2);
	if(data_string3) free(data_string3);

	return;
}


/* Handle messages of type 4 (read ICQread.h for more
 * information.
 */
void handle_x04(FILE *datafil)
{
	struct startfields sf;
	struct endfields se;
	__int32 uin;
	__int16 length;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2, junk4;
	__int16 junk3;

	char *data_string;
	fread(&uin, 4, 1, datafil);
	count +=4;
	fread(&length, 2, 1, datafil);
	count +=2;

	data_string = malloc(length);
	assert(data_string);
	fread(data_string, length, 1, datafil);
	count +=length;

	fread(&junk1, 4, 1, datafil);
	count +=4;
	fread(&junk2, 4, 1, datafil);
	count +=4;
	fread(&junk3, 2, 1, datafil);
	count +=2;

	fread(&date,4,1,datafil);
	count +=4;
	newtime = localtime(&date);


	fread(&junk4, 4, 1, datafil);
	count +=4;

	printf("Message X04 (Incoming (outgoing?) URL)\nuin: %d,  length = %d\nDescription and URL: '%s'\n", 
		uin, length,data_string);

	printf("junk1: %x\n", junk1);
	printf("junk2: %x\n", junk2);
	printf("junk3: %x\n", junk3);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid: %x>", date);
	}
	printf("junk4: %x\n", junk4);
	printf("\n");

	free(data_string);

	return;
}



/* Handle messages of type 6 (read ICQread.h for more
 * information.
 */
void handle_x06(FILE *datafil)
{
	struct startfields sf;
	struct endfields se;
	__int32 uin;
	__int16 length;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2, junk4;
	__int16 junk3;

	char *data_string;
	fread(&uin, 4, 1, datafil);
	count +=4;
	fread(&length, 2, 1, datafil);
	count +=2;

	data_string = malloc(length);
	assert(data_string);
	fread(data_string, length, 1, datafil);
	count +=length;

	fread(&junk1, 4, 1, datafil);
	count +=4;
	fread(&junk2, 4, 1, datafil);
	count +=4;
	fread(&junk3, 2, 1, datafil);
	count +=2;

	fread(&date,4,1,datafil);
	count +=4;
	newtime = localtime(&date);


	fread(&junk4, 4, 1, datafil);
	count +=4;

	printf("Message X06(Asked for authorization)\nuin: %d,  length = %d\nString: %s\n", 
		uin, length,data_string);

	printf("junk1: %x\n", junk1);
	printf("junk2: %x\n", junk2);
	printf("junk3: %x\n", junk3);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid: %x>", date);
	}
	printf("junk4: %x\n", junk4);
	printf("\n");

	free(data_string);

	return;
}

/* Handle messages of type 8 (read ICQread.h for more
 * information.
 */
void handle_x08(FILE *datafil)
{
	struct startfields sf;
	struct endfields se;
	__int32 uin;
	__int16 length;
	char *data_string;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2, junk4;
	__int16 junk3;

	fread(&uin, 4, 1, datafil);
	count +=4;
	fread(&length, 2, 1, datafil);
	count +=2;

	data_string = malloc(length);
	assert(data_string);
	fread(data_string, length, 1, datafil);
	count +=length;

	fread(&junk1, 4, 1, datafil);
	count +=4;
	fread(&junk2, 4, 1, datafil);
	count +=4;
	fread(&junk3, 2, 1, datafil);
	count +=2;

	fread(&date,4,1,datafil);
	count +=4;
	newtime = localtime(&date);


	fread(&junk4, 4, 1, datafil);
	count +=4;

	printf("Message X08 (Receipt?)\nuin: %d,  length = %d\nString: %s\n", 
		uin, length,data_string);

	printf("junk1: %x\n", junk1);
	printf("junk2: %x\n", junk2);
	printf("junk3: %x\n", junk3);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid: %x>", date);
	}
	printf("junk4: %x\n", junk4);
	printf("\n");
	
	free(data_string);
	return;
}

/* Handle messages of type 9 (read ICQread.h for more
 * information.
 */
void handle_x09(FILE *datafil)
{
	struct startfields sf;
	struct endfields se;
	__int32 uin;
	__int16 length;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2, junk4;
	__int16 junk3;

	char *data_string;
	fread(&uin, 4, 1, datafil);
	count +=4;
	fread(&length, 2, 1, datafil);
	count +=2;

	data_string = malloc(length);
	assert(data_string);
	fread(data_string, length, 1, datafil);
	count +=length;

	fread(&junk1, 4, 1, datafil);
	count +=4;
	fread(&junk2, 4, 1, datafil);
	count +=4;
	fread(&junk3, 2, 1, datafil);
	count +=2;

	fread(&date,4,1,datafil);
	count +=4;
	newtime = localtime(&date);


	fread(&junk4, 4, 1, datafil);
	count +=4;

	printf("Message X09 (Incoming System message)\nuin: %d,  length = %d\nString: %s\n", 
		uin, length,data_string);

	printf("junk1: %x\n", junk1);
	printf("junk2: %x\n", junk2);
	printf("junk3: %x\n", junk3);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid: %x>", date);
	}
	printf("junk4: %x\n", junk4);
	printf("\n");

	free(data_string);

	return;
}


/* Handle messages of type 10 (read ICQread.h for more
 * information.
 */
void handle_x0A(FILE *datafil)
{
	struct startfields sf;
	struct endfields se;
	__int32 uin;
	__int16 length1, length2;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2, junk4, junk5, junk6;
	__int16 junk3, junk7;

	char *data_string1, *data_string2;
	fread(&uin, 4, 1, datafil);
	count +=4;
	fread(&length1, 2, 1, datafil);
	count +=2;

	data_string1 = malloc(length1);
	assert(data_string1);
	fread(data_string1, length1, 1, datafil);
	count +=length1;

	fread(&junk1, 4, 1, datafil);
	count +=4;
	fread(&junk2, 4, 1, datafil);
	count +=4;
	fread(&junk3, 2, 1, datafil);
	count +=2;

	fread(&date,4,1,datafil);
	count +=4;
	newtime = localtime(&date);

	fread(&length2, 2, 1, datafil);
	count +=2;

	data_string2 = malloc(length2);
	assert(data_string2);
	fread(data_string2, length2, 1, datafil);
	count +=length2;

	fread(&junk4, 4, 1, datafil);
	count +=4;
	fread(&junk5, 4, 1, datafil);
	count +=4;
	fread(&junk6, 4, 1, datafil);
	count +=4;
	fread(&junk7, 2, 1, datafil);
	count +=2;

	printf("Message X0A (External program)\nuin: %d,  length = %d\nReason given: %s\n", 
		uin, length1,data_string1);

	printf("junk1: %x\n", junk1);
	printf("junk2: %x\n", junk2);
	printf("junk3: %x\n", junk3);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid: %x>", date);
	}
	printf("Program:\nLength = %d\nString: '%s'\n", 
		length2,data_string2);

	printf("junk4: %x\n", junk4);
	printf("junk5: %x\n", junk5);
	printf("junk6: %x\n", junk6);
	printf("junk7: %x\n", junk7);
	printf("\n");

	free(data_string1);
	free(data_string2);

	return;
}





/* Handle messages of type 11 (read ICQread.h for more
 * information.
 */
void handle_x0B(FILE *datafil)
{
	struct startfields sf;
	struct endfields se;
	__int32 uin;
	__int16 length;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2, junk4;
	__int16 junk3;

	char *data_string;

	fread(&uin, 4, 1, datafil);
	count +=4;
	fread(&length, 2, 1, datafil);
	count +=2;

	data_string = malloc(length);
	assert(data_string);
	fread(data_string, length, 1, datafil);
	count +=length;

	fread(&junk1, 4, 1, datafil);
	count +=4;
	fread(&junk2, 4, 1, datafil);
	count +=4;
	fread(&junk3, 2, 1, datafil);
	count +=2;

	fread(&date,4,1,datafil);
	count +=4;
	newtime = localtime(&date);

	fread(&junk4, 4, 1, datafil);
	count +=4;

	printf("Message X0B (The user has asked to be added to your Contact List)\nuin: %d,  length = %d\nString: %s\n", 
		uin, length,data_string);

	printf("junk1: %x\n", junk1);
	printf("junk2: %x\n", junk2);
	printf("junk3: %x\n", junk3);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid: %x>", date);
	}
	printf("junk4: %x\n", junk4);
	printf("\n");

	free(data_string);

	return;
}

/* Handle messages of type 12 (read ICQread.h for more
 * information.
 */
void handle_x0C(FILE *datafil)
{
	struct startfields sf;
	struct endfields se;
	__int32 uin;
	__int16 length;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2, junk4;
	__int16 junk3;

	char *data_string;

	fread(&uin, 4, 1, datafil);
	count +=4;
	fread(&length, 2, 1, datafil);
	count +=2;

	data_string = malloc(length);
	assert(data_string);
	fread(data_string, length, 1, datafil);
	count +=length;

	fread(&junk1, 4, 1, datafil);
	count +=4;
	fread(&junk2, 4, 1, datafil);
	count +=4;
	fread(&junk3, 2, 1, datafil);
	count +=2;


	fread(&date,4,1,datafil);
	count +=4;
	newtime = localtime(&date);

	fread(&junk3, 4, 1, datafil);
	count +=4;

	printf("Message X0C (You were added by)\nuin: %d,  length = %d\nString: %s\n", 
		uin, length,data_string);

	printf("junk1: %x\n", junk1);
	printf("junk2: %x\n", junk2);
	printf("junk3: %x\n", junk3);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid: %x>", date);
	}
	printf("junk4: %x\n", junk4);
	printf("\n");

	free(data_string);

	return;
}


void read_and_print_message(FILE *datafil)
{
	struct tm *newtime;

	int time;
	int junk1;
	int junk2;
	int uin;
	short length;
	char *buffer;

	/* Read time */
	fread(&time,4,1,datafil);
	count +=4;
	newtime = localtime(&time);

	fread(&junk1, 4, 1, datafil);
	count +=4;
	fread(&junk2, 4, 1, datafil);
	count +=4;
	fread(&uin, 4, 1, datafil);
	count +=4;
	fread(&length, 2, 1, datafil);
	count +=2;

	buffer = malloc(length);
	assert(buffer);

	fread(buffer, length, 1, datafil);
	count +=length;

	printf("tid: %s", asctime(newtime));
	printf("uin: %d\n", uin);
	printf("Meddelande:\n%s\n", buffer);

	free(buffer);
}

int main(int argc, char *argv[])
{
	char buf[4];
	__int32 *data32;
	__int16 *data16;

	int type, version;

	int go_on = TRUE;

	FILE *datafil;

	buf[0]=buf[1]=buf[2]=buf[3]=0;

	data32 = (__int32 *)&buf[0];
	data16 = (__int16 *)&buf[2];

	if(argc!=2) {
		printf("icqread <filnamn>\n");
		exit(1);
	}

	/* Open datafile in readonly binary mode. */
	datafil = fopen(argv[1], "rb");
	if(!datafil) {
		perror("Datafil");
		exit(1);
	}

#ifdef DEBUG
	printf("Öppnade datafil\n");
#endif


	while(go_on) {
		printf("Next...\n");
		buf[0]=buf[2];
		buf[1]=buf[3];
		fread(&buf[2], 2, 1, datafil);
		count += 2;

		/* Assuming all versions has the same 
		 * format */
		while((*data16 != INTRO_V72) && 
			(*data16 != INTRO_V73) && 
			(*data16 != INTRO_V74) && 
			(*data16 != INTRO_V96)) {
			printf("Searching for start\n");
			buf[0]=buf[2];
			buf[1]=buf[3];
			fread(&buf[2], 2, 1, datafil);
			count += 2;
		}

		version = *data16;

		buf[0]=buf[2];
		buf[1]=buf[3];
		fread(&buf[2], 2, 1, datafil);
		count += 2;

		type = *data16;

		switch(type) {
		case TYPE_X01:
			handle_x01(datafil, version);
			break;

		case TYPE_X02:
			handle_x02(datafil);
			break;

		case TYPE_X03:
			handle_x03(datafil);
			break;

		case TYPE_X04:
			handle_x04(datafil);
			break;

		case TYPE_X06:
			handle_x06(datafil);
			break;

		case TYPE_X08:
			handle_x08(datafil);
			break;

		case TYPE_X09:
			handle_x09(datafil);
			break;

		case TYPE_X0A:
			handle_x0A(datafil);
			break;

		case TYPE_X0B:
			handle_x0B(datafil);
			break;

		case TYPE_X0C:
			handle_x0C(datafil);
			break;

		default:
			printf("Unknown type: %d\n", type);
			go_on = FALSE;
			break;
		}
	}

	return 0;

	// Now we have read the intro
	// Let's read what's coming up.



	while(!feof(datafil))
	{
		buf[0]=buf[1];
		buf[1]=buf[2];
		buf[2]=buf[3];

		fread(&buf[3], 1, 1, datafil);
		count++;

#ifdef DEBUG
		printf("%d: data16: %04hx,\t data32 %08x\n", count, *data16, *data32);
/*		fflush(stdout); */
		
		if(*data32==-1) {
			printf("pip!\n");
		}
#endif
		if(count>=4) {
			if((*data32)==0x00720000) {
				/* Message starting */
				read_and_print_message(datafil);
				/* Reset buffer */
				fread(&buf[1], 3, 1, datafil);
				count +=3;
			}
		}
	}


	return 0;
}