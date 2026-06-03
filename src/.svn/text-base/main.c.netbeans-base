//*********************************************************************
//********************** STEP_3 PIC PROJECT ***************************
//*********************************************************************

#include <p18cxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include <usart.h>
#include <string.h>
#include <timers.h>
#include <delays.h>
#include <spi.h>


#pragma config FOSC = HSPLL     // HS oscillator
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = NOSLP      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 30        // Brown Out Reset Voltage bits (VBOR set to 3.0 V nominal)

#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT is controlled by SWDTEN bit of the WDTCON register)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config HFOFST = OFF     // HFINTOSC Fast Start-up (The system clock is held off until the HFINTOSC is stable.)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)

#include "board.h"
#include "setup.h"
#include "uartbuf.h"
#include "functions.h"

#pragma udata large_udata
unsigned char fIn[MAX_FRAME_SIZE], fOut[MAX_FRAME_SIZE];
char data[MAX_DATA_SIZE], result[SMALL_DATA_SIZE];
char fpre[10];
BYTE done, done2, done3, done4, done_ins, done6, done7;
BYTE HvStatusOld[4];
UINT HVmeas;
UINT32 lcAdcReadA1, lcAdcReadA2, lcAdcReadB1, lcAdcReadB2;
UINT32 HVmeas_bin;
int temperature_array[nbcapteurs + 2];
BYTE co;
UINT rd_data_fpga1, rd_data_fpga2, get_data_fpga1;
char mmax[20];
UINT i;
char kw;
UINT idb;
enum slaveid ids;
BYTE cmd;
BYTE cmdres, funcres;
BYTE seq;
UINT crc, docrc;
UINT flen, foffset;
UINT ferr;
char myC[2];

/*global variables*/
BYTE valeur_portA;
BYTE valeur_trisA;
BYTE valeur_portB;
BYTE valeur_trisB;
BYTE valeur_portC;
BYTE valeur_trisC;
BYTE valeur_portD;
BYTE valeur_trisD;
BYTE valeur_trisE;

//global variable for high voltage modules
BYTE enableHVMeas;
UINT HvValueTab[4][2];
UINT HvInc[4];
BYTE HvStatus[4];
UINT HvPhysTarget[4];
UINT HvPhysCorrect[4];
BYTE cal_preampli_offset;
BYTE marge_pa_offset;
BYTE default_DA1; //warning for a problem detected for the A1 detector
BYTE default_DA2; //warning for a problem detected for the A2 detector
BYTE default_DB1; //warning for a problem detected for the B1 detector
BYTE default_DB2; //warning for a problem detected for the B2 detector
UINT HV_borne_sup_A1; //maximum value to reach for 200V high voltage module (telescope A)
UINT HV_borne_sup_A2; //maximum value to reach for 400V high voltage module (telescope A)
UINT HV_borne_sup_B1; //maximum value to reach for 200V high voltage module (telescope B)
UINT HV_borne_sup_B2; //maximum value to reach for 400V high voltage module (telescope B)
UINT32 coefA_A1; // coefficient required to determine the leakage current
UINT32 coefA_A2; // coefficient required to determine the leakage current
UINT32 coefA_B1; // coefficient required to determine the leakage current
UINT32 coefA_B2; // coefficient required to determine the leakage current
UINT32 coefB_A1; // coefficient required to determine the leakage current
UINT32 coefB_A2; // coefficient required to determine the leakage current
UINT32 coefB_B1; // coefficient required to determine the leakage current
UINT32 coefB_B2; // coefficient required to determine the leakage current

