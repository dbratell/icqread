#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "ICQread.h"
#include "people.h"

int count = 0;

int LOGLEVEL = 6;

void printheader(char *type, int dest, int uin, int time)
{
	struct people *p;
	char *name;
	struct tm *t;

	p = people_lookup(uin);
	if(p == NULL) {
		name = "<unknown>";
	} else {
		name = p->name;
	}

	if(dest == 1) {
		if(LOGLEVEL>3) printf("%s sent to %s\n", 
			type, name);
		if(p) {
			p->number_of_messages_to++;
		}
	} else if(dest == 0) {
		if(LOGLEVEL>3) printf("%s arrived from %s:\n", 
			type, name);
		if(p) {
			p->number_of_messages_from++;
		}
	} else {
		if(LOGLEVEL>2) printf("%s with unknown destination(%x) from %s\n",
			type, dest, name);
	}

	if(p) {
		p->enddate=time;
		if(p->startdate == 0) {
			p->startdate=time;
		}
	}

	t = localtime(&time);
	if(t != NULL) {
		if(LOGLEVEL>5) printf("Time: %s", asctime(t));
	} else {
		if(LOGLEVEL>5) printf("Time: <skum tid>\n");
	}

	if(LOGLEVEL>6) printf("UIN: %d\n", uin);

}


char *string2nick(unsigned char *str)
{
	int i=0;
	char *result;

	while(str[i] != 0xFE) {
		i++;
	}

	result = malloc(i+1);
	assert(result);

	strncpy(result, str, i);
	result[i]='\0';

	return result;
}

char *string2name(unsigned char *str)
{
	int start;
	int i=0, j=0;
	char *result;

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


char *string2email(unsigned char *str)
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

int readdate(FILE *datafil, int version)
{
	__int32 date;

	fread(&date,4,1,datafil);
	count +=4;
	return date;

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

	int newtime;

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);
	newtime = readdate(datafil, version);

	readv96data(datafil, version, &vd);
	readend(datafil, version, &se);

	printheader("Message", info.destination, sf.uin, newtime);

	if(LOGLEVEL>5) printf("Text: '%s'\n", sf.string);

	/* printf("junk1: %x\n", info.junk1); */
	if(LOGLEVEL>8) printf("protocol: %x\n", info.protocolversion);

	if(LOGLEVEL>3) printf("\n");

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

	int newtime;

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);

	newtime = readdate(datafil, version);
	readv96data(datafil, version, &vd);

	readstring(datafil, version, &people);

	readend(datafil, version, &se);

	printheader("Chat request", info.destination, sf.uin, newtime);

	if(LOGLEVEL>8) printf("protocolversion: %x\n", info.protocolversion);
	if(LOGLEVEL>5) printf("Other people?\nString: '%s'\n", 
		people.string);

	if(LOGLEVEL>3) printf("\n");

	free(sf.string);
	free(people.string);

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

	int newtime;

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

	printheader("File", info.destination, sf.uin, newtime);

	/* printf("junk1: %x\n", info.junk1); */
	if(LOGLEVEL>8) printf("protocolversion: %x\n", info.protocolversion);
	if(LOGLEVEL>5) printf("File:\nName: '%s'\nSize : %d bytes\n", 
		filename.string, filelength);

	if(junkstring.length != 0) {
		if(LOGLEVEL>6) printf("Response?:\nString:'%s'\n", 
			junkstring.string);
	} else {
		if(LOGLEVEL>6) printf("Empty response(?).\n");
	}
	if(LOGLEVEL>3) printf("\n");

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

	int newtime;

	readstart(datafil, version, &sf);
	
	readinfo(datafil, version, &info);

	newtime = readdate(datafil, version);

	readv96data(datafil, version, &vd);
	readend(datafil, version, &se);

	printheader("URL", info.destination, sf.uin, newtime);

	/* printf("junk1: %x\n", info.junk1); */
	if(LOGLEVEL>8) printf("protocolversion: %x\n", info.protocolversion);
	if(LOGLEVEL>3) printf("\n");

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

	int newtime;

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);

	newtime = readdate(datafil, version);

	readv96data(datafil, version, &vd);
	readend(datafil, version, &se);

	printheader("Authorization request", info.destination, sf.uin, newtime);

	/* printf("junk1: %x\n", info.junk1); */
	if(LOGLEVEL>8) printf("protocolversion: %x\n", info.protocolversion);
	if(LOGLEVEL>3) printf("\n");

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

	int newtime;

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);

	newtime = readdate(datafil, version);

	readv96data(datafil, version, &vd);
	readend(datafil, version, &se);

	printheader("Message X08 (Receipt?)", info.destination, sf.uin, newtime);

	if(LOGLEVEL>8) printf("protocolversion: %x\n", info.protocolversion);
	if(LOGLEVEL>3) printf("\n");
	
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

	int newtime;

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);

	newtime = readdate(datafil, version);

	readv96data(datafil, version, &vd);
	readend(datafil, version, &se);

	printheader("System Message", info.destination, sf.uin, newtime);

	/* printf("junk1: %x\n", info.junk1); */
	if(LOGLEVEL>8) printf("protocolversion: %x\n", info.protocolversion);
	if(LOGLEVEL>3) printf("\n");

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

	int newtime;

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

	printheader("External program request", info.destination, sf.uin, newtime);

	/* printf("junk1: %x\n", info.junk1); */
	if(LOGLEVEL>8) printf("protocolversion: %x\n", info.protocolversion);
	if(LOGLEVEL>5) printf("Program:\nString: '%s'\n", 
		programname.string);

	if(LOGLEVEL>9) printf("junk4: %x\n", junk4);
	if(LOGLEVEL>9) printf("junk5: %x\n", junk5);
	if(LOGLEVEL>9) printf("junk6: %x\n", junk6);
	if(LOGLEVEL>3) printf("\n");

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

	int newtime;

	char *nick;
	char *name;
	char *email;

	struct people *p;

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);

	newtime = readdate(datafil, version);

	readv96data(datafil, version, &vd);
	readend(datafil, version, &se);

	nick = string2nick(sf.string);
	name = string2name(sf.string);
	email = string2email(sf.string);
	people_add(sf.uin, nick, name, email);
	free(nick);
	free(name);
	free(email);

	p = people_lookup(sf.uin);
	assert(p);

	printheader("User asked to be added", info.destination, sf.uin, newtime);

	/* printf("junk1: %x\n", info.junk1); */
	if(LOGLEVEL>8) printf("protocolversion: %x\n", info.protocolversion);
	if(LOGLEVEL>3) printf("\n");

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

	int newtime;

	char *nick;
	char *name;
	char *email;

	struct people *p;

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);


	newtime = readdate(datafil, version);

	readv96data(datafil, version, &vd);
	readend(datafil, version, &se);

	nick = string2nick(sf.string);
	name = string2name(sf.string);
	email = string2email(sf.string);
	people_add(sf.uin, nick, name, email);
	free(nick);
	free(name);
	free(email);

	p = people_lookup(sf.uin);
	assert(p);

	printheader("You were added by", info.destination, sf.uin, newtime);

	/* printf("junk1: %x\n", info.junk1); */
	if(LOGLEVEL>8) printf("protocolversion: %x\n", info.protocolversion);
	if(LOGLEVEL>3) printf("\n");

	free(sf.string);

	return;
}


