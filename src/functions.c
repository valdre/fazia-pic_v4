#include "utils.h"
#include "functions.h"
#include "frame.h"
#include "uartbuf.h"
#include "setup.h"

#include <p18cxxx.h>
#include <string.h>
#include <stdlib.h>

extern BYTE valeur_portD;
extern ram UINT HV_borne_sup_A1; //maximum value to reach for 200V high voltage module (telescope A)
extern ram UINT HV_borne_sup_A2; //maximum value to reach for 400V high voltage module (telescope A)
extern ram UINT HV_borne_sup_B1; //maximum value to reach for 200V high voltage module (telescope B)
extern ram UINT HV_borne_sup_B2; //maximum value to reach for 400V high voltage module (telescope B)
extern UINT HvValueTab[4][2];
extern UINT HvInc[4];
extern UINT HvPhysTarget[4];
extern UINT HvPhysCorrect[4];
extern BYTE HvStatus[4];
extern BYTE enableHVMeas;
extern BYTE cal_preampli_offset;
extern BYTE marge_pa_offset;
extern ram BYTE CSI_relay;
extern ram UINT32 time_scheduling;
extern ram UINT32 time_lc_prec;
extern struct parametres pa;
extern ram long int HV_read_coefA[4];
extern ram long int HV_read_coefB[4];
extern ram UINT lcA1;
extern ram UINT lcA2;
extern ram UINT lcB1;
extern ram UINT lcB2;
extern ram UINT GeneDacVoltage;
extern ram UINT32 timing_inspection;
extern ram UINT32 shortInspecTime;
extern ram UINT32 longInspecTime;
extern ram UINT max;

typedef BYTE(*func_p)(char *, char *);

//func_p hpfunc[] = {&f_reset, &f_clear, &f_echo};

func_p fplist[MAX_FUNC_NUM];

void func_init(void) {
    fplist[0] = &get_temp;                  // 0x83 : getting sensor temperatures
    fplist[1] = &spi_set_regvalue;          // 0x84 : writing the content of a FPGA register
    fplist[2] = &spi_get_regvalue;          // 0x85 : reading the content of a FPGA register
    fplist[3] = &set_vhv;                   // Ox86 : setting high voltages to bias the detectors
    fplist[4] = &get_leak_current;          // 0x87 : getting the leakage current values which biases a detector
    fplist[5] = &get_HV;                    // 0x88 : getting the high voltage value which biases a detector
    fplist[6] = &set_voltage_preamplifier;  // 0x89 : setting the offset voltage at the preamplifier output
    fplist[7] = &set_pulse_parameters;      // 0x8A : setting the pulse parameters to calibrate the preamplifiers
    fplist[8] = &get_pic_version;           // 0x8B : getting the version of the PIC firmware
    fplist[9] = &get_fpga_version;          // 0x8C : getting the version of the VHDL code for FPGA devices
    fplist[10] = &incdecHV;                 // 0x8D : increase or decrease by step of some units
    fplist[11] = &store_param;              // 0x8E : storing offset values into the EEPROM of the PIC
    fplist[12] = &get_ads8332;              // 0x8F : get adc results
    fplist[13] = &get_data_eeprom_address;  // 0x90 : get the content of a data eeprom register
    fplist[14] = &get_hvmax;                // 0x91 : get maximum high voltage
    fplist[15] = &set_hvmax;                // 0x92 : set maximum high voltage to reach per high voltage module
    fplist[16] = &setautoffset;             // 0x93 : set automatically offset voltages at the output of the preamplifiers
    fplist[17] = &uart_ask_hv_calibration;  // 0x94 : asking the PIC to know how many HV power supplies have been calibrated
    fplist[18] = &set_data_eeprom_address;  // 0x95 : store the content into a data eeprom register
    fplist[19] = &uart_reset_both_fpga;     // 0x96 : send by SPI link a reset order for both fpga
    fplist[20] = &setHVCsiAB;               // 0x97 : control the HV way to bias the CSI detectors
    fplist[21] = &uart_preamplifier_test;   // 0x98 : indicates which preampilifier works
    fplist[22] = &uart_csi_relay;           // 0x99 : indicates the status of CSI relays
    fplist[23] = &getGeneDacVoltage;        // Ox9A : indicates the amplitude of the pulser
    fplist[24] = &getVoltages;              // 0x9B : get the voltage measurements from PIC ADC
    fplist[25] = &getLTClinVoltages;        // 0x9C : get the voltage measurements from LTC ADC for linear regulators
    fplist[26] = &getLTCswVoltages;         // 0x9D : get the voltage measurements from LTC ADC for switching regulators
    fplist[27] = &enableDisableHVMeas;      // 0x9E : enable or disable HV meas (I and V)
    fplist[28] = &resetPIC;                 // 0x9F : reset the PIC µC only
    fplist[29] = &giveHvStatus;             // 0xA0 : give HV status
    fplist[30] = &setInspecTime;            // 0xA1 : set times for automatic HV corrections
    fplist[31] = &getInspecTime;            // 0xA2 : get times for automatic HV corrections
    fplist[32] = &getSoftStack;             // 0xA3 : get software stack max
    fplist[33] = &f_echo;                   // 0xA4 : echo function
    fplist[34] = &setGetSN;                 // 0xA5 : setting or getting the serial number of the FEE card
    fplist[35] = &enDesHVdev;               // 0xA6 : enable or disable the HV devices
}

BYTE func_invoke(unsigned char code, char *data, char *result) {
    if (code < BASE_CMD_ID)
        return FUNC_CMD_NOT_VALID;

    if ((code - BASE_CMD_ID) >= MAX_FUNC_NUM)
        return FUNC_CMD_NOT_VALID;

    if (fplist[code - BASE_CMD_ID] == NULL)
        return FUNC_CMD_NOT_DEFINED;

    return (fplist[code - BASE_CMD_ID](data, result));
}


BYTE f_echo(char *data, char *result) 
{
    result[0]='0';
    result[1]='|';
    result[2]='\0';
    myStrCpyChar(result,data,'\0');

    return FUNC_CMD_OK;
}

BYTE resetPIC(char *data, char *result) 
{
    result[0]='0';
    result[1]='|';
    result[2]='\0';
    
    Reset();

    return FUNC_CMD_OK;
}

