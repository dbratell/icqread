#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "ICQread.h"
#include "people.h"

int count = 0;

struct tm *readdate(FILE *datafil, int version)
{
	__int32 date;

	fread(&date,4,1,datafil);
	count +=4;
	return localtime(&date);

}

void readstring(FILE *datafil, int version, struct textdata *td)
{
	fread(&(td->length), 2, 1, datafil);
	count +=2;

	if(td->length>0) {
		td->string = malloc(td->length);
		assert(td->string);
		fread(td->string, td->length, 1, datafil);
		count +=td->length;
	} else {
		td->string = 0;
	}
	
}


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

void readinfo(FILE *datafil, int version, struct infofields *info)
{
	fread(&(info->junk1), 4, 1, datafil);
	count +=4;
	fread(&(info->destination), 4, 1, datafil);
	count +=4;
	fread(&(info->protocolversion), 2, 1, datafil);
	count +=2;
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
	struct infofields info;
	struct v96data vd;

	struct tm *newtime;

	struct people *p;
	char *name;

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);
	newtime = readdate(datafil, version);

	readend(datafil, version, &se);
	readv96data(datafil, version, &vd);

	p = people_lookup(sf.uin);
	if(p == NULL) {
		name = "<unknown>";
	} else {
		name = p->name;
	}

	if(info.destination == 1) {
		printf("Message sent to: %s\n", 
			name);
	} else if(info.destination == 0) {
		printf("Message arrived from %s:\n", name);
	} else {
		printf("Message (X01) with unknown destination: %x\n");
	}

	printf("uin: %d,  length = %d\nText: '%s'\n", 
		sf.uin, sf.length,sf.string);

	printf("junk1: %x\n", info.junk1);
	printf("protocol: %x\n", info.protocolversion);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid>");
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
	struct infofields info;
	struct v96data vd;
	struct textdata people;

	/* __int16 length2; */

	struct tm *newtime;

/*	char *data_string2; */

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);

	newtime = readdate(datafil, version);
	readv96data(datafil, version, &vd);

	readstring(datafil, version, &people);

	readend(datafil, version, &se);

	printf("Message X02 (Outgoing (Incoming?) chat request)\nuin: %d,  length = %d\nReason given: %s\n", 
		sf.uin, sf.length,sf.string);

	printf("junk1: %x\n", info.junk1);
	printf("destination: %x\n", info.destination);
	printf("protocolversion: %x\n", info.protocolversion);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid>");
	}
	printf("Other people?\nLength = %d\nString: '%s'\n", 
		people.length,people.string);

	printf("\n");

	free(sf.string);
	free(people.string);
/*	free(data_string2); */

	return;
}



/* Handle messages of type 3 (read ICQread.h for more
 * information.
 */
void handle_x03(FILE *datafil, int version)
{
	struct startfields sf;
	struct endfields se; 
	struct infofields info;
	struct textdata filename;
	struct textdata junkstring;
	struct v96data vd;

	struct tm *newtime;

	__int32 filelength;

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);

	newtime = readdate(datafil, version);

	readv96data(datafil, version, &vd);

	readstring(datafil, version, &filename);


	fread(&filelength, 4, 1, datafil);
	count +=4;

	readstring(datafil, version, &junkstring);

	readend(datafil, version, &se);

	printf("Message X03 (Outgoing (Incoming?) file)\nuin: %d,  length = %d\nDescription: %s\n", 
		sf.uin, sf.length,sf.string);

	printf("junk1: %x\n", info.junk1);
	printf("info.destination: %x\n", info.destination);
	printf("protocolversion: %x\n", info.protocolversion);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid>");
	}
	printf("File:\nLength_of_name = %d\nName: '%s'\nSize : %d bytes\n", 
		filename.length, filename.string, filelength);

	if(junkstring.length != 0) {
		printf("Response?:\nLength: %d\nString:'%s'\n", 
			junkstring.length, junkstring.string);
	} else {
		printf("Empty response(?).\n");
	}
	printf("\n");

	free(sf.string);
	free(filename.string);
	if(junkstring.string) free(junkstring.string);

	return;
}


/* Handle messages of type 4 (read ICQread.h for more
 * information.
 */