/* Handle messages of type 15 (read ICQread.h for more
 * information.
 */
void handle_x0F(FILE *datafil, int version)
{
	struct startfields sf;
	struct endfields se;
	struct infofields info;
	struct v96data vd;

	int newtime;

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);

	newtime = readdate(datafil, version);

	readv96data(datafil, version, &vd);
	readend(datafil, version, &se);

	printheader("Mail", info.destination, sf.uin, newtime);

	/* printf("junk1: %x\n", info.junk1); */
	if(LOGLEVEL>8) printf("protocolversion: %x\n", info.protocolversion);
	if(LOGLEVEL>3) printf("\n");

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

	int newtime;

	readstart(datafil, version, &sf);

	readinfo(datafil, version, &info);

	newtime = readdate(datafil, version);

	readv96data(datafil, version, &vd);
	readend(datafil, version, &se);

	printheader("Contact list", info.destination, sf.uin, newtime);

	/* printf("junk1: %x\n", info.junk1); */
	if(LOGLEVEL>8) printf("protocolversion: %x\n", info.protocolversion);
	if(LOGLEVEL>3) printf("\n");

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
		if(LOGLEVEL>1) printf("-----------------------\n");
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

		case TYPE_X0F:
			handle_x0F(datafil, version);
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

	int loglevel;

	FILE *datafil;

	if(argc<2 || argc >3) {
		printf("icqread <filnamn> [LOGLEVEL (1-10)]\n");
		exit(1);
	}

	/* Open datafile in readonly binary mode. */
	datafil = fopen(argv[1], "rb");
	if(!datafil) {
		perror("Datafil");
		exit(1);
	}

	printf("Öppnade datafil\n");

	if(argc==3) {
		loglevel=atoi(argv[2]);
		if((loglevel > 0) && (loglevel <= 10)) {
			LOGLEVEL = loglevel;
		}
		printf("Using loglevel of %d.\n", LOGLEVEL);
	}
		

	/* init people database */

	people_init(67);

	people_add(1, "System", "ICQ System", "N/A");

	readfile(datafil);

	people_print_info();
	
	people_release();

	return 0;

}