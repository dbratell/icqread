/*
 * $Header: /mnt/cistern/cvsroot/icqread/icqread.c,v 1.15 1998/05/03 10:26:18 bratell Exp $
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#ifdef WIN32
#include <windows.h>
#endif
#include "ICQread.h"
#include "people.h"
#include "string_utils.h"

#define NR_OF_YEARS 300 /* From 1900 to 2200, should be enough */
int count = 0;
int date_distribution[NR_OF_YEARS][12];

int LOGLEVEL = 6;


unsigned char *make_temp_copy(unsigned char *org_filename, int make_and_not_remove);
struct people *get_people(int uin);

FILE *open_data_file(unsigned char *filename)
{
	unsigned char *tempfilename;
	int use_temp;
	FILE *datafil;

	tempfilename = make_temp_copy(filename, TRUE);
	if(!tempfilename) {
		/* Couldn't make temporary copy!
		 * use ordinary file
		 */
		use_temp = FALSE;
	} else {
		use_temp = TRUE;
	}

	/* Open datafile in readonly binary mode. */
	if(use_temp) {
		datafil = fopen(tempfilename, "rb");
	} else {
		datafil = fopen(filename, "rb");
	}
	if(datafil==NULL) {
		if(use_temp) {
			perror(tempfilename);
			make_temp_copy(tempfilename, FALSE);
		} else {
			perror(filename);
		}
		return NULL;
	}

	if(!use_temp) {
		fprintf(stderr, "Making temporary copy failed, thus working on\noriginal history file. I don't think that should\nbe a problem but I don't guarantee anything!\n");
	}

	return datafil;
}


#ifdef WIN32
void print_win32_error(unsigned char *details)
{
	char *error_string;
	int res;

	res = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL,
		GetLastError(),
		0,
		(unsigned char *)&error_string,
		0,
		NULL);

	if(res == 0) {
		/* error */
		return;
	}

	printf("%s:\n%s\n",details,error_string);

	/* Free the buffer. */
	LocalFree(error_string);
}
#endif
		
		
/* make_and_not_remove = TRUE (1) to create tempfile.
 *						= FALSE (0) to remove it.
 */
unsigned char *make_temp_copy(unsigned char *org_filename, int make_and_not_remove)
{
	unsigned char tempdir[1024];
	static unsigned char tempfilename[1024];
	static int created = FALSE;
	
#ifdef WIN32
	DWORD res;
	if(make_and_not_remove == FALSE) {
		if(created) {
			res = DeleteFile(tempfilename);
			created = FALSE;
			if(res == 0) {
				/* Failed */
				return NULL;
			} else {
				return tempfilename;
			}
		} else {
			/* Do nothing since there is nothing to remove */
			return NULL;
		}
	}

	/* User wants to make temporary file */
	if(created) {
		/* Already created */
		printf("There is already a temporary file created\n");
		return NULL;
	}

	res = GetTempPath(1024, tempdir);
	if((res>1024) || (res == 0)) {
		/* Failed */
		print_win32_error("Tried to find temporary directory");
		return NULL;
	}
	
	res = GetTempFileName(tempdir, "icqread", 0, tempfilename);

	if(res == 0) {
		/* Failed */
		print_win32_error("Tried to create temporary file");
		return NULL;
	}
	
	res = CopyFile(org_filename, tempfilename, FALSE);

	if(res == 0) {
		/* Failed */
		DeleteFile(tempfilename);
		/* print_win32_error("When accessing historyfile to make safety copy of it"); */
		return NULL;
	}

	created = TRUE;
	return tempfilename;
 

#else
	fprintf(stderr, 
		"Copy to tempfile not implemented yet\n");
	return NULL;
#endif
}

void printdate_distribution()
{
	int year;
	int month;
	for(year=0; year<NR_OF_YEARS; year++) {
		for(month=0; month<12; month++) {
			if(date_distribution[year][month] != 0)
			{
				printf("%s %d: %7d messages\n", 
					month_string[month], 
					year+1900,
					date_distribution[year][month]);
			} else {
				/* No messages. Don't print anything */
			}
		}
	}
}

