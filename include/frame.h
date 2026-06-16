#ifndef _FRAME_H_
#define _FRAME_H_

#include <Generic.h>

/**
 * @file frame.h
 * @brief UART protocol frame functions and constants.
 */

#define KW_STX       0x02
#define KW_END       0x03
#define KW_ACK       0x06
#define KW_NAK       0x15
#define KW_TOUT      0x18
#define KW_ERR       0x1A
#define KW_ESC       0x1B

enum slaveid {
    FEC0 = 0x30,
    FEC1,
    FEC2,
    FEC3,
    FEC4,
    FEC5,
    FEC6,
    FEC7,
    PSC,
    BC,
    MUL1 = 0x41,
    MUL2,
    MUL3,
    MUL4,
    MUL5,
    BRO
};

#define MIN_FRAME_SIZE   9
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
#define SEQNUM_DELIMITER        0x23

/**
 * @brief Initialize a communication frame buffer.
 * @param f Pointer to the frame buffer.
 */
void frame_init(unsigned char *f);

/**
 * @brief Compute the CRC for a protocol frame.
 * @param f Frame buffer.
 * @return Computed CRC value.
 */
uint16_t frame_docrc(unsigned char *f);

#define is_startkw(ch)      ((ch == KW_STX) || (ch == KW_ACK) || (ch == KW_ESC) || (ch == KW_NAK) || (ch == KW_ERR))
#define is_endkw(ch)        (ch == KW_END)
#define is_hpcmd(ch)        ((ch == RESET_CMD) || (ch == CLEAR_CMD) || (ch == ECHO_CMD))
#define frame_getkw(f)      (f[0])
#define frame_getids(f)     (f[4])
#define frame_getcmd(f)     (f[5])

/**
 * @brief Extract the IDB field from a protocol frame.
 * @param f Frame buffer.
 * @return Decoded IDB value.
 */
uint16_t frame_getidb(unsigned char *f);

/**
 * @brief Extract the sequence byte from a protocol frame.
 * @param f Frame buffer.
 * @return Sequence identifier.
 */
unsigned char frame_getseq(unsigned char *f);

/**
 * @brief Copy payload data from a protocol frame into a destination buffer.
 * @param f Frame buffer.
 * @param d Output data buffer.
 */
void frame_getdata(unsigned char *f, char *d);

/**
 * @brief Extract the CRC stored in a protocol frame.
 * @param f Frame buffer.
 * @return Stored CRC value.
 */
uint16_t frame_getcrc(unsigned char *f);

#endif

// EOF
