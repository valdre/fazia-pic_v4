#include "functions.h"

extern BYTE valeur_portB;
extern BYTE valeur_portD;
//extern ram unsigned int data_vhva1; // 200V high voltage module for A telescope
//extern ram unsigned int data_vhva2; // 400V high voltage module for A telescope
//extern ram unsigned int data_vhvb1; // 200V high voltage module for B telescope
//extern ram unsigned int data_vhvb2; // 400V high voltage module for B telescope
extern ram UINT HV_borne_sup_A1; //maximum value to reach for 200V high voltage module (telescope A)
extern ram UINT HV_borne_sup_A2; //maximum value to reach for 400V high voltage module (telescope A)
extern ram UINT HV_borne_sup_B1; //maximum value to reach for 200V high voltage module (telescope B)
extern ram UINT HV_borne_sup_B2; //maximum value to reach for 400V high voltage module (telescope B)
extern UINT32 coefA_A1;
extern UINT32 coefA_A2;
extern UINT32 coefA_B1;
extern UINT32 coefA_B2;
extern UINT32 coefB_A1;
extern UINT32 coefB_A2;
extern UINT32 coefB_B1;
extern UINT32 coefB_B2;

extern UINT HvValueTab[4][2];
extern UINT HvInc[4];
extern BYTE HvStatus[4];
extern UINT HvPhysCorrect[4];
extern UINT32 time_scheduling;
extern UINT32 time_scheduling_copy;

/**
 * @brief Initialize the ADS8332 analog-to-digital converter and configure SPI communication.
 */
void adc_init(void)
{
    unsigned char sequence[2];
    unsigned char co;
    /*we make sure that the CONVST pin is high*/
    valeur_portD = valeur_portD|0b01000000;
    PORTD = valeur_portD;

    /*-----------------------------------------*/
    /*-------Initialisation de l'ADC-----------*/
    /*--------------Ecriture CFR---------------*/
    /*le Chip select de l'ADC est mis a 1*/
    valeur_portB=valeur_portB|0b00100000;
    PORTB=valeur_portB;
    sequence[0]=0xE7;
    sequence[1]=0xFD;
    /*le Chip select de l'ADC est mis a 0*/
    valeur_portB=valeur_portB&0b11011111;
    PORTB=valeur_portB;
    putcSPI(sequence[0]);
    putcSPI(sequence[1]);
    /*le Chip select de l'ADC est mis a 1*/
    valeur_portB=valeur_portB|0b00100000;
    PORTB=valeur_portB;
}

/**
 * @brief Measure and compute the leakage current from an ADC channel reading.
 * @param tel Telescope identifier (A or B)
 * @param module Module number (1 or 2)
 * @param lcAdcRead Pointer to input/output buffer containing command data..
 * @return UINT status or result code.
 */
