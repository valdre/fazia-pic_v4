#ifndef _FUNC_H
#define _FUNC_H

/**
 * @file functions.h
 * @brief Main firmware API for PIC_V4 command handling, sensor acquisition, and HV control.
 * @details This header declares the central firmware functions used to process UART commands, access
 *          onboard ADC/DAC devices, manage EEPROM calibration, and monitor temperatures.
 */

#include "Generic.h"
// #include <p18cxxx.h>
#include <xc.h>
#include <stddef.h>
#include <stdio.h>
// #include <timers.h>
// #include <delays.h>
// #include <spi.h>
// #include <usart.h>

#ifndef XC8_STUBS_H
#define XC8_STUBS_H

#include <xc.h>

#define KHz *1000UL
#define MHz *1000000UL

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 64000000UL
#endif

// Emulation des Delays C18 en cycles instruction (TCY) pour garder le meme timing.
#define Delay10TCYx(x)   _delay((unsigned long)(x) * 10UL)
#define Delay100TCYx(x)  _delay((unsigned long)(x) * 100UL)
#define Delay10KTCYx(x)  _delay((unsigned long)(x) * 10000UL)

// Compatibilité des anciennes fonctions SPI ---
#define SPI_FOSC_16 0
#define MODE_00     0
#define MODE_10     1
#define SMPEND      0
#define SMPMID      1
void OpenSPI(char m, char edge, char smp);
void CloseSPI(void);
void putcSPI(unsigned char data);
uint8_t getcSPI(void);

// Compatibilité des anciennes fonctions USART ---
char DataRdyUSART(void);
char ReadUSART(void);
void putcUSART(char data);
char BusyUSART(void);

// Compatibilité des anciennes fonctions de timer ---
#define TIMER_INT_ON   0x8000U
#define TIMER_INT_OFF  0x00
#define T2_PS_1_16     0x0002U
#define T2_POST_1_16   0x0078U
#define T1_16BIT_RW    0x0080U
#define T1_SOURCE_INT  0x00
#define T1_PS_1_8      0x0030U
#define T1_OSC1EN_OFF  0x00
#define T1_SYNC_EXT_OFF 0x00
#define T3_16BIT_RW    0x0080U
#define T3_SOURCE_INT  0x00
#define T3_PS_1_1      0x00
#define T3_SYNC_EXT_OFF 0x00
void OpenTimer1(unsigned int config);
void OpenTimer2(unsigned int config);
void OpenTimer3(unsigned int config);
void WriteTimer1(unsigned int timer);
unsigned int ReadTimer1(void);

#endif

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
#define timing_HV 25 // real time : 4ms * timing_HV
#define shortInspecDelay 30000
#define normalInspecDelay 300000 //number * 4ms = delay
#define HVSi1Max 300
#define HVSi2Max 400
#define keyWordC 0x38
#define snLSB 362
#define snMSB 363
#define SPI_KEY 0x38
#define COEFF_SCALE_FACTOR 100

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