ram UINT tsensor_1_min;
ram UINT tsensor_1_max;
ram UINT tsensor_limit;
ram BYTE time_reset;
ram BYTE time_start;
ram BYTE time_bit0;
ram BYTE time_bit1;
ram BYTE time_wait;
ram BYTE CSI_relay;
ram UINT32 time_scheduling;
ram UINT32 time_scheduling_copy;
ram BOOL both_fpga_ok = FALSE;
ram BOOL check = TRUE;
ram long int HV_read_coefA[4];
ram long int HV_read_coefB[4];
ram UINT lcA1;
ram UINT lcA2;
ram UINT lcB1;
ram UINT lcB2;
ram UINT32 timing_inspection;
ram UINT32 time_lc_prec;
ram UINT32 shortInspecTime;
ram UINT32 longInspecTime;
ram UINT GeneDacVoltage;
ram UINT max;
struct parametres pa;
#pragma udata

extern CBuffer_large *Uart;

void main(void) 
{
    static BYTE canal;
    
    ucsetup();
    memsetup();


    uartbuf_init();

    frame_init(fIn);
    frame_init(fOut);

    func_init();


    time_scheduling = 0;
    done = 0;
    done2 = 0;
    done3 = 0;
    done4 = 0;
    done_ins = 0;
    done6 = 0;

    lcAdcReadA1 = 0;
    lcAdcReadA2 = 0;
    lcAdcReadB1 = 0;
    lcAdcReadB2 = 0;

    while (!both_fpga_ok) 
    {
        get_data_fpga1 = (UINT) (getid() << 1);

        wrspi(1, 0x02, get_data_fpga1);
        wrspi(2, 0x02, get_data_fpga1 + 0x01);

        rd_data_fpga1 = rdspi(1, 0x02);
        rd_data_fpga2 = rdspi(2, 0x02);

        if ((rd_data_fpga1 == get_data_fpga1) && (rd_data_fpga2 == get_data_fpga1 + 0x01)) {
            both_fpga_ok = TRUE;
        }
    } // end while

    do {

        temp_init();
        temp(temperature_array);

        check = TRUE;

        for (co = 0; co < 4; co++) {
            if (temperature_array[co] == 0)
                check = FALSE;
        }

        if (check)
            setparam();

    } while (!check);


    //évaluation d'une tension résiduelle au démarrage après reset
    if (enableHVMeas == 0x38)
        for (co = 0; co < 4; co++) {
            HvValueTab[co][1] = 0;
            canal = co + 4;
            HVmeas = getHvValue(&canal);
            HVmeas /= 10;

            if (HVmeas > 3) {
                if ((co == 0) || (co == 2)) {
                    HVmeas_bin = ((UINT32) HVmeas * coefHV_M200) / 1000;
                    HvInc[co] = coefHV_M200 / 100;
                }

                if ((co == 1) || (co == 3)) {
                    HVmeas_bin = (((UINT32) HVmeas * coefHV_M400)) / 1000;
                    HvInc[co] = coefHV_M400 / 100;
                }

                HvValueTab[co][0] = (UINT) HVmeas_bin;
                HvStatus[co] = 0;
            } else {
                HvValueTab[co][0] = 0;
                HvInc[co] = 0;
                HvStatus[co] = 1;
            }
        }

    max = 0;

    RCONbits.IPEN = 0; // enable interrupt priority levels
    INTCONbits.GIE = 1; // enable the high priority interrupts
    INTCONbits.PEIE = 1; // enable the low priority interrupts
    PIE1bits.RCIE = 1; // Enable Rx interrupts
    OpenTimer2(TIMER_INT_ON & T2_PS_1_16 & T2_POST_1_16);
    PR2 = 0xFA;

//    mmax[0] = '\0';
//    myStrCpyChar2(mmax, "fffmax:", '\0');
//    myStrCpyUint(mmax, max, '\0');
//    putsUSART(mmax);
//    while (BusyUSART());
//    putcUSART('\r');
//    while (BusyUSART());
//    putcUSART('\n');
//    while (BusyUSART());

    timing_inspection = shortInspecTime; //short delay first between each inspection

    time_lc_prec = 0;

    while (1) 
    {
        ferr = cbuffer_large_getframe_length(&Uart[SLAVE_RX], &flen, &foffset);

        if ((ferr == ERR_FRAME_NONE) && (flen > 8)) 
        {
            uartbuf_getframe(SLAVE_RX, fIn, flen, foffset);

            kw = frame_getkw(fIn);
            idb = frame_getidb(fIn);
            ids = frame_getids(fIn);
            cmd = frame_getcmd(fIn);
            seq = frame_getseq(fIn);
            frame_getdata(fIn, data);
            crc = frame_getcrc(fIn);

            if ((ids == '0' + getid())) 
            {
                fpre[0] = '\0';
                if (seq) 
                {
                    myStrCpyHex(fpre, idb, 3, '\0');
                    myStrCpy1Char(fpre, ids, '\0');
                    myStrCpy1Char(fpre, cmd, '\0');
                    myStrCpy1Char(fpre, seq, '\0');
                    myStrCpy1Char(fpre, SEQNUM_DELIMITER, '\0');
                }
                else 
                {
                    myStrCpyHex(fpre, idb, 3, '\0');
                    myStrCpy1Char(fpre, ids, '\0');
                    myStrCpy1Char(fpre, cmd, '\0');
                }

                docrc = 0;
                for (i = 0; fIn[i] != KW_END; i++)
                    docrc = docrc ^ fIn[i];

                if ((docrc != crc) || (kw != KW_STX)) 
                { // CRC fail
                    fIn[0] = '\0';
                    myStrCpy1Char((char *) fIn, KW_ERR, '\0');
                    myStrCpyChar((char *) fIn, fpre, '\0');
                    myStrCpyChar((char *) fIn, data, '\0');
                    myStrCpy1Char((char *) fIn, KW_END, '\0');
                    crc = frame_docrc(fIn);
                    fOut[0] = '\0';
                    myStrCpyChar((char *) fOut, (char *) fIn, '\0');
                    myStrCpyHex((char *) fOut, crc, -1, '\0');
                    uartbuf_putframe(SLAVE_TX, fOut);
                    uartbuf_flush(SLAVE_TX);
                } 
                else 
                {
                    cmdres = func_invoke(cmd, data, result);
                    fIn[0] = '\0';

                    if (cmdres == FUNC_CMD_OK) 
                    {
                        myC[0] = result[0];
                        myC[1] = '\0';
                        funcres = atoi(myC);

                        if (funcres == FUNC_EXEC_OK) 
                        {
                            myStrCpy1Char((char *) fIn, KW_ACK, '\0');
                            myStrCpyChar((char *) fIn, fpre, '\0');
                            myStrCpyChar((char *) fIn, result, '\0');
                            myStrCpy1Char((char *) fIn, KW_END, '\0');
                        }
                        else 
                        {
                            myStrCpy1Char((char *) fIn, KW_NAK, '\0');
                            myStrCpyChar((char *) fIn, fpre, '\0');
                            myStrCpyChar((char *) fIn, result, '\0');
                            myStrCpy1Char((char *) fIn, KW_END, '\0');
                        }
                    } 
                    else 
                    {
                        myStrCpy1Char((char *) fIn, KW_ERR, '\0');
                        myStrCpyChar((char *) fIn, fpre, '\0');
                        myStrCpyChar((char *) fIn, data, '\0');
                        myStrCpy1Char((char *) fIn, KW_END, '\0');
                    }

                    // compute new CRC and append to output frame
                    crc = frame_docrc(fIn);
                    fOut[0] = '\0';
                    myStrCpyChar((char *) fOut, (char *) fIn, '\0');
                    myStrCpyHex((char *) fOut, crc, -1, '\0');

                    uartbuf_putframe(SLAVE_TX, fOut);
                    uartbuf_flush(SLAVE_TX);
                }
            }
        } // end if(ferr)

        //--------------------------------------------------------------
        if (HvStatus[0] == 1) {
            lcAdcReadA1 = lcAdcReadA1 * 9;
            lcAdcReadA1 = lcAdcReadA1 / 10;
            canal = 0;
            lcAdcReadA1 = lcAdcReadA1 + ((UINT32) adc_getvalue(&canal)) / 10;
        } else
            lcAdcReadA1 = 0;

        if (HvStatus[1] == 1) {
            lcAdcReadA2 = lcAdcReadA2 * 9;
            lcAdcReadA2 = lcAdcReadA2 / 10;
            canal = 1;
            lcAdcReadA2 = lcAdcReadA2 + ((UINT32) adc_getvalue(&canal)) / 10;
        } else
            lcAdcReadA2 = 0;

        if (HvStatus[2] == 1) {
            lcAdcReadB1 = lcAdcReadB1 * 9;
            lcAdcReadB1 = lcAdcReadB1 / 10;
            canal = 2;
            lcAdcReadB1 = lcAdcReadB1 + ((UINT32) adc_getvalue(&canal)) / 10;
        } else
            lcAdcReadB1 = 0;

        if (HvStatus[3] == 1) {
            lcAdcReadB2 = lcAdcReadB2 * 9;
            lcAdcReadB2 = lcAdcReadB2 / 10;
            canal = 3;
            lcAdcReadB2 = lcAdcReadB2 + ((UINT32) adc_getvalue(&canal)) / 10;
        } else
            lcAdcReadB2 = 0;

        time_scheduling_copy = time_scheduling;
        if ((time_scheduling_copy % timing_HV) == 0) 
        {
            if (done3 == 0) 
            {
                HvStatusOld[0] = HvStatus[0];
                HvStatusOld[1] = HvStatus[1];
                HvStatusOld[2] = HvStatus[2];
                HvStatusOld[3] = HvStatus[3];

                HVfunc();

                if ((HvStatus[0] == 1) && (HvStatusOld[0] == 0)) 
                {
                    timing_inspection = shortInspecTime;
                    time_lc_prec = time_scheduling_copy;
                    done_ins = 1;
                }

                if ((HvStatus[1] == 1) && (HvStatusOld[1] == 0)) {
                    timing_inspection = shortInspecTime;
                    time_lc_prec = time_scheduling_copy;
                    done_ins = 1;
                }

                if ((HvStatus[2] == 1) && (HvStatusOld[2] == 0)) {
                    timing_inspection = shortInspecTime;
                    time_lc_prec = time_scheduling_copy;
                    done_ins = 1;
                }

                if ((HvStatus[3] == 1) && (HvStatusOld[3] == 0)) {
                    timing_inspection = shortInspecTime;
                    time_lc_prec = time_scheduling_copy;
                    done_ins = 1;
                }

                done3 = 1;
            }
        } else
            done3 = 0;

        time_scheduling_copy = time_scheduling;
        if (((time_scheduling_copy % 205) == 0) && (cal_preampli_offset == 1)) {
            if (done6 == 0) {
                pa_offset_settings();
                done6 = 1;
            }
        } else
            done6 = 0;

        time_scheduling_copy = time_scheduling;
        if ((time_scheduling_copy % 251) == 0) 
        {
//            mmax[0] = '\0';
//            myStrCpyChar2(mmax, "max:", '\0');
//            myStrCpyUint(mmax, max, '\0');
//            putsUSART(mmax);
//            while (BusyUSART());
//            putcUSART('\r');
//            while (BusyUSART());
//            putcUSART('\n');
//            while (BusyUSART());


            if (RCSTAbits.OERR || RCSTAbits.FERR) {
                RCSTAbits.CREN = 0; // Clearing CREN clears any Overrun (OERR) errors
                Nop();
                RCSTAbits.CREN = 1; // Re-enable continuous USART receive
                RCSTAbits.SPEN = 0;
                RCSTAbits.SPEN = 1;
            }
        }

        time_scheduling_copy = time_scheduling;
        if (time_scheduling_copy % 1000) {
            if (done7 == 0) {
                if ((coefA_A1 != 0) && (HvStatus[0] == 1))
                {
                    lcA1 = leak_current('A', '1', lcAdcReadA1);
                }

                if ((coefA_A2 != 0) && (HvStatus[1] == 1))
                {
                    lcA2 = leak_current('A', '2', lcAdcReadA2);
                }
                    

                if ((coefA_B1 != 0) && (HvStatus[2] == 1))
                {
                    lcB1 = leak_current('B', '1', lcAdcReadB1);
                }

                if ((coefA_B2 != 0) && (HvStatus[3] == 1))
                {
                    lcB2 = leak_current('B', '2', lcAdcReadB2);
                }
                done7 = 1;
            }
        } else
            done7 = 0;

        time_scheduling_copy = time_scheduling;
        if ((diffLcTime(time_scheduling, time_lc_prec, timing_inspection) % timing_inspection) == 0) {
            if (done_ins == 0) {
                time_lc_prec = time_scheduling_copy;
                timing_inspection = current_leak_inspection();
                done_ins = 1;
            }
        } else
            done_ins = 0;

    } // end while

}

