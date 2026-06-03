#include "functions.h"

extern BYTE valeur_portB;
extern BYTE valeur_trisB;
extern ram UINT tsensor_1_min;
extern ram UINT tsensor_1_max;
extern ram UINT tsensor_limit;
extern ram BYTE time_reset;
extern ram BYTE time_start;
extern ram BYTE time_bit0;
extern ram BYTE time_bit1;
extern ram BYTE time_wait;



BYTE temp_init(void)
{

	/*En écriture pour le registre A : mettre à 1 le bit n°1 relatif à l'Enable Sensor*/
	/*****************C3 C2 C1 N5 N4 N3 N2 N1 N0 RW D15 D14 D13 D12 D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0 EP ACK Sentinelle*/
	char BitTab[] = { 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -1 };

	/*En écriture pour le registre 5 : mettre à 1 le bit n°4 relatif à l'Enable function*/
	/*****************C3 C2 C1 N5 N4 N3 N2 N1 N0 RW D15 D14 D13 D12 D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0 EP ACK Sentinelle*/
	char BitTab2[] = { 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, -1 };

	char j,co,t;

	for (j=0;j<nbcapteurs;j++)
	{

		BitTab[0]=(((j+1)&0b00000100)>>2);
		BitTab[1]=(((j+1)&0b00000010)>>1);
		BitTab[2]=(((j+1)&0b00000001));
		BitTab2[0]=BitTab[0];
		BitTab2[1]=BitTab[1];
		BitTab2[2]=BitTab[2];

		if (j==2)
		{
			BitTab[26]=1;
			BitTab2[26]=1;
		}

		if (j==3)
		{
			BitTab[26]=0;
			BitTab2[26]=0;
		}

		/*On initialise la valeur du PORT A pour que le bit 2 reste toujours à 0*/
		valeur_portB=valeur_portB&(0b11111111^mask_capteur);

		/*on relache le bit A2 du port A pdt 10 µs avant la transaction*/
		attente_bit1(0);

		/*génération d'un signal de reset*/
		valeur_trisB=valeur_trisB&(0b11111111^mask_capteur); 
		PORTB = valeur_portB;
		TRISB = valeur_trisB;
                //WriteTimer3(0);
                //while(ReadTimer3()<640);
		Delay100TCYx(time_reset);

		/*on met le bit A2 du port A à 1 pdt 10 µs avant la transaction*/
		attente_bit1(0);

		write_bit_start();
		attente_bit1(0);


		co=0;
		while (BitTab[co]!=-1)
		{
			write_bit(BitTab[co]);
			attente_bit1(BitTab[co]);
			co++;
		}

		co=0;
		attente_bit1(0);

		write_bit_start();
		attente_bit1(0);

		while (BitTab2[co]!=-1)
		{
			write_bit(BitTab2[co]);
			attente_bit1(BitTab2[co]);
			co++;		
		}

	}

        return 0;
}



BYTE temp(int *temperature)
{

    /*En lecture pour le registre 9*/
    /*****************C3 C2 C1 N5 N4 N3 N2 N1 N0 RW D15 D14 D13 D12 D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0 EP ACK Sentinelle*/
    char BitTab3[] = { 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1 };

    BYTE erreur;
    char co, parity;
    char *s,j;
    unsigned int timer1,mask;
    long result;

    for (co=0;co<nbcapteurs+2;co++)
        temperature[co]=0;

    for (j=0;j<nbcapteurs;j++)
    {
	co=0;
	parity=0;
	erreur=0;

	BitTab3[0]=(((j+1)&0b00000100)>>2);
	BitTab3[1]=(((j+1)&0b00000010)>>1);
	BitTab3[2]=(((j+1)&0b00000001));
		
	attente_bit1(0);


	write_bit_start();
		

	attente_bit1(0);


	while (BitTab3[co]!=-1)
	{
		if (co<10)
		{
                	write_bit(BitTab3[co]);
			attente_bit1(BitTab3[co]);
			if (BitTab3[co]==1) parity++;
		}

		if ((co>9)&&(co<27))
		{
			timer1=0;
			write_bit(BitTab3[co]);
			valeur_trisB |= mask_capteur; 
			WriteTimer1(0); /*réinitialise le timer 1*/
			TRISB = valeur_trisB;
			
			while ((capteur_io==0)&&(timer1<tsensor_limit))
				timer1=ReadTimer1();


			if ((timer1>tsensor_1_min)&&(timer1<tsensor_1_max))
			{

				if ((co>9)&&(co<20)) temperature[j]|=(((int)1)<<(10-co+9));
                        		parity++;
			}

			if (timer1>=tsensor_limit)
				erreur=1;
		}

		if (co==27)
		{
			write_bit((parity&1)^1);
			attente_bit1((parity&1)^1);
		}

		co++;
	}
    }

    for (j=0;j<nbcapteurs;j++)
        temperature[j]=(25*temperature[j])/100;

    wrspi(1,0x200,0x001); //we write into the register whatever the value
    wrspi(2,0x200,0x001);
    
    result=(long)rdspi(1,0x200);
    result=(long)rdspi(2,0x200);

    wrspi(1,0x200,0x001);
    wrspi(2,0x200,0x001);


    for (j=0;j<2;j++)
    {
        do
        {
            result=(long)rdspi(j+1,0x200);
            mask=((unsigned int)result & 0b1000000000000000)>>15;
        }
        while(mask!=1);

        temperature[nbcapteurs+j]=(int)((492*(result&0x03FF)-273150)/1000);
    }


    return erreur;
}



void write_bit_start(void)
{
	/*Bit de START*/
	/*on met le bit A2 du port A à 0 pdt 90 µs*/
	valeur_trisB=valeur_trisB & (0b11111111^mask_capteur); 
	PORTB = valeur_portB;
	TRISB = valeur_trisB;
	Delay10TCYx(time_start);

}

void write_bit(char c)
{
	valeur_trisB=valeur_trisB & (0b11111111^mask_capteur); 
	PORTB = valeur_portB;
	TRISB = valeur_trisB;

	if (c==1)
        {
            Delay10TCYx(time_bit1);
        }
	else
        {
            Delay10TCYx(time_bit0);
        }
}

void attente_bit1(char c)
{
	/*temps d'attente*/
	/*on configure le bit 2 du port A en sortie de telle manière à laisser passer le 0 */
	valeur_trisB |= mask_capteur; 
	PORTB = valeur_portB;
	TRISB = valeur_trisB;

	Delay10TCYx(time_wait);

	/*on relache maintenant le bit 2 du portA*/
	/*la résistance de pull-up prend le relais*/
	valeur_trisB=valeur_trisB | mask_capteur; 
	TRISB = valeur_trisB;
}