#define EEPROM_CAL_HV_FIRST_ADR 13
#define EEPROM_CAL_HV_WIDTH 9
#define EEPROM_CAL_HV_CALIBRATED 50 // EEPROM_CAL_HV_FIRST_ADR + 4 * EEprom_HV_width
#define EEPROM_COEF_HV_READING 54
#define EEPROM_IS_CAL_HV_DISCRET 78
#define EEPROM_SI1A_CAL_HV_DISCRET 82   // EEprom address from 82  to 141 DAC calibration for channel A1 -> 142-82 =60 so 30 values
#define EEPROM_SI2A_CAL_HV_DISCRET 142  // EEprom address from 142 to 221 DAC calibration for channel A2 -> 222-142=80 so 40 values
#define EEPROM_SI1B_CAL_HV_DISCRET 222  // EEprom address from 222 to 281 DAC calibration for channel B1 -> 282-222=60 so 30 values
#define EEPROM_SI2B_CAL_HV_DISCRET 282  // EEprom address from 282 to 361 DAC calibration for channel B2 -> 362-282=80 so 40 values
#define EEPROM_SI1A_CAL_IHV_DISCRET 376 // EEprom address from 376 to 435 ADC calibration for channel A1 -> 436-376=60 so 30 values
#define EEPROM_SI2A_CAL_IHV_DISCRET 436 // EEprom address from 436 to 515 ADC calibration for channel A2 -> 516-436=80 so 40 values
#define EEPROM_SI1B_CAL_IHV_DISCRET 516 // EEprom address from 516 to 575 ADC calibration for channel B1 -> 576-516=60 so 30 values
#define EEPROM_SI2B_CAL_IHV_DISCRET 576 // EEprom address from 576 to 655 ADC calibration for channel B2 -> 656-576=80 so 40 values
#define EEPROM_HV_BORNE_SUP_A1 364
#define EEPROM_HV_SHORT_INSPEC_TIME 372
#define EEPROM_HV_LONG_INSPEC_TIME 374
#define EEPROM_HIGH_LC_TRSH_LSB 656
#define EEPROM_HIGH_LC_TRSH_MSB 657
#define EEPROM_LOW_LC_TRSH_LSB 658
#define EEPROM_LOW_LC_TRSH_MSB 659
#define EEPROM_CAL_DAC_A1_LINEAR_COEFF 660
#define EEPROM_CAL_DAC_A1_LINEAR_CONST 662
#define EEPROM_CAL_DAC_A2_LINEAR_COEFF 664
#define EEPROM_CAL_DAC_A2_LINEAR_CONST 666
#define EEPROM_CAL_DAC_B1_LINEAR_COEFF 668
#define EEPROM_CAL_DAC_B1_LINEAR_CONST 670
#define EEPROM_CAL_DAC_B2_LINEAR_COEFF 672
#define EEPROM_CAL_DAC_B2_LINEAR_CONST 674

#define EEPROM_CAL_ADC_A1_LINEAR_COEFF 676
#define EEPROM_CAL_ADC_A1_LINEAR_CONST 678
#define EEPROM_CAL_ADC_A2_LINEAR_COEFF 680
#define EEPROM_CAL_ADC_A2_LINEAR_CONST 682
#define EEPROM_CAL_ADC_B1_LINEAR_COEFF 684
#define EEPROM_CAL_ADC_B1_LINEAR_CONST 686
#define EEPROM_CAL_ADC_B2_LINEAR_COEFF 688
#define EEPROM_CAL_ADC_B2_LINEAR_CONST 690
struct parametres {
    uint16_t voltage_preamp1b;
    uint16_t voltage_preamp2b;
    uint16_t voltage_preamp3b;
    uint16_t voltage_preamp1a;
    uint16_t voltage_preamp2a;
    uint16_t voltage_preamp3a;
};

/**
 * @brief Initialize the firmware function table for UART command dispatch.
 * @details Called one time during startup to configure all supported command handlers.
 */
void func_init(void);

/**
 * @brief Execute a command received over UART.
 * @param code Command identifier.
 * @param data Input payload provided by the remote host.
 * @param result Buffer that receives the command response payload.
 * @return Execution status code.
 * @retval FUNC_EXEC_OK Command executed successfully.
 * @retval FUNC_EXEC_BAD_ARGS_TYPE Invalid argument type in request.
 * @retval FUNC_EXEC_BAD_ARGS_VALUE Invalid argument value in request.
 * @retval FUNC_EXEC_INPROGRESS Command accepted and still executing asynchronously.
 */
uint8_t func_invoke(unsigned char code, char *data, char *result);

/**
 * @brief Simple echo command implementation used for communication tests.
 * @param data Input data to echo.
 * @param result Output buffer that receives the same data.
 * @return Execution status code.
 */