UINT32 diffLcTime(UINT32 t1, UINT32 t2, UINT32 t3) {
    UINT32 t4;

    if (t2 < 4294967294 - t3)
        t4 = t1 - t2;
    else
        t4 = 4294967294 - t2 + t1;

    return t4;
}

void pa_offset_settings(void) {
    static BYTE state = 0;
    static char i;
    static long borne_lim_inf, borne_lim_sup;
    static UINT value;
    int regAdc1;
    UINT regfpga, regfpga2, *p;
    BYTE cp, co;
    char id;

    cp = 0;

    switch (state) {
        case 0:
            if (marge_pa_offset > 100)
                borne_lim_inf = -7500;
            else {
                if (marge_pa_offset != 100)
                    borne_lim_inf = (((long) marge_pa_offset * 16384) / 100) - 8192;
                else
                    borne_lim_inf = 7800;
            }

            borne_lim_sup = borne_lim_inf + 100;

            i = -1;
            state = 1;

            break;

        case 1:

            i++;

            if (i == 6) {
                storeparam();
                cal_preampli_offset = 0;
                state = 0;
            } else {
                value = 400;
                state = 2;
            }
            break;

        case 2:

            if ((i % 3) == 0) {
                regfpga = REG_FPGA_QH1;
                regfpga2 = REG_FPGA_PA_SI1;
            }

            if ((i % 3) == 1) {
                regfpga = REG_FPGA_Q2;
                regfpga2 = REG_FPGA_PA_SI2;
            }

            if ((i % 3) == 2) {
                regfpga = REG_FPGA_Q3;
                regfpga2 = REG_FPGA_PA_CSI;
            }

            if (i < 3)
                id = 1;
            else
                id = 2;


            do {
                value += 5;
                wrspi(id, regfpga2, value);
                Delay10KTCYx(2);
                wrspi(id, regfpga2, value);

                co = 0;
                do {
                    Delay10KTCYx(2);
                    regAdc1 = (int) rdspi(id, regfpga);
                    Delay10KTCYx(1);

                    while (((regAdc1 > ((int) rdspi(id, regfpga)) - 30) && (regAdc1 < ((int) rdspi(id, regfpga)) + 30)) && (co < 20))
                        co++;

                    if (co < 20)
                        co = 0;
                } while (co != 20);

                if ((regAdc1 >= borne_lim_inf) && (regAdc1 < borne_lim_sup)) {
                    state = 1;
                    p = ((UINT *) & pa) + 5 - i;
                    *p = value;
                    cp = 25;

                }

                cp++;
            } while ((cp < 25) && (value < 1024));

            if (value > 1024) {
                state = 1;
            }

            break;

        default:
            state = 0;
            break;
    }
}

