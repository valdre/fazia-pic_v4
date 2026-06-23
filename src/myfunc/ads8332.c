#include "functions.h"

extern uint8_t valeur_portB;
extern uint8_t valeur_portD;
//extern const unsigned int data_vhva1; // 200V high voltage module for A telescope
//extern const unsigned int data_vhva2; // 400V high voltage module for A telescope
//extern const unsigned int data_vhvb1; // 200V high voltage module for B telescope
//extern const unsigned int data_vhvb2; // 400V high voltage module for B telescope
extern const uint16_t HV_borne_sup_A1; //maximum value to reach for 200V high voltage module (telescope A)
extern const uint16_t HV_borne_sup_A2; //maximum value to reach for 400V high voltage module (telescope A)
extern const uint16_t HV_borne_sup_B1; //maximum value to reach for 200V high voltage module (telescope B)
extern const uint16_t HV_borne_sup_B2; //maximum value to reach for 400V high voltage module (telescope B)
extern uint32_t coefA_A1;
extern uint32_t coefA_A2;
extern uint32_t coefA_B1;
extern uint32_t coefA_B2;
extern uint32_t coefB_A1;
extern uint32_t coefB_A2;
extern uint32_t coefB_B1;
extern uint32_t coefB_B2;

extern uint16_t HvValueTab[4][2];
extern uint16_t HvInc[4];
extern uint8_t HvStatus[4];
extern uint16_t HvPhysCorrect[4];
extern uint32_t time_scheduling;
extern uint32_t time_scheduling_copy;

/**
 * @brief Initialize the ADS8332 analog-to-digital converter and configure SPI communication.
 */
void adc_init(void) {
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
    valeur_portB=valeur_portB & 0b11011111;
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
 * @return uint16_t status or result code.
 */
uint16_t leak_current(char tel, char module, uint32_t lcAdcRead) {
    uint8_t   canal;
    uint8_t   sign;
    uint8_t   ad;
    uint32_t *coeffA;
    uint32_t *coeffB;
    uint32_t inside_current;
    uint32_t corrected_voltage;
    uint16_t   lc = 0;
    static uint32_t leakage_current;
    static uint32_t leakage_current_na;
    
    if (module == '1') {
        if (tel == 'B') {
            coeffA  =& coefA_B1;
            coeffB  =& coefB_B1;
            corrected_voltage = HvPhysCorrect[2];
            canal   = 2;
            ad      = EEPROM_CAL_HV_FIRST_ADR + 2 * EEPROM_CAL_HV_WIDTH + 8;
            inside_current = get_value_dec(corrected_voltage, EEPROM_CAL_ADC_B1_LINEAR_COEFF, EEPROM_CAL_ADC_B1_LINEAR_CONST);
        } else if (tel == 'A') {
            coeffA  =& coefA_A1;
            coeffB  =& coefB_A1;
            corrected_voltage = HvPhysCorrect[0];
            canal   = 0;
            ad      = EEPROM_CAL_HV_FIRST_ADR + 8;
            inside_current = get_value_dec(corrected_voltage, EEPROM_CAL_ADC_A1_LINEAR_COEFF, EEPROM_CAL_ADC_A1_LINEAR_CONST);
        }
    }
    if (module == '2') {
        if (tel == 'A') {
            coeffA  =& coefA_A2;
            coeffB  =& coefB_A2;
            corrected_voltage = HvPhysCorrect[1];
            canal   = 1;
            ad      = EEPROM_CAL_HV_FIRST_ADR + EEPROM_CAL_HV_WIDTH + 8;
            inside_current = get_value_dec(corrected_voltage, EEPROM_CAL_ADC_A2_LINEAR_COEFF, EEPROM_CAL_ADC_A2_LINEAR_CONST);
        } else if (tel == 'B') {
            coeffA  =& coefA_B2;
            coeffB  =& coefB_B2;
            corrected_voltage = HvPhysCorrect[3];
            canal   = 3;
            ad      = EEPROM_CAL_HV_FIRST_ADR + 3 * EEPROM_CAL_HV_WIDTH + 8;
            inside_current = get_value_dec(corrected_voltage, EEPROM_CAL_ADC_B2_LINEAR_COEFF, EEPROM_CAL_ADC_B2_LINEAR_CONST);
        }
    }
    
    if (inside_current > 30000) {
		while (TMR2 > 50);
		inside_current = (*coeffA) * corrected_voltage;
		while (TMR2 > 50);
		inside_current = inside_current / 10000;
		
		sign = EERead(ad);
		
		if(sign) {
            inside_current = inside_current + (*coeffB);
        } else {
			if (inside_current > (*coeffB)){
                inside_current = inside_current - (*coeffB);
            } else {
                inside_current = 0;
            }
		}
	}
    if (lcAdcRead >= inside_current) { 
        leakage_current = lcAdcRead - inside_current;
    } else {
        leakage_current = 0;
    }
    while (TMR2 > 20); // wait 
    leakage_current_na = leakage_current * 9683; //correspond a 2.5/(6.62*39000)*10?
    while (TMR2 > 20); // wait
    leakage_current_na = (leakage_current_na & 0xFFFF0000); //je divise par 65536
    while (TMR2 > 20); // wait
    leakage_current_na = leakage_current_na >> 8;
    while (TMR2 > 20); // wait
    leakage_current_na = leakage_current_na >> 8;
    lc = (uint16_t)leakage_current_na;
    return lc;
}



/**
 * @brief Read a value from the specified ADC channel via SPI.
 * @param canal ADC channel number
 * @return uint16_t status or result code.
 */
uint16_t adc_getvalue(unsigned char *canal)
{
    static uint8_t data[2];
    static uint16_t value,value2;
    static unsigned char sequence[2];

    valeur_portB = valeur_portB|0b00100000;
    PORTB = valeur_portB;

    Delay10TCYx(1);

    sequence[0] = (*canal)<<4;
    sequence[1] = 0;

    /*le Chip select de l'ADC est mis a 0*/
    valeur_portB = valeur_portB & 0b11011111;
    PORTB = valeur_portB;
    
    putcSPI(sequence[0]);
    putcSPI(sequence[1]);

    /*le Chip select de l'ADC est mis a 1*/
    valeur_portB = valeur_portB|0b00100000;
    PORTB = valeur_portB;
    
    Delay10TCYx(1);

    /*on toggle le signal CONVST*/
    valeur_portD = valeur_portD & 0b10111111;
    PORTD = valeur_portD;
    Delay10TCYx(1); /*d�lai de 156 ns. En r�alit�, 40 ns suffisent*/
    valeur_portD = valeur_portD|0b01000000;
    PORTD = valeur_portD;

    /*On attend que la fin de conversion se termine*/
    while(EOC == 0);

    Delay10TCYx(1);

    /*on remonte le Chip select*/
    valeur_portB = valeur_portB|0b00100000;
    PORTB = valeur_portB;

    Delay10TCYx(1);

    /*On descend le Chip select*/
    valeur_portB = valeur_portB  &  0b11011111;
    PORTB = valeur_portB;

    data[0] = getcSPI();
    data[1] = getcSPI();

    valeur_portB = valeur_portB|0b00100000;
    PORTB = valeur_portB;
    
    //value = ((unsigned int)data[0]<<8) + (unsigned int)data[1];
    value2 = (uint16_t)data[0];
    value = (uint16_t)data[1];
    while (TMR2 > 50);
    value2 = value2<<8;
    value = value + value2;

    return value;
}

