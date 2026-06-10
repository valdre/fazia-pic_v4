#ifndef _FUNC_H
#define _FUNC_H

/**
 * @file functions.h
 * @brief Main firmware API for PIC_V4 command handling, sensor acquisition, and HV control.
 * @details This header declares the central firmware functions used to process UART commands, access
 *          onboard ADC/DAC devices, manage EEPROM calibration, and monitor temperatures.
 */

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
#define timing_HV 25 // real time : 4ms * timing_HV
#define shortInspecDelay 30000
#define normalInspecDelay 300000 //number * 4ms = delay
#define HVSi1Max 300
#define HVSi2Max 400
#define keyWordC 0x38
#define snLSB 362
#define snMSB 363
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
#define EEPROM_CAL_DAC_A1_LINEAR_CONST 664
#define EEPROM_CAL_DAC_A2_LINEAR_COEFF 668
#define EEPROM_CAL_DAC_A2_LINEAR_CONST 672
#define EEPROM_CAL_DAC_B1_LINEAR_COEFF 676
#define EEPROM_CAL_DAC_B1_LINEAR_CONST 680
#define EEPROM_CAL_DAC_B2_LINEAR_COEFF 684
#define EEPROM_CAL_DAC_B2_LINEAR_CONST 688
ram struct parametres
{
    UINT voltage_preamp1b;
    UINT voltage_preamp2b;
    UINT voltage_preamp3b;
    UINT voltage_preamp1a;
    UINT voltage_preamp2a;
    UINT voltage_preamp3a;
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
BYTE func_invoke(unsigned char code, char *data, char *result);

/**
 * @brief Simple echo command implementation used for communication tests.
 * @param data Input data to echo.
 * @param result Output buffer that receives the same data.
 * @return Execution status code.
 */
BYTE f_echo(char *data, char *result);

/**
 * @brief Perform a software reset of the PIC MCU.
 * @param data Unused input buffer.
 * @param result Output buffer for status or diagnostic data.
 * @return Execution status code.
 */
BYTE resetPIC(char *data, char *result);

/**
 * @brief Read the board serial number and return it to the host.
 * @param data Command payload.
 * @param result Output buffer containing the serial number string.
 * @return Execution status code.
 */
BYTE setGetSN(char *data, char *result);

/**
 * @brief Configure the CSI relay state from UART command data.
 * @param data Input command buffer.
 * @param result Output status buffer.
 * @return Execution status code.
 */
BYTE uart_csi_relay(char *data, char *result);

/**
 * @brief Issue a reset sequence to both connected FPGA devices.
 * @param data Command request buffer.
 * @param result Reply buffer.
 * @return Execution status code.
 */
BYTE uart_reset_both_fpga(char *data, char *result);

/**
 * @brief Retrieve the current high-voltage module status flags.
 * @param data Command input buffer.
 * @param result Output buffer containing HV status bytes.
 * @return Execution status code.
 */
BYTE giveHvStatus(char *data, char *result);

/**
 * @brief Configure the high-voltage routing for CSI and AB detector chains.
 * @param data Command input buffer.
 * @param result Output state buffer.
 * @return Execution status code.
 */
BYTE setHVCsiAB(char *data, char *result);

/**
 * @brief Execute a preamplifier diagnostic routine.
 * @param preamp Identifier of the preamplifier channel to test.
 * @return Execution status code.
 */
BYTE preamplifier_test(BYTE preamp);

/**
 * @brief Handle a UART request to execute a preamplifier test.
 * @param data Command input buffer.
 * @param result Output buffer containing the test result.
 * @return Execution status code.
 */
BYTE uart_preamplifier_test(char *data, char *result);

/**
 * @brief Apply automatic offset calibration to preamplifier outputs.
 * @param data Input command containing offset configuration.
 * @param result Output status or error code.
 * @return Execution status code.
 */
BYTE setautoffset(char *data, char *result);

/**
 * @brief Enable or disable high-voltage measurement mode.
 * @param data Input command buffer specifying enable or disable state.
 * @param result Output status buffer.
 * @return Execution status code.
 */
BYTE enDesHVdev(char *data, char *result);

/**
 * @brief Report the current generator DAC voltage.
 * @param data Command input buffer.
 * @param result Output buffer containing the generator DAC voltage.
 * @return Execution status code.
 */
BYTE getGeneDacVoltage(char *data, char *result);

/**
 * @brief Query the current leak current for a detector channel.
 * @param tel Telescope identifier ('A' or 'B').
 * @param module Detector module identifier ('1' or '2').
 * @param lc Pointer to output the computed leak current.
 * @return Execution status code.
 */
BYTE getCurrent(char tel, char module, UINT *lc);

/**
 * @brief Retrieve the configured high-voltage values.
 * @param data Command payload.
 * @param result Output buffer for formatted HV data.
 * @return Execution status code.
 */
BYTE get_HV(char *data, char *result);

/**
 * @brief Read a converted ADC value from the selected ADS8332 channel.
 * @param canal Pointer to the channel identifier.
 * @return 16-bit ADC conversion result.
 */
UINT getHvValue(BYTE *canal);

/**
 * @brief Return the FPGA firmware version information.
 * @param data Input command buffer.
 * @param result Output buffer receiving FPGA version data.
 * @return Execution status code.
 */
BYTE get_fpga_version(char *data, char *result);

/**
 * @brief Placeholder command handler for undefined command slots.
 * @param data Command payload.
 * @param result Output buffer.
 * @return Execution status code indicating undefined command.
 */
BYTE nullFunc(char *data, char *result);

/**
 * @brief Read raw ADS8332 sample data and format it for the UART response.
 * @param data Input command payload.
 * @param result Output buffer containing ADC data.
 * @return Execution status code.
 */
BYTE get_ads8332(char *data, char *result);

/**
 * @brief Retrieve the PIC firmware version.
 * @param data Command input buffer.
 * @param result Output buffer receiving version text.
 * @return Execution status code.
 */
BYTE get_pic_version(char *data, char *result);

/**
 * @brief Compute and return leak current metrics from ADC readback.
 * @param data Command input buffer.
 * @param result Output buffer for leak current values.
 * @return Execution status code.
 */
BYTE get_leak_current(char *data, char *result);

/**
 * @brief Set pulse generator timing parameters.
 * @param data Input buffer containing period and high time.
 * @param result Output buffer for status.
 * @return Execution status code.
 */
BYTE set_pulse_parameters(char *data, char *result);

/**
 * @brief Configure a preamplifier voltage using UART command data.
 * @param data Input buffer containing voltage and channel information.
 * @param result Output status buffer.
 * @return Execution status code.
 */
BYTE set_voltage_preamplifier(char *data, char *result);

/**
 * @brief Increment or decrement the high-voltage setpoint.
 * @param data Command input specifying adjustment direction and value.
 * @param result Output buffer containing the new state.
 * @return Execution status code.
 */
BYTE incdecHV(char *data, char *result);

/**
 * @brief Return the configured inspection interval.
 * @param data Command input buffer.
 * @param result Output buffer receiving the inspection time.
 * @return Execution status code.
 */
BYTE getInspecTime(char *data, char *result);

/**
 * @brief Return software stack information for diagnostics.
 * @param data Command input buffer.
 * @param result Output buffer receiving stack usage or status.
 * @return Execution status code.
 */
BYTE getSoftStack(char *data, char *result);

/**
 * @brief Configure the inspection interval used by the leak current scheduler.
 * @param data Input buffer containing the desired inspection interval.
 * @param result Output buffer for status or error.
 * @return Execution status code.
 */
BYTE setInspecTime(char *data, char *result);

/**
 * @brief Set the HV output value for a detector channel.
 * @param data Input buffer containing the desired voltage value.
 * @param result Output buffer for status.
 * @return Execution status code.
 */
BYTE set_vhv(char *data, char *result);

/**
 * @brief Query the current temperatures from onboard sensors.
 * @param data Command input buffer.
 * @param result Output buffer receiving formatted temperature values.
 * @return Execution status code.
 */
BYTE get_temp(char *data, char *result);

/**
 * @brief Return the maximum configured high-voltage limits.
 * @param data Command input buffer.
 * @param result Output buffer for the HV limits.
 * @return Execution status code.
 */
BYTE get_hvmax(char *data, char *result);

/**
 * @brief Apply a hardware maximum voltage limit to a detector module.
 * @param tel Telescope identifier ('A' or 'B').
 * @param module Module identifier ('1' or '2').
 * @param tension_max Maximum voltage limit in applied units.
 * @return Execution status code.
 */
BYTE apply_hvmax(char tel, char module, UINT tension_max);

/**
 * @brief Enable or disable the HV measurement channel in the firmware.
 * @param data Command input buffer.
 * @param result Output status buffer.
 * @return Execution status code.
 */
BYTE enableDisableHVMeas(char *data, char *result);

/**
 * @brief Set the high-voltage maximum value from UART command input.
 * @param data Input buffer containing new HV maximum value.
 * @param result Output status buffer.
 * @return Execution status code.
 */
BYTE set_hvmax(char *data, char *result);

/**
 * @brief Request a high-voltage calibration procedure over UART.
 * @param data Command payload.
 * @param result Output buffer receiving calibration status.
 * @return Execution status code.
 */
BYTE uart_ask_hv_calibration(char *data, char *result);

/**
 * @brief Store runtime parameters into EEPROM persistent storage.
 * @param data Input buffer containing values to store.
 * @param result Output status buffer.
 * @return Execution status code.
 */
BYTE store_param(char *data, char *result);

/**
 * @brief Set the EEPROM address pointer used by subsequent data reads.
 * @param data Command input buffer containing the address.
 * @param result Output status buffer.
 * @return Execution status code.
 */
BYTE set_data_eeprom_address(char *data, char *result);

/**
 * @brief Read data from the current EEPROM address pointer.
 * @param data Command input buffer.
 * @param result Output buffer receiving EEPROM data.
 * @return Execution status code.
 */
BYTE get_data_eeprom_address(char *data, char *result);

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
unsigned char EERead(unsigned int ad);

/**
 * @brief Parse an ASCII string and convert it into an unsigned integer.
 * @param pr Null-terminated ASCII numeric string.
 * @param converted_value Output pointer for the parsed value.
 * @return Zero on success, non-zero on failure.
 */
BYTE analyze_string(char *pr,UINT *converted_value);

/**
 * @brief Convert a decimal ASCII string to an unsigned integer.
 * @param pr Input ASCII decimal string.
 * @return Parsed unsigned integer.
 */
UINT chardectoi(char *pr);

/**
 * @brief Convert a hexadecimal ASCII string to an unsigned integer.
 * @param pr Input ASCII hexadecimal string.
 * @return Parsed unsigned integer.
 */
UINT charhextoi(char *pr);

/**
 * @brief Compute 10 raised to the given power.
 * @param c Exponent value.
 * @return 10^c.
 */
UINT power10(BYTE c);

/**
 * @brief Compute a 32-bit power of 10.
 * @param c Exponent value.
 * @return 10^c as a 32-bit unsigned value.
 */
UINT32 power10_32(BYTE c);

/**
 * @brief Initialize the temperature sensor interface.
 * @return Execution status code.
 */
BYTE temp_init(void);

/**
 * @brief Read the temperature sensors and store their results.
 * @param temperature Output array for temperature values.
 * @return Number of successful temperature reads.
 */
BYTE temp(int *temperature);

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
BYTE asciiconv(BYTE c);

/**
 * @brief Convert a 16-bit unsigned value to an ASCII hex string.
 * @param value Value to convert.
 * @param s Output buffer for 4 ASCII hex digits.
 */
void uinttoa(UINT value, BYTE *s);

/**
 * @brief Output a 16-bit unsigned integer over USART as ASCII.
 * @param value Value to send.
 * @return Last transmitted character.
 */
char dispuinttochar(UINT value);

/**
 * @brief Output a 32-bit unsigned integer over USART as ASCII.
 * @param value Value to send.
 * @return Last transmitted character.
 */
char dispuint32tochar(UINT32 value);

/**
 * @brief Output an unsigned integer in binary format over USART.
 * @param a Value to display.
 * @return Last transmitted character.
 */
char dispinttobin (UINT a);

/**
 * @brief Output a byte in binary format over USART.
 * @param c Value to display.
 * @return Last transmitted character.
 */
char dispchartobin(BYTE c);

/**
 * @brief Copy an unsigned integer into a character buffer with terminator.
 * @param container Destination buffer.
 * @param a Value to copy.
 * @param cend Terminator character.
 */
void myStrCpyUint(char *container,UINT a,char cend);

/**
 * @brief Copy a byte value into a string buffer as digits.
 * @param container Destination buffer.
 * @param a Byte value.
 * @param cend Terminator character.
 */
void myStrCpyByte(char *container,BYTE a,char cend);

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
void myStrCpyChar2(char *container,const rom char *chaine,char cend);

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
void myStrCpyHex(char *container,UINT a,int format,char cend);

/**
 * @brief Configure a register value via SPI from a UART command.
 * @param data Input command payload.
 * @param result Output response buffer.
 * @return Execution status code.
 */
BYTE spi_set_regvalue(char *data, char *result);

/**
 * @brief Read a register value via SPI and return it over UART.
 * @param data Input command buffer.
 * @param result Output buffer containing register data.
 * @return Execution status code.
 */
BYTE spi_get_regvalue(char *data,char *result);

/**
 * @brief Read an FPGA register over SPI.
 * @param id_fpga FPGA identifier.
 * @param adr Register address to read.
 * @return Register value.
 */
UINT rdspi(char id_fpga,unsigned int adr);

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
BYTE set_off_V_preamp1a(UINT value);

/**
 * @brief Set the offset voltage for preamplifier channel 1B.
 * @param value Offset code.
 * @return Execution status code.
 */
BYTE set_off_V_preamp1b(UINT value);

/**
 * @brief Set the offset voltage for preamplifier channel 2A.
 * @param value Offset code.
 * @return Execution status code.
 */
BYTE set_off_V_preamp2a(UINT value);

/**
 * @brief Set the offset voltage for preamplifier channel 2B.
 * @param value Offset code.
 * @return Execution status code.
 */
BYTE set_off_V_preamp2b(UINT value);

/**
 * @brief Set the offset voltage for preamplifier channel 3A.
 * @param value Offset code.
 * @return Execution status code.
 */
BYTE set_off_V_preamp3a(UINT value);

/**
 * @brief Set the offset voltage for preamplifier channel 3B.
 * @param value Offset code.
 * @return Execution status code.
 */
BYTE set_off_V_preamp3b(UINT value);

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
BYTE func_test(char *data, char *result);

/**
 * @brief Read the board identity jumpers.
 * @return ASCII character representing the board ID.
 */
BYTE getid(void);

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
char pulser(UINT data, UINT period, UINT high_time);

/**
 * @brief Initiate an HV calibration request string.
 * @param str Null-terminated calibration request string.
 */
void ask_hv_calibration(char *str);

/**
 * @brief Convert a desired tension into a DAC/ADC calibration value.
 * @param tension Target voltage.
 * @param adrCal EEPROM calibration address.
 * @return Computed calibration result.
 */
UINT32 get_value_dec(UINT tension, UINT adrCal);

/**
 * @brief Apply a slope correction curve to a high-voltage setpoint.
 * @param tel Telescope identifier.
 * @param module Module identifier.
 * @param tension Target voltage.
 * @param slopeVS Slope coefficient.
 * @return Execution status code.
 */
BYTE slop_vhv(char tel,BYTE module,UINT tension,UINT32 slopeVS);

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
UINT htoi(const char *pr);

/**
 * @brief Acquire PIC ADC channel voltages into the provided buffer.
 * @param voltages Output array receiving ADC measurements.
 */
void get_PIC_AD_voltages(unsigned int *voltages);

/**
 * @brief Read and format analog voltage measurements for UART output.
 * @param data Command payload.
 * @param result Output buffer receiving formatted values.
 * @return Execution status code.
 */
BYTE getVoltages(char *data, char *result);

/**
 * @brief Read LTC switch analog voltages and report them.
 * @param data Input command buffer.
 * @param result Output buffer containing voltage values.
 * @return Execution status code.
 */
BYTE getLTCswVoltages(char * data, char *result);

/**
 * @brief Read LTC linear converter input voltages and report them.
 * @param data Input command buffer.
 * @param result Output buffer containing voltage values.
 * @return Execution status code.
 */
BYTE getLTClinVoltages(char * data, char *result);

/**
 * @brief Read multiple LTC2308 ADC channels selected by a mask.
 * @param mask Bit mask selecting channels.
 * @param ADvoltages Output buffer receiving ADC results.
 * @return Execution status code.
 */
BYTE getLTC2308Voltages(BYTE mask, UINT *ADvoltages);

/**
 * @brief Initialize the ADS8332 ADC device.
 */
void adc_init(void);

/**
 * @brief Read an ADS8332 channel and return the converted value.
 * @param canal Pointer to the channel index.
 * @return 16-bit channel reading.
 */
UINT adc_getvalue(unsigned char *canal);

/**
 * @brief Compute leakage current for the selected detector channel.
 * @param tel Telescope identifier.
 * @param module Detector module identifier.
 * @param lcAdcRead Raw ADC leak current reading.
 * @return Computed leak current in board units.
 */
UINT leak_current (char tel, char module, UINT32 lcAdcRead);

/**
 * @brief Compute the actual low-level leak current from the measured ADC value.
 * @param data Input command payload.
 * @param result Output buffer containing computed leak current.
 * @return Execution status code.
 */
BYTE get_leak_current(char *data, char *result);

/**
 * @brief Enable or disable the high-voltage measurement function.
 * @param data Input command buffer.
 * @param result Output buffer for status.
 * @return Execution status code.
 */
BYTE enDesHVdev(char *data, char *result);

/**
 * @brief Request the maximum allowed high-voltage value for the current board.
 * @param data Command payload.
 * @param result Output buffer containing max HV values.
 * @return Execution status code.
 */
BYTE get_hvmax(char *data, char *result);

/**
 * @brief Set the maximum allowable high-voltage threshold via UART.
 * @param data Input buffer containing the new limit.
 * @param result Output buffer for status.
 * @return Execution status code.
 */
BYTE set_hvmax(char *data, char *result);

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
void cal_HVfunc(UINT lcAdcReadA1,UINT lcAdcReadA2, UINT lcAdcReadB1, UINT lcAdcReadB2);

/**
 * @brief Perform the scheduled leak current inspection routine.
 * @return Computed interval or leak inspection status.
 */
UINT32 current_leak_inspection(void);

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
UINT32 diffLcTime(UINT32 t1,UINT32 t2,UINT32 t3);

/* EEPROM-backed leak-current thresholds (16-bit, stored as two 8-bit cells) */
UINT getHighLcTrsh(void);

UINT getLowLcTrsh(void);

#endif

// EOF
