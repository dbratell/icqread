#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
		if(p->name) {
			free(p->name);
			p->name = NULL;
		}
		free(p);
	}
}

void people_add(int uin, char *name)
{
	int hash, count=0;

	struct people *p;
	p = malloc(sizeof(struct people));
	assert(p);

	p->uin = uin;
	p->name = malloc(strlen(name)+1);
	assert(p->name);
	strcpy(p->name, name);

	hash = uin % people_db_size;
	while(people_db[hash] != NULL) {
		if(people_db[hash]->uin == uin) {
			/* Already here 
			 * Change to new string
			 */
			if(people_db[hash]->name) {
				free(people_db[hash]->name);
			}
			people_db[hash]->name = p->name;
			free(p);
			return;
		}
		hash = (hash+1) % people_db_size;
		count++;
		if(count = people_db_size) {
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
