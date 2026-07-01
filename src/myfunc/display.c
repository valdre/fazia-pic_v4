#include "functions.h"


/**********************Fonction qui retourne le num�ro d'identification de la carte*******************/
/**
 * @brief Return the hardware board identifier.
 * @return uint8_t status or result code.
 */
uint8_t getid(void) {
	uint8_t id=0;
	id |= GEOA0;
	id |= GEOA1<<1;
	id |= GEOA2<<2;
	return id;
}

/**
 * @brief Convert a numeric nibble to its ASCII hexadecimal character.
 * @param c Nibble value (0-15)
 * @return uint8_t status or result code.
 */
uint8_t asciiconv(uint8_t c) {
    uint8_t asciivalue;
    asciivalue=0;
    if (c<=9) {
        asciivalue='0'+c;
    }
    if (c>9) {
        asciivalue='A'+(c-10);
    }
    return asciivalue;
}

/**
 * @brief Convert a 16-bit unsigned integer to its 4-character hexadecimal string.
 * @param value 16-bit integer to convert
 * @param s Output buffer (4 chars)
 */
void uinttoa(uint16_t value, uint8_t *s) {
    uint8_t co;
    uint16_t mask;
    mask=0xF000;
    for(co=0;co<4;co++) {
        s[co]=asciiconv((uint8_t)((value&(mask>>(4*co)))>>((3-co)*4)));
    }
}

/**
 * @brief Transmit a 16-bit unsigned integer as ASCII hex over UART.
 * @param value 16-bit integer to transmit
 * @return char status or result code.
 */
char dispuinttochar(uint16_t value) {
    uint8_t co;
    uint8_t s[4];
    uinttoa(value,s);
    for (co=0;co<4;co++) {
        putcUSART(s[co]);
    }
    return 0;
}

/**
 * @brief Transmit a 32-bit unsigned integer as binary digits over UART.
 * @param value 32-bit integer to transmit as binary
 * @return char status or result code.
 */
char dispuint32tochar(uint32_t value) {
    int8_t co;
    for (co=31;co>=0;co--) {
        putcUSART('0'+(char)((value>>co)&1));
    }
    return 0;
}


/**
 * @brief Transmit a 16-bit value as 16 binary digits over UART.
 * @param a 16-bit value to transmit as binary
 * @return char status or result code.
 */
char dispinttobin(uint16_t a) {
    int co;
    for (co=0;co<16;co++) {
        putcUSART('0'+((a>>(16-1-co))&0b00000001));
        while (BusyUSART());
    }
    return 0;
}

/**
 * @brief Transmit an 8-bit byte as 8 binary digits over UART.
 * @param c 8-bit byte to transmit as binary
 * @return char status or result code.
 */
char dispchartobin(uint8_t c) {
    int co;
    for(co=0;co<8;co++) {
        putcUSART('0'+((c>>(8-1-co))&0b00000001));
        while (BusyUSART());
    }
    return 0;
}

/**
 * @brief Append a decimal integer to the output buffer with optional terminator.
 * @param container Output buffer
 * @param a Integer value
 * @param cend Terminator character
 */
void myStrCpyUint(char *container,uint16_t a,char cend) {
    uint16_t u[5];
    uint16_t compt,comptInt;
    
    u[4] = a-10*(a/10);  //unit�s
    u[3] = a-100*(a/100)-u[4];  //dizaines
    u[3] = u[3]/10;
    u[2] = a-1000*(a/1000)-(10*u[3])-u[4];  //centaines
    u[2] = u[2]/100;
    u[1] = a-10000*(a/10000)-(100*u[2])-(10*u[3])-u[4];   //milliers
    u[1] = u[1]/1000;
    u[0] = a/10000;  //dizaines de milliers
    
    compt=0;
    comptInt=0;
    
    while (container[compt]!='\0') {
        compt++;
    }
    while ((u[comptInt]==0)&&(comptInt<5)) {
        comptInt++;
    }
    if (comptInt<5) {
        while (comptInt<5) {
            if (u[comptInt]<=9)
                container[compt++]='0'+(char)u[comptInt];
            comptInt++;
        }
    } else {
        container[compt++]='0';
    }
    if (cend!='\0') {
        container[compt++]=cend;
        container[compt]='\0';
    } else {
        container[compt]='\0';
    }
}