BYTE setGetSN(char *data, char *result)
{
    BYTE retval,error,comp;
    UINT sn,data_value;
    char charDataSN[6];
    
    if ((data[0]=='Q')&&(data[1]=='\0'))
    {
        data_value = (UINT)EERead(snLSB);
        data_value += ((UINT)EERead(snMSB))<<8; 
        retval = FUNC_EXEC_OK;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
        myStrCpyUint(result,data_value,'\0');
    }
    else
    {
        comp = 0;
        while (data[comp]!='\0')
        {
            charDataSN[comp]=data[comp];
            comp++;
        }
        charDataSN[comp]='\0';
        error = analyze_string(charDataSN,&sn);
        
        if (error==0)
        {
            EEWrite(snLSB,(BYTE)(sn&0xFF));
            EEWrite(snMSB,(BYTE)((sn&0xFF00)>>8));
            retval = FUNC_EXEC_OK;
        }
        else
        {
            retval = FUNC_EXEC_BAD_ARGS_TYPE;
        }
        
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';     
    }
    
    return FUNC_CMD_OK;
}

BYTE uart_csi_relay(char *data, char *result) 
{   
    result[0]='0';
    result[1]='|';
    result[2]='\0';
    myStrCpyUint(result,(CSI_relay & 0x2) >> 1,',');
    myStrCpyUint(result,CSI_relay & 0x1,'\0');

    return FUNC_CMD_OK;
}

BYTE uart_reset_both_fpga(char *data, char *result) 
{
    reset_both_fpga();

    result[0]='0';
    result[1]='|';
    result[2]='\0';

    return FUNC_CMD_OK;
}

BYTE giveHvStatus(char *data, char *result) 
{
    BYTE retval, error, status;
    
    error = 1;
    status = 4;
    
    if ((data[0]=='A')||(data[0]=='B'))
        if (data[1] == ',')
            if ((data[2]=='1')||(data[2]=='2'))
                if (data[3]=='\0')
                    error = 0;

    if (error == 0) 
    {
        if ((data[0] == 'A') && (data[2] == '1'))
            status = HvStatus[0];

        if ((data[0] == 'A') && (data[2] == '2'))
            status = HvStatus[1];

        if ((data[0] == 'B') && (data[2] == '1'))
            status = HvStatus[2];

        if ((data[0] == 'B') && (data[2] == '2'))
            status = HvStatus[3];
    }

    retval = FUNC_EXEC_OK;
    result[0]='0'+(char)retval;
    result[1]='|';
    result[2]='\0';
    myStrCpyUint(result,status,'\0');
    return FUNC_CMD_OK;
}

BYTE setHVCsiAB(char *data, char *result) 
{
    BYTE retval,error;

    error = 1;
    
    if ((data[0]=='0')||(data[0]=='1'))
        if (data[1] == ',')
            if ((data[2]=='0')||(data[2]=='1'))
                if (data[3]=='\0')
                    error = 0;

    if (error == 0) 
    {    
        if (data[0] == '0')
            valeur_portD = valeur_portD | 0x04;
        else
            valeur_portD = valeur_portD & 0xFB;

        if (data[2] == '0')
            valeur_portD = valeur_portD | 0x08;
        else
            valeur_portD = valeur_portD & 0xF7;

        PORTD = valeur_portD;
        CSI_relay = 2 * ((BYTE) (data[2] - '0'))+(BYTE) (data[0] - '0');
        
        retval = FUNC_EXEC_OK;
    }
    else 
    {
        retval = FUNC_EXEC_BAD_ARGS_TYPE;
    }
    
    result[0]='0'+(char)retval;
    result[1]='|';
    result[2]='\0';

    return FUNC_CMD_OK;
}

BYTE preamplifier_test(BYTE preamp)
{
    UINT tab[2], *p,value;
    BYTE co, i,valeur;
    char id;
    int essais, regAdc[2], regfpga[3];

    tab[0] = 0;
    tab[1] = 512;

    regfpga[0] = REG_FPGA_Q3;
    regfpga[1] = REG_FPGA_Q2;
    regfpga[2] = REG_FPGA_QH1;

    p = (UINT *) & pa;
    valeur = 0;


    if (preamp > 2)
        id = 2;
    else
        id = 1;

    for (i = 0; i < 2; i++)
    {
        wrspi(id, REG_FPGA_PA_CSI - (preamp % 3), tab[i]);
        Delay10KTCYx(100); //il faut 60ms soit 960000 cycles d'instructions : 0.625ms par coup

        regAdc[i] = (int) rdspi(id, regfpga[preamp % 3]);
        Delay10KTCYx(1);

        co = 0;

        do
        {
            essais = (int) rdspi(id, regfpga[preamp % 3]);

            if (((regAdc[i] > essais - 150) && (regAdc[i] < essais + 150)) && (co < 20))
            {
                co++;
            } 
            else
            {
                if (co < 20)
                {
                    regAdc[i] = essais;
                    co = 0;
                }
            }
        } while (co != 20);
    }

    if ((regAdc[0] > regAdc[1] + 200) || (regAdc[1] > regAdc[0] + 200))
    {
        valeur = 1;
    }

    regAdc[0] = 0;
    regAdc[1] = 0;

    value = *(p + (((UINT) preamp) % 3) + 3 * (2 - (UINT) id));

    if (value < 0x400)
    {
        wrspi(id, REG_FPGA_PA_CSI - (preamp % 3), value);
        Delay10KTCYx(100);
    }

    return valeur;
}

BYTE uart_preamplifier_test(char *data, char *result) 
{
    BYTE retval,tel,module,error,valeur;
    
    error = 1;
    
    if ((data[0] == 'A')||(data[0] == 'B'))
        if (data[1] == ',')
            if ((data[2] == '1')||(data[2] == '2')||(data[2] == '3'))
                if (data[3] == '\0')
                    error = 0;

    if (error == 0) 
    {
        tel = data[0];
        module = (BYTE) (data[2] - '0');
        valeur = 0;
        
        if ((tel == 'A') && (module == 3))
            valeur = preamplifier_test(0);

        if ((tel == 'A') && (module == 2))
            valeur = preamplifier_test(1);

        if ((tel == 'A') && (module == 1))
            valeur = preamplifier_test(2);

        if ((tel == 'B') && (module == 3))
            valeur = preamplifier_test(3);

        if ((tel == 'B') && (module == 2))
            valeur = preamplifier_test(4);

        if ((tel == 'B') && (module == 1))
            valeur = preamplifier_test(5);
        
        retval = FUNC_EXEC_OK;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
        myStrCpyUint(result,valeur,'\0');
    }
    else
    {
        retval = FUNC_EXEC_BAD_ARGS_TYPE;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
    }

    return FUNC_CMD_OK;
}

