#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "ICQread.h"

#define DEBUG 1

int count = 0;

/* Handle messages of type 1 (read ICQread.h for more
 * information.
 */
void handle_x01(FILE *datafil)
{
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

	printf("Message X01(Incoming (outgoing?) message)\nuin: %d,  length = %d\nString: %s\n", 
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

	printf("Message X09 (Incoming message)\nuin: %d,  length = %d\nString: %s\n", 
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



/* Handle messages of type 11 (read ICQread.h for more
 * information.
 */
void handle_x0B(FILE *datafil)
{
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

	int type;

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

		while(*data16 != INTRO_V2) {
			printf("Searching for start\n");
			buf[0]=buf[2];
			buf[1]=buf[3];
			fread(&buf[2], 2, 1, datafil);
			count += 2;
		}

		buf[0]=buf[2];
		buf[1]=buf[3];
		fread(&buf[2], 2, 1, datafil);
		count += 2;

		type = *data16;

		switch(type) {
		case TYPE_X01:
			handle_x01(datafil);
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