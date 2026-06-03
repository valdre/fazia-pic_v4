#ifndef _FUNC_H
#define _FUNC_H

#include "Generic.h"
#include <p18cxxx.h>
#include <stddef.h>
#include <stdio.h>
#include <timers.h>
#include <delays.h>
#include <spi.h>
#include <usart.h>

#define MAX_FUNC_NUM 36
//#define MAX_HPFUNC_NUM   3

#define FUNC_CMD_OK                 0x00
#define FUNC_CMD_NOT_VALID          0x01
#define FUNC_CMD_NOT_DEFINED        0x02

#define FUNC_EXEC_OK                0x00
#define FUNC_EXEC_BAD_ARGS_TYPE     0x01
#define FUNC_EXEC_BAD_ARGS_VALUE    0x02
#define FUNC_EXEC_INPROGRESS        0x03

#define capteur_io PORTBbits.RB4
#define mask_capteur 0b00010000
#define nbcapteurs 4 /*nbcapteurs doit être inférieur ou égal à 4*/
#define GEOA0 PORTAbits.RA0
#define GEOA1 PORTAbits.RA1
#define GEOA2 PORTAbits.RA2
#define EOC   PORTDbits.RD7
#define coefHV_M200 141100
#define coefHV_M400 87400
#define SPI_KEY 0x38
#define REG_FPGA_REQUEST 0x220
#define REG_FPGA_CMDE 0x221
#define REG_FPGA_FIRST_PARAM 0x222
#define REG_FPGA_FIRST_DATA 0x230
#define REG_FPGA_QH1 0x1036
#define REG_FPGA_Q2 0x1336
#define REG_FPGA_Q3 0x1536
#define REG_FPGA_PA_SI1 0x04
#define REG_FPGA_PA_SI2 0x05
#define REG_FPGA_PA_CSI 0x06
#define REG_FPGA_ID 0x7
#define EEprom_cal_HV_first_adr 13
#define EEprom_cal_HV_width 9
#define EEprom_cal_HV_calibrated 50 // EEprom_cal_HV_first_adr + 4 * EEprom_cal_HV_width
#define EEprom_coef_HV_reading EEprom_cal_HV_calibrated+4
#define EEprom_is_cal_HV_discret 78
#define EEprom_si1A_cal_Hv_discret 82
#define EEprom_si2A_cal_Hv_discret 142
#define EEprom_si1B_cal_Hv_discret 222
#define EEprom_si2B_cal_Hv_discret 282
#define EEprom_si1A_cal_IHv_discret 376
#define EEprom_si2A_cal_IHv_discret 436
#define EEprom_si1B_cal_IHv_discret 516
#define EEprom_si2B_cal_IHv_discret 576
#define EEprom_HV_borne_sup_A1 364
#define EEprom_HV_short_Inspec_Time 372
#define EEprom_HV_long_Inspec_Time 374
#define timing_HV 25 // real time : 4ms * timing_HV
#define shortInspecDelay 30000
#define normalInspecDelay 300000 //number * 4ms = delay
#define HVSi1Max 300
#define HVSi2Max 400
#define keyWordC 0x38
#define snLSB 362
#define snMSB 363

ram struct parametres
{
    UINT voltage_preamp1b;
    UINT voltage_preamp2b;
    UINT voltage_preamp3b;
    UINT voltage_preamp1a;
    UINT voltage_preamp2a;
    UINT voltage_preamp3a;
};

void func_init(void);
BYTE func_invoke(unsigned char code, char *data, char *result);
//BYTE hpfunc_invoke(unsigned char code, char *data, char *result);

//BYTE f_reset(char *data, char *result);
//BYTE f_clear(char *data, char *result);
BYTE f_echo(char *data, char *result);
//BYTE f_echo2(char *data, char *result);
BYTE get_temp(char *data, char *result);
BYTE resetPIC(char *data, char *result);

//EEPROM functions
void setparam(void);
char storeparam(void);
void EEWrite(unsigned int ad, unsigned char data);
unsigned char EERead(unsigned int ad);
BYTE store_param(char *data, char *result);
BYTE get_data_eeprom_address(char *data, char *result);
BYTE set_data_eeprom_address(char *data, char *result);

//converting && math functions
BYTE analyze_string(char *pr,UINT *converted_value);
UINT chardectoi(char *pr);
UINT charhextoi(char *pr);
UINT power10(BYTE c);
UINT32 power10_32(BYTE c);
BYTE analyze_string32(char *pr,UINT32 *converted_value);