BYTE setautoffset(char *data, char *result)
{
    BYTE retval, error, comp;
    UINT marge;
    char charDataMarge[10];

    error = 1;
    
    if (data[0] == '\0')
    {
        error = 0;
        marge_pa_offset = 101;
    }
    else
    {
        comp = 0;
        while ((data[comp]!='\0')&&(data[comp]!=','))
        {
            charDataMarge[comp] = data[comp];
            comp++;
        }
        charDataMarge[comp]='\0';
        
        if ((data[comp]=='\0')&&(comp!=0))
        {
            error = analyze_string(charDataMarge, &marge);
            marge_pa_offset = (BYTE) marge;
        }
    }
    
    if (error == 0)
    {
        retval = FUNC_EXEC_OK;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
        cal_preampli_offset = 1;
    }
    else
    {
        cal_preampli_offset = 0;
        retval = FUNC_EXEC_BAD_ARGS_TYPE;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
    }
    
    return FUNC_CMD_OK;
}

BYTE enDesHVdev(char *data, char *result)
{
    BYTE error,retval;
    
    result[0]='0';
    result[1]='|';
    result[2]='\0';
    
    if ((data[0]=='Q')&&(data[1]=='\0'))
    {
        error = 0;
        result[2]='A';
        result[3]=',';
        
        if ((valeur_portD&1)==0)
            result[4]='0';
        else
            result[4]='1';
        
        result[5]=',';
        result[6]='B';
        result[7]=',';
        
        if ((valeur_portD&2)==0)
            result[8]='0';
        else
            result[8]='1';
        
        result[9]='\0';
    }
    
    if ((data[0]=='A')||(data[0]=='B'))
        if (data[1] == ',')
            if ((data[2]=='0')||(data[2]=='1'))
            {
                error = 1;
                
                if (data[3]==',')
                {
                    if ((data[4]=='A')||(data[4]=='B'))
                        if (data[5] == ',')
                            if ((data[6]=='0')||(data[6]=='1'))
                                if (data[7]=='\0')
                                {
                                    if ((BYTE)(data[6]-'0')==0)
                                        valeur_portD = valeur_portD & (0xFF-1-(BYTE)(data[4]-'A'));
                                    else
                                        valeur_portD = valeur_portD | (1+(BYTE)(data[4]-'A'));
                                    error = 0;
                                }
                }
                
                if (data[3]=='\0')
                {   
                    if ((BYTE)(data[2]-'0')==0)
                        valeur_portD = valeur_portD & (0xFF-1-(BYTE)(data[0]-'A'));
                    else
                        valeur_portD = valeur_portD | (1+(BYTE)(data[0]-'A'));
                    error = 0;
                }
                
                if (error == 0)
                {
                    PORTD=valeur_portD;
                    retval = FUNC_EXEC_OK;
                }
                else
                    retval = FUNC_EXEC_BAD_ARGS_TYPE;
                
                result[0]='0'+(char)retval;
            }
            
    return FUNC_CMD_OK;
}


BYTE getGeneDacVoltage(char *data, char *result) 
{   
    result[0]='0';
    result[1]='|';
    result[2]='\0';
    myStrCpyUint(result,GeneDacVoltage,'\0');

    return FUNC_CMD_OK;
}

BYTE getCurrent(char tel, char module, UINT *lc) {
    BYTE error;

    error = 1;

    if ((tel == 'A') && (module == '1')) {
        error = 0;
        *lc = lcA1;
    }

    if ((tel == 'A') && (module == '2')) {
        *lc = lcA2;
        error = 0;
    }

    if ((tel == 'B') && (module == '1')) {
        *lc = lcB1;
        error = 0;
    }

    if ((tel == 'B') && (module == '2')) {
        *lc = lcB2;
        error = 0;
    }

    return error;
}

BYTE get_HV(char *data, char *result) 
{
    static BYTE canal;
    BYTE module,error,retval;
    char tel;
    UINT HVvalue, HVvalueDetector, lc;
    
    error = 1;
    if ((data[0]=='A')||(data[0]=='B'))
        if (data[1]==',')
            if ((data[2]=='1')||(data[2]=='2'))
                if (data[3]=='\0')
                    error = 0;

    if (error == 0) 
    {
        tel = data[0];
        module = (BYTE) (data[2] - '0');
        
        lc = 0;
        canal = 3 + module + 2 * ((BYTE) (tel - 'A'));
        HVvalue = getHvValue(&canal);

        if (HvStatus[module - 1 + 2 * ((BYTE) (tel - 'A'))] == 0)
        {
            retval=FUNC_EXEC_OK;
            result[0]='0'+(char)retval;
            result[1]='|';
            result[2]='-';
            result[3]='\0';
            myStrCpyUint(result,HVvalue/10,',');
            myStrCpyChar2(result,"NC",'\0');
        }
        else 
        {
            if (HvValueTab[module - 1 + 2 * (BYTE) (tel - 'A')][0] == 0)
            {
                retval=FUNC_EXEC_OK;
                result[0]='0'+(char)retval;
                result[1]='|';
                result[2]='\0';
                myStrCpyChar2(result,"0,0",'\0');
            }
            else 
            {
                retval=FUNC_EXEC_OK;
                getCurrent(tel, (char) (module + '0'), &lc);
                HVvalueDetector = lc / 25;
                HVvalueDetector = HVvalueDetector * 102;
                HVvalueDetector = HVvalueDetector / 10;
                HVvalueDetector = HVvalueDetector / 4;
                HVvalueDetector = HVvalue - HVvalueDetector;
                result[0]='0'+(char)retval;
                result[1]='|';
                result[2]='-';
                result[3]='\0';
                myStrCpyUint(result,HVvalue/10,',');
                myStrCpy1Char(result,'-','\0');
                myStrCpyUint(result,HVvalueDetector/10,'\0');
            }
        }
    }

    if (error == 1) 
    {
        retval = FUNC_EXEC_BAD_ARGS_VALUE;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
    }
    
    return FUNC_CMD_OK;
}

UINT getHvValue(BYTE *canal) 
{
    UINT ADCResult;
    long int HVValue1, HVValue2;

    ADCResult = adc_getvalue(canal);
    HVValue1 = 0;
    HVValue2 = 0;

    HVValue1 = HV_read_coefB[*canal - 4];
    HVValue2 = (((long int) ADCResult) * HV_read_coefA[*canal - 4]);
        
    if (HVValue2 + HVValue1 > 0)
        HVValue1 = HVValue1 + HVValue2;
    else
    {
        HVValue1 = 0; //mis à 0 artificiellement
    }
	
	//S.V. 31/3/2017 added for better approximation
	HVValue1 = HVValue1 + 50000;
	
    HVValue1 = HVValue1 / 100000; //10000000

    return (UINT)HVValue1;
}

