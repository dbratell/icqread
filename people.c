#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "people.h"

void people_free(struct people *p);

struct people **people_db=NULL;
int people_db_size;

void people_init(int size)
{
	if(people_db!=NULL) {
		/* Already inited */
		return;
	}

	people_db = malloc(size * sizeof(struct people*));
	assert(people_db);

	people_db_size = size;

	memset((void *)people_db, 0, size * sizeof(struct people*));
}


/* Frees all memory allocated in the people database */
void people_release()
{
	int i;
	if(people_db != NULL) {
		for(i=0; i<people_db_size; i++) {
			if(people_db[i] != NULL) {
				people_free(people_db[i]);
				people_db[i] = NULL;
			}
		}

		free(people_db);
		people_db = NULL;
	}
}

void people_free(struct people *p)
{
	if(p) {
		if(p->nick) {
			free(p->nick);
			p->nick = NULL;
		}
		if(p->name) {
			free(p->name);
			p->name = NULL;
		}
		if(p->email) {
			free(p->email);
			p->email = NULL;
		}
		free(p);
	}
}

void people_add(int uin, unsigned char *nick, unsigned char *name, unsigned char *email)
{
	int hash, count=0;

	struct people *p;
	p = malloc(sizeof(struct people));
	assert(p);

	p->uin = uin;
	p->nick = malloc(strlen(nick)+1);
	assert(p->nick);
	strcpy(p->nick, nick);

	p->name = malloc(strlen(name)+1);
	assert(p->name);
	strcpy(p->name, name);

	p->email = malloc(strlen(email)+1);
	assert(p->email);
	strcpy(p->email, email);

	p->startdate = 0;
	p->enddate = 0;
	p->number_of_messages_to=0;
	p->number_of_messages_from=0;
	p->number_of_words_to=0;
	p->number_of_words_from=0;


	hash = uin % people_db_size;
	while(people_db[hash] != NULL) {
		if(people_db[hash]->uin == uin) {
			/* Already here 
			 * Change to new string
			 */
			if(people_db[hash]->nick) {
				free(people_db[hash]->nick);
			}
			people_db[hash]->nick = p->nick;
			if(people_db[hash]->name) {
				free(people_db[hash]->name);
			}
			people_db[hash]->name = p->name;
			if(people_db[hash]->email) {
				free(people_db[hash]->email);
			}
			people_db[hash]->email = p->email;
			free(p);
			return;
		}
		hash = (hash+1) % people_db_size;
		count++;
		if(count == people_db_size) {
			/* Database full */
			assert(0);
		}
	}
	/* Room for me */
	people_db[hash] = p;
	return;

}

struct people *people_lookup(int uin)
{
	int hash, count=0;

	hash = uin % people_db_size;
	while(count < people_db_size) {
		if(people_db[hash] != NULL) {
			if(people_db[hash]->uin == uin) {
				return people_db[hash];
			}
		}
		hash = (hash+1) % people_db_size;
		count++;
	}
	/* Nothing found */
	return NULL;
}

void people_print_info()
{
	struct people *p;
	struct tm *t;

	int timespan = 0;

	int total_sent_mess = 0, total_received_mess = 0;
	int total_sent_words = 0, total_received_words = 0;
	int i=0;
	for(i=0; i<people_db_size; i++) {
		if(people_db[i] != NULL) {
			p = people_db[i];
			if((p->number_of_messages_to == 0) && 
				(p->number_of_messages_from== 0)) {
				/* No activity. Don't display */
				continue;
			}
			printf("Uin: %d\n", p->uin);
			printf("Nick: %s\nName: %s\ne-Mail:%s\n",
				p->nick, p->name, p->email);
			t = localtime(&p->startdate);
			if(t != NULL) {
				printf("Between %.24s and ", asctime(t));
				t = localtime(&p->enddate);
				if(t != NULL) {
					printf("%.24s.\n", asctime(t));
				} else {
					printf("now.\n");
				}
				timespan = time(NULL) - p->startdate;
				timespan = timespan / (24*60*60);
				timespan +=1;
			} else {
				t = localtime(&p->enddate);
				if(t != NULL) {
					printf("Until %.24s.\n", asctime(t));
				} else {
					printf("During all time.\n");
				}
			}

			if(timespan >0) {
				printf("Sent messages:     %d \t(%.1f per day since first message)\nReceived messages: %d \t(%.1f per day since first message)\n",
				p->number_of_messages_to, (float)p->number_of_messages_to/(float)timespan, 
				p->number_of_messages_from, (float)p->number_of_messages_from/(float)timespan);
			} else {
				printf("Sent messages: %d\nReceived messages: %d\n",
					p->number_of_messages_to, p->number_of_messages_from);
			}

			printf("Sent words: %d\nReceived words: %d\n",
				p->number_of_words_to, p->number_of_words_from);
			
			printf("\n");
			total_sent_mess += p->number_of_messages_to;
			total_received_mess += p->number_of_messages_from;
			total_sent_words+= p->number_of_words_to;
			total_received_words += p->number_of_words_from;
		}
	}

	printf("\nTotal sent: %d  (%d words)\nTotal received: %d  (%d words)\n\n", 
		total_sent_mess, total_sent_words,
		total_received_mess, total_received_words);
}