//display functions
BYTE asciiconv(BYTE c);
void uinttoa(UINT value, BYTE *s);
char dispuinttochar(UINT value);
char dispinttobin (UINT a);
char dispuint32tochar(UINT32 value);
char dispchartobin(BYTE c);
void myStrCpyUint(char *container,UINT a,char cend);
void myStrCpyByte(char *container,BYTE a,char cend);
void myStrCpyChar(char *container,char *chaine,char cend);
void myStrCpyChar2(char *container,const rom char *chaine,char cend);
void myStrCpy1Char(char *container,char c,char cend);
void myStrCpyHex(char *container,UINT a,int format,char cend);
BYTE get_pic_version(char *data, char *result);
BYTE get_fpga_version(char *data,char *result);

//temperature functions
BYTE temp_init(void);
BYTE temp(int *temperature);
void write_bit_start(void);
void write_bit(char c);
void attente_bit1(char c);

//SPI functions
BYTE spi_set_regvalue(char *data, char *result);
BYTE spi_get_regvalue(char *data,char *result);
UINT rdspi(char id_fpga,unsigned int adr);
void wrspi(char id_fpga,unsigned int adr, unsigned int data);
void myputsspi(int co, unsigned char *wrptr);
BYTE set_voltage_preamplifier(char *data, char *result);
BYTE set_off_V_preamp1a(UINT value);
BYTE set_off_V_preamp1b(UINT value);
BYTE set_off_V_preamp2a(UINT value);
BYTE set_off_V_preamp2b(UINT value);
BYTE set_off_V_preamp3a(UINT value);
BYTE set_off_V_preamp3b(UINT value);
BYTE setautoffset(char *data, char *result);
BYTE preamplifier_test(BYTE preamp);
BYTE uart_preamplifier_test(char *data, char *result);
void reset_both_fpga(void);
BYTE uart_reset_both_fpga(char *data, char *result);

BYTE func_test(char *data, char *result);

//to get the id FEE card
BYTE getid(void);

//DAC8568 functions
char dac_init(void);
void dac_sequence(char ad,unsigned int data);
BYTE set_vhv(char *data, char *result);
BYTE enableDisableHVMeas(char *data, char *result);
UINT32 get_value_dec(UINT tension, UINT adrCal);
UINT getHvValue(BYTE *canal);
BYTE slop_vhv(char tel,BYTE module,UINT tension,UINT32 slopeVS);
BYTE giveHvStatus(char *data, char *result);
//BYTE slop_vhv_required(char tel,BYTE module,UINT tension,UINT32 slopeVS);
char pulser(UINT data, UINT period, UINT high_time);
BYTE getGeneDacVoltage(char *data, char *result);
BYTE set_pulse_parameters(char *data, char *result);
//BYTE set_data_leak_current(char *data, char *result);
BYTE uart_ask_hv_calibration(char *data,char *result);
void ask_hv_calibration(char *str);
//BYTE update_vhv(char tel, BYTE module, UINT tension);
BYTE setHVCsiAB(char *data,char *result);
BYTE setGetSN(char *data, char *result);

// voltages and A/D PIC converter
void get_PIC_AD_voltages(unsigned int *voltages);
BYTE getVoltages(char *data, char *result);
BYTE getLTClinVoltages(char * data, char *result);
BYTE getLTCswVoltages(char * data, char *result);
BYTE getLTC2308Voltages(BYTE mask, UINT *ADvoltages);

//ads8332 functions
void adc_init(void);
UINT adc_getvalue(unsigned char *canal);
//UINT adc_read(BYTE canal);
//UINT32 acq_dac(unsigned char c,unsigned int nb_iterations);
BYTE get_leak_current(char *data, char *result);
BYTE get_HV(char *data, char *result);
BYTE get_ads8332(char *data,char *result);
BYTE set_hvmax(char *data, char *result);
BYTE apply_hvmax(char tel,char module,UINT tension_max);
BYTE get_hvmax(char *data, char *result);
UINT leak_current (char tel, char module, UINT32 lcAdcRead);
BYTE getCurrent(char tel, char module, UINT *lc);
BYTE incdecHV(char *data,char *result);
BYTE setInspecTime(char *data, char *result);
BYTE getInspecTime(char *data, char *result);
BYTE enDesHVdev(char *data, char *result);
//void config_slop_vhv(char tel,BYTE module,UINT tension,UINT32 slopeVS);

BYTE uart_csi_relay(char *data, char *result);

// multi-tasking functions within the main procedure
//BYTE uartfunc(void);
void HVfunc(void);
void cal_HVfunc(UINT lcAdcReadA1,UINT lcAdcReadA2, UINT lcAdcReadB1, UINT lcAdcReadB2);
UINT32 current_leak_inspection(void);
void led_blinking(void);
void led_blinking2(void);
void pa_offset_settings(void);

BYTE nullFunc(char *data, char *result);
UINT32 diffLcTime(UINT32 t1,UINT32 t2,UINT32 t3);

BYTE getSoftStack(char *data, char *result);


#endif

// EOF