uint8_t f_echo(char *data, char *result);

/**
 * @brief Perform a software reset of the PIC MCU.
 * @param data Unused input buffer.
 * @param result Output buffer for status or diagnostic data.
 * @return Execution status code.
 */
uint8_t resetPIC(char *data, char *result);

/**
 * @brief Read the board serial number and return it to the host.
 * @param data Command payload.
 * @param result Output buffer containing the serial number string.
 * @return Execution status code.
 */
uint8_t setGetSN(char *data, char *result);

/**
 * @brief Configure the CSI relay state from UART command data.
 * @param data Input command buffer.
 * @param result Output status buffer.
 * @return Execution status code.
 */
uint8_t uart_csi_relay(char *data, char *result);

/**
 * @brief Issue a reset sequence to both connected FPGA devices.
 * @param data Command request buffer.
 * @param result Reply buffer.
 * @return Execution status code.
 */
uint8_t uart_reset_both_fpga(char *data, char *result);

/**
 * @brief Retrieve the current high-voltage module status flags.
 * @param data Command input buffer.
 * @param result Output buffer containing HV status bytes.
 * @return Execution status code.
 */
uint8_t giveHvStatus(char *data, char *result);

/**
 * @brief Configure the high-voltage routing for CSI and AB detector chains.
 * @param data Command input buffer.
 * @param result Output state buffer.
 * @return Execution status code.
 */
uint8_t setHVCsiAB(char *data, char *result);

/**
 * @brief Execute a preamplifier diagnostic routine.
 * @param preamp Identifier of the preamplifier channel to test.
 * @return Execution status code.
 */
uint8_t preamplifier_test(uint8_t preamp);

/**
 * @brief Handle a UART request to execute a preamplifier test.
 * @param data Command input buffer.
 * @param result Output buffer containing the test result.
 * @return Execution status code.
 */
uint8_t uart_preamplifier_test(char *data, char *result);

/**
 * @brief Apply automatic offset calibration to preamplifier outputs.
 * @param data Input command containing offset configuration.
 * @param result Output status or error code.
 * @return Execution status code.
 */
uint8_t setautoffset(char *data, char *result);

/**
 * @brief Enable or disable high-voltage measurement mode.
 * @param data Input command buffer specifying enable or disable state.
 * @param result Output status buffer.
 * @return Execution status code.
 */
uint8_t enDesHVdev(char *data, char *result);

/**
 * @brief Report the current generator DAC voltage.
 * @param data Command input buffer.
 * @param result Output buffer containing the generator DAC voltage.
 * @return Execution status code.
 */
uint8_t getGeneDacVoltage(char *data, char *result);

/**
 * @brief Query the current leak current for a detector channel.
 * @param tel Telescope identifier ('A' or 'B').
 * @param module Detector module identifier ('1' or '2').
 * @param lc Pointer to output the computed leak current.
 * @return Execution status code.
 */
uint8_t getCurrent(char tel, char module, uint16_t *lc);

/**
 * @brief Retrieve the configured high-voltage values.
 * @param data Command payload.
 * @param result Output buffer for formatted HV data.
 * @return Execution status code.
 */
uint8_t get_HV(char *data, char *result);

/**
 * @brief Read a converted ADC value from the selected ADS8332 channel.
 * @param canal Pointer to the channel identifier.
 * @return 16-bit ADC conversion result.
 */
uint16_t getHvValue(uint8_t *canal);

/**
 * @brief Return the FPGA firmware version information.
 * @param data Input command buffer.
 * @param result Output buffer receiving FPGA version data.
 * @return Execution status code.
 */
uint8_t get_fpga_version(char *data, char *result);

/**
 * @brief Placeholder command handler for undefined command slots.
 * @param data Command payload.
 * @param result Output buffer.
 * @return Execution status code indicating undefined command.
 */
uint8_t nullFunc(char *data, char *result);