void HVfunc(void) {
    UINT HVvalue_meas;
    BYTE cp, HV_direction;
    UINT32 deltaV, delta, gapV;
    static BOOL finish[4] = {FALSE, FALSE, FALSE, FALSE};

    delta = 0;
    HVvalue_meas = 0;

    for (cp = 0; cp < 4; cp++)
        if (HvStatus[cp] == 0) { //down by default
            HV_direction = 0;

            if (HvValueTab[cp][0] < HvValueTab[cp][1]) {
                gapV = (UINT32) HvValueTab[cp][1]-(UINT32) HvValueTab[cp][0];
                HV_direction = 1; //up
            }

            if (HvValueTab[cp][1] < HvValueTab[cp][0])
                gapV = (UINT32) HvValueTab[cp][0]-(UINT32) HvValueTab[cp][1];


            if (HvValueTab[cp][0] == HvValueTab[cp][1])
                gapV = 0;

            deltaV = (UINT32) HvInc[cp]*4 * timing_HV / 1000;


            if (!finish[cp]) {
                if (gapV <= deltaV) {
                    HvValueTab[cp][0] = HvValueTab[cp][1];


                    dac_sequence(cp * 16, HvValueTab[cp][1]);

                    if (EERead(EEprom_is_cal_HV_discret + cp) == 0)
                        HvStatus[cp] = 1;
                    else
                        finish[cp] = TRUE;
                } else {
                    if (HV_direction == 1)
                        HvValueTab[cp][0] += (UINT) deltaV;


                    if (HV_direction == 0)
                        HvValueTab[cp][0] -= (UINT) deltaV;

                    dac_sequence(cp * 16, HvValueTab[cp][0]);
                }
            } else {

                HvValueTab[cp][1] = HvValueTab[cp][0];
                HvStatus[cp] = 1;
                finish[cp] = FALSE;
            }
        }
}

