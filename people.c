#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "people.h"

struct people_array {
	int size;
	struct people **array;
};


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

void people_free_array(struct people_array *pa)
{
	assert(pa);
	if(pa->array) {
		free(pa->array);
		pa->array = NULL;
		pa->size = 0;
	}

	return;
}

/* Returns a pointer to an array of pointers to people
 * structs. NULL if there's no people in db.
 */
struct people_array people_make_array()
{
	struct people_array pa;
	int number,i,j;

	void *array;
	/* Count number of people */
	number = 0;
	for(i=0; i<people_db_size; i++) {
		if(people_db[i] != NULL) {
			number++;
		}
	}

	pa.size = number;

	if(number == 0) {
		pa.array = NULL;
		return pa;
	}

	array = malloc(number * sizeof(struct people *));
	assert(array);

	pa.array = array;

	/* Insert pointers to people in array */
	j=0;
	for(i=0; i<people_db_size; i++) {
		if(people_db[i] != NULL) {
			pa.array[j] = people_db[i];
			j++;
		}
	}

	return pa;
}

/* Returns a number in the struct according to the 
 * criteria.
 */

int people_getvalue(struct people *p, int criteria)
{
	switch(criteria)
	{
	case STARTDATE: return p->startdate;
	case ENDDATE:	return p->enddate;
	case MESSAGES_TO: return p->number_of_messages_to;
	case MESSAGES_FROM: return p->number_of_messages_from;
	case WORDS_TO:	return p->number_of_words_to;
	case WORDS_FROM:	return p->number_of_words_from;
	default:
		fprintf(stderr, "Not a known criterie in people_getvalue (%d).\n", criteria);
		return 0;
	}
}


void people_swap(struct people **p1, struct people **p2)
{
	struct people *temp_p;
	temp_p = *p1;
	*p1 = *p2;
	*p2 = temp_p;
}

/* Sorts in descending order */
/* Algorithm from "Data Structures & Their Algorithms" (1991)
 * by Lewis and Denenberg. Page 392.
 */
void people_array_qsort(struct people_array *pa, int first, int last, int criteria)
{
	int pivot;
	int i,j,middle;

	if(first>=last) {
		/* Nothing to sort */
		return;
	}

	if((pa->array == NULL) || 
		(pa->size < first) ||
		(last<0) ||
		(first<0))
	{
		/* Error */
		fprintf(stderr, "Error in call to people_array_qsort");
		return;
	}

	/* Find pivot */
	/* Pick the middle number of the three: the first,
	 * the last and the one in the middle.
	 * Also place the numbers in that order.
	 */
	middle = (first+last)/2;
	if(people_getvalue(pa->array[first],criteria) <
		people_getvalue(pa->array[middle],criteria)) {
		/* first < middle */
		if(people_getvalue(pa->array[last],criteria) <
			people_getvalue(pa->array[first],criteria)) {
			/* last < first < middle */
			people_swap(&(pa->array[first]),&(pa->array[middle]));
			people_swap(&(pa->array[first]),&(pa->array[last]));
		} else {
			/* first < middle, first <= last */
			if(people_getvalue(pa->array[middle],criteria) <
				people_getvalue(pa->array[last],criteria)) {
				/* first < middle < last */
				/* Already in order */
			} else {
				/* first < last <= middle */
				people_swap(&(pa->array[middle]),&(pa->array[last]));
			}
		}
	} else {
		/* middle <= first */
		if(people_getvalue(pa->array[last],criteria) <
			people_getvalue(pa->array[middle],criteria)) {
			/* last < middle <= first */
			people_swap(&(pa->array[first]),&(pa->array[last]));
		} else {
			/* middle <= first, middle <= last*/
			if(people_getvalue(pa->array[first],criteria) <
				people_getvalue(pa->array[last],criteria)) {
				/* middle >= first > last */
				people_swap(&(pa->array[first]),&(pa->array[middle]));
			} else {
				/* middle <= last <= first */
				people_swap(&(pa->array[first]),&(pa->array[last]));
				people_swap(&(pa->array[first]),&(pa->array[middle]));
			}
		}
	}

	if((last-first)<=2) {
		/* Sorted by the previous swaps */
		return;
	}

	pivot = people_getvalue(pa->array[middle],criteria);
	/* pivot now contains the middle value */

	/* Move pivot to a better place. */
	people_swap(&(pa->array[first+1]),&(pa->array[middle]));


	i=first+1;
	j=last;

	while(i<j) {
		i++;
		/* Search for value that should be to the right 
		 * of the pivot.
		 */
		while(people_getvalue(pa->array[i], criteria)<pivot) {
			i++;
		}
		j--;
		/* Search for value that should be to the left 
		 * of the pivot.
		 */
		while(people_getvalue(pa->array[j], criteria)>pivot) {
			j--;
		}

		/* Swap */
		people_swap(&(pa->array[i]),&(pa->array[j]));
	}

	/* Undo extra swap */
	people_swap(&(pa->array[i]),&(pa->array[j]));

	/* Move pivot to it's proper place */
	people_swap(&(pa->array[first+1]),&(pa->array[j]));

	people_array_qsort(pa, first, j-1, criteria);
	people_array_qsort(pa, j+1, last, criteria);
}


void people_print_info(int sort_criteria)
{
	struct people *p;
	struct tm *t;

	int timespan = 0;

	int total_sent_mess = 0, total_received_mess = 0;
	int total_sent_words = 0, total_received_words = 0;
	int i=0;

	struct people_array pa;
	pa = people_make_array();
	if(pa.size == 0) {
		/* Nothing to display */
		printf("No record of any events\n");
	}
	people_array_qsort(&pa, 0, pa.size-1, sort_criteria);
	
	for(i=0; i<pa.size; i++) {
		p = pa.array[i];
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
			printf("Sent messages:     %d\nReceived messages: %d\n",
				p->number_of_messages_to, p->number_of_messages_from);
		}

		printf("Sent words:     %d\nReceived words: %d\n",
			p->number_of_words_to, p->number_of_words_from);
		
		printf("\n");
		total_sent_mess += p->number_of_messages_to;
		total_received_mess += p->number_of_messages_from;
		total_sent_words+= p->number_of_words_to;
		total_received_words += p->number_of_words_from;
	}

	printf("\nTotal sent: %d  (%d words)\nTotal received: %d  (%d words)\n\n", 
		total_sent_mess, total_sent_words,
		total_received_mess, total_received_words);

	people_free_array(&pa);

}