void printstatus(int type, int status)
{
	if(LOGLEVEL>5) {
		/* Print status */
		switch(status) {
		case -1:
			printf("Recived message ok.\n");
			break;
			
		case 0:
			printf("Sent message arrived ok/accepted.\n");
			break;

		case 1:
			if(type == TYPE_MESS) {
				/* Message */
				/* Maybe that the receiver wasn't online. */
			} else if(type == TYPE_CHAT) {
				/* Chat */
				printf("And you accepted the invitation\n");
			} else if(type == TYPE_FILE) {
				printf("The user has denied your file request.\n");
			} else if(type == TYPE_URL) {
				/* Nothing special */
			} else if(type == TYPE_X0B) {
				/* User asked to be added */
				/* Nothing special */
			} else if(type == TYPE_X0C) {
				/* You were added */
				/* Nothing special */
			} else if(type == TYPE_EXTERNAL) {
				printf("You have denied the request.\n");
			} else {
				/* Normal response for some types of messages */
				printf("xyzstatus: %x\n", status);
			}
			break;

		case 2:
			if(type == TYPE_EXTERNAL) {
				printf("The user did not have the program.\n");
			} else {
				printf("xyzstatus: %x\n", status);
			}
			break;

		case 3:
			if(type == TYPE_URL) {
				/* URL */
				printf("You opened it in your browser\n");
			} else {
				printf("xyzstatus: %x\n", status);
			}
			break;

		case 4:
			printf("The receiver was away\n");
			break;

		case 6:
			printf("No contact with intended receiver\n");
			break;
		
		case 7:
			if(type == TYPE_CHAT) {
				printf("User has canceled the chat request before you answered it.\n");
			} else if(type == TYPE_FILE) {
				printf("User has canceled the file request before you answered it.\n");
			} else if(type == TYPE_EXTERNAL) {
				printf("User has canceled the external request before you answered it.\n");			
			} else {
				printf("xyzstatus: %x\n", status);
			}
			break;

		case 8:
			if(type == TYPE_CHAT) {
				printf("You canceled the chat request before anyone answered it.\n");
			} else if(type == TYPE_FILE) {
				printf("You canceled the file request before anyone answered it.\n");
			} else if(type == TYPE_EXTERNAL) {
				printf("You canceled the external request before anyone answered it.\n");
			} else {
				printf("(canceled) xyzstatus: %x\n", status);
			}
			break;

		case 11:
			if(type == TYPE_MESS) {
				printf("And the user was out.\n");
			} else if(type == TYPE_FILE) {
				printf("A file request was received while you were N/A.\n");
			} else {
				printf("xyzstatus: %x\n", status);
			}
			break;

		case 14:
			if(type == TYPE_MESS) {
				printf("And the user was out.\n");
			} else if(type == TYPE_FILE) {
				printf("But the user was out\n");
			} else if(type == TYPE_URL) {
				printf("And the user was out\n");
			} else {
				printf("(out) xyzstatus: %x\n", status);
			}
			break;

		default:
			printf("xyzstatus: %x\n", status);
			break;
		}
	}
}

/* Print some basic information and make some statistics
 * collection.
 */
void printheader(int type, struct startfields *sf)
{
	struct people *p;
	unsigned char *name;
	struct tm *t;

	p = get_people(sf->uin);
	name = p->name;

	if(sf->destination == 1) {
		if(LOGLEVEL>3) printf("%s sent to %s\n", 
			typelabel[type], name);
		if(p) {
			p->number_of_messages_to++;
		}
	} else if(sf->destination == 0) {
		if(LOGLEVEL>3) printf("%s arrived from %s\n", 
			typelabel[type], name);
		if(p) {
			p->number_of_messages_from++;
		}
	} else {
		if(LOGLEVEL>2) printf("%s with unknown destination(%x) from %s\n",
			typelabel[type], sf->destination, name);
	}

	if(p) {
		p->enddate=sf->date;
		if(p->startdate == 0) {
			p->startdate=sf->date;
		}
	}

	t = localtime(&sf->date);

	if(t != NULL) {
		if(t->tm_year<NR_OF_YEARS) {
			date_distribution[t->tm_year][t->tm_mon]++;
		} else {
			/* Too far into the future. Probably an error! */
		}
	}

	if(t != NULL) {
		if(LOGLEVEL>5) printf("Time: %s", asctime(t));
	} else {
		if(LOGLEVEL>5) printf("Time: <illegal time>\n");
	}

	if(LOGLEVEL>6) printf("UIN: %d\n", sf->uin);
	if(LOGLEVEL>8) printf("protocol: %x\n", sf->protocolversion);
	if(LOGLEVEL>5) printstatus(type, sf->status);
}