BYTE get_fpga_version(char *data, char *result) 
{
    BYTE retval, error, t, m, d, v;
    UINT version, y;
    
    error = 1;
    
    if (((data[0]=='A')||(data[0]=='B'))&&(data[1]=='\0'))
        error = 0;

    if (error == 0) 
    {
        if (data[0] == 'A') 
            version = rdspi(1, 0x0000);

        if (data[0] == 'B') 
            version = rdspi(2, 0x0000);

        t = (BYTE) ((version & 32768) >> 15);
        y = 2012 + ((version & 30720) >> 11);
        m = (BYTE) ((version & 1920) >> 7);
        d = (BYTE) ((version & 124) >> 2);
        v = (BYTE) (version & 3); 
    
        retval = FUNC_EXEC_OK;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
        myStrCpyChar2(result,"tel=",'\0');
        myStrCpy1Char(result,'A'+(char)t,',');
        myStrCpyChar2(result,"day=",'\0');
        myStrCpyUint(result,d,',');
        myStrCpyChar2(result,"month=",'\0');
        myStrCpyUint(result,m,',');
        myStrCpyChar2(result,"year=",'\0');
        myStrCpyUint(result,y,',');
        myStrCpyChar2(result,"variant=",'\0');
        myStrCpyUint(result,v,'\0');
    } 
    else 
    {
        retval = FUNC_EXEC_BAD_ARGS_TYPE;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
    }
    
    return FUNC_CMD_OK;
}

BYTE nullFunc(char *data, char *result) 
{
    result[0]='0';
    result[1]='|';
    result[2]='\0';
    return FUNC_CMD_OK;
}

BYTE get_ads8332(char *data, char *result) 
{   
    static BYTE canal;
    BYTE retval, error;
    UINT value;

    value = 0;
    error = 1;

    if (((data[0] >= '0') && (data[0] <= '7'))&&(data[1]=='\0')) 
    {
        error = 0;
        canal = (BYTE)(data[0] - '0');
        value = adc_getvalue(&canal);
    }

    if (error == 0) 
    {
        retval = FUNC_EXEC_OK;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
        myStrCpyUint(result,value,'\0');
    } 
    else 
    {
        retval = FUNC_EXEC_BAD_ARGS_TYPE;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
    }
    
    return FUNC_CMD_OK;
}

//version 21/12/2015
// j'ai rajouté une partie pour la correction automatique des modules HV
// si la tension à corriger dépasse les 350V, il se cantonnera à 350V et basta

//version du 28/01/2016
// j'ai rajouté une fonction pour le numéro de série et pour l'activation et la désactivation du module HV
//pour la version step_3, l'activation et la désactivation du module HV est une fonction nulle

//version du 12/04/2017
// many bugfixes on voltage and current readings (V4/5 only)

//version du 13/03/2018
// two small bugfixes (V4/5 only)

BYTE get_pic_version(char *data, char *result) 
{
    BYTE retval;

    retval = FUNC_EXEC_OK;
    result[0]='0'+(char)retval;
    result[1]='|';
    result[2]='\0';
    myStrCpyChar2(result,"13,03,2018,V01",'\0');

    return FUNC_CMD_OK;
}

BYTE get_leak_current(char *data, char *result) 
{
    BYTE retval, error;
    UINT lc;
    char tel, module;

    error = 1;
    if ((data[0]=='A')||(data[0]=='B'))
        if (data[1]==',')
            if ((data[2]=='1')||(data[2]=='2'))
                if (data[3]=='\0')
                    error = 0;

    if (error == 0)
    {
        tel = data[0];
        module = data[2];

        if (HvStatus[(BYTE) (module - '1') + 2 * (BYTE) (tel - 'A')] == 1)
            error=getCurrent(tel, module, &lc);

        if (error == 0) 
        {
            retval = FUNC_EXEC_OK;

            if (HvStatus[(BYTE) (module - '1') + 2 * (BYTE) (tel - 'A')] == 1)
            {
                result[0]='0'+(char)retval;
                result[1]='|';
                result[2]='\0';
                myStrCpyUint(result,lc,'\0'); //expressed in nA
            }
            else
            {
                result[0]='0'+(char)retval;
                result[1]='|';
                result[2]='N';
                result[3]='C';
                result[4]='\0';
            }
        }
    }

    if (error == 1) 
    {
        retval = FUNC_EXEC_BAD_ARGS_VALUE;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
    }
    return FUNC_CMD_OK;
}

BYTE set_pulse_parameters(char *data, char *result) 
{
    BYTE retval, error, comp, dep;
    char charDataValue[10];
    char charDataPeriod[10];
    char charDataHighTime[10];
    UINT value, period, high_time;
    UINT32 value2;

    error = 1;
    comp = 0;
    while ((data[comp]!='\0')&&(data[comp]!=','))
    {
        charDataValue[comp]=data[comp];
        comp++;
    }
    charDataValue[comp]='\0';
    
    if ((data[comp]==',')&&(comp!=0))
    {
        comp++;
        dep=comp;
        while ((data[comp]!='\0')&&(data[comp]!=','))
        {
            charDataPeriod[comp-dep]=data[comp];
            comp++;
        }
        charDataPeriod[comp-dep]='\0';
        
        if ((data[comp]==',')&&(comp!=dep))
        {
            comp++;
            dep=comp;
            while ((data[comp]!='\0')&&(data[comp]!=','))
            {
                charDataHighTime[comp-dep]=data[comp];
                comp++;
            }
            charDataHighTime[comp-dep]='\0';
            
            if ((data[comp]=='\0')&&(comp!=dep))
                error = 0;
        }
    }

    if (error == 0) 
    {
        error = analyze_string(charDataValue, &value);

        if ((error == 0) && (value <= 2000))
            value2 = (33294 * ((UINT32)value)+1000) / 2000;
        else
            error = 1;

        error = error + analyze_string(charDataPeriod, &period);
        error = error + analyze_string(charDataHighTime, &high_time);

        if ((error == 0) && ((period > high_time) || (period == 0))) 
        {
            GeneDacVoltage = value;
            pulser((UINT) value2, period, high_time);
        } 
        else
            error = 1;
    }

    if (error == 0) 
    {
        retval = FUNC_EXEC_OK;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
    } 
    else 
    {
        retval = FUNC_EXEC_BAD_ARGS_VALUE;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
    }
    return FUNC_CMD_OK;
}