UINT32 current_leak_inspection(void) {
    BYTE compteur, module, flag;
    float Rd, HV;
    UINT leakCur, hvInt;
    UINT32 timing;
    char tel;

    flag = 0;

    if (enableHVMeas == keyWordC) {
        for (compteur = 0; compteur < 4; compteur++) {
            if ((HvStatus[compteur] == 1) && (HvValueTab[compteur][1] != 0)) {
                switch (compteur) {
                    case 0:
                        tel = 'A';
                        module = 1;
                        leakCur = lcA1;
                        break;

                    case 1:
                        tel = 'A';
                        module = 2;
                        leakCur = lcA2;
                        break;

                    case 2:
                        tel = 'B';
                        module = 1;
                        leakCur = lcB1;
                        break;

                    case 3:
                        tel = 'B';
                        module = 2;
                        leakCur = lcB2;
                        break;

                    default:
                        break;
                }

                if (leakCur != 0) 
                {
                    Rd = ((float) HvPhysCorrect[compteur]*(float) (1000000000));
                    Rd = Rd / ((float) leakCur);
                    Rd = Rd - 10200000;
                    HV = ((float) HvPhysTarget[compteur])*(1 + 10200000 / Rd);
                    hvInt = (UINT) HV;

                    if (HvPhysCorrect[compteur] != hvInt) 
                    {
                        if (hvInt != 0) 
                        {
                            if (((module == 1) && (hvInt < HVSi1Max + 1)) || ((module == 2) && (hvInt < HVSi2Max + 1)))
                                slop_vhv(tel, module, hvInt, 5);
                            
                            if ((module == 1) && (hvInt > HVSi1Max))
                                slop_vhv(tel, module, HVSi1Max, 5);
                            
                            if ((module == 2) && (hvInt > HVSi2Max))
                                slop_vhv(tel, module, HVSi2Max, 5);
                        }

                        HvPhysCorrect[compteur] = hvInt;
                        flag = 1;
                    }
                }
            }
        }
    }

    if (flag == 1)
        timing = shortInspecTime;
    else
        timing = longInspecTime;

    return timing;
}

