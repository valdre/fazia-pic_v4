#include "functions.h"

extern uint8_t valeur_portC;

//extern const uint16_t data_vhva1; // 200V high voltage module for A telescope
//extern const uint16_t data_vhva2; // 400V high voltage module for A telescope
//extern const uint16_t data_vhvb1; // 200V high voltage module for B telescope
//extern const uint16_t data_vhvb2; // 400V high voltage module for B telescope
//extern const uint16_t data_vhva1_2; //actual value for 200V high voltage module (telescope A) after the 20.2Mohm resistor
//extern const uint16_t data_vhva2_2; //actual value for 400V high voltage module (telescope A) after the 20.2Mohm resistor
//extern const uint16_t data_vhvb1_2; //actual value for 200V high voltage module (telescope B) after the 20.2Mohm resistor
//extern const uint16_t data_vhvb2_2; //actual value for 400V high voltage module (telescope B) after the 20.2Mohm resistor


extern uint8_t hv_slop_inc_flag;
extern uint16_t HV_borne_sup_A1; //maximum value to reach for 200V high voltage module (telescope A)
extern uint16_t HV_borne_sup_A2; //maximum value to reach for 400V high voltage module (telescope A)
extern uint16_t HV_borne_sup_B1; //maximum value to reach for 200V high voltage module (telescope B)
extern uint16_t HV_borne_sup_B2; //maximum value to reach for 400V high voltage module (telescope B)


//extern uint8_t HV_order;
//extern uint8_t cal_HV_order;
//extern uint8_t HV_direction;
//extern uint8_t dac_hv;
//extern uint16_t *data_ptr;
//extern uint16_t *data_ptr_2;
//extern uint32_t value_dec;
//extern uint32_t inc;

extern uint16_t HvValueTab[4][2];
extern uint16_t HvInc[4];
extern uint8_t HvStatus[4];
extern uint16_t HvPhysTarget[4];

/**
 * @brief Initialize the DAC8568 digital-to-analog converter with internal reference.
 * @return char status or result code.
 */
char dac_init(void) {
    unsigned char mode_sync[] = {0x06, 0, 0, 0xFF};
    unsigned char ref_intern[] = {0x09, 0x0A, 0, 0};

    valeur_portC = valeur_portC | 0b00000001; /*on remet la patte sync du DAC a 1*/
    PORTC = valeur_portC;
    valeur_portC = valeur_portC & 0b11111110; /*on impose la patte sync a� 0V pour ecrire une sequence*/
    PORTC = valeur_portC;
    myputsspi(4, mode_sync);
    valeur_portC = valeur_portC | 0b00000001; /*on remet la patte sync du DAC a 1*/
    PORTC = valeur_portC;
    valeur_portC = valeur_portC & 0b11111110; /*on impose la patte sync a� 0V pour ecrire une sequence*/
    PORTC = valeur_portC;
    myputsspi(4, ref_intern);
    valeur_portC = valeur_portC | 0b00000001; /*on remet la patte sync du DAC a 1*/
    PORTC = valeur_portC;
    return 0;
}

/**
 * @brief Send a DAC update sequence to the selected high-voltage module.
 * @param ad DAC channel address
 * @param data Data value to output
 */
void dac_sequence(char ad, unsigned int data) {
    unsigned char sequence[4];

    valeur_portC = valeur_portC | 0b00000001; /*on remet la patte sync du DAC a 1*/
    PORTC = valeur_portC;

    sequence[0] = 0x03;
    sequence[1] = ad | ((unsigned char) (((data & 0xF000) >> 12)));
    sequence[2] = (unsigned char) (((data & 0x0FF0) >> 4));
    sequence[3] = (unsigned char) (((data & 0x000F) << 4));

    valeur_portC = valeur_portC & 0b11111110; /*on impose la patte sync a� 0V pour ecrire une sequence*/
    PORTC = valeur_portC;

    myputsspi(4, sequence);

    valeur_portC = valeur_portC | 0b00000001; /*on remet la patte sync du DAC a 1*/
    PORTC = valeur_portC;
}

/**
 * @brief Configure the pulser output with the requested amplitude and timing.
 * @param data Pulse amplitude in DAC units
 * @param period Period in clock cycles
 * @param high_time High pulse duration
 * @return char status or result code.
 */
char pulser(uint16_t data, uint16_t period, uint16_t high_time) {
    dac_sequence(0x40, data);
    wrspi(2, 0x100, period);
    wrspi(2, 0x101, high_time);
    return 0;
}

/**
 * @brief Query the calibrated high-voltage power supply status via SPI.
 * @param str Output buffer for calibration status string
 */
void ask_hv_calibration(char *str) {
    uint8_t co, cp;
    uint8_t nbmodules; //contains the number of high voltage modules which are calibrated
    nbmodules = 0;
    str[0] = '0';
    cp = 1;
    for (co = 0; co < 4; co++) {
        if (EERead(EEPROM_CAL_HV_CALIBRATED + co) == SPI_KEY) {
            nbmodules++;
            str[0] = (char) nbmodules + '0';
            str[cp++] = ',';
            str[cp++] = 'A' + (char) ((co & 0b10) >> 1);
            str[cp++] = '1' + (char) (co & 0b01);
        }
    }
    str[cp] = '\0';
}