BYTE set_voltage_preamplifier(char *data, char *result) 
{
    BYTE retval, error, comp;
    char charDataValue[10];
    UINT value;
    
    error = 1;
    
    if ((data[0]=='A')||(data[0]=='B'))
        if (data[1] == ',')
            if (((data[2]=='1')||(data[2]=='2'))||(data[2]=='3'))
                if (data[3]==',')
                {
                    comp=4;
                    while ((data[comp]!='\0')&&(data[comp]!=','))
                    {
                        charDataValue[comp-4]=data[comp];
                        comp++;
                    }
                    charDataValue[comp-4]='\0';
                    
                    if ((data[comp]=='\0')&&(comp!=4))
                        error = 0;
                }

    if (error == 0) 
    {
        if (data[2] == '1') 
        {
            if (data[0] == 'A') 
            {
                error = analyze_string(charDataValue, &value);

                if (error == 0)
                    set_off_V_preamp3a(value);

            }

            if (data[0] == 'B') 
            {
                error = analyze_string(charDataValue, &value);
                if (error == 0)
                    set_off_V_preamp3b(value);
            }
        }

        if (data[2] == '2') 
        {
            if (data[0] == 'A') 
            {
                error = analyze_string(charDataValue, &value);
                if (error == 0)
                    set_off_V_preamp2a(value);
            }

            if (data[0] == 'B') 
            {
                error = analyze_string(charDataValue, &value);
                if (error == 0)
                    set_off_V_preamp2b(value);
            }
        }

        if (data[2] == '3') 
        {
            if (data[0] == 'A') 
            {
                error = analyze_string(charDataValue, &value);
                if (error == 0)
                    set_off_V_preamp1a(value);
            }

            if (data[0] == 'B') 
            {
                error = analyze_string(charDataValue, &value);
                if (error == 0)
                    set_off_V_preamp1b(value);
            }
        }
    } 
    else
        error = 1;

    if (error == 0) 
    {
        retval = FUNC_EXEC_OK;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
    } 
    else 
    {
        retval = FUNC_EXEC_BAD_ARGS_VALUE;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
    }
    
    return FUNC_CMD_OK;
}

BYTE incdecHV(char *data, char *result) 
{
    BYTE retval, error, comp;
    char module, tel, sens;
    char charDataStep[10];
    UINT step;
    
    error = 1;
    retval = FUNC_EXEC_BAD_ARGS_TYPE;
    
    if ((data[0]=='A')||(data[0]=='B'))
        if (data[1]==',')
            if ((data[2]=='1')||(data[2]=='2'))
                if (data[3]==',')
                    if ((data[4]=='+')||(data[4]=='-'))
                        if (data[5]==',')
                        {
                            comp = 6;
                            while ((data[comp]!='\0')&&(data[comp]!=','))
                            {
                                charDataStep[comp-6]=data[comp];
                                comp++;
                            }
                            charDataStep[comp-6]='\0';
                            
                            if ((data[comp]=='\0')&&(comp!=6))
                                error = 0;
                        }
                        

    if (error == 0) 
    {
        tel = data[0];
        module = data[2];
        sens = data[4];
        error = 1;

        error = analyze_string(charDataStep, &step);

        if (((tel == 'A') && (module == '1'))&&(error == 0)) 
        {
            if (HvStatus[0] == 1) 
            {
                if (sens == '+') 
                {
                    HvValueTab[0][1] += step;
                    HvValueTab[0][0] = HvValueTab[0][1];
                    dac_sequence(0, HvValueTab[0][1]);
                    retval = FUNC_EXEC_OK;
                    result[0]='0'+(char)retval;
                    result[1]='|';
                    result[2]='\0';
                    myStrCpyUint(result,HvValueTab[0][1],'\0');
                }

                if (sens == '-') 
                {
                    HvValueTab[0][1] -= step;
                    HvValueTab[0][0] = HvValueTab[0][1];
                    dac_sequence(0, HvValueTab[0][1]);
                    retval = FUNC_EXEC_OK;
                    result[0]='0'+(char)retval;
                    result[1]='|';
                    result[2]='\0';
                    myStrCpyUint(result,HvValueTab[0][1],'\0');
                }
            } 
            else 
            {
                retval = FUNC_EXEC_OK;
                result[0]='0'+(char)retval;
                result[1]='|';
                result[2]='N';
                result[3]='\0';
            }

        }

        if (((tel == 'A') && (module == '2'))&&(error == 0)) 
        {
            if (HvStatus[1] == 1) 
            {
                if (sens == '+') 
                {
                    HvValueTab[1][1] += step;
                    HvValueTab[1][0] = HvValueTab[1][1];
                    dac_sequence(16, HvValueTab[1][1]);
                    retval = FUNC_EXEC_OK;
                    result[0]='0'+(char)retval;
                    result[1]='|';
                    result[2]='\0';
                    myStrCpyUint(result,HvValueTab[1][1],'\0');
                }

                if (sens == '-') {
                    HvValueTab[1][1] -= step;
                    HvValueTab[1][0] = HvValueTab[1][1];
                    dac_sequence(16, HvValueTab[1][1]);
                    retval = FUNC_EXEC_OK;
                    result[0]='0'+(char)retval;
                    result[1]='|';
                    result[2]='\0';
                    myStrCpyUint(result,HvValueTab[1][1],'\0');
                }
            } 
            else 
            {
                retval = FUNC_EXEC_OK;
                result[0]='0'+(char)retval;
                result[1]='|';
                result[2]='N';
                result[3]='\0';
            }

        }

        if (((tel == 'B') && (module == '1'))&&(error == 0)) 
        {
            if (HvStatus[2] == 1) 
            {
                if (sens == '+') 
                {
                    HvValueTab[2][1] += step;
                    HvValueTab[2][0] = HvValueTab[2][1];
                    dac_sequence(32, HvValueTab[2][1]);
                    retval = FUNC_EXEC_OK;
                    result[0]='0'+(char)retval;
                    result[1]='|';
                    result[2]='\0';
                    myStrCpyUint(result,HvValueTab[2][1],'\0');
                }

                if (sens == '-') 
                {
                    HvValueTab[2][1] -= step;
                    HvValueTab[2][0] = HvValueTab[2][1];
                    dac_sequence(32, HvValueTab[2][1]);
                    retval = FUNC_EXEC_OK;
                    result[0]='0'+(char)retval;
                    result[1]='|';
                    result[2]='\0';
                    myStrCpyUint(result,HvValueTab[2][1],'\0');
                }
            } 
            else 
            {
                retval = FUNC_EXEC_OK;
                result[0]='0'+(char)retval;
                result[1]='|';
                result[2]='N';
                result[3]='\0';
            }
        }

        if (((tel == 'B') && (module == '2'))&&(error == 0)) 
        {
            if (HvStatus[3] == 1) 
            {
                if (sens == '+') 
                {
                    HvValueTab[3][1] += step;
                    HvValueTab[3][0] = HvValueTab[3][1];
                    dac_sequence(48, HvValueTab[3][1]);
                    retval = FUNC_EXEC_OK;
                    result[0]='0'+(char)retval;
                    result[1]='|';
                    result[2]='\0';
                    myStrCpyUint(result,HvValueTab[3][1],'\0');
                }

                if (sens == '-') 
                {
                    HvValueTab[3][1] -= step;
                    HvValueTab[3][0] = HvValueTab[3][1];
                    dac_sequence(48, HvValueTab[3][1]);
                    retval = FUNC_EXEC_OK;
                    result[0]='0'+(char)retval;
                    result[1]='|';
                    result[2]='\0';
                    myStrCpyUint(result,HvValueTab[3][1],'\0');
                }
            } 
            else 
            {
                retval = FUNC_EXEC_OK;
                result[0]='0'+(char)retval;
                result[1]='|';
                result[2]='N';
                result[3]='\0';
            }
        }
    } 
    else 
    {
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='N';
        result[3]='O';
        result[4]='K';
        result[5]='\0';
    }
    
    return FUNC_CMD_OK;
}

