#include "functions.h"
extern BYTE valeur_portD;

/******************* PIC A/D converter******************/

void get_PIC_AD_voltages(unsigned int *ADvoltages) {
    //VP5REFAM or AN5 measurement
    ADCON0 = 0b00010111;
    while (!PIR1bits.ADIF);
    ADCON0 = 0b00010100;
    ADvoltages[0] = (((UINT) (ADRESH)) << 8) + ADRESL;
    //the real measured value is divided by two. So we have to multiply by 2
    ADvoltages[0] = ADvoltages[0] << 1;

    //VP5REFBM or AN6 measurement
    ADCON0 = 0b00011011;
    while (!PIR1bits.ADIF);
    ADCON0 = 0b00011000;
    ADvoltages[1] = (((UINT) (ADRESH)) << 8) + ADRESL;
    //the real measured value is divided by two. So we have to multiply by 2
    ADvoltages[1] = ADvoltages[1] << 1;

    //VM27M or AN7 measurement -> no division by 2
    ADCON0 = 0b00011111;
    while (!PIR1bits.ADIF);
    ADCON0 = 0b00011100;
    ADvoltages[2] = (((UINT) (ADRESH)) << 8) + ADRESL;
}

BYTE getVoltages(char *data, char *result) {
    UINT voltages[3], decimaux,intpart;
    UINT32 prov;
    BYTE co, retval;
    char tab[18];

    get_PIC_AD_voltages(voltages);

    retval = FUNC_EXEC_OK;

    for (co = 0; co < 3; co++)
    {
        intpart = voltages[co] / 310;
        prov = ((UINT32) (voltages[co]-(intpart*310)))*3300;
        prov = prov / 1024;
        decimaux = (UINT) prov;
        tab[0+6*co] = '0' + (char) (intpart);
        tab[1+6*co] = ',';
        tab[2+6*co] = '0' + (char) (decimaux / 100);
        tab[3+6*co] = '0' + (char) (decimaux / 10)-((char) (decimaux / 100))*10;
        tab[4+6*co] = '0' + (char) (decimaux)-((char) (decimaux / 10))*10;
        tab[5+6*co] = ' ';
    }

    tab[17]='\0';

    result[0]='0'+(char)retval;
    result[1]='|';
    result[2]='v';
    result[3]=':';
    result[4]='\0';
    myStrCpyChar(result,tab,'\0');

    return FUNC_CMD_OK;
}

/***********LTC2308 converter for switching regulators*************/

BYTE getLTCswVoltages(char * data, char *result)
{
    BYTE retval;
    UINT v[8];

    // Switching regulators
    getLTC2308Voltages(0b00100000,v);

    retval = FUNC_EXEC_OK;
    result[0]='0'+(char)retval;
    result[1]='|';
    result[2]='\0';
    myStrCpyUint(result,v[0],',');
    myStrCpyUint(result,v[1],',');
    myStrCpyUint(result,v[2],',');
    myStrCpyUint(result,v[3],',');
    myStrCpyUint(result,v[4],',');
    myStrCpyUint(result,v[5],',');
    myStrCpyUint(result,v[6],',');
    myStrCpyUint(result,v[7],'\0');
    
    return FUNC_CMD_OK;
}

BYTE getLTClinVoltages(char * data, char *result)
{
    BYTE retval;
    UINT u[8];

    // Linear regulators
    getLTC2308Voltages(0b00010000,u);

    retval = FUNC_EXEC_OK;
    result[0]='0'+(char)retval;
    result[1]='|';
    result[2]='\0';
    myStrCpyUint(result,u[0],',');
    myStrCpyUint(result,u[1],',');
    myStrCpyUint(result,u[2],',');
    myStrCpyUint(result,u[3],',');
    myStrCpyUint(result,u[4],',');
    myStrCpyUint(result,u[5],',');
    myStrCpyUint(result,u[6],',');
    myStrCpyUint(result,u[7],'\0');
    
    return FUNC_CMD_OK;
}


BYTE getLTC2308Voltages(BYTE mask, UINT *ADvoltages)
{
    BYTE din,co,os;
    BYTE elt_ET,elt_OU;
    UINT mot,mot2;

    CloseSPI();
    OpenSPI(SPI_FOSC_16, MODE_00,SMPEND); // Mode (CKP,/CKE) with CKP = 0 and CKE = 1

    os=0b10001000;
    elt_ET = valeur_portD & (mask^255);
    elt_OU = valeur_portD | mask;

    PORTD = elt_ET;    //Port D returns low
    PIR1bits.SSPIF=0;
    putcSPI(os);
    while (!PIR1bits.SSPIF); //SPI transmission and reception at the same time
        mot=((UINT)SSPBUF)<<4;
    getcSPI();

    for (co=1;co<8;co++)
    {
        PORTD = elt_OU; //Port D returns high
        Delay10TCYx(3); //je suis à 2µs pile poil
        PORTD = elt_ET; //Port D returns low
        PIR1bits.SSPIF=0;
        din = os + (co<<4);
        putcSPI(din);
        while (!PIR1bits.SSPIF);
        mot=(((UINT)SSPBUF)<<4)&65520;
        mot2=(UINT)(getcSPI());
        mot+=((mot2>>4)&15);
        ADvoltages[co-1]=mot;
    }

    PORTD = elt_OU; //Port D returns high
    Delay10TCYx(3); //je suis à 2µs pil poil
    PORTD = elt_ET; //Port D returns low
    PIR1bits.SSPIF=0;
    putcSPI(os);
    while (!PIR1bits.SSPIF);
        mot=((((UINT)SSPBUF)<<4)&65520);
    mot2=(UINT)(getcSPI());
    mot+=((mot2>>4)&15);

    ADvoltages[7]=mot;

    PORTD = elt_OU; //Port D returns high

    CloseSPI();
    OpenSPI(SPI_FOSC_16, MODE_10,SMPMID);
}