/* Looks up the uin and returns the person 
 * associated with it. If there's no one yet a
 * dummy person will be created. In no way will
 * a NULL be returned.
 */
struct people *get_people(int uin)
{
	struct people *p;

	p = people_lookup(uin);

	if(p == NULL) {
		/* Insert in database as an empty uin */
		people_add(uin, UNKNOWN, UNKNOWN, UNKNOWN);
		p = people_lookup(uin);
		if(p == NULL) assert(0);
	} 

	return p;
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


void readrecordstart(FILE *datafil, int version, struct startfields *sf)
{
	/* Check for the Microsoft Visual C++ compiler,
	since then I've defined a pragma that makes all
	fields in the struct packed close, making it
	possible to read directly into the struct.
	*/
#if _MSC_VER>=1100
	fread(&(sf->type), 8, 1, datafil);
	count +=8;
#else	
	fread(&(sf->type), 2, 1, datafil);
	count +=2;
	fread(&(sf->uin), 4, 1, datafil);
	count +=4;
	fread(&(sf->length), 2, 1, datafil);
	count +=2;
#endif

	if(sf->length>0) {
		sf->string = malloc(sf->length);
		assert(sf->string);
		fread(sf->string, sf->length, 1, datafil);
		count +=sf->length;
	} else {
		sf->string = 0;
	}

#if _MSC_VER>=1100
	fread(&(sf->status), 14, 1, datafil);
	count +=14;
#else
	fread(&(sf->junk1), 4, 1, datafil);
	count +=4;
	fread(&(sf->destination), 4, 1, datafil);
	count +=4;
	fread(&(sf->protocolversion), 2, 1, datafil);
	count +=2;
	fread(&sf->date,4,1,datafil);
	count +=4;
#endif


	if(version >= INTRO_V8B) {
#if _MSC_VER>=1100
		fread(&(sf->junk2), 5, 1, datafil);
		count += 5;
#else
		fread(&(sf->junk2), 1, 1, datafil);
		count +=1;
		fread(&(sf->junk3), 4, 1, datafil);
		count +=4;
#endif
	} else {
		sf->junk2 = 0;
		sf->junk3 = 0;
	}
}

void readrecordend(FILE *datafil, int version, struct endfields *se)
{
	fread(&(se->junk1), 4, 1, datafil);
	count +=4;
}


/* Handle messages of type 1 (read ICQread.h for more
 * information.
 */
void handle_mess(FILE *datafil, int version, struct startfields *sf)
{
	struct people *p;
	struct endfields se;

	readrecordend(datafil, version, &se);

	printheader(TYPE_MESS, sf);
	if(LOGLEVEL>5) printf("Text: '%s'\n", sf->string);

	p = get_people(sf->uin);
	if(sf->destination == 1) {
		p->number_of_words_to += wordcount(sf->string);
	} else if(sf->destination == 0) {
		p->number_of_words_from += wordcount(sf->string);
	}

	if(LOGLEVEL>3) printf("\n");

	return;
}




/* Handle messages of type 2 (read ICQread.h for more
 * information.
 */
void handle_chat(FILE *datafil, int version, struct startfields *sf)
{
	struct endfields se;

	struct textdata people;

	readstring(datafil, version, &people);
	readrecordend(datafil, version, &se);

	printheader(TYPE_CHAT, sf);
	if(LOGLEVEL>5) printf("Text: '%s'\n", sf->string);
	if(LOGLEVEL>5) printf("People already in: '%s'\n", 
		people.string);
	if(LOGLEVEL>3) printf("\n");

	free(people.string);

	return;
}



/* Handle messages of type 3 (read ICQread.h for more
 * information.
 */
void handle_file(FILE *datafil, int version, struct startfields *sf)
{
	struct endfields se; 
	struct textdata filename;
	struct textdata directoryname;
	
	__int32 filelength;

	readstring(datafil, version, &filename);

	fread(&filelength, 4, 1, datafil);
	count +=4;

	readstring(datafil, version, &directoryname);
	readrecordend(datafil, version, &se);

	printheader(TYPE_FILE, sf);
	if(LOGLEVEL>5) printf("Text: '%s'\n", sf->string);
	if(LOGLEVEL>5) printf("Filename: '%s'\nSize : %d bytes\n", 
		filename.string, filelength);
	if(directoryname.length != 0) {
		if(LOGLEVEL>6) printf("Saved in:'%s'\n", 
			directoryname.string);
	} else {
		/* The file was sent from here. No
		 * directory "involved".
		 */
	}
	if(LOGLEVEL>3) printf("\n");

	free(filename.string);
	if(directoryname.string) free(directoryname.string);

	return;
}


/* Handle messages of type 4 (read ICQread.h for more
 * information.
 */
void handle_url(FILE *datafil, int version, struct startfields *sf)
{
	struct endfields se;
	

	readrecordend(datafil, version, &se);

	printheader(TYPE_URL, sf);
	if(LOGLEVEL>5) printf("Data: '%s'\n", sf->string);
	if(LOGLEVEL>3) printf("\n");


	return;
}



/* Handle messages of type 6 (read ICQread.h for more
 * information.
 */
void handle_x06(FILE *datafil, int version, struct startfields *sf)
{
	struct endfields se;
	
	readrecordend(datafil, version, &se);

	printheader(TYPE_X06, sf);
	if(LOGLEVEL>5) printf("Text: '%s'\n", sf->string);
	if(LOGLEVEL>3) printf("\n");

	return;
}

/* Handle messages of type 7 (read ICQread.h for more
 * information.
 */
void handle_x07(FILE *datafil, int version, struct startfields *sf)
{
	struct endfields se;
	
	readrecordend(datafil, version, &se);

	printheader(TYPE_X07, sf);
	if(LOGLEVEL>5) printf("Text: '%s'\n", sf->string);
	if(LOGLEVEL>3) printf("\n");

	return;
}

/* Handle messages of type 8 (read ICQread.h for more
 * information.
 */
void handle_x08(FILE *datafil, int version, struct startfields *sf)
{
	struct endfields se;
	
	readrecordend(datafil, version, &se);

	printheader(TYPE_X08, sf);
	if(LOGLEVEL>5) printf("Text: '%s'\n", sf->string);
	if(LOGLEVEL>3) printf("\n");
	
	return;
}

/* Handle messages of type 9 (read ICQread.h for more
 * information.
 */
void handle_x09(FILE *datafil, int version, struct startfields *sf)
{
	struct endfields se;
	
	readrecordend(datafil, version, &se);

	printheader(TYPE_X09, sf);
	if(LOGLEVEL>5) printf("Text: '%s'\n", sf->string);
	if(LOGLEVEL>3) printf("\n");

	return;
}


/* Handle messages of type 10 (read ICQread.h for more
 * information.
 */
void handle_external(FILE *datafil, int version, struct startfields *sf)
{
	struct endfields se;
	struct textdata programname;
	
	__int32 junk4, junk5;
	__int16 junk6;


	readstring(datafil, version, &programname);
	fread(&junk4, 4, 1, datafil);
	count +=4;
	fread(&junk5, 4, 1, datafil);
	count +=4;
	fread(&junk6, 2, 1, datafil);
	count +=2;
	readrecordend(datafil, version, &se);

	printheader(TYPE_EXTERNAL, sf);
	if(LOGLEVEL>5) printf("Reason: '%s'\n", sf->string);
	if(LOGLEVEL>5) printf("Program: '%s'\n", 
		programname.string);
	if(LOGLEVEL>8) printf("junk4: %x\n", junk4);
	if(LOGLEVEL>8) printf("junk5: %x\n", junk5);
	if(LOGLEVEL>8) printf("junk6: %x\n", junk6);
	if(LOGLEVEL>3) printf("\n");

	free(programname.string);

	return;
}





/* Handle messages of type 11 (read ICQread.h for more
 * information.
 */
void handle_x0B(FILE *datafil, int version, struct startfields *sf)
{
	struct endfields se;

	unsigned char *nick;
	unsigned char *name;
	unsigned char *email;

	struct people *p;

	readrecordend(datafil, version, &se);

	nick = string2nick(sf->string);
	name = string2name(sf->string);
	email = string2email(sf->string);
	people_add(sf->uin, nick, name, email);

	p = people_lookup(sf->uin);
	assert(p);

	printheader(TYPE_X0B, sf);
	if(LOGLEVEL>5) printf("Nickname: %s\nName:     %s\ne-Mail:   %s\n", 
		nick, name, email);
	if(LOGLEVEL>3) printf("\n");

	free(nick);
	free(name);
	free(email);

	return;
}

/* Handle messages of type 12 (read ICQread.h for more
 * information.
 */
void handle_x0C(FILE *datafil, int version, struct startfields *sf)
{
	struct endfields se;

	unsigned char *nick;
	unsigned char *name;
	unsigned char *email;

	struct people *p;

	readrecordend(datafil, version, &se);

	nick = string2nick(sf->string);
	name = string2name(sf->string);
	email = string2email(sf->string);
	people_add(sf->uin, nick, name, email);

	p = people_lookup(sf->uin);
	assert(p);

	printheader(TYPE_X0C, sf);
	if(LOGLEVEL>5) printf("Nickname: %s\nName:     %s\ne-Mail:   %s\n", 
		nick, name, email);
	if(LOGLEVEL>3) printf("\n");

	free(nick);
	free(name);
	free(email);

	return;
}


/* Handle messages of type 15 (read ICQread.h for more
 * information.
 */
void handle_mail(FILE *datafil, int version, struct startfields *sf)
{
	struct endfields se;
	

	readrecordend(datafil, version, &se);

	printheader(TYPE_MAIL, sf);
	if(LOGLEVEL>5) printf("Text: '%s'\n", sf->string);
	if(LOGLEVEL>3) printf("\n");

	return;
}


/* Handle messages of type 19 (read ICQread.h for more
 * information.
 */
void handle_contactlist(FILE *datafil, int version, struct startfields *sf)
{
	struct endfields se;

	struct people *p;

	unsigned char *strpek;
	int i, j;
	int nr_of_contacts;
	int uin;

	readrecordend(datafil, version, &se);

	printheader(TYPE_CONTACTLIST, sf);
	if(LOGLEVEL>5) printf("Contacts:\n%s\n", sf->string);
	if(LOGLEVEL>3) printf("\n");


	for(i=0; (unsigned char)sf->string[i]!=0xFE; i++);

	sf->string[i]='\0';
	
	nr_of_contacts = atoi(sf->string);
	strpek = &(sf->string[i+1]);

	for(j=0; j<nr_of_contacts; j++) {
		/* Pick up UIN */
		for(i=0; (unsigned char)strpek[i]!=0xFE; i++);
		strpek[i]='\0';
		uin = atoi(strpek);
		strpek = &(strpek[i+1]);
		/* Pick up nick */
		for(i=0; (unsigned char)strpek[i]!=0xFE; i++);
		strpek[i]='\0';

		/* Add to database */
		p = get_people(uin);
		/* See if we have a name yet */
		if(strcmp(p->nick, UNKNOWN) == 0) {
			/* No name yet. Let's add one */
			people_add(uin, strpek, UNKNOWN, UNKNOWN);
		}
		strpek = &(strpek[i+1]);
	}

	return;
}

void readfile(FILE *datafil) 
{
	unsigned char buf[4];
	__int32 *data32;
	__int16 *data16;

	int version;

	struct startfields sf;

	int go_on = TRUE;

	buf[0]=buf[1]=buf[2]=buf[3]=0;

	data32 = (__int32 *)&buf[0];
	data16 = (__int16 *)&buf[2];

	while(go_on) {
		if(LOGLEVEL>4) printf("-----------------------\n");
		buf[0]=buf[2];
		buf[1]=buf[3];
		fread(&buf[2], 2, 1, datafil);
		count += 2;

		/* Assuming all versions has the same 
		 * format as the ones I looked into.
		 */
		while(((*data16 < INTRO_V72) ||
			(*data16 > INTRO_V9C)) &&
			(!feof(datafil))) {
			printf("Couldn't find header. Could be newer or older version than previous met (%x).\n", *data16);

			buf[0]=buf[2];
			buf[1]=buf[3];
			fread(&buf[2], 2, 1, datafil);
			count += 2;
		}

		if(feof(datafil)) return;
		
		if((*data16 != INTRO_V72) && 
			(*data16 != INTRO_V73) && 
			(*data16 != INTRO_V74) && 
			(*data16 != INTRO_V78) && 
			(*data16 != INTRO_V8B) && 
			(*data16 != INTRO_V96) && 
			(*data16 != INTRO_V97) && 
			(*data16 != INTRO_V98) &&
			(*data16 != INTRO_V9C)) {
			printf("Didn't recognize version (%x). Assuming similarities with other versions\n", *data16);
		}
		
		version = *data16;

		readrecordstart(datafil, version, &sf);

		switch(sf.type) {
		case TYPE_MESS:
			handle_mess(datafil, version, &sf);
			break;

		case TYPE_CHAT:
			handle_chat(datafil, version, &sf);
			break;

		case TYPE_FILE:
			handle_file(datafil, version, &sf);
			break;

		case TYPE_URL:
			handle_url(datafil, version, &sf);
			break;

		case TYPE_X06:
			handle_x06(datafil, version, &sf);
			break;

		case TYPE_X07:
			handle_x07(datafil, version, &sf);
			break;

		case TYPE_X08:
			handle_x08(datafil, version, &sf);
			break;

		case TYPE_X09:
			handle_x09(datafil, version, &sf);
			break;

		case TYPE_EXTERNAL:
			handle_external(datafil, version, &sf);
			break;

		case TYPE_X0B:
			handle_x0B(datafil, version, &sf);
			break;

		case TYPE_X0C:
			handle_x0C(datafil, version, &sf);
			break;

		case TYPE_MAIL:
			handle_mail(datafil, version, &sf);
			break;

		case TYPE_CONTACTLIST:
			handle_contactlist(datafil, version, &sf);
			break;

		default:
			printf("Unknown type: %d\n", sf.type);
			go_on = FALSE;
			break;
		}

		free(sf.string);
	}
	return;

}

/* Print program usage */
void usage()
{
	printf("Usage: icqread <filename> [LOGLEVEL (1-10)]\n");
	printf("\nWhere:\n");
	printf("<filename> \tis the name of the historyfile to analyze. Normally it's you\n\t\tuin with msg.dat at the end. I recommend you working\n\t\ton a copy of the file since I haven't analyzed ICQ behaviour\n\t\twhen someone else is reading the file.\n");
	printf("LOGLEVEL   \tcontrols how much information is shown. 1 gives only summary\n\t\tstatistics and 10 gives all info collected. You may try it\n\t\tout yourself.\n");
	printf("\n");
	printf("This version compiled %s %s\n\n", __DATE__, __TIME__);
}

int main(int argc, char *argv[])
{
	int loglevel;

	FILE *datafil;

	if(argc<2 || argc >3) {
		usage();
		exit(1);
	}

	datafil = open_data_file(argv[1]);
	if(datafil==NULL) {
		exit(1);
	}

	/* printf("Öppnade datafil\n"); */

	if(argc==3) {
		loglevel=atoi(argv[2]);
		if((loglevel > 0) && (loglevel <= 10)) {
			LOGLEVEL = loglevel;
		}
	}

	printf("Using loglevel of %d.\n", LOGLEVEL);
		

	/* Clear the date_distribution */
	memset(date_distribution, 0, 
		sizeof(date_distribution[0][0])*NR_OF_YEARS*12);
	
	/* init people database */
	/* It may not be full since the program will crash if
	 * it is.
	 */
	people_init(313);

	people_add(1, "System", "ICQ Server", "N/A");
	readfile(datafil);
	
	fclose(datafil);
	make_temp_copy(NULL, FALSE);

	people_print_info();
	people_release();

	printdate_distribution();

	return 0;
}