/**
 * @brief Read raw ADS8332 sample data and format it for the UART response.
 * @param data Input command payload.
 * @param result Output buffer containing ADC data.
 * @return Execution status code.
 */
uint8_t get_ads8332(char *data, char *result);

/**
 * @brief Retrieve the PIC firmware version.
 * @param data Command input buffer.
 * @param result Output buffer receiving version text.
 * @return Execution status code.
 */
uint8_t get_pic_version(char *data, char *result);

/**
 * @brief Compute and return leak current metrics from ADC readback.
 * @param data Command input buffer.
 * @param result Output buffer for leak current values.
 * @return Execution status code.
 */
uint8_t get_leak_current(char *data, char *result);

/**
 * @brief Set pulse generator timing parameters.
 * @param data Input buffer containing period and high time.
 * @param result Output buffer for status.
 * @return Execution status code.
 */
uint8_t set_pulse_parameters(char *data, char *result);

/**
 * @brief Configure a preamplifier voltage using UART command data.
 * @param data Input buffer containing voltage and channel information.
 * @param result Output status buffer.
 * @return Execution status code.
 */
uint8_t set_voltage_preamplifier(char *data, char *result);

/**
 * @brief Increment or decrement the high-voltage setpoint.
 * @param data Command input specifying adjustment direction and value.
 * @param result Output buffer containing the new state.
 * @return Execution status code.
 */
uint8_t incdecHV(char *data, char *result);

/**
 * @brief Return the configured inspection interval.
 * @param data Command input buffer.
 * @param result Output buffer receiving the inspection time.
 * @return Execution status code.
 */
uint8_t getInspecTime(char *data, char *result);

/**
 * @brief Return software stack information for diagnostics.
 * @param data Command input buffer.
 * @param result Output buffer receiving stack usage or status.
 * @return Execution status code.
 */
uint8_t getSoftStack(char *data, char *result);

/**
 * @brief Configure the inspection interval used by the leak current scheduler.
 * @param data Input buffer containing the desired inspection interval.
 * @param result Output buffer for status or error.
 * @return Execution status code.
 */
uint8_t setInspecTime(char *data, char *result);

/**
 * @brief Set the HV output value for a detector channel.
 * @param data Input buffer containing the desired voltage value.
 * @param result Output buffer for status.
 * @return Execution status code.
 */
uint8_t set_vhv(char *data, char *result);

/**
 * @brief Query the current temperatures from onboard sensors.
 * @param data Command input buffer.
 * @param result Output buffer receiving formatted temperature values.
 * @return Execution status code.
 */
uint8_t get_temp(char *data, char *result);

/**
 * @brief Return the maximum configured high-voltage limits.
 * @param data Command input buffer.
 * @param result Output buffer for the HV limits.
 * @return Execution status code.
 */
uint8_t get_hvmax(char *data, char *result);

/**
 * @brief Apply a hardware maximum voltage limit to a detector module.
 * @param tel Telescope identifier ('A' or 'B').
 * @param module Module identifier ('1' or '2').
 * @param tension_max Maximum voltage limit in applied units.
 * @return Execution status code.
 */
uint8_t apply_hvmax(char tel, char module, uint16_t tension_max);

/**
 * @brief Enable or disable the HV measurement channel in the firmware.
 * @param data Command input buffer.
 * @param result Output status buffer.
 * @return Execution status code.
 */
uint8_t enableDisableHVMeas(char *data, char *result);

/**
 * @brief Set the high-voltage maximum value from UART command input.
 * @param data Input buffer containing new HV maximum value.
 * @param result Output status buffer.
 * @return Execution status code.
 */
uint8_t set_hvmax(char *data, char *result);

/**
 * @brief Request a high-voltage calibration procedure over UART.
 * @param data Command payload.
 * @param result Output buffer receiving calibration status.
 * @return Execution status code.
 */
uint8_t uart_ask_hv_calibration(char *data, char *result);

