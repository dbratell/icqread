#define TRUE 1
#define FALSE 0

#define UNKNOWN "<unknown>"


#define INTRO_V72 0x0072
#define INTRO_V73 0x0073
#define INTRO_V74 0x0074
#define INTRO_V78 0x0078
#define INTRO_V8B 0x008B
#define INTRO_V96 0x0096
#define INTRO_V97 0x0097
#define INTRO_V98 0x0098
#define INTRO_V9C 0x009C

/*
 * Incoming (and outgoing?) message.
 * Starts with a 4-byte uin, followed by a 2-byte
 * message length. Then follows the message string
 * and 10 bytes of unknown content. Then the date
 * and 4 more bytes of unknown content.
 *
 * In V96 (V8B?) there seems to be 5 more bytes at the end.
 * Strange odd number.
 */
#define TYPE_MESS 0x0001

/*
 * Incoming (and outgoing?) chat.
 * Starts with a 4-byte uin, followed by a 2-byte
 * message length. Then follows the message string
 * and first 4 bytes of unknown content. They are
 * mostly FFFFFFFF and 00000000. Then there are 
 * another 4-byte group. They seem to be 00000000 when
 * a message arrived from somewhere else and 00000001
 * when a message was sent. Then there are two more
 * bytes which probably is the version number. 
 * Then the date
 * and another length of 2 bytes followed by a new
 * string of that length. Finally there are 4 more 
 * bytes of unknown content.
 */
#define TYPE_CHAT 0x0002
 
 
/*
 * Incoming (and outgoing?) file.
 * Starts with a 4-byte uin, followed by a 2-byte
 * message length. Then follows the message string 
 * containing the description of the file.
 * Then there are 10 bytes of unknown content
 * followed by the 4 byte date and 
 * only in V96 (V8B?) and later 5 unknown bytes. Then there
 * is another length. 
 * Then there is a string (filename) of the the specified
 * length. Then there are 4 bytes which is the 
 * filelength. 
 * Then there are another 2-byte length and a string
 * of that length. 
 * And last there are 4 bytes of unknown content.
 */
#define TYPE_FILE 0x0003

 
 
 /*
 * Incoming (and outgoing?) URL.
 * Starts with a 4-byte uin, followed by a 2-byte
 * message length. Then follows the message string
 * and 10 bytes of unknown content. Then the date
 * and 4 more bytes of unknown content.
 *
 * In V96 (V8B?) there seems to be 5 more bytes at the end.
 * Strange odd number.
 */
#define TYPE_URL 0x0004

 
 
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


/* Unknown. Found with Bjoern. */
#define TYPE_X07 0x0007

 
 /*
 * Strange message. (Receipt?)
 * Starts with a 4-byte uin, followed by a 2-byte
 * message length. Then follows the message string
 * and 10 bytes of unknown content. Then the date
 * and 4 more bytes of unknown content.
 */
#define TYPE_X08 0x0008

 
/*
 * Probably "System Message". (System Request?)
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
 * In V96 (V8B?) there are 5 more unknown bytes there.
 * After that
 * there are a second length and string which is 
 * the program to be run. Last there are 14 unknown
 * bytes.
 */
#define TYPE_EXTERNAL 0x000A
 

 
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
 * Something with mail.
 * First uin (system = 1). 4 bytes.
 * Then length (2 bytes) and string if that length.
 * Then the normal 10 bytes and the date (4 bytes).
 * Then the v96(V8B?)-part and the final 4 bytes.
 */
#define TYPE_MAIL 0x000F

/*
 * Contact list
 * Consists of an uin (4 bytes). A length (2 bytes)
 * and a string of that length, followed by 10 strange
 * bytes and then a date. And then finally 4 more
 * undefined bytes. (V96 (V8B?)+ 5 more bytes)
 */
#define TYPE_CONTACTLIST 0x0013

#if _MSC_VER>=1100
/* To be able to read directly into the struct */
#pragma pack(push,1)
#endif

struct startfields {
	__int16 type;
	__int32 uin;
	__int16 length;
	char *string;
	__int32 status;
	__int32 destination;
	__int16 protocolversion;
	__int32 date;
	/* These two only in v96 (V8B?) and later */
	__int8 junk2;
	__int32 junk3;
};
#if _MSC_VER>=1100
#pragma pack(pop)
#endif

/* The format of the statusfield is mostly unknown 
 * but I have gathered the following data:
 *
 * -1 (0xFFFFFFFF) Recived message ok.
 * 0 = Sent message arrived ok.
 * 4 = Receiver was "away"
 * 6 = No contact with receiver.
 * 7 = User has canceled the chat request before you answered it.
 *
 */

struct endfields {
	__int32 junk1;
};

struct textdata {
	__int16 length;
	char *string;
};
