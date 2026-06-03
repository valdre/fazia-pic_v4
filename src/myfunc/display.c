#include "functions.h"


/**********************Fonction qui retourne le numéro d'identification de la carte*******************/
BYTE getid(void)
{
	BYTE id=0;

	id |= GEOA0;
	id |= GEOA1<<1;
	id |= GEOA2<<2;

	return id;
}

BYTE asciiconv(BYTE c)
{
    BYTE asciivalue;

    asciivalue=0;

    if ((c>=0)&&(c<=9))
        asciivalue='0'+c;

    if (c>9)
        asciivalue='A'+(c-10);

    return asciivalue;
}

void uinttoa(UINT value, BYTE *s)
{
    BYTE co;
    UINT mask;

    mask=0xF000;

    for(co=0;co<4;co++)
        s[co]=asciiconv((UINT)((value&(mask>>(4*co)))>>((3-co)*4)));
}

char dispuinttochar(UINT value)
{
    BYTE co;
    BYTE s[4];

    uinttoa(value,s);
    for (co=0;co<4;co++)
        putcUSART(s[co]);

    return 0;
}

char dispuint32tochar(UINT32 value)
{
    char co;

    for (co=31;co>=0;co--)
    {
        putcUSART('0'+(char)((value>>co)&1));
    }

    return 0;
}


char dispinttobin (UINT a)
{
    int co;

    for (co=0;co<16;co++)
    {
        putcUSART('0'+((a>>(16-1-co))&0b00000001));
        while (BusyUSART());
    }
    return 0;
}

char dispchartobin(BYTE c)
{
    int co;

    for(co=0;co<8;co++)
    {
        putcUSART('0'+((c>>(8-1-co))&0b00000001));
        while (BusyUSART());
    }
    return 0;
}

void myStrCpyUint(char *container,UINT a,char cend)
{
    UINT u[5];
    UINT compt,comptInt;
    
    u[4] = a-10*(a/10);  //unités
    u[3] = a-100*(a/100)-u[4];  //dizaines
    u[3] = u[3]/10;
    u[2] = a-1000*(a/1000)-(10*u[3])-u[4];  //centaines
    u[2] = u[2]/100;
    u[1] = a-10000*(a/10000)-(100*u[2])-(10*u[3])-u[4];   //milliers
    u[1] = u[1]/1000;
    u[0] = a/10000;  //dizaines de milliers
    
    compt=0;
    comptInt=0;
    
    while (container[compt]!='\0')
        compt++;
    
    while ((u[comptInt]==0)&&(comptInt<5))
        comptInt++;
    
    if (comptInt<5)
        while (comptInt<5)
        {
            if ((u[comptInt]>=0)&&(u[comptInt]<=9))
                container[compt++]='0'+(char)u[comptInt];
            comptInt++;
        }
    else
        container[compt++]='0';
    
    if (cend!='\0')
    {
        container[compt++]=cend;
        container[compt]='\0';
    }
    else
        container[compt]='\0';
}

void myStrCpyByte(char *container,BYTE a,char cend)
{
    BYTE b[3];
    UINT compt,comptInt;
    
    b[2] = a-10*(a/10);
    b[1] = a-100*(a/100)-b[2];
    b[1] = b[1]/10;
    b[0] = 100*(a/100);
    
    compt=0;
    comptInt=0;
    while (container[compt]!='\0')
        compt++;
    
    while ((b[comptInt]==0)&&(comptInt<3))
        comptInt++;
    
    if (comptInt<3)
        while (comptInt<3)
        {
            if ((b[comptInt]>=0)&&(b[comptInt]<=9))
                container[compt++]='0'+(char)b[comptInt];
            comptInt++;
        }
    else
        container[compt++]='0';
    
    if (cend!='\0')
    {
        container[compt++]=cend;
        container[compt]='\0';
    }
    else
        container[compt]='\0';
}

void myStrCpyChar(char *container,char *chaine,char cend)
{
    UINT comptChain,compt;
    
    comptChain=0;
    compt=0;
    while (container[compt]!='\0')
        compt++;
    while (chaine[comptChain]!='\0')
        container[compt++]=chaine[comptChain++];
    
    if (cend!='\0')
    {
        container[compt++]=cend;
        container[compt]='\0';
    }
    else
        container[compt]='\0';
}

void myStrCpyChar2(char *container,const rom char *chaine,char cend)
{
    UINT comptChain,compt;
    
    comptChain=0;
    compt=0;
    while (container[compt]!='\0')
        compt++;
    while (chaine[comptChain]!='\0')
        container[compt++]=chaine[comptChain++];
    
    if (cend!='\0')
    {
        container[compt++]=cend;
        container[compt]='\0';
    }
    else
        container[compt]='\0';
}

void myStrCpy1Char(char *container,char c,char cend)
{
    UINT compt;
    
    compt=0;
    while (container[compt]!='\0')
        compt++;
    
    container[compt++]=c;
    
    if (cend!='\0')
    {
        container[compt++]=cend;
        container[compt]='\0';
    }
    else
        container[compt]='\0';
}

void myStrCpyHex(char *container,UINT a,int format,char cend)
{
    UINT compt,comptInt;
    BYTE co;
    char b[4];
    
    comptInt=0;
    compt=0;
    while (container[compt]!='\0')
        compt++;
    
    b[0] = (char)((a&0xF000)>>12);
    b[1] = (char)((a&0x0F00)>>8);
    b[2] = (char)((a&0x00F0)>>4);
    b[3] = (char)((a&0x000F));
    
    while ((b[comptInt]==0)&&(comptInt<4))
        comptInt++;
    
    if (comptInt<4)
    {
        if ((format!=-1)&&(format>4-comptInt))
            for (co=4-comptInt;co<format;co++)
               container[compt++]='0';
        
        
        while (comptInt<4)
        {
            if ((b[comptInt]>=0)&&(b[comptInt]<=9))
                container[compt++]='0'+b[comptInt];
            
            if (b[comptInt]>9)
                container[compt++]='A'+b[comptInt]-10;
            comptInt++;
        }
    }
    else
    {
        if ((format!=-1)&&(format<4))
            for(co=0;co<format;co++)
                container[compt++]='0';
    }
    
    if (cend!='\0')
    {
        container[compt++]=cend;
        container[compt]='\0';
    }
    else
        container[compt]='\0';
}

