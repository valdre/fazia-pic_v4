#ifndef _FRAME_H_
#define _FRAME_H_

#include <Generic.h>

#define KW_STX       0x02
#define KW_END       0x03
#define KW_ACK       0x06
#define KW_NAK       0x15
#define KW_TOUT      0x18       // timeout from slave
#define KW_ERR       0x1A
#define KW_ESC       0x1B       // high priority start of packet

//#define DEBUG_FRAME

enum slaveid {
    FEC0 = 0x30,    // '0'
    FEC1,           // '1'
    FEC2,           // '2'
    FEC3,           // '3'
    FEC4,           // '4'
    FEC5,           // '5'
    FEC6,           // '6'
    FEC7,           // '7'
    PSC,            // '8'
    BC,             // '9'
    MUL1 = 0x41,    // 'A'
    MUL2,           // 'B'
    MUL3,           // 'C'
    MUL4,           // 'D'
    MUL5,           // 'E'
    BRO             // 'F'
};

#define MIN_FRAME_SIZE   9     // STX, IDB(1), IDB(2), IDB(3), IDS, CMD, ETX, CRC(1), CRC(2)
#define MAX_FRAME_SIZE  (MAX_DATA_SIZE + MIN_FRAME_SIZE)
#define MAX_DATA_SIZE   256

#define SMALL_DATA_SIZE  64

#define BASE_HPCMD_ID   0x80
#define MAX_HPCMD_ID    0x82

#define BASE_CMD_ID     0x83
#define MAX_CMD_ID      0xFF

#define RESET_CMD       0x80
#define CLEAR_CMD       0x81
#define ECHO_CMD        0x82

#define ERR_FRAME_NONE          0x00
#define ERR_FRAME_TOO_LONG      0x01
#define ERR_FRAME_TOO_SHORT     0x02
#define ERR_FRAME_INCOMPLETE    0x03
#define ERR_FRAME_EMPTY         0x04

#define SEQNUM_DELIMITER        0x23   // '#'

void frame_init(unsigned char *f);
UINT frame_docrc(unsigned char *f);

#define is_startkw(ch)      ((ch == KW_STX) || (ch == KW_ACK) || (ch == KW_ESC) || (ch == KW_NAK) || (ch == KW_ERR))
#define is_endkw(ch)        (ch == KW_END)
#define is_hpcmd(ch)        ((ch == RESET_CMD) || (ch == CLEAR_CMD) || (ch == ECHO_CMD))

#define frame_getkw(f)      (f[0])
#define frame_getids(f)     (f[4])
#define frame_getcmd(f)     (f[5])

UINT frame_getidb(unsigned char *f);
unsigned char frame_getseq(unsigned char *f);
void frame_getdata(unsigned char *f, char *d);
UINT frame_getcrc(unsigned char *f);

#endif

// EOF