/**
 * @brief Store runtime parameters into EEPROM persistent storage.
 * @param data Input buffer containing values to store.
 * @param result Output status buffer.
 * @return Execution status code.
 */
uint8_t store_param(char *data, char *result);

/**
 * @brief Set the EEPROM address pointer used by subsequent data reads.
 * @param data Command input buffer containing the address.
 * @param result Output status buffer.
 * @return Execution status code.
 */
uint8_t set_data_eeprom_address(char *data, char *result);

/**
 * @brief Read data from the current EEPROM address pointer.
 * @param data Command input buffer.
 * @param result Output buffer receiving EEPROM data.
 * @return Execution status code.
 */
uint8_t get_data_eeprom_address(char *data, char *result);

/**
 * @brief Persist a byte value into the on-board EEPROM.
 * @param ad EEPROM address.
 * @param data Byte value to store.
 */
void EEWrite(unsigned int ad, unsigned char data);

/**
 * @brief Read a byte value from on-board EEPROM.
 * @param ad EEPROM address.
 * @return Value stored at the given EEPROM address.
 */
uint8_t EERead(unsigned int ad);

/**
 * @brief Parse an ASCII string and convert it into an unsigned integer.
 * @param pr Null-terminated ASCII numeric string.
 * @param converted_value Output pointer for the parsed value.
 * @return Zero on success, non-zero on failure.
 */
uint8_t analyze_string(char *pr,uint16_t *converted_value);

/**
 * @brief Convert a decimal ASCII string to an unsigned integer.
 * @param pr Input ASCII decimal string.
 * @return Parsed unsigned integer.
 */
uint16_t chardectoi(char *pr);

/**
 * @brief Convert a hexadecimal ASCII string to an unsigned integer.
 * @param pr Input ASCII hexadecimal string.
 * @return Parsed unsigned integer.
 */
uint16_t charhextoi(char *pr);

/**
 * @brief Compute 10 raised to the given power.
 * @param c Exponent value.
 * @return 10^c.
 */
uint16_t power10(uint8_t c);

/**
 * @brief Compute a 32-bit power of 10.
 * @param c Exponent value.
 * @return 10^c as a 32-bit unsigned value.
 */
uint32_t power10_32(uint8_t c);

/**
 * @brief Initialize the temperature sensor interface.
 * @return Execution status code.
 */
uint8_t temp_init(void);

/**
 * @brief Read the temperature sensors and store their results.
 * @param temperature Output array for temperature values.
 * @return Number of successful temperature reads.
 */
uint8_t temp(int *temperature);

/**
 * @brief Send the initial start sequence for 1-Wire temperature conversion.
 */
void write_bit_start(void);

/**
 * @brief Write one bit on the 1-Wire temperature bus.
 * @param c Bit value to write.
 */
void write_bit(char c);

/**
 * @brief Wait for the 1-Wire bus to return a logic 1 signal.
 * @param c Reference bit value used during timing.
 */
void attente_bit1(char c);

/**
 * @brief Convert a numeric nibble to its ASCII representation.
 * @param c Nibble value 0..15.
 * @return ASCII character corresponding to c.
 */
uint8_t asciiconv(uint8_t c);

/**
 * @brief Convert a 16-bit unsigned value to an ASCII hex string.
 * @param value Value to convert.
 * @param s Output buffer for 4 ASCII hex digits.
 */
void uinttoa(uint16_t value, uint8_t *s);

/**
 * @brief Output a 16-bit unsigned integer over USART as ASCII.
 * @param value Value to send.
 * @return Last transmitted character.
 */
char dispuinttochar(uint16_t value);

/**
 * @brief Output a 32-bit unsigned integer over USART as ASCII.
 * @param value Value to send.
 * @return Last transmitted character.
 */
char dispuint32tochar(uint32_t value);