BYTE getInspecTime(char *data, char *result)
{
    BYTE retval;
    UINT sTime,lTime;

    sTime = (UINT)(shortInspecTime/15000);
    lTime = (UINT)(longInspecTime/15000);

    retval = FUNC_EXEC_OK;
    
    result[0]='0'+(char)retval;
    result[1]='|';
    result[2]='\0';
    myStrCpyUint(result,sTime,',');
    myStrCpyUint(result,lTime,'\0');

    return FUNC_CMD_OK;
}

BYTE getSoftStack(char *data, char *result)
{
    BYTE retval;

    retval = FUNC_EXEC_OK;
    
    result[0]='0'+(char)retval;
    result[1]='|';
    result[2]='\0';
    myStrCpyUint(result,max,'\0');

    return FUNC_CMD_OK;
}

BYTE setInspecTime(char *data, char *result)
{
    BYTE retval, error, comp, dep;
    char charDataStime[10];
    char charDataLtime[10];
    UINT sTime,lTime;
    
    error = 1;
    comp = 0;
    while ((data[comp]!='\0')&&(data[comp]!=','))
    {
        charDataStime[comp]=data[comp];
        comp++;
    }
    charDataStime[comp]='\0';
    
    if ((data[comp]==',')&&(comp!=0))
    {
        comp++;
        dep=comp;
        while ((data[comp]!='\0')&&(data[comp]!=','))
        {
            charDataLtime[comp-dep]=data[comp];
            comp++;
        }
        charDataLtime[comp-dep]='\0';
        
        if ((data[comp]=='\0')&&(comp!=dep))
            error = 0;
    }

    if (error == 0)
    {

        error = analyze_string(charDataStime, &sTime);
        error += analyze_string(charDataLtime, &lTime);

        if (error == 0)
        {
            if ((sTime<lTime)&&(sTime!=0)&&(lTime!=0))
            {
                shortInspecTime = 15000*((UINT32)sTime);
                longInspecTime = 15000*((UINT32)lTime);

                EEWrite(EEprom_HV_short_Inspec_Time + 1, (BYTE) ((sTime& 0xFF00) >> 8));
                EEWrite(EEprom_HV_short_Inspec_Time, (BYTE) ((sTime&0xFF)));

                EEWrite(EEprom_HV_long_Inspec_Time + 1, (BYTE) ((lTime& 0xFF00) >> 8));
                EEWrite(EEprom_HV_long_Inspec_Time, (BYTE) ((lTime&0xFF)));

                retval = FUNC_EXEC_OK;
            }
        }
    }
    
    if (error != 0)
        retval = FUNC_EXEC_BAD_ARGS_TYPE;
    
    result[0]='0'+(char)retval;
    result[1]='|';
    result[2]='\0';

    return FUNC_CMD_OK;
}

BYTE set_vhv(char *data, char *result)
{
    BYTE retval, tel, module, error, comp, dep, nb_param;
    UINT tension, intslopeVS;
    char charDataVoltage[10];
    char charDataInc[10];
    UINT32 slopeVS;
    
    error = 1;
    retval = FUNC_EXEC_BAD_ARGS_TYPE;
    
    if ((data[0]=='A')||(data[0]=='B'))
        if (data[1]==',')
            if ((data[2]=='1')||(data[2]=='2'))
                if (data[3]==',')
                {
                    dep = 4;
                    comp = 4;
                    
                    while ((data[comp]!='\0')&&(data[comp]!=','))
                    {
                        charDataVoltage[comp-dep]=data[comp];
                        comp++;
                    }
                    charDataVoltage[comp-dep]='\0';
                    
                    if ((data[comp]=='\0')&&(comp!=dep))
                    {
                        nb_param = 3;
                        error = 0;
                    }
                    
                    if ((data[comp]==',')&&(comp!=dep))
                    {
                        comp++;
                        dep = comp;
                        
                        while ((data[comp]!='\0')&&(data[comp]!=','))
                        {
                            charDataInc[comp-dep]=data[comp];
                            comp++;
                        }
                        charDataInc[comp-dep]='\0';
                        
                        if ((data[comp]=='\0')&&(comp!=dep))
                        {
                            nb_param = 4;
                            error = 0;
                        }
                    }
                }

    if (error == 0) 
    {
        tel = data[0];
        module = (BYTE) (data[2] - '0');

        error = analyze_string(charDataVoltage, &tension);

        if (nb_param == 4)
        {
            error = error + analyze_string(charDataInc, &intslopeVS);

            if (error == 0)
                slopeVS = (UINT32) intslopeVS;
        } else
            slopeVS = 10;


        if (error == 0) 
        {
            error = 1;
            if (((tel == 'A') && (module == 1)) && (tension <= HV_borne_sup_A1))
                error = 0;

            if (((tel == 'A') && (module == 2)) && (tension <= HV_borne_sup_A2))
                error = 0;

            if (((tel == 'B') && (module == 1)) && (tension <= HV_borne_sup_B1))
                error = 0;

            if (((tel == 'B') && (module == 2)) && (tension <= HV_borne_sup_B2))
                error = 0;
        }

        if (error == 0) 
        {
            if (HvStatus[module - 1 + 2 * ((BYTE) (tel - 'A'))] == 0)
            retval = FUNC_EXEC_INPROGRESS;
        
            if ((tel=='A')&&((valeur_portD & 0x01) == 0))
            {
                valeur_portD = valeur_portD | 0x01;
                PORTD=valeur_portD;
            }
        
            if ((tel=='B')&&((valeur_portD & 0x02) == 0))
            {
                valeur_portD = valeur_portD | 0x02;
                PORTD=valeur_portD;
            }
            
            if (retval != FUNC_EXEC_INPROGRESS)
            {
                retval = slop_vhv(tel, module, tension, slopeVS);
                HvPhysTarget[module - 1 + 2 * ((BYTE) (tel - 'A'))] = tension;
                HvPhysCorrect[module - 1 + 2 * ((BYTE) (tel - 'A'))] = tension;
            } 
        }
    }

    result[0]='0'+(char)retval;
    result[1]='|';
    result[2]='\0';
    return FUNC_CMD_OK;
}

