#define TRUE 1
#define FALSE 0


#define INTRO_V72 0x0072
#define INTRO_V73 0x0073
#define INTRO_V74 0x0074
#define INTRO_V96 0x0096
#define INTRO_V98 0x0098

/*
 * Incoming (and outgoing?) message.
 * Starts with a 4-byte uin, followed by a 2-byte
 * message length. Then follows the message string
 * and 10 bytes of unknown content. Then the date
 * and 4 more bytes of unknown content.
 *
 * In V96 there seems to be 5 more bytes at the end.
 * Strange odd number.
 */
#define TYPE_X01 0x0001

/*
 * Incoming (and outgoing?) message. (this number too?)
 * Starts with a 4-byte uin, followed by a 2-byte
 * message length. Then follows the message string
 * and 10 bytes of unknown content. Then the date
 * and another length of 2 bytes followed by a new
 * string of that length. Finally there are 4 more 
 * bytes of unknown content.
 */
#define TYPE_X02 0x0002
 
 
/*
 * Incoming (and outgoing?) file.
 * Starts with a 4-byte uin, followed by a 2-byte
 * message length. Then follows the message string 
 * containing the description of the file.
 * Then there are 10 bytes of unknown content
 * followed by the 4 byte date and 
 * only in V96 and later 5 unknown bytes. Then there
 * is another length. 
 * Then there is a string (filename) of the the specified
 * length. Then there are 4 bytes which is the 
 * filelength. 
 * Then there are another 2-byte length and a string
 * of that length. 
 * And last there are 4 bytes of unknown content.
 */
#define TYPE_X03 0x0003

 
 
 /*
 * Incoming (and outgoing?) URL.
 * Starts with a 4-byte uin, followed by a 2-byte
 * message length. Then follows the message string
 * and 10 bytes of unknown content. Then the date
 * and 4 more bytes of unknown content.
 *
 * In V96 there seems to be 5 more bytes at the end.
 * Strange odd number.
 */
#define TYPE_X04 0x0004

 
 
/* "Asked for authorization"
 * It's an uin (4 bytes) and then maybe
 * the length (2 bytes).
 * The string consists of info of self.
 * The string seems to be Nickname <FE> Real Name
 * <FE> e-Mail <FE> Number <FE> Reason to be added.
 *
 * "(MESSAGE contains: <nick name> FE <first
 * name> FE <last name> FE <e-mail> FE <ASCII authorize>. The <ASCII
 * authorize> is '1' (31) if the user allows anyone to add him/her to their
 * contact list, and '0' (30) otherwise.)"
 *
 * Then there are 10 unknown bytes, a date and then
 * 4 more bytes.
 * 
 */
#define TYPE_X06 0x0006


/*
 * Strange message. (Receipt?)
 * Starts with a 4-byte uin, followed by a 2-byte
 * message length. Then follows the message string
 * and 10 bytes of unknown content. Then the date
 * and 4 more bytes of unknown content.
 */
#define TYPE_X08 0x0008

 
/*
 * Probably "Incoming message". (System Request?)
 * Starts with a 4-byte uin, followed by a 2-byte
 * message length. Then follows the message string
 * and 10 bytes of unknown content. Then the date
 * and 4 more bytes of unknown content.
 */
#define TYPE_X09 0x0009


/*
 * External program.
 * First 4-byte uin. Then a length of a string and
 * that string (reason given). Then there are 
 * 10 unknown bytes and a 4 byte date. 
 * In V96 there are 5 more unknown bytes there.
 * After that
 * there are a second length and string which is 
 * the program to be run. Last there are 14 unknown
 * bytes.
 */
#define TYPE_X0A 0x000A
 

 
 /*
 * "User asked to be added"
 * Consists of an uin (4 bytes). A length (2 bytes)
 * and a string of that length, followed by 10 strange
 * bytes and then a date. And then finally 4 more
 * undefined bytes.
 */
#define TYPE_X0B 0x000B

/*
 * "You were added by someone to his/her contact list"
 * Consists of an uin (4 bytes). A length (2 bytes)
 * and a string of that length, followed by 10 strange
 * bytes and then a date. And then finally 4 more
 * undefined bytes.
 */
#define TYPE_X0C 0x000C


/*
 * Contact list
 * Consists of an uin (4 bytes). A length (2 bytes)
 * and a string of that length, followed by 10 strange
 * bytes and then a date. And then finally 4 more
 * undefined bytes. (V96+ 5 more bytes)
 */
#define TYPE_X13 0x0013


struct startfields {
	__int32 uin;
	__int16 length;
	char *string;
};

struct endfields {
	__int32 junk1;
};

struct v96data {
	__int8 junk1;
	__int32 junk2;
};