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
}

void readv96data(FILE *datafil, int version, struct v96data *vd)
{
	if(version >= INTRO_V96) {
		fread(&(vd->junk1), 1, 1, datafil);
		count +=1;
		fread(&(vd->junk2), 4, 1, datafil);
		count +=4;
	} else {
		vd->junk1 = 0;
		vd->junk2 = 0;
	}
}


/* Handle messages of type 1 (read ICQread.h for more
 * information.
 */
void handle_x01(FILE *datafil, int version)
{
	struct startfields sf;
	struct endfields se;
	struct v96data vd;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2;
	__int16 junk3;

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
	readv96data(datafil, version, &vd);

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

	printf("\n");

	free(sf.string);

	return;
}




/* Handle messages of type 2 (read ICQread.h for more
 * information.
 */
void handle_x02(FILE *datafil, int version)
{
	struct startfields sf;
	struct endfields se;
	struct v96data vd;

	__int16 length2;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2;
	__int16 junk3;

	char *data_string2;

	readstart(datafil, version, &sf);

	fread(&junk1, 4, 1, datafil);
	count +=4;
	fread(&junk2, 4, 1, datafil);
	count +=4;
	fread(&junk3, 2, 1, datafil);
	count +=2;

	fread(&date,4,1,datafil);
	readv96data(datafil, version, &vd);
	count +=4;
	newtime = localtime(&date);

	fread(&length2, 2, 1, datafil);
	count +=2;

	data_string2 = malloc(length2);
	assert(data_string2);
	fread(data_string2, length2, 1, datafil);
	count +=length2;

	readend(datafil, version, &se);

	printf("Message X02 (Outgoing (Incoming?) chat request)\nuin: %d,  length = %d\nReason given: %s\n", 
		sf.uin, sf.length,sf.string);

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

	printf("\n");

	free(sf.string);
	free(data_string2);

	return;
}



/* Handle messages of type 3 (read ICQread.h for more
 * information.
 */
void handle_x03(FILE *datafil, int version)
{
	struct startfields sf;
	struct endfields se; 
	struct v96data vd;

	__int16 length2, length3;

	__int32 date;
	struct tm *newtime;

	__int32 filelength; /* (?) */
	__int32 junk1, junk2;
	__int16 junk3;

	char *data_string2, *data_string3;

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

	readv96data(datafil, version, &vd);

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

	readend(datafil, version, &se);

	printf("Message X03 (Outgoing (Incoming?) file)\nuin: %d,  length = %d\nDescription: %s\n", 
		sf.uin, sf.length,sf.string);

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
	printf("\n");

	free(sf.string);
	free(data_string2);
	if(data_string3) free(data_string3);

	return;
}


/* Handle messages of type 4 (read ICQread.h for more
 * information.
 */
void handle_x04(FILE *datafil, int version)
{
	struct startfields sf;
	struct endfields se;
	struct v96data vd;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2;
	__int16 junk3;

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
	readv96data(datafil, version, &vd);

	printf("Message X04 (Incoming (outgoing?) URL)\nuin: %d,  length = %d\nDescription and URL: '%s'\n", 
		sf.uin, sf.length, sf.string);

	printf("junk1: %x\n", junk1);
	printf("junk2: %x\n", junk2);
	printf("junk3: %x\n", junk3);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid: %x>", date);
	}
	printf("\n");

	free(sf.string);

	return;
}



/* Handle messages of type 6 (read ICQread.h for more
 * information.
 */
void handle_x06(FILE *datafil, int version)
{
	struct startfields sf;
	struct endfields se;
	struct v96data vd;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2;
	__int16 junk3;

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
	readv96data(datafil, version, &vd);

	printf("Message X06(Asked for authorization)\nuin: %d,  length = %d\nString: %s\n", 
		sf.uin, sf.length, sf.string);

	printf("junk1: %x\n", junk1);
	printf("junk2: %x\n", junk2);
	printf("junk3: %x\n", junk3);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid: %x>", date);
	}
	printf("\n");

	free(sf.string);

	return;
}

/* Handle messages of type 8 (read ICQread.h for more
 * information.
 */
void handle_x08(FILE *datafil, int version)
{
	struct startfields sf;
	struct endfields se;
	struct v96data vd;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2;
	__int16 junk3;

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
	readv96data(datafil, version, &vd);

	printf("Message X08 (Receipt?)\nuin: %d,  length = %d\nString: %s\n", 
		sf.uin, sf.length, sf.string);

	printf("junk1: %x\n", junk1);
	printf("junk2: %x\n", junk2);
	printf("junk3: %x\n", junk3);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid: %x>", date);
	}
	printf("\n");
	
	free(sf.string);
	return;
}