/**
 * @brief Output an unsigned integer in binary format over USART.
 * @param a Value to display.
 * @return Last transmitted character.
 */
char dispinttobin (uint16_t a);

/**
 * @brief Output a byte in binary format over USART.
 * @param c Value to display.
 * @return Last transmitted character.
 */
char dispchartobin(uint8_t c);

/**
 * @brief Copy an unsigned integer into a character buffer with terminator.
 * @param container Destination buffer.
 * @param a Value to copy.
 * @param cend Terminator character.
 */
void myStrCpyUint(char *container,uint16_t a,char cend);

/**
 * @brief Copy a byte value into a string buffer as digits.
 * @param container Destination buffer.
 * @param a Byte value.
 * @param cend Terminator character.
 */
void myStrCpyByte(char *container,uint8_t a,char cend);

/**
 * @brief Append a string into a destination buffer.
 * @param container Destination buffer.
 * @param chaine Source null-terminated string.
 * @param cend Terminator character appended to the result.
 */
void myStrCpyChar(char *container,char *chaine,char cend);

/**
 * @brief Append a constant ROM string into a destination buffer.
 * @param container Destination buffer.
 * @param chaine ROM source string.
 * @param cend Terminator character appended to the result.
 */
void myStrCpyChar2(char *container,const char *chaine,char cend);

/**
 * @brief Append a single character to a destination buffer.
 * @param container Destination buffer.
 * @param c Character to append.
 * @param cend Terminator character appended after c.
 */
void myStrCpy1Char(char *container,char c,char cend);

/**
 * @brief Append a hexadecimal value to a destination buffer.
 * @param container Destination buffer.
 * @param a Value to convert.
 * @param format Field width or -1 for minimal output.
 * @param cend Terminator character appended after the value.
 */
void myStrCpyHex(char *container,uint16_t a,int format,char cend);

/**
 * @brief Configure a register value via SPI from a UART command.
 * @param data Input command payload.
 * @param result Output response buffer.
 * @return Execution status code.
 */
uint8_t spi_set_regvalue(char *data, char *result);

/**
 * @brief Read a register value via SPI and return it over UART.
 * @param data Input command buffer.
 * @param result Output buffer containing register data.
 * @return Execution status code.
 */
uint8_t spi_get_regvalue(char *data,char *result);

/**
 * @brief Read an FPGA register over SPI.
 * @param id_fpga FPGA identifier.
 * @param adr Register address to read.
 * @return Register value.
 */
uint16_t rdspi(char id_fpga,unsigned int adr);

/**
 * @brief Write a register into an FPGA via SPI.
 * @param id_fpga FPGA identifier.
 * @param adr Register address to write.
 * @param data Value to write.
 */
void wrspi(char id_fpga,unsigned int adr, unsigned int data);

/**
 * @brief Send a multi-byte data sequence to SPI.
 * @param co Number of bytes to write.
 * @param wrptr Pointer to the write buffer.
 */
void myputsspi(int co, unsigned char *wrptr);

/**
 * @brief Set the offset voltage for preamplifier channel 1A.
 * @param value Offset code.
 * @return Execution status code.
 */
uint8_t set_off_V_preamp1a(uint16_t value);

/**
 * @brief Set the offset voltage for preamplifier channel 1B.
 * @param value Offset code.
 * @return Execution status code.
 */
uint8_t set_off_V_preamp1b(uint16_t value);

/**
 * @brief Set the offset voltage for preamplifier channel 2A.
 * @param value Offset code.
 * @return Execution status code.
 */
uint8_t set_off_V_preamp2a(uint16_t value);

/**
 * @brief Set the offset voltage for preamplifier channel 2B.
 * @param value Offset code.
 * @return Execution status code.
 */
uint8_t set_off_V_preamp2b(uint16_t value);

/**
 * @brief Set the offset voltage for preamplifier channel 3A.
 * @param value Offset code.
 * @return Execution status code.
 */