UINT leak_current(char tel, char module, UINT32 lcAdcRead)
{
    BYTE canal,sign,ad;
    UINT32 *coeffA,*coeffB,inside_current;
    UINT lc;
    static UINT32 leakage_current,leakage_current_na,tns;

    lc=0;

    if (module=='1')
    {
        if (tel=='B')
        {
            coeffA=&coefA_B1;
            coeffB=&coefB_B1;
            tns=HvPhysCorrect[2];
            canal=2;
            ad=EEPROM_CAL_HV_FIRST_ADR+2*EEPROM_CAL_HV_WIDTH+8;
            inside_current = get_value_dec(tns,EEPROM_SI1B_CAL_IHV_DISCRET);
        }

        if (tel=='A')
        {
            coeffA=&coefA_A1;
            coeffB=&coefB_A1;
            tns=HvPhysCorrect[0];
            canal=0;
            ad=EEPROM_CAL_HV_FIRST_ADR+8;
            inside_current = get_value_dec(tns,EEPROM_SI1A_CAL_IHV_DISCRET);
        }
    }

    if (module=='2')
    {
        if (tel=='A')
        {
            coeffA=&coefA_A2;
            coeffB=&coefB_A2;
            tns=HvPhysCorrect[1];
            canal=1;
            ad=EEPROM_CAL_HV_FIRST_ADR+EEPROM_CAL_HV_WIDTH+8;
            inside_current = get_value_dec(tns,EEPROM_SI2A_CAL_IHV_DISCRET);
        }

        if (tel=='B')
        {
            coeffA=&coefA_B2;
            coeffB=&coefB_B2;
            tns=HvPhysCorrect[3];
            canal=3;
            ad=EEPROM_CAL_HV_FIRST_ADR+3*EEPROM_CAL_HV_WIDTH+8;
            inside_current = get_value_dec(tns,EEPROM_SI2B_CAL_IHV_DISCRET);
        }
    }
    
    if (inside_current>30000) {
		while (TMR2>50);
		inside_current=(*coeffA)*tns;
		while (TMR2>50);
		inside_current=inside_current/10000;
		
		sign=EERead(ad);
		
		if(sign) inside_current=inside_current+(*coeffB);
		else {
			if (inside_current>(*coeffB)) inside_current=inside_current-(*coeffB);
			else inside_current=0;
			//else inside_current=(*coeffB)-inside_current; //WRONG!
		}
	}
    
    
    if (lcAdcRead>=inside_current) leakage_current=lcAdcRead-inside_current;
	else leakage_current=0;
    //if (lcAdcRead<inside_current) leakage_current=inside_current-lcAdcRead; //WRONG!
    
//    if ((module=='1')&&(tel=='B'))
//    {
//        printf("-------------\r\n");
//        while(BusyUSART());
//        
//        printf("tns:%lu\r\n",tns);
//        while(BusyUSART());
//    
//        printf("lcAdcRead:%lu\r\n",lcAdcRead);
//        while(BusyUSART());
//        
//        printf("inside_current:%lu\r\n",inside_current);
//        while(BusyUSART());
//        
//        printf("diff:%lu\r\n",leakage_current);
//        while(BusyUSART());
//    }

    while (TMR2>20);
    leakage_current_na=leakage_current*9683; //correspond � 2.5/(6.62*39000)*10?
    while (TMR2>20);
    leakage_current_na=(leakage_current_na&0xFFFF0000); //je divise par 65536
    while (TMR2>20);
    leakage_current_na=leakage_current_na>>8;
    while (TMR2>20);
    leakage_current_na=leakage_current_na>>8;
    
    
    lc=(UINT)leakage_current_na;
    return lc;
}



/**
 * @brief Read a value from the specified ADC channel via SPI.
 * @param canal ADC channel number
 * @return UINT status or result code.
 */
UINT adc_getvalue(unsigned char *canal)
{
    static BYTE data[2];
    static UINT value,value2;
    static unsigned char sequence[2];

    valeur_portB=valeur_portB|0b00100000;
    PORTB=valeur_portB;

    Delay10TCYx(1);

    sequence[0]=(*canal)<<4;
    sequence[1]=0;

    /*le Chip select de l'ADC est mis a 0*/
    valeur_portB=valeur_portB&0b11011111;
    PORTB=valeur_portB;
    
    putcSPI(sequence[0]);
    putcSPI(sequence[1]);

    /*le Chip select de l'ADC est mis a 1*/
    valeur_portB=valeur_portB|0b00100000;
    PORTB=valeur_portB;
    
    Delay10TCYx(1);

    /*on toggle le signal CONVST*/
    valeur_portD = valeur_portD&0b10111111;
    PORTD = valeur_portD;
    Delay10TCYx(1); /*d�lai de 156 ns. En r�alit�, 40 ns suffisent*/
    valeur_portD = valeur_portD|0b01000000;
    PORTD = valeur_portD;

    /*On attend que la fin de conversion se termine*/
    while(EOC==0);

    Delay10TCYx(1);

    /*on remonte le Chip select*/
    valeur_portB=valeur_portB|0b00100000;
    PORTB=valeur_portB;

    Delay10TCYx(1);

    /*On descend le Chip select*/
    valeur_portB=valeur_portB&0b11011111;
    PORTB=valeur_portB;

    data[0]=getcSPI();
    data[1]=getcSPI();

    valeur_portB=valeur_portB|0b00100000;
    PORTB=valeur_portB;
    
    //value = ((unsigned int)data[0]<<8) + (unsigned int)data[1];
    value2 = (UINT)data[0];
    value = (UINT)data[1];
    while (TMR2>50);
    value2 = value2<<8;
    
    value = value + value2;

    return value;
}

