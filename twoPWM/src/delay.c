#include "common.h"
/*
void testdelay(void)
{
	while(1)
	{
		P01 = 1;
		Delay1us(1);
		P01 = 0;
		Delay1us(1);
	}
}
*/
void Delay1us(unsigned int n)
{
    unsigned int i;
    for (i=0; i<n; i++)
	{
		_nop_();             //1¸ö_nop_()=6us.
		_nop_();             //1¸ö_nop_()=6us.
		_nop_();             //1¸ö_nop_()=6us.
		_nop_();             //1¸ö_nop_()=6us.
		_nop_();             //1¸ö_nop_()=6us.
		_nop_();             //1¸ö_nop_()=6us.
    }
}

void Delay1ms(unsigned int n)
{
    unsigned int i, j;
    i = 0;
    j = 0;
    for (i=0; i<n; i++) {
        for (j=0; j<40; j++) {
           _nop_();        
        }
    }
}
void Delay1s(void)
{
    unsigned int i, j;
    i = 0;
    j = 0;
    for (i = 0; i < 218; i++) 
	{
        for (j = 0; j < 200; j++) 
		{
			_nop_(); 
        }
    }
}