void handle_x04(FILE *datafil, int version)
{
	struct startfields sf;
	struct endfields se;
	struct infofields info;
	struct v96data vd;

	struct tm *newtime;

	readstart(datafil, version, &sf);
	
	readinfo(datafil, version, &info);

	newtime = readdate(datafil, version);

	readend(datafil, version, &se);
	readv96data(datafil, version, &vd);

	printf("Message X04 (Incoming (outgoing?) URL)\nuin: %d,  length = %d\nDescription and URL: '%s'\n", 
		sf.uin, sf.length, sf.string);

	printf("junk1: %x\n", info.junk1);
	printf("info.destination: %x\n", info.destination);
	printf("protocolversion: %x\n", info.protocolversion);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid>");
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
	struct infofields info;
	struct v96data vd;

	struct tm *newtime;

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);

	newtime = readdate(datafil, version);

	readend(datafil, version, &se);
	readv96data(datafil, version, &vd);

	printf("Message X06(Asked for authorization)\nuin: %d,  length = %d\nString: %s\n", 
		sf.uin, sf.length, sf.string);

	printf("junk1: %x\n", info.junk1);
	printf("info.destination: %x\n", info.destination);
	printf("protocolversion: %x\n", info.protocolversion);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid>");
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
	struct infofields info;
	struct v96data vd;

	struct tm *newtime;

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);

	newtime = readdate(datafil, version);

	readend(datafil, version, &se);
	readv96data(datafil, version, &vd);

	printf("Message X08 (Receipt?)\nuin: %d,  length = %d\nString: %s\n", 
		sf.uin, sf.length, sf.string);

	printf("junk1: %x\n", info.junk1);
	printf("info.destination: %x\n", info.destination);
	printf("protocolversion: %x\n", info.protocolversion);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid>");
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
	struct infofields info;
	struct v96data vd;

	struct tm *newtime;

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);

	newtime = readdate(datafil, version);

	readend(datafil, version, &se);
	readv96data(datafil, version, &vd);

	printf("Message X09 (Incoming System message)\nuin: %d,  length = %d\nString: %s\n", 
		sf.uin, sf.length, sf.string);

	printf("junk1: %x\n", info.junk1);
	printf("info.destination: %x\n", info.destination);
	printf("protocolversion: %x\n", info.protocolversion);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid>");
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
	struct infofields info;
	struct textdata programname;
	struct v96data vd;

	struct tm *newtime;

	__int32 junk4, junk5;
	__int16 junk6;


	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);

	newtime = readdate(datafil, version);

	readv96data(datafil, version, &vd);

	readstring(datafil, version, &programname);

	fread(&junk4, 4, 1, datafil);
	count +=4;
	fread(&junk5, 4, 1, datafil);
	count +=4;
	fread(&junk6, 2, 1, datafil);
	count +=2;

	readend(datafil, version, &se);

	printf("Message X0A (External program)\nuin: %d,  length = %d\nReason given: %s\n", 
		sf.uin, sf.length, sf.string);

	printf("junk1: %x\n", info.junk1);
	printf("info.destination: %x\n", info.destination);
	printf("protocolversion: %x\n", info.protocolversion);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid>");
	}
	printf("Program:\nLength = %d\nString: '%s'\n", 
		programname.length,programname.string);

	printf("junk4: %x\n", junk4);
	printf("junk5: %x\n", junk5);
	printf("junk6: %x\n", junk6);
	printf("\n");

	free(sf.string);
	free(programname.string);

	return;
}





/* Handle messages of type 11 (read ICQread.h for more
 * information.
 */
void handle_x0B(FILE *datafil, int version)
{
	struct startfields sf;
	struct endfields se;
	struct infofields info;
	struct v96data vd;

	struct tm *newtime;

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);

	newtime = readdate(datafil, version);

	readend(datafil, version, &se);
	readv96data(datafil, version, &vd);

	people_add(sf.uin, sf.string);

	printf("Message X0B (The user has asked to be added to your Contact List)\nuin: %d,  length = %d\nString: %s\n", 
		sf.uin, sf.length,sf.string);

	printf("junk1: %x\n", info.junk1);
	printf("info.destination: %x\n", info.destination);
	printf("protocolversion: %x\n", info.protocolversion);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid>");
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
	struct infofields info;
	struct v96data vd;

	struct tm *newtime;

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);


	newtime = readdate(datafil, version);

	readend(datafil, version, &se);
	readv96data(datafil, version, &vd);

	people_add(sf.uin, sf.string);

	printf("Message X0C (You were added by)\nuin: %d,  length = %d\nString: %s\n", 
		sf.uin, sf.length,sf.string);

	printf("junk1: %x\n", info.junk1);
	printf("info.destination: %x\n", info.destination);
	printf("protocolversion: %x\n", info.protocolversion);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid>");
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
	struct infofields info;
	struct v96data vd;

	struct tm *newtime;

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);

	newtime = readdate(datafil, version);

	readv96data(datafil, version, &vd);
	readend(datafil, version, &se);

	printf("Message X13 (Contactlist)\nuin: %d,  length = %d\nString: %s\n", 
		sf.uin, sf.length,sf.string);

	printf("junk1: %x\n", info.junk1);
	printf("info.destination: %x\n", info.destination);
	printf("protocolversion: %x\n", info.protocolversion);
	if(newtime != NULL) {
		printf("tid: %s", asctime(newtime));
	} else {
		printf("tid: <skum tid>");
	}
	printf("\n");

	free(sf.string);

	return;
}

void readfile(FILE *datafil) 
{
	char buf[4];
	__int32 *data32;
	__int16 *data16;

	int type, version;

	int go_on = TRUE;

	buf[0]=buf[1]=buf[2]=buf[3]=0;

	data32 = (__int32 *)&buf[0];
	data16 = (__int16 *)&buf[2];

	while(go_on) {
		printf("Next post...\n");
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
			printf("Couldn't find header. Searching...\n");
			buf[0]=buf[2];
			buf[1]=buf[3];
			fread(&buf[2], 2, 1, datafil);
			count += 2;
		}

		if(feof(datafil)) return;

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

	return;

}


int main(int argc, char *argv[])
{

	FILE *datafil;

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

	printf("Öppnade datafil\n");

	/* init people database */

	people_init(67);

	readfile(datafil);
	
	people_release();

	return 0;

}