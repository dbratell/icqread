struct people {
	int uin;
	unsigned char *nick;
	unsigned char *name;
	unsigned char *email;
	int startdate;
	int enddate;
	int number_of_messages_to;
	int number_of_messages_from;
	int number_of_words_to;
	int number_of_words_from;
};

void people_init(int size);
void people_release();
struct people *people_lookup(int uin);
void people_add(int uin, unsigned char *nick, unsigned char *name, unsigned char *email);
void people_print_info(int sort_criteria);

#define STARTDATE 1
#define ENDDATE 2
#define MESSAGES_TO 3
#define MESSAGES_FROM 4
#define WORDS_TO 5
#define WORDS_FROM 6
/* 7 and 8 is the number of messages per day multiplied
 * by 100.
 */
#define MESSAGES_TO_INTENSITY 7
#define MESSAGES_FROM_INTENSITY 8