///////////////////UARTFUNC FUNCTION//////////////////////////////

//BYTE uartfunc(void) {
//    static UINT i;
//    static char kw;
//    static UINT idb;
//    static enum slaveid ids;
//    static BYTE cmd;
//    static BYTE cmdres, funcres;
//    static BYTE seq;
//    static UINT crc, docrc;
//    static UINT flen, foffset;
//    static UINT ferr;
//    static char myC[2];
//
//    ferr = cbuffer_large_getframe_length(&Uart[SLAVE_RX], &flen, &foffset);
//
//    if (ferr == ERR_FRAME_NONE) {
//        uartbuf_getframe(SLAVE_RX, fIn, flen, foffset);
//
//        if (flen < 9) // incorrect length
//            return 0;
//
//        kw = frame_getkw(fIn);
//        idb = frame_getidb(fIn);
//        ids = frame_getids(fIn);
//        cmd = frame_getcmd(fIn);
//        seq = frame_getseq(fIn);
//        frame_getdata(fIn, data);
//        crc = frame_getcrc(fIn);
//
//        if ((ids == '0' + getid())) {
//            fpre[0] = '\0';
//            if (seq) {
//                myStrCpyHex(fpre, idb, 3, '\0');
//                myStrCpy1Char(fpre, ids, '\0');
//                myStrCpy1Char(fpre, cmd, '\0');
//                myStrCpy1Char(fpre, seq, '\0');
//                myStrCpy1Char(fpre, SEQNUM_DELIMITER, '\0');
//            } else {
//                myStrCpyHex(fpre, idb, 3, '\0');
//                myStrCpy1Char(fpre, ids, '\0');
//                myStrCpy1Char(fpre, cmd, '\0');
//            }
//
//            docrc = 0;
//            for (i = 0; fIn[i] != KW_END; i++)
//                docrc = docrc ^ fIn[i];
//
//            if ((docrc != crc) || (kw != KW_STX)) { // CRC fail
//                fIn[0] = '\0';
//                myStrCpy1Char((char *) fIn, KW_ERR, '\0');
//                myStrCpyChar((char *) fIn, fpre, '\0');
//                myStrCpyChar((char *) fIn, data, '\0');
//                myStrCpy1Char((char *) fIn, KW_END, '\0');
//                crc = frame_docrc(fIn);
//                fOut[0] = '\0';
//                myStrCpyChar((char *) fOut, (char *) fIn, '\0');
//                myStrCpyHex((char *) fOut, crc, -1, '\0');
//                uartbuf_putframe(SLAVE_TX, fOut);
//                uartbuf_flush(SLAVE_TX);
//                return 0;
//            }
//
//            cmdres = func_invoke(cmd, data, result);
//            fIn[0] = '\0';
//
//            if (cmdres == FUNC_CMD_OK) {
//                myC[0] = result[0];
//                myC[1] = '\0';
//                funcres = atoi(myC);
//
//                if (funcres == FUNC_EXEC_OK) {
//                    myStrCpy1Char((char *) fIn, KW_ACK, '\0');
//                    myStrCpyChar((char *) fIn, fpre, '\0');
//                    myStrCpyChar((char *) fIn, result, '\0');
//                    myStrCpy1Char((char *) fIn, KW_END, '\0');
//                } else {
//                    myStrCpy1Char((char *) fIn, KW_NAK, '\0');
//                    myStrCpyChar((char *) fIn, fpre, '\0');
//                    myStrCpyChar((char *) fIn, result, '\0');
//                    myStrCpy1Char((char *) fIn, KW_END, '\0');
//                }
//            }
//            else {
//                myStrCpy1Char((char *) fIn, KW_ERR, '\0');
//                myStrCpyChar((char *) fIn, fpre, '\0');
//                myStrCpyChar((char *) fIn, data, '\0');
//                myStrCpy1Char((char *) fIn, KW_END, '\0');
//            }
//
//            // compute new CRC and append to output frame
//            crc = frame_docrc(fIn);
//            fOut[0] = '\0';
//            myStrCpyChar((char *) fOut, (char *) fIn, '\0');
//            myStrCpyHex((char *) fOut, crc, -1, '\0');
//
//            uartbuf_putframe(SLAVE_TX, fOut);
//            uartbuf_flush(SLAVE_TX);
//        }
//    } // end if(ferr)
//
//    //uartbuf_flush(SLAVE_HPTX);
//    //uartbuf_flush(SLAVE_TX);
//    return 0;
//}

// EOF