BYTE get_temp(char *data, char *result) 
{

    BYTE retval;
    int temperature_array[nbcapteurs + 2];

    PIE1bits.TMR2IE=0;
    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;
    time_scheduling = time_scheduling + 3;
    temp(temperature_array);
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
    PIE1bits.TMR2IE=1;

    retval = FUNC_EXEC_OK;
    result[0]='0'+(char)retval;
    result[1]='|';
    result[2]='\0';
    myStrCpyUint(result,(UINT)temperature_array[0],',');
    myStrCpyUint(result,(UINT)temperature_array[1],',');
    myStrCpyUint(result,(UINT)temperature_array[2],',');
    myStrCpyUint(result,(UINT)temperature_array[3],',');
    myStrCpyUint(result,(UINT)temperature_array[4],',');
    myStrCpyUint(result,(UINT)temperature_array[5],'\0');
    
    return FUNC_CMD_OK;
}

BYTE get_hvmax(char *data, char *result) 
{
    BYTE retval, tel, module, error;
    UINT valeur;

    error = 1;
    
    if ((data[0]=='A')||(data[0]=='B'))
        if (data[1]==',')
            if ((data[2]=='1')||(data[2]=='2'))
                if (data[3]=='\0')
                    error = 0;
    
    if (error == 0) 
    {
        tel = data[0];
        module = (BYTE) (data[2] - '0');

        
        if ((tel == 'A') && (module == 1))
            valeur = HV_borne_sup_A1;

        if ((tel == 'A') && (module == 2))
            valeur = HV_borne_sup_A2;

        if ((tel == 'B') && (module == 1))
            valeur = HV_borne_sup_B1;

        if ((tel == 'B') && (module == 2))
            valeur = HV_borne_sup_B2;

        retval = FUNC_EXEC_OK;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
        myStrCpyUint(result,valeur,'\0');
    }
    else 
    {
        retval = FUNC_EXEC_BAD_ARGS_TYPE;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
    }

    return FUNC_CMD_OK;
}

BYTE apply_hvmax(char tel, char module, UINT tension_max) 
{
    BYTE error;

    error = 0;

    if ((tel == 'A') && (module == '1')) 
    {
        if (tension_max < HVSi1Max + 1) 
        {
            HV_borne_sup_A1 = tension_max;
            EEWrite(EEprom_HV_borne_sup_A1 + 1, (BYTE) ((HV_borne_sup_A1 & 0xFF00) >> 8));
            EEWrite(EEprom_HV_borne_sup_A1, (BYTE) ((HV_borne_sup_A1 & 0xFF)));
        } 
        else
            error = 1;
    }

    if ((tel == 'A') && (module == '2')) {
        if (tension_max < HVSi2Max + 1) {
            HV_borne_sup_A2 = tension_max;
            EEWrite(EEprom_HV_borne_sup_A1 + 3, (BYTE) ((HV_borne_sup_A2 & 0xFF00) >> 8));
            EEWrite(EEprom_HV_borne_sup_A1 + 2, (BYTE) ((HV_borne_sup_A2 & 0xFF)));
        } else
            error = 1;
    }

    if ((tel == 'B') && (module == '1')) {
        if (tension_max < HVSi1Max + 1) {
            HV_borne_sup_B1 = tension_max;
            EEWrite(EEprom_HV_borne_sup_A1 + 5, (BYTE) ((HV_borne_sup_B1 & 0xFF00) >> 8));
            EEWrite(EEprom_HV_borne_sup_A1 + 4, (BYTE) ((HV_borne_sup_B1 & 0xFF)));
        } else
            error = 1;
    }

    if ((tel == 'B') && (module == '2')) {
        if (tension_max < HVSi2Max + 1) {
            HV_borne_sup_B2 = tension_max;
            EEWrite(EEprom_HV_borne_sup_A1 + 7, (BYTE) ((HV_borne_sup_B2 & 0xFF00) >> 8));
            EEWrite(EEprom_HV_borne_sup_A1 + 6, (BYTE) ((HV_borne_sup_B2 & 0xFF)));
        } else
            error = 1;
    }

    return error;
}

BYTE enableDisableHVMeas(char *data, char *result) 
{
    BYTE retval,error;

    error = 1;
    
    if ((data[0]=='1')&&(data[1]=='\0'))
    {
        error = 0;
        if (enableHVMeas != keyWordC)
        {
            enableHVMeas = keyWordC;
            EEWrite(49, keyWordC);
            timing_inspection = shortInspecTime;
            time_lc_prec = time_scheduling;
        }
    }
    
    if ((data[0]=='0')&&(data[1]=='\0'))
    {
        error = 0;
        enableHVMeas = 0xFF;
        EEWrite(49, 0xFF);
    }

    if (error == 0) 
        retval = FUNC_EXEC_OK;
    else 
        retval = FUNC_EXEC_BAD_ARGS_TYPE;
    

    result[0]='0'+(char)retval;
    result[1]='|';
    result[2]='\0';
    return FUNC_CMD_OK;
}