uint8_t set_off_V_preamp3a(uint16_t value);

/**
 * @brief Set the offset voltage for preamplifier channel 3B.
 * @param value Offset code.
 * @return Execution status code.
 */
uint8_t set_off_V_preamp3b(uint16_t value);

/**
 * @brief Reset both FPGA devices through control pins.
 */
void reset_both_fpga(void);

/**
 * @brief Validate that a command can be dispatched to a handler.
 * @param data Input command payload.
 * @param result Output buffer for diagnostic data.
 * @return Execution status code.
 */
uint8_t func_test(char *data, char *result);

/**
 * @brief Read the board identity jumpers.
 * @return ASCII character representing the board ID.
 */
uint8_t getid(void);

/**
 * @brief Initialize the DAC8568 and apply default configuration.
 * @return Zero on success, non-zero on failure.
 */
char dac_init(void);

/**
 * @brief Send a single DAC write sequence.
 * @param ad DAC command address.
 * @param data Data to write.
 */
void dac_sequence(char ad,unsigned int data);

/**
 * @brief Generate a pulse on the HV pulser output.
 * @param data Pulse amplitude or command code.
 * @param period Pulse period in timer ticks.
 * @param high_time High time in timer ticks.
 * @return Execution status or error code.
 */
char pulser(uint16_t data, uint16_t period, uint16_t high_time);

/**
 * @brief Initiate an HV calibration request string.
 * @param str Null-terminated calibration request string.
 */
void ask_hv_calibration(char *str);

/**
 * @improve : Transfrom a reading of value in EEprom to a 2 value  in eeprom (instead of ~30,40) 
 * and then create a equation to get the desired values
 * @brief Convert a high-voltage target value to DAC decimal output code.
 * @param tension Target voltage in volts
 * @param eeprom_adr_coeff EEPROM address for the DAC calibration linear coefficient
 * @param eeprom_adr_const EEPROM address for the DAC calibration linear constant
 * @return Signed 32-bit DAC code corresponding to the target voltage
 */
long int get_value_dec(int tension, uint16_t eeprom_adr_coeff, uint16_t eeprom_adr_const);

/**
 * @brief Validate linear calibration coefficient/constant pair stored in EEPROM.
 * @param coeff_addr EEPROM address of signed 16-bit coefficient.
 * @param const_addr EEPROM address of signed 16-bit constant.
 * @return 1 if calibration looks valid, 0 otherwise.
 */
uint8_t is_linear_calibration_valid(uint16_t coeff_addr, uint16_t const_addr);

/**
 * @brief Apply a slope correction curve to a high-voltage setpoint.
 * @param tel Telescope identifier.
 * @param module Module identifier.
 * @param tension Target voltage.
 * @param slopeVS Slope coefficient.
 * @return Execution status code.
 */
uint8_t slop_vhv(char tel,uint8_t module,uint16_t tension,uint32_t slopeVS);

/**
 * @brief Load runtime parameters from onboard EEPROM and sensors.
 */
void setparam(void);

/**
 * @brief Persist runtime parameters into EEPROM.
 * @return Non-zero if the write succeeded.
 */
char storeparam(void);

/**
 * @brief Parse a buffer containing a hexadecimal string into an unsigned integer.
 * @param pr Null-terminated input string.
 * @return Parsed integer value.
 */
unsigned int htoi(const char *pr);

/**
 * @brief Acquire PIC ADC channel voltages into the provided buffer.
 * @param voltages Output array receiving ADC measurements.
 */
void get_PIC_AD_voltages(uint16_t *voltages);

/**
 * @brief Read and format analog voltage measurements for UART output.
 * @param data Command payload.
 * @param result Output buffer receiving formatted values.
 * @return Execution status code.
 */
uint8_t getVoltages(char *data, char *result);

/**
 * @brief Read LTC switch analog voltages and report them.
 * @param data Input command buffer.
 * @param result Output buffer containing voltage values.
 * @return Execution status code.
 */
