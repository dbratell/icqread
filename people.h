struct people {
	int uin;
	char *name;
};

void people_init(int size);
void people_release();
struct people *people_lookup(int uin);
void people_add(int uin, char *name);
