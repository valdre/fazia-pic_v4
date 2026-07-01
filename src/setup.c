// #include <usart.h>
// #include <timers.h>
// #include <spi.h>

#include "setup.h"
#include "Generic.h"
#include "board.h"
#include "functions.h"

#define BAUDRATE  115200
#define _SPBRG (((BOARD_FOSC / BAUDRATE) / 16) - 1)

extern uint8_t valeur_portA;
extern uint8_t valeur_trisA;
extern uint8_t valeur_portB;
extern uint8_t valeur_trisB;
extern uint8_t valeur_portC;
extern uint8_t valeur_trisC;
extern uint8_t valeur_portD;
extern uint8_t valeur_trisD;
extern uint8_t valeur_trisE;


extern uint16_t HvValueTab[4][2];
extern uint16_t HvInc[4];
extern uint8_t HvStatus[4];
extern uint8_t enableHVMeas;
extern uint16_t HvPhysTarget[4];
extern uint16_t HvPhysCorrect[4];

extern uint16_t HV_borne_sup_A1; //maximum value to reach for 200V high voltage module (telescope A)
extern uint16_t HV_borne_sup_A2; //maximum value to reach for 400V high voltage module (telescope A)
extern uint16_t HV_borne_sup_B1; //maximum value to reach for 200V high voltage module (telescope B)
extern uint16_t HV_borne_sup_B2; //maximum value to reach for 400V high voltage module (telescope B)

extern uint8_t default_DA1; //warning for a problem detected for the A1 detector
extern uint8_t default_DA2; //warning for a problem detected for the A2 detector
extern uint8_t default_DB1; //warning for a problem detected for the B1 detector
extern uint8_t default_DB2; //warning for a problem detected for the B2 detector


extern uint32_t coefA_A1;
extern uint32_t coefA_A2;
extern uint32_t coefA_B1;
extern uint32_t coefA_B2;
extern uint32_t coefB_A1;
extern uint32_t coefB_A2;
extern uint32_t coefB_B1;
extern uint32_t coefB_B2;

extern uint8_t cal_preampli_offset;
extern uint8_t marge_pa_offset;

extern uint16_t tsensor_1_min;
extern uint16_t tsensor_1_max;
extern uint16_t tsensor_limit;
extern uint8_t time_reset;
extern uint8_t time_start;
extern uint8_t time_bit0;
extern uint8_t time_bit1;
extern uint8_t time_wait;

extern uint8_t CSI_relay;

extern uint32_t time_scheduling;
extern uint32_t shortInspecTime;
extern uint32_t longInspecTime;

extern long int HV_read_coefA[4];
extern long int HV_read_coefB[4];

extern uint16_t lcA1;
extern uint16_t lcA2;
extern uint16_t lcB1;
extern uint16_t lcB2;

extern uint16_t GeneDacVoltage;

/**
 * @brief Initialize the microcontroller peripherals and I/O ports.
 */