uint8_t getLTCswVoltages(char * data, char *result);

/**
 * @brief Read LTC linear converter input voltages and report them.
 * @param data Input command buffer.
 * @param result Output buffer containing voltage values.
 * @return Execution status code.
 */
uint8_t getLTClinVoltages(char * data, char *result);

/**
 * @brief Read multiple LTC2308 ADC channels selected by a mask.
 * @param mask Bit mask selecting channels.
 * @param ADvoltages Output buffer receiving ADC results.
 * @return Execution status code.
 */
uint8_t getLTC2308Voltages(uint8_t mask, uint16_t *ADvoltages);

/**
 * @brief Initialize the ADS8332 ADC device.
 */
void adc_init(void);

/**
 * @brief Read an ADS8332 channel and return the converted value.
 * @param canal Pointer to the channel index.
 * @return 16-bit channel reading.
 */
uint16_t adc_getvalue(unsigned char *canal);

/**
 * @brief Compute leakage current for the selected detector channel.
 * @param tel Telescope identifier.
 * @param module Detector module identifier.
 * @param lcAdcRead Raw ADC leak current reading.
 * @return Computed leak current in board units.
 */
uint16_t leak_current (char tel, char module, uint32_t lcAdcRead);

/**
 * @brief Compute the actual low-level leak current from the measured ADC value.
 * @param data Input command payload.
 * @param result Output buffer containing computed leak current.
 * @return Execution status code.
 */
uint8_t get_leak_current(char *data, char *result);

/**
 * @brief Enable or disable the high-voltage measurement function.
 * @param data Input command buffer.
 * @param result Output buffer for status.
 * @return Execution status code.
 */
uint8_t enDesHVdev(char *data, char *result);

/**
 * @brief Request the maximum allowed high-voltage value for the current board.
 * @param data Command payload.
 * @param result Output buffer containing max HV values.
 * @return Execution status code.
 */
uint8_t get_hvmax(char *data, char *result);

/**
 * @brief Set the maximum allowable high-voltage threshold via UART.
 * @param data Input buffer containing the new limit.
 * @param result Output buffer for status.
 * @return Execution status code.
 */
uint8_t set_hvmax(char *data, char *result);

/**
 * @brief Run the high-voltage control routine.
 */
void HVfunc(void);

/**
 * @brief Run the high-voltage calibration routine for all detector channels.
 * @param lcAdcReadA1 ADC leak current reading for A1.
 * @param lcAdcReadA2 ADC leak current reading for A2.
 * @param lcAdcReadB1 ADC leak current reading for B1.
 * @param lcAdcReadB2 ADC leak current reading for B2.
 */
void cal_HVfunc(uint16_t lcAdcReadA1,uint16_t lcAdcReadA2, uint16_t lcAdcReadB1, uint16_t lcAdcReadB2);

/**
 * @brief Perform the scheduled leak current inspection routine.
 * @return Computed interval or leak inspection status.
 */
uint32_t current_leak_inspection(void);

/**
 * @brief Blink status LEDs in the primary pattern.
 */
void led_blinking(void);

/**
 * @brief Blink status LEDs in an alternate pattern.
 */
void led_blinking2(void);

/**
 * @brief Apply preamplifier offset settings from calibration parameters.
 */
void pa_offset_settings(void);

/**
 * @brief Compute the difference between three timing values used in leak current scheduling.
 * @param t1 First timestamp.
 * @param t2 Second timestamp.
 * @param t3 Third timestamp.
 * @return Timing difference.
 */
uint32_t diffLcTime(uint32_t t1,uint32_t t2,uint32_t t3);

/* EEPROM-backed leak-current thresholds (16-bit, stored as two 8-bit cells) */
uint16_t getHighLcTrsh(void);

uint16_t getLowLcTrsh(void);

#endif

// EOF
