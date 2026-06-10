#include "functions.h"

extern BYTE valeur_portC;

//extern ram UINT data_vhva1; // 200V high voltage module for A telescope
//extern ram UINT data_vhva2; // 400V high voltage module for A telescope
//extern ram UINT data_vhvb1; // 200V high voltage module for B telescope
//extern ram UINT data_vhvb2; // 400V high voltage module for B telescope
//extern ram UINT data_vhva1_2; //actual value for 200V high voltage module (telescope A) after the 20.2Mohm resistor
//extern ram UINT data_vhva2_2; //actual value for 400V high voltage module (telescope A) after the 20.2Mohm resistor
//extern ram UINT data_vhvb1_2; //actual value for 200V high voltage module (telescope B) after the 20.2Mohm resistor
//extern ram UINT data_vhvb2_2; //actual value for 400V high voltage module (telescope B) after the 20.2Mohm resistor


extern ram BYTE hv_slop_inc_flag;
extern ram UINT HV_borne_sup_A1; //maximum value to reach for 200V high voltage module (telescope A)
extern ram UINT HV_borne_sup_A2; //maximum value to reach for 400V high voltage module (telescope A)
extern ram UINT HV_borne_sup_B1; //maximum value to reach for 200V high voltage module (telescope B)
extern ram UINT HV_borne_sup_B2; //maximum value to reach for 400V high voltage module (telescope B)


//extern BYTE HV_order;
//extern BYTE cal_HV_order;
//extern BYTE HV_direction;
//extern BYTE dac_hv;
//extern UINT *data_ptr;
//extern UINT *data_ptr_2;
//extern UINT32 value_dec;
//extern UINT32 inc;

extern UINT HvValueTab[4][2];
extern UINT HvInc[4];
extern BYTE HvStatus[4];
extern UINT HvPhysTarget[4];

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
char pulser(UINT data, UINT period, UINT high_time) {
    dac_sequence(0x40, data);
    wrspi(2, 0x100, period);
    wrspi(2, 0x101, high_time);
}

/**
 * @brief Query the calibrated high-voltage power supply status via SPI.
 * @param str Output buffer for calibration status string
 */
void ask_hv_calibration(char *str) {
    BYTE co, cp;
    BYTE nbmodules; //contains the number of high voltage modules which are calibrated
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

/**
 * @improve : Transfrom a reading of value in EEprom to a 2 value  in eeprom (instead of ~30,40) 
 * and then create a equation to get the desired values
 * @brief Convert a high-voltage target value to DAC decimal output code.
 * @param tension Target voltage in volts
 * @param eeprom_adr_coeff EEPROM address for the DAC calibration linear coefficient
 * 
 * @return value UINT32 DAC code corresponding to the target voltage
 */

UINT32 get_value_dec(UINT tension, UINT eeprom_adr_coeff, UINT eeprom_adr_const) {
    float dac_cal_linear_coeff = ((float) 
        (EERead(eeprom_adr_coeff + 3) << 24) + 
        (EERead(eeprom_adr_coeff + 2) << 16) + 
        (EERead(eeprom_adr_coeff + 1) <<  8) + 
        EERead(eeprom_adr_coeff));
    float dac_cal_linear_const = ((float)
        (EERead(eeprom_adr_const + 3) << 24) +
        (EERead(eeprom_adr_const + 2) << 16) +
        (EERead(eeprom_adr_const + 1) <<  8) +
        EERead(eeprom_adr_const));
    return (UINT32) ((dac_cal_linear_coeff * tension) + dac_cal_linear_const);
}

/**
 * @brief Apply a high-voltage ramp with configurable slope to reach target voltage.
 * @param tel Telescope (A or B)
 * @param module Module (1 or 2)
 * @param tension Target voltage
 * @param slopeVS Ramp slope
 * @return BYTE status or result code.
 */ 
BYTE slop_vhv(char tel, BYTE module, UINT tension, UINT32 slopeVS) {
    BYTE verdict            = FUNC_EXEC_BAD_ARGS_TYPE;
    UINT32 value_dec        = 0;
    UINT32 inc              = 0;
    UINT32 default_value    = 0;
    UINT32 max_dac          = 0;
    UINT coef               = 0;
    UINT calibration_addr_coeff = 0;
    UINT calibration_addr_const = 0;
    BYTE use_linear         = 0;
    BYTE channel = module - 1 + 2 * ((BYTE)(tel - 'A'));

    if ((module == 1) && (tel == 'A')) {
        coef = coefHV_M200;
        max_dac = (UINT32)HVSi1Max * coef / 1000;
        calibration_addr_coeff = EEPROM_CAL_DAC_A1_LINEAR_COEFF;
        calibration_addr_const = EEPROM_CAL_DAC_A1_LINEAR_CONST;
        use_linear = (EERead(EEPROM_IS_CAL_HV_DISCRET) == 0);
    } else if ((module == 1) && (tel == 'B')) {
        coef = coefHV_M200;
        max_dac = (UINT32)HVSi1Max * coef / 1000;
        calibration_addr_coeff = EEPROM_CAL_DAC_B1_LINEAR_COEFF;
        calibration_addr_const = EEPROM_CAL_DAC_B1_LINEAR_CONST;
        use_linear = (EERead(EEPROM_IS_CAL_HV_DISCRET + 1) == 0);
    } else if ((module == 2) && (tel == 'A')) {
        coef = coefHV_M400;
        max_dac = (UINT32)HVSi2Max * coef / 1000;
        calibration_addr_coeff = EEPROM_CAL_DAC_A2_LINEAR_COEFF;
        calibration_addr_const = EEPROM_CAL_DAC_A2_LINEAR_CONST;
        use_linear = ((EERead(EEPROM_IS_CAL_HV_DISCRET + 2) == 0) && (tension <= HVSi2Max));
    } else if ((module == 2) && (tel == 'B')) {
        coef = coefHV_M400;
        max_dac = (UINT32)HVSi2Max * coef / 1000;
        calibration_addr_coeff = EEPROM_CAL_DAC_B2_LINEAR_COEFF;
        calibration_addr_const = EEPROM_CAL_DAC_B2_LINEAR_CONST;
        use_linear = ((EERead(EEPROM_IS_CAL_HV_DISCRET + 3) == 0) && (tension <= HVSi2Max));
    }

    if (coef != 0) {
        default_value = ((UINT32)tension) * coef / 1000;
        if (use_linear) {
            value_dec = get_value_dec(tension, calibration_addr_coeff, calibration_addr_const);
            if (value_dec > max_dac) {
                value_dec = default_value;
            }
        } else {
            value_dec = default_value;
        }
        inc = (slopeVS * coef) / 1000;
        HvValueTab[channel][1] = (UINT)value_dec;
        HvInc[channel] = (UINT)inc;
        HvStatus[channel] = 0;
        verdict = FUNC_EXEC_OK;
    }

    return verdict;
}



