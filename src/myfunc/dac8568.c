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

long int get_value_dec(int tension, UINT eeprom_adr_coeff, UINT eeprom_adr_const) {
    int dac_cal_linear_coeff = ((int)EERead(eeprom_adr_coeff + 1) << 8) | (int)EERead(eeprom_adr_coeff);
    int dac_cal_linear_const = ((int)EERead(eeprom_adr_const + 1) << 8) | (int)EERead(eeprom_adr_const);
    long int lin;

    /* Integer-only arithmetic for PIC18. */
    lin = ((long int)dac_cal_linear_coeff * (long int)tension) + (long int)dac_cal_linear_const;
    return lin / COEFF_SCALE_FACTOR;
}

/**
 * @brief Convert a high-voltage target value to a code using the old table-based
 *        calibration method (reimplemented from the initial commit). The value is
 *        interpolated from a discrete calibration table stored in EEPROM.
 * @param tension Target voltage in volts
 * @param adrCal EEPROM base address of the discrete calibration table
 * @return UINT32 code interpolated from the calibration table
 */
UINT32 get_value_dec_table(UINT tension, UINT adrCal) {
    UINT vinf, vsup, unites, dec, adr;
    UINT32 value;

    if (tension == 0) value = 0;
    else {
        vinf = 0; vsup = 0;
        dec = tension / 10;
        unites = tension - 10 * dec;
        if (unites == 0) {
            adr = 2 * dec + adrCal - 2;
            value = 256 * EERead(adr + 1) + EERead(adr);
        }
        else {
            if (tension < 10) vinf = 0;
            else {
                adr = 2 * dec + adrCal - 2;
                vinf = 256 * EERead(adr + 1) + EERead(adr);
            }
            adr = 2 * dec + adrCal;
            vsup = 256 * EERead(adr + 1) + EERead(adr);
            value = vinf + ((vsup - vinf) * unites + 5) / 10;
        }
    }
    return value;
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
    UINT32 coef             = 0; // UINT32: coefHV_M200 (141100) does not fit in 16 bits
    UINT calibration_addr_coeff = 0;
    UINT calibration_addr_const = 0;
    UINT table_addr         = 0;
    BYTE is_calibrated      = 0;
    BYTE channel = module - 1 + 2 * ((BYTE)(tel - 'A'));

    if ((module == 1) && (tel == 'A')) {
        coef = coefHV_M200;
        max_dac = (UINT32)HVSi1Max * coef / 1000;
        calibration_addr_coeff = EEPROM_CAL_DAC_A1_LINEAR_COEFF;
        calibration_addr_const = EEPROM_CAL_DAC_A1_LINEAR_CONST;
        table_addr = EEPROM_SI1A_CAL_HV_DISCRET;
        is_calibrated = (EERead(EEPROM_IS_CAL_HV_DISCRET) == 0);
    } else if ((module == 1) && (tel == 'B')) {
        coef = coefHV_M200;
        max_dac = (UINT32)HVSi1Max * coef / 1000;
        calibration_addr_coeff = EEPROM_CAL_DAC_B1_LINEAR_COEFF;
        calibration_addr_const = EEPROM_CAL_DAC_B1_LINEAR_CONST;
        table_addr = EEPROM_SI1B_CAL_HV_DISCRET;
        is_calibrated = (EERead(EEPROM_IS_CAL_HV_DISCRET + 1) == 0);
    } else if ((module == 2) && (tel == 'A')) {
        coef = coefHV_M400;
        max_dac = (UINT32)HVSi2Max * coef / 1000;
        calibration_addr_coeff = EEPROM_CAL_DAC_A2_LINEAR_COEFF;
        calibration_addr_const = EEPROM_CAL_DAC_A2_LINEAR_CONST;
        table_addr = EEPROM_SI2A_CAL_HV_DISCRET;
        is_calibrated = ((EERead(EEPROM_IS_CAL_HV_DISCRET + 2) == 0) && (tension <= HVSi2Max));
    } else if ((module == 2) && (tel == 'B')) {
        coef = coefHV_M400;
        max_dac = (UINT32)HVSi2Max * coef / 1000;
        calibration_addr_coeff = EEPROM_CAL_DAC_B2_LINEAR_COEFF;
        calibration_addr_const = EEPROM_CAL_DAC_B2_LINEAR_CONST;
        table_addr = EEPROM_SI2B_CAL_HV_DISCRET;
        is_calibrated = ((EERead(EEPROM_IS_CAL_HV_DISCRET + 3) == 0) && (tension <= HVSi2Max));
    }

    if (coef != 0) {
        default_value = ((UINT32)tension) * coef / 1000;
        if (is_calibrated) {
            if (use_linear_calibration()) {
                if (is_linear_calibration_valid(calibration_addr_coeff, calibration_addr_const)) {
                    value_dec = get_value_dec(tension, calibration_addr_coeff, calibration_addr_const);
                } else {
                    //TODO Error calibration routine: invalid linear DAC calibration, fallback to default transfer.
                    value_dec = default_value;
                }
            } else {
                // Old table-based calibration method (reimplemented from the initial commit)
                value_dec = get_value_dec_table(tension, table_addr);
            }

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



