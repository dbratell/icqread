#define TRUE 1
#define FALSE 0


#define INTRO_V2 0x0072

/*
 * Incoming (and outgoing?) message.
 * Starts with a 4-byte uin, followed by a 2-byte
 * message length. Then follows the message string
 * and 10 bytes of unknown content. Then the date
 * and 4 more bytes of unknown content.
 */
#define TYPE_X01 0x0001

 
 
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