/**
 * @brief Append a decimal byte to the output buffer with optional terminator.
 * @param container Output buffer
 * @param a Byte value
 * @param cend Terminator character
 */
void myStrCpyByte(char *container,uint8_t a,char cend) {
    uint8_t b[3];
    uint16_t compt,comptInt;
    
    b[2] = a-10*(a/10);
    b[1] = a-100*(a/100)-b[2];
    b[1] = b[1]/10;
    b[0] = 100*(a/100);
    
    compt=0;
    comptInt=0;
    while (container[compt]!='\0') {
        compt++;
    }
    while ((b[comptInt]==0)&&(comptInt<3)) {
        comptInt++;
    }
    if (comptInt<3) {
        while (comptInt<3) {
            if (b[comptInt]<=9)
                container[compt++]='0'+(char)b[comptInt];
            comptInt++;
        }
    } else {
        container[compt++]='0';
    }
    if (cend!='\0') {
        container[compt++]=cend;
        container[compt]='\0';
    } else {
        container[compt]='\0';
    }
}

/**
 * @brief Append a const string to the output buffer with optional terminator.
 * @param container Output buffer
 * @param chaine const string to append
 * @param cend Terminator character
 */
void myStrCpyChar(char *container,char *chaine,char cend) {
    uint16_t comptChain,compt;
    
    comptChain=0;
    compt=0;
    while (container[compt]!='\0') {
        compt++;
    }
    while (chaine[comptChain]!='\0') {
        container[compt++]=chaine[comptChain++];
    }
    if (cend!='\0') {
        container[compt++]=cend;
        container[compt]='\0';
    } else {
        container[compt]='\0';
    }
}

/**
 * @brief Append a ROM string to the output buffer with optional terminator.
 * @param container Output buffer
 * @param chaine ROM string to append
 * @param cend Terminator character
 */
void myStrCpyChar2(char *container,const char *chaine,char cend) {
    uint16_t comptChain,compt;
    comptChain=0;
    compt=0;
    while (container[compt]!='\0') {
        compt++;
    }
    while (chaine[comptChain]!='\0') {
        container[compt++]=chaine[comptChain++];
    }
    if (cend!='\0') {
        container[compt++]=cend;
        container[compt]='\0';
    } else {
        container[compt]='\0';
    }
}

/**
 * @brief Append a single character to the output buffer with optional terminator.
 * @param container Output buffer
 * @param c Character to append
 * @param c Character to append
 */
void myStrCpy1Char(char *container,char c,char cend) {
    uint16_t compt;
    compt=0;
    while (container[compt]!='\0') {
        compt++;
    }
    container[compt++]=c;
    
    if (cend!='\0') {
        container[compt++]=cend;
        container[compt]='\0';
    } else {
        container[compt]='\0';
    }
}

/**
 * @brief Append a hexadecimal representation to the output buffer.
 * @param container Output buffer
 * @param a Integer value
 * @param format Hex format (8 or 16 bit)
 * @param cend Terminator character
 */
void myStrCpyHex(char *container,uint16_t a,int format,char cend) {
    uint16_t compt,comptInt;
    int co;
    char b[4];
    
    comptInt=0;
    compt=0;
    while (container[compt]!='\0') {
        compt++;
    }
    b[0] = (char)((a&0xF000)>>12);
    b[1] = (char)((a&0x0F00)>>8);
    b[2] = (char)((a&0x00F0)>>4);
    b[3] = (char)((a&0x000F));
    
    while ((b[comptInt]==0)&&(comptInt<4)) {
        comptInt++;
    }
    if (comptInt<4) {
        if ((format!=-1)&&(format>4-comptInt)) {
            for (co=(int)(4-comptInt);co<format;co++) {
                container[compt++]='0';
            }
        }
        while (comptInt<4) {
            if (b[comptInt]<=9) {
                container[compt++]='0'+b[comptInt];
            }
            if (b[comptInt]>9) {
                container[compt++]='A'+b[comptInt]-10;
            comptInt++;
            }
        }
    } else {
        if ((format!=-1)&&(format<4)) {
            for(co=0;co<format;co++) {
                container[compt++]='0';
            }
        }
    }
    if (cend!='\0') {
        container[compt++]=cend;
        container[compt]='\0';
    } else {
        container[compt]='\0';
    }   
}

