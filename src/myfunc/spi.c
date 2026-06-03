#include "functions.h"
#include <spi.h>
#include <p18cxxx.h>


extern BYTE valeur_portC;
extern struct parametres pa;


unsigned int rdspi(char id_fpga,unsigned int adr)
{
    unsigned char adr_msb;
    unsigned char adr_lsb;
    unsigned char mot[2];
    unsigned int data_value;

    adr_msb=(unsigned char)((adr&0xFF00)>>8);
    adr_lsb=(unsigned char)adr;

    if (id_fpga==1)
        valeur_portC=valeur_portC&0b11111101;

    if (id_fpga==2)
        valeur_portC=valeur_portC&0b11111011;

    PORTC=valeur_portC;

    putcSPI(adr_msb);
    putcSPI(adr_lsb);
    putcSPI(0);

    mot[0] = getcSPI();
    mot[1] = getcSPI();

    data_value=(((unsigned int)(mot[0]))<<8)+(unsigned int)(mot[1]);

    if (id_fpga==1)
        valeur_portC=valeur_portC|0b00000010;

    if (id_fpga==2)
        valeur_portC=valeur_portC|0b00000100;

    PORTC=valeur_portC;

    return data_value;
}



void wrspi(char id_fpga,unsigned int adr, unsigned int data)
{

	unsigned char adr_msb, adr_lsb;
	unsigned char data_msb,data_lsb;

	valeur_portC=valeur_portC|0b00000111;
	PORTC=valeur_portC;

	if (id_fpga==1)
		valeur_portC=valeur_portC&0b11111101;

	if (id_fpga==2)
		valeur_portC=valeur_portC&0b11111011;

	PORTC=valeur_portC;

    while(TMR2>20);
	adr_msb=(unsigned char)((adr&0xFF00)>>8);
    while(TMR2>20);
	adr_lsb=(unsigned char)adr;

	while(TMR2>20);
	data_msb=(unsigned char)(data>>8);
    while(TMR2>20);
	data_lsb=(unsigned char)data;

	while(TMR2>20);
	putcSPI(adr_msb);


	putcSPI(adr_lsb);

	putcSPI(1);
	putcSPI(data_msb);

	putcSPI(data_lsb);

	valeur_portC=valeur_portC|0b00000111;
	PORTC=valeur_portC;
}

void reset_both_fpga(void)
{
    wrspi(1,3,4);
    wrspi(2,3,4);
}

void myputsspi(int co, unsigned char *wrptr)
{
    int cp;

    for (cp=0;cp<co;cp++)
        putcSPI(wrptr[cp]);
}

BYTE set_off_V_preamp3a(UINT value) //Si1A
{   
    pa.voltage_preamp3a=value;
    wrspi(1,REG_FPGA_PA_SI1,value);
    Delay10KTCYx(100);
}

BYTE set_off_V_preamp3b(UINT value) //Si1B
{
    pa.voltage_preamp3b=value;
    wrspi(2,REG_FPGA_PA_SI1,value);
    Delay10KTCYx(100);
}

BYTE set_off_V_preamp2a(UINT value) //Si2A
{
    pa.voltage_preamp2a=value;
    wrspi(1,REG_FPGA_PA_SI2,value);
    Delay10KTCYx(100);
}

BYTE set_off_V_preamp2b(UINT value) //Si2B
{
    pa.voltage_preamp2b=value;
    wrspi(2,REG_FPGA_PA_SI2,value);
    Delay10KTCYx(100);
}

BYTE set_off_V_preamp1a(UINT value) //CSIA
{
    pa.voltage_preamp1a=value;
    wrspi(1,REG_FPGA_PA_CSI,value);
    Delay10KTCYx(100);
}

BYTE set_off_V_preamp1b(UINT value) //CSIB
{
    pa.voltage_preamp1b=value;
    wrspi(2,REG_FPGA_PA_CSI,value);
    Delay10KTCYx(100);
}