void ucsetup(void) {

    uint16_t co,cp,dep;
    uint16_t sTime,lTime;
    uint32_t *coefA,*coefB;

    time_scheduling=0;

    time_bit0=22;
    time_bit1=63;
    time_reset=64;
    time_start=144;
    time_wait=64;
    tsensor_1_min=(uint16_t)(6);
    tsensor_1_max=(uint16_t)(800);
    tsensor_limit=(uint16_t)(800);

    for (cp=0;cp<4;cp++) {
        EERead(cp+EEPROM_HV_BORNE_SUP_A1);
    }
    HV_borne_sup_A1 = (((uint16_t)EERead(EEPROM_HV_BORNE_SUP_A1+1))<<8)+((uint16_t)EERead(EEPROM_HV_BORNE_SUP_A1));
    HV_borne_sup_A2 = (((uint16_t)EERead(EEPROM_HV_BORNE_SUP_A1+3))<<8)+((uint16_t)EERead(EEPROM_HV_BORNE_SUP_A1+2));
    HV_borne_sup_B1 = (((uint16_t)EERead(EEPROM_HV_BORNE_SUP_A1+5))<<8)+((uint16_t)EERead(EEPROM_HV_BORNE_SUP_A1+4));
    HV_borne_sup_B2 = (((uint16_t)EERead(EEPROM_HV_BORNE_SUP_A1+7))<<8)+((uint16_t)EERead(EEPROM_HV_BORNE_SUP_A1+6));
    if (HV_borne_sup_A1>HVSi1Max) {
        apply_hvmax('A','1',HVSi1Max);    
    }
    if (HV_borne_sup_A2>HVSi2Max) {
        apply_hvmax('A','2',HVSi2Max);
    }
    if (HV_borne_sup_B1>200) {
        apply_hvmax('B','1',HVSi1Max);
    }
    if (HV_borne_sup_B2>350) {
        apply_hvmax('B','2',HVSi2Max);
    }
    sTime = (((uint16_t)EERead(EEPROM_HV_SHORT_INSPEC_TIME+1))<<8)+((uint16_t)EERead(EEPROM_HV_SHORT_INSPEC_TIME));
    lTime = (((uint16_t)EERead(EEPROM_HV_LONG_INSPEC_TIME+1))<<8)+((uint16_t)EERead(EEPROM_HV_LONG_INSPEC_TIME));
    if ((lTime<=sTime)||(lTime==0)||(sTime==0)) {
        shortInspecTime = shortInspecDelay;
        longInspecTime = normalInspecDelay;
        EEWrite(EEPROM_HV_SHORT_INSPEC_TIME + 1, (uint8_t) (((shortInspecTime/15000)& 0xFF00) >> 8));
        EEWrite(EEPROM_HV_SHORT_INSPEC_TIME, (uint8_t) (((shortInspecTime/15000)&0xFF)));
        EEWrite(EEPROM_HV_LONG_INSPEC_TIME + 1, (uint8_t) (((longInspecTime/15000)& 0xFF00) >> 8));
        EEWrite(EEPROM_HV_LONG_INSPEC_TIME, (uint8_t) (((longInspecTime/15000)&0xFF)));
    } else {
        shortInspecTime = 15000*((uint32_t)sTime);
        longInspecTime = 15000*((uint32_t)lTime);
    }
    default_DA1=0;
    default_DA2=0;
    default_DB1=0;
    default_DB2=0;
    valeur_portB=0b11100000;
    valeur_trisB=0b00010010;
    valeur_portA=0b00000000;
    valeur_trisA=0b00111111;
    valeur_trisC=0b10010000;
    valeur_portC=0b00000111;
    valeur_trisD=0b10000000;
    valeur_portD=0b01111100;
    valeur_trisE=0b00000111;

    TRISA=valeur_trisA;
    PORTA=valeur_portA;
    TRISB=valeur_trisB;
    PORTB=valeur_portB;
    TRISC=valeur_trisC;
    PORTC=valeur_portC;
    TRISD=valeur_trisD;
    PORTD=valeur_portD;
    TRISE=valeur_trisE;

    //Analog conversion
    ANSEL=0xE0;
    ANSELH=0;
    ADCON2=0b10100100; //ADFM = 1, ACQ time = 100 for 8 TAD and ADCS = 100 for FOSC/4
    ADCON1=0; //bit 5 : VCFG1 = 0 to select VSS and bit 4 : VCFG0 = 0 to select VDD
    
    GeneDacVoltage=0;
    
    // configure USART
    TXSTA = 0; // Reset USART registers to POR state
    RCSTA = 0;
    TXSTAbits.BRGH = 1;
    SPBRG = _SPBRG; // Write baudrate to SPBRG1
    SPBRGH = _SPBRG >> 8; // For 16-bit baud rate generation
    TXSTAbits.SYNC = 0;
    TXSTAbits.TXEN = 1; // Enable transmitter
    RCSTAbits.SPEN = 1; // Enable receiver
    RCSTAbits.CREN = 1;
    //The Timer1 device is required for T�C sensor functions
    OpenTimer1(TIMER_INT_OFF & T1_16BIT_RW & T1_SOURCE_INT & T1_PS_1_8 & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF);
    //The Timer3 device is required for the High voltage slope
    OpenTimer3(TIMER_INT_OFF & T3_16BIT_RW & T3_SOURCE_INT & T3_PS_1_1 & T3_SYNC_EXT_OFF);
    IPR2bits.TMR3IP=1; // TMR3 overflow interrupt priority bit : low priority
    PIE2bits.TMR3IE=0; // disable the TMR3 overflow interrupt. It will be enabled when the setHTV function will be called
    OpenSPI(SPI_FOSC_16, MODE_10,SMPMID);
    dac_init();
    adc_init();
    dac_sequence(0,0);
    
    for (cp=0;cp<4;cp++) {
        HvValueTab[cp][0]=0;
        HvValueTab[cp][1]=0;
        HvInc[cp]=0;
        HvStatus[cp]=1;
        HvPhysTarget[cp]=0;
        HvPhysCorrect[cp]=0;
    }
    cal_preampli_offset=0;
    marge_pa_offset=101;
    //initialize the high voltage modules
    dac_sequence(0x10,HvValueTab[0][0]);
    dac_sequence(0x20,HvValueTab[1][0]);
    dac_sequence(0x30,HvValueTab[2][0]);
    dac_sequence(0x40,HvValueTab[3][0]);
    coefA_A1=0;
    coefA_A2=0;
    coefA_B1=0;
    coefA_B2=0;
    coefB_A1=0;
    coefB_A2=0;
    coefB_B1=0;
    coefB_B2=0;

    lcA1=0;
    lcA2=0;
    lcB1=0;
    lcB2=0;
    CSI_relay=0;
    
    for (cp=0;cp<4;cp++) {
        dep=EEPROM_CAL_HV_FIRST_ADR+cp*EEPROM_CAL_HV_WIDTH;
        if (EERead(cp+EEPROM_CAL_HV_CALIBRATED)==SPI_KEY) {
            switch(cp) {
                case 0:
                    coefA=&coefA_A1;
                    coefB=&coefB_A1;
                    break;
                case 1:
                    coefA=&coefA_A2;
                    coefB=&coefB_A2;
                    break;
                case 2:
                    coefA=&coefA_B1;
                    coefB=&coefB_B1;
                    break;
                case 3:
                    coefA=&coefA_B2;
                    coefB=&coefB_B2;
                    break;
                default:
                    break;
            }
            *coefA=0;
            *coefB=0;
            for(co=dep;co<dep+4;co++) {
                *coefA=*coefA+(((uint32_t)(EERead(co)))<<(8*(co-dep)));
            }
            for(co=dep+4;co<dep+8;co++) {
                *coefB=*coefB+(((uint32_t)(EERead(co)))<<(8*(co-dep-4)));
            }
        }
    }

    for (cp=0;cp<4;cp++) {
        HV_read_coefA[cp]=0;
        HV_read_coefB[cp]=0;
        dep=EEPROM_COEF_HV_READING+6*cp;
        if ((EERead(dep+1)==0xFF)&&(EERead(dep)==0xFF)) {
            if ((cp==0)||(cp==2)) {
                HV_read_coefA[cp]=6167;
                HV_read_coefB[cp]=180235;
            } else {
                HV_read_coefA[cp]=9819;
                HV_read_coefB[cp]=669642;
            }
        } else {
            HV_read_coefA[cp]=(((long int)(EERead(dep+1)))<<8)+(long int)(EERead(dep));
            HV_read_coefB[cp]=0;
            for (co=dep+2;co<dep+5;co++) {
                HV_read_coefB[cp]=HV_read_coefB[cp]+(((long int)(EERead(co)))<<(8*(co-dep-2)));
            }
            if (EERead(dep+5)==0) {
                HV_read_coefB[cp]=0-HV_read_coefB[cp];
            }
        }
    }
    enableHVMeas = EERead(49);
}

/**
 * @brief Set up memory and runtime data structures.
 */
void memsetup(void) {

}

// EOF
