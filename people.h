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
void people_print_info();