/* Handle messages of type 9 (read ICQread.h for more
 * information.
 */
void handle_x09(FILE *datafil, int version)
{
	struct startfields sf;
	struct endfields se;
	struct v96data vd;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2;
	__int16 junk3;

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
	readv96data(datafil, version, &vd);

	printf("Message X09 (Incoming System message)\nuin: %d,  length = %d\nString: %s\n", 
		sf.uin, sf.length, sf.string);

	printf("junk1: %x\n", junk1);
	printf("junk2: %x\n", junk2);
	printf("junk3: %x\n", junk3);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid: %x>", date);
	}
	printf("\n");

	free(sf.string);

	return;
}


/* Handle messages of type 10 (read ICQread.h for more
 * information.
 */
void handle_x0A(FILE *datafil, int version)
{
	struct startfields sf;
	struct endfields se;
	struct v96data vd;

	__int16 length2;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2, junk4, junk5;
	__int16 junk3, junk6;


	char *data_string2;

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

	readv96data(datafil, version, &vd);

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
	fread(&junk6, 2, 1, datafil);
	count +=2;

	readend(datafil, version, &se);

	printf("Message X0A (External program)\nuin: %d,  length = %d\nReason given: %s\n", 
		sf.uin, sf.length, sf.string);

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
	printf("\n");

	free(sf.string);
	free(data_string2);

	return;
}





/* Handle messages of type 11 (read ICQread.h for more
 * information.
 */
void handle_x0B(FILE *datafil, int version)
{
	struct startfields sf;
	struct endfields se;
	struct v96data vd;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2;
	__int16 junk3;

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
	readv96data(datafil, version, &vd);

	printf("Message X0B (The user has asked to be added to your Contact List)\nuin: %d,  length = %d\nString: %s\n", 
		sf.uin, sf.length,sf.string);

	printf("junk1: %x\n", junk1);
	printf("junk2: %x\n", junk2);
	printf("junk3: %x\n", junk3);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid: %x>", date);
	}
	printf("\n");

	free(sf.string);

	return;
}

/* Handle messages of type 12 (read ICQread.h for more
 * information.
 */
void handle_x0C(FILE *datafil, int version)
{
	struct startfields sf;
	struct endfields se;
	struct v96data vd;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2;
	__int16 junk3;

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
	readv96data(datafil, version, &vd);

	printf("Message X0C (You were added by)\nuin: %d,  length = %d\nString: %s\n", 
		sf.uin, sf.length,sf.string);

	printf("junk1: %x\n", junk1);
	printf("junk2: %x\n", junk2);
	printf("junk3: %x\n", junk3);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid: %x>", date);
	}
	printf("\n");

	free(sf.string);

	return;
}


/* Handle messages of type 19 (read ICQread.h for more
 * information.
 */
void handle_x13(FILE *datafil, int version)
{
	struct startfields sf;
	struct endfields se;
	struct v96data vd;

	__int32 date;
	struct tm *newtime;

	__int32 junk1, junk2;
	__int16 junk3;

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

	readv96data(datafil, version, &vd);
	readend(datafil, version, &se);

	printf("Message X13 (Contactlist)\nuin: %d,  length = %d\nString: %s\n", 
		sf.uin, sf.length,sf.string);

	printf("junk1: %x\n", junk1);
	printf("junk2: %x\n", junk2);
	printf("junk3: %x\n", junk3);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid: %x>", date);
	}
	printf("\n");

	free(sf.string);

	return;
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
			(*data16 != INTRO_V96) && 
			(*data16 != INTRO_V98) &&
			(!feof(datafil))) {
			printf("Searching for start\n");
			buf[0]=buf[2];
			buf[1]=buf[3];
			fread(&buf[2], 2, 1, datafil);
			count += 2;
		}

		if(feof(datafil)) return 0;

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
			handle_x02(datafil, version);
			break;

		case TYPE_X03:
			handle_x03(datafil, version);
			break;

		case TYPE_X04:
			handle_x04(datafil, version);
			break;

		case TYPE_X06:
			handle_x06(datafil, version);
			break;

		case TYPE_X08:
			handle_x08(datafil, version);
			break;

		case TYPE_X09:
			handle_x09(datafil, version);
			break;

		case TYPE_X0A:
			handle_x0A(datafil, version);
			break;

		case TYPE_X0B:
			handle_x0B(datafil, version);
			break;

		case TYPE_X0C:
			handle_x0C(datafil, version);
			break;

		case TYPE_X13:
			handle_x13(datafil, version);
			break;

		default:
			printf("Unknown type: %d\n", type);
			go_on = FALSE;
			break;
		}
	}

	return 0;


}