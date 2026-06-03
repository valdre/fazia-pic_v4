#include "functions.h"

/**
 * @brief Compute 10 raised to power c as a 32-bit value.
 * @param c Exponent (power of 10)
 * @return UINT32 status or result code.
 */
UINT32 power10_32(BYTE c)
{
    UINT32 value;
    BYTE k;

    value=1;
    k=0;

    if ((c>0)&&(c<10))
    {
        while (k<c)
        {
            k++;
            value*=10;
        }
    }

    return value;
}

/**
 * @brief Parse a decimal or hexadecimal string into a 32-bit unsigned integer.
 * @param pr Input decimal or hex string
 * @param converted_value Output parsed value
 * @return BYTE status or result code.
 */
BYTE analyze_string32(char *pr,UINT32 *converted_value)
{
    BYTE co,cp,error;
    
    co=0;
    error=0;
    cp=0;
    *converted_value=0;
    
    do
    {
        co++;
    }
    while (pr[co]!='\0');
    
    //co is the length of the string
    
    if (co<11)
    {
        while ((cp<co)&&(error==0))
        {
            if ((pr[cp]>='0')&&(pr[cp]<='9'))
            {
                *converted_value=*converted_value+(((UINT32)(pr[cp]-'0'))*power10_32(co-cp-1));
                cp++;
            }
            else
                error=1;
        }        
    }
    else
        error=1;
    
    return error;
}

/**
 * @brief Parse a decimal or hexadecimal (0x prefix) string into a 16-bit integer.
 * @param pr Input decimal or 0x-prefixed hex string
 * @param converted_value Output parsed value
 * @return BYTE status or result code.
 */
BYTE analyze_string(char *pr,UINT *converted_value)
{
    BYTE co,error;
    
    if ((pr[0]=='0')&&((pr[1]=='x')||(pr[1]=='X')))
    {
        co=2;
        do
        {
            error=1;
            
            if ((pr[co] >='0')&&(pr[co]<='9'))
                error=0;
            
            if ((pr[co]>='A')&&(pr[co]<='F'))
                error=0;
            
            if ((pr[co]>='a')&&(pr[co]<='f'))
                error=0;
            co++;
        }
        while((pr[co]!='\0')&&(error==0));

//        putrsUSART((const far rom char *)"error:");
//        while(BusyUSART());
//        putcUSART(error+'0');
//        while(BusyUSART());
//        putcUSART('\n');
//        while(BusyUSART());
//        putcUSART('\r');
//        while(BusyUSART());


        if ((co<7)&&(error==0))
        {
//            putrsUSART((const far rom char *)"conversion:");
//            while(BusyUSART());
            *converted_value=charhextoi(pr);
//            dispinttobin(*converted_value);
//            putcUSART('\n');
//            while(BusyUSART());
//            putcUSART('\r');
//            while(BusyUSART());
        }
        else
            error=1;

    }
    else
    {
        co=0;
        error=0;
        while((pr[co]!='\0')&&(error==0))
        {
            if ((pr[co] >='0')&&(pr[co]<='9'))
                error=0;
            else
                error=1;
            co++;
        }

        if ((co<6)&&(error==0))
            *converted_value=chardectoi(pr);
        else
            error=1;
    }

    return error;
}


/**
 * @brief Convert a decimal ASCII string to a 16-bit unsigned integer.
 * @param pr Decimal ASCII string
 * @return UINT status or result code.
 */
UINT chardectoi(char *pr)
{
    BYTE co,max;
    UINT32 result;

    co=0;
    result=0;

    while (pr[co]!='\0')
        co++;

    max=co-1;
    co=0;

    if (max<5)
    {
        do
        {
            if ((pr[co] >='0')&&(pr[co]<='9'))
            {
                result+=(UINT32)((pr[co]-'0')*power10(max-co));
                co++;
            }
            else
            {
                co=0;
                result=0;
            }
        }
        while(pr[co]!='\0');
    }
    else
        result=0;

    if (result>65535)
        result=0;

    return (UINT)result;

}

/**
 * @brief Convert a hexadecimal ASCII string with 0x prefix to unsigned integer.
 * @param pr Hexadecimal ASCII string with 0x prefix
 * @return UINT status or result code.
 */
UINT charhextoi(char *pr)
{
	int co;
	int erreur=0;
	unsigned int accumulateur=0;
        unsigned char nbre;

	if ((pr[0]=='0')&&((pr[1]=='x')||(pr[1]=='X'))) 
	{
		co = 2;
		while ((pr[co] != '\0')&&(!erreur)) 
		{
			if ((pr[co] >='0')&&(pr[co]<='9')) nbre = pr[co]-'0';
			else if ((pr[co]>='A')&&(pr[co]<='F')) nbre = pr[co]-'A'+10;
			else if ((pr[co]>='a')&&(pr[co]<='f')) nbre = pr[co]-'a'+10;
			else 
			{
				erreur=-1;
			}
			if (erreur==0)
			{
				accumulateur = (accumulateur << 4)|(int)nbre;
				co++;
			}
		}
	} 
	else
		erreur = 0;

	if (erreur==0)
		return accumulateur;
	else
		return erreur;
}

/**
 * @brief Compute 10 raised to power c as a 16-bit value.
 * @param c Exponent (power of 10)
 * @return UINT status or result code.
 */
UINT power10(BYTE c)
{
    unsigned int value;
    unsigned char k;

    value=1;
    k=0;

    if ((c>0)&&(c<5))
    {
        while (k<c)
        {
            k++;
            value*=10;
        }
    }

    return value;
}