static uint32_t read_eeprom_s16(uint16_t addr) {
    return  ((uint32_t)EERead(addr + 3) << 24) |
            ((uint32_t)EERead(addr + 2) << 16) |
            ((uint32_t)EERead(addr + 1) <<  8) |
            (uint32_t)EERead(addr);
}

/**
 * @improve : Transfrom a reading of value in EEprom to a 2 value  in eeprom (instead of ~30,40) 
 * and then create a equation to get the desired values
 * @brief Convert a high-voltage target value to DAC decimal output code.
 * @param tension Target voltage in volts
 * @param eeprom_adr_coeff EEPROM address for the DAC calibration linear coefficient
 * 
 * @return value uint32_t DAC code corresponding to the target voltage
 */

uint32_t get_value_dec(uint32_t tension, uint16_t eeprom_adr_coeff, uint16_t eeprom_adr_const) {
    uint32_t dac_cal_linear_coeff = read_eeprom_s16(eeprom_adr_coeff);
    uint32_t dac_cal_linear_const = read_eeprom_s16(eeprom_adr_const);
    /* Integer-only arithmetic for PIC18. */
    return (dac_cal_linear_coeff * (uint32_t)tension  + dac_cal_linear_const) / COEFF_SCALE_FACTOR;
}

/**
 * @brief Apply a high-voltage ramp with configurable slope to reach target voltage.
 * @param tel Telescope (A or B)
 * @param module Module (1 or 2)
 * @param tension Target voltage
 * @param slopeVS Ramp slope
 * @return uint8_t status or result code.
 */ 
uint8_t slop_vhv(char tel, uint8_t module, uint16_t tension, uint32_t slopeVS) {
    uint8_t verdict            = FUNC_EXEC_BAD_ARGS_TYPE;
    uint32_t value_dec        = 0;
    uint32_t inc              = 0;
    uint32_t default_value    = 0;
    uint32_t max_dac          = 0;
    uint32_t coef               = 0;
    uint16_t calibration_addr_coeff = 0;
    uint16_t calibration_addr_const = 0;
    uint8_t use_linear         = 0;
    uint8_t channel = module - 1 + 2 * ((uint8_t)(tel - 'A'));

    if ((module == 1) && (tel == 'A')) {
        coef = coefHV_M200;
        max_dac = (uint32_t)HVSi1Max * coef / 1000;
        calibration_addr_coeff = EEPROM_CAL_DAC_A1_LINEAR_COEFF;
        calibration_addr_const = EEPROM_CAL_DAC_A1_LINEAR_CONST;
        use_linear = (EERead(EEPROM_IS_CAL_HV_DISCRET) == 0);
    } else if ((module == 1) && (tel == 'B')) {
        coef = coefHV_M200;
        max_dac = (uint32_t)HVSi1Max * coef / 1000;
        calibration_addr_coeff = EEPROM_CAL_DAC_B1_LINEAR_COEFF;
        calibration_addr_const = EEPROM_CAL_DAC_B1_LINEAR_CONST;
        use_linear = (EERead(EEPROM_IS_CAL_HV_DISCRET + 1) == 0);
    } else if ((module == 2) && (tel == 'A')) {
        coef = coefHV_M400;
        max_dac = (uint32_t)HVSi2Max * coef / 1000;
        calibration_addr_coeff = EEPROM_CAL_DAC_A2_LINEAR_COEFF;
        calibration_addr_const = EEPROM_CAL_DAC_A2_LINEAR_CONST;
        use_linear = ((EERead(EEPROM_IS_CAL_HV_DISCRET + 2) == 0) && (tension <= HVSi2Max));
    } else if ((module == 2) && (tel == 'B')) {
        coef = coefHV_M400;
        max_dac = (uint32_t)HVSi2Max * coef / 1000;
        calibration_addr_coeff = EEPROM_CAL_DAC_B2_LINEAR_COEFF;
        calibration_addr_const = EEPROM_CAL_DAC_B2_LINEAR_CONST;
        use_linear = ((EERead(EEPROM_IS_CAL_HV_DISCRET + 3) == 0) && (tension <= HVSi2Max));
    }

    if (coef != 0) {
        default_value = ((uint32_t)tension) * coef / 1000;
        if (use_linear) {
            value_dec = get_value_dec(tension, calibration_addr_coeff, calibration_addr_const);
            if (value_dec > max_dac) {
                value_dec = default_value;
            }
        } else {
            value_dec = default_value;
        }
        inc = (slopeVS * coef) / 1000;
        HvValueTab[channel][1] = (uint16_t)value_dec;
        HvInc[channel] = (uint16_t)inc;
        HvStatus[channel] = 0;
        verdict = FUNC_EXEC_OK;
    }

    return verdict;
}



