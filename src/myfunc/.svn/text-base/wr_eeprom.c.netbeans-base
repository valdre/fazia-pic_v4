#include "functions.h"

extern struct parametres pa;

void setparam(void)
{
	unsigned int nb_bytes,co;
	unsigned char *p=(unsigned char *)&pa;
        unsigned char cflag;

        cflag=EERead(0);

        if (cflag==0x38)
        {

            nb_bytes=sizeof(pa);

            for(co=1;co<nb_bytes+1;co++)
            {
                    *p=EERead(co);
                    p++;
            }

            if (pa.voltage_preamp1b < 0x400)
              set_off_V_preamp1b(pa.voltage_preamp1b);

            if (pa.voltage_preamp2b < 0x400)
              set_off_V_preamp2b(pa.voltage_preamp2b);

            if (pa.voltage_preamp3b < 0x400)
              set_off_V_preamp3b(pa.voltage_preamp3b);

            if (pa.voltage_preamp1a < 0x400)
              set_off_V_preamp1a(pa.voltage_preamp1a);

            if (pa.voltage_preamp2a < 0x400)
              set_off_V_preamp2a(pa.voltage_preamp2a);

            if (pa.voltage_preamp3a < 0x400)
              set_off_V_preamp3a(pa.voltage_preamp3a);
        }
}

char storeparam(void)
{
    UINT nb_bytes;
    UINT co;
    BYTE *p=(unsigned char *)&pa;

    nb_bytes=sizeof(pa);

    EEWrite(0,0x38); //it is a flag to prevent that some data have been stored in a previous session

    for(co=1;co<nb_bytes+1;co++)
    {
        EEWrite(co,*p);
        p++;
    }

    return 0;
}


void EEWrite(unsigned int ad, unsigned char data)
{

	unsigned char ad_lsb,ad_msb;

	ad_lsb=(unsigned char)(ad&255);
	ad_msb=(unsigned char)((ad&768)>>8);

	EEADRH=ad_msb;
	EEADR=ad_lsb;
	EEDATA=data;
	EECON1bits.EEPGD=0;
	EECON1bits.CFGS=0;
	EECON1bits.WREN=1;
	INTCONbits.GIE=0;
	EECON2=0x55;
	EECON2=0xAA;
	EECON1bits.WR=1;
	INTCONbits.GIE=1;
	while (EECON1bits.WR);
	EECON1bits.WREN=0;
}

BYTE EERead(UINT ad)
{

	BYTE ad_lsb,ad_msb;

	ad_lsb=(BYTE)(ad&255);
	ad_msb=(BYTE)((ad&768)>>8);

	EECON1bits.EEPGD = 0;
	EECON1bits.CFGS = 0;
	EEADRH=ad_msb;
	EEADR = ad_lsb;
	EECON1bits.RD = 1;
	return EEDATA;
}