BYTE set_hvmax(char *data, char *result) 
{
    BYTE retval,error,comp, dep;
    char charDataVoltage[10], tel, module;
    UINT tension_max;

    error = 1;
    
    if (((data[0]=='A')||(data[0]=='B'))&&(data[1]==','))
    {
        tel = data[0];
        if (((data[2]=='1')||(data[2]=='2'))&&(data[3]==','))
        {
            dep = 4;
            comp = dep;
            while ((data[comp]!='\0')&&(data[comp]!=','))
            {
                charDataVoltage[comp-dep]=data[comp];
                comp++;
            }
            charDataVoltage[comp-dep]='\0';
            
            if ((data[comp]=='\0')&&((comp-dep)!=0))
            {
                error = analyze_string(charDataVoltage, &tension_max);
                tel = data[0];
                module = data[2];
                
                if (error == 0) 
                    error = apply_hvmax(tel, module, tension_max);
            }
        }
    }    

    if (error == 0) 
        retval = FUNC_EXEC_OK;
    else 
        retval = FUNC_EXEC_BAD_ARGS_TYPE;

    result[0]='0'+(char)retval;
    result[1]='|';
    result[2]='\0';
    return FUNC_CMD_OK;
}

BYTE uart_ask_hv_calibration(char *data, char *result) 
{
    char myresult[14];

    ask_hv_calibration(myresult);

    result[0]='0';
    result[1]='|';
    result[2]='\0';
    myStrCpyChar(result,myresult,'\0');

    return FUNC_CMD_OK;
}

BYTE store_param(char *data, char *result) 
{
    storeparam();
    result[0]='0';
    result[1]='|';
    result[2]='\0';

    return FUNC_CMD_OK;
}

BYTE set_data_eeprom_address(char *data, char *result) 
{
    BYTE retval, error,dep, comp;
    char charDataAdr[10],charDataValue[10];
    UINT data_adr, data_value;

    error = 1;
    
    comp = 0;
    while ((data[comp]!='\0')&&(data[comp]!=','))
    {
        charDataAdr[comp] = data[comp];
        comp++;
    }
    charDataAdr[comp]='\0';
    
    if ((data[comp] == ',')&&(comp!=0))
    {
        comp++;
        dep = comp;
        while ((data[comp]!='\0')&&(data[comp]!=','))
        {
            charDataValue[comp-dep] = data[comp];
            comp++;
        }
        charDataValue[comp-dep]='\0';
        
        if ((data[comp] == '\0')&&((comp-dep)!=0))
        {
            error = analyze_string(charDataAdr, &data_adr);
            error += analyze_string(charDataValue, &data_value);
            EEWrite(data_adr, (BYTE)data_value);
        }
    }

    if (error == 0) 
        retval = FUNC_EXEC_OK;
    else 
        retval = FUNC_EXEC_BAD_ARGS_VALUE;

    result[0]='0'+(char)retval;
    result[1]='|';
    result[2]='\0';
    return FUNC_CMD_OK;
}

BYTE get_data_eeprom_address(char *data, char *result) 
{
    BYTE retval, error;
    char charDataAdr[10];
    UINT data_adr;
    BYTE data_value,comp;

    error = 1;
    
    comp = 0;
    while ((data[comp]!='\0')&&(data[comp]!=','))
    {
        charDataAdr[comp] = data[comp];
        comp++;
    }
    charDataAdr[comp]='\0';
    
    if ((data[comp]=='\0')&&(comp!=0))
    {
        error = analyze_string(charDataAdr, &data_adr);
        data_value = EERead(data_adr);
    }

    if (error == 0) 
    {
        retval = FUNC_EXEC_OK;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
        myStrCpyUint(result,data_value,'\0');
        //dispchartobin(data_value);        
    } 
    else 
    {
        retval = FUNC_EXEC_BAD_ARGS_VALUE;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
    }
    
    return FUNC_CMD_OK;
}

BYTE spi_get_regvalue(char *data, char *result) 
{
    BYTE retval,error,comp,dep;
    char charDataAdr[10];
    UINT data_adr, data_value;
    
    error = 1;
    
    if (((data[0]=='A')||(data[0]=='B'))&&(data[1]==','))
    {
        dep = 2;
        comp = dep;
        while ((data[comp]!=',')&&(data[comp]!='\0'))
        {
            charDataAdr[comp-dep]=data[comp];
            comp++;
        }
        charDataAdr[comp-dep]='\0';
        
        if ((data[comp]=='\0')&&(comp!=dep))
        {
            error = analyze_string(charDataAdr, &data_adr);
            data_value = rdspi(data[0] - 'A' + 1, data_adr);
        }
    }

    if (error == 0) 
    {
        retval = FUNC_EXEC_OK;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
        myStrCpyUint(result,data_value,'\0');
    } 
    else 
    {
        retval = FUNC_EXEC_BAD_ARGS_TYPE;
        result[0]='0'+(char)retval;
        result[1]='|';
        result[2]='\0';
    }

    return FUNC_CMD_OK;
}

BYTE spi_set_regvalue(char *data, char *result) 
{
    BYTE retval,error,comp,dep;
    char charDataAdr[10];
    char charDataValue[10];
    UINT data_adr,data_value;
    char nb_fpga;
    
    error = 1;
    
    if (((data[0]=='A')||(data[0]=='B'))&&(data[1]==','))
    {
        nb_fpga = data[0] - 'A' + 1;
        
        dep = 2;
        comp = dep;
        while ((data[comp]!=',')&&(data[comp]!='\0'))
        {
            charDataAdr[comp-dep]=data[comp];
            comp++;
        }
        charDataAdr[comp-dep]='\0';
        
        if ((data[comp]==',')&&(comp!=dep))
        {
            comp++;
            dep=comp;
            while ((data[comp]!=',')&&(data[comp]!='\0'))
            {
                charDataValue[comp-dep]=data[comp];
                comp++;
            }
            charDataValue[comp-dep]='\0';
            
            if ((data[comp]=='\0')&&((comp-dep)!=0))
            {
                error = analyze_string(charDataAdr, &data_adr);
                error = error + analyze_string(charDataValue, &data_value);
                if (error == 0)
                    wrspi(nb_fpga, data_adr, data_value);
            }
        }
    }

    if (error == 0) 
        retval = FUNC_EXEC_OK;
    else 
        retval = FUNC_EXEC_BAD_ARGS_TYPE;
    
    result[0]='0'+(char)retval;
    result[1]='|';
    result[2]='\0';
    return FUNC_CMD_OK;
}

// EOF
