//-----------------------------------------------------------------------------
//  FILE:   Timer0.c
//-----------------------------------------------------------------------------
//  Copyright 2014 Sonix Technology Corp. All rights reserved. 
//
//  AUTHOR:     Sonix
//  DATE:       2015/08/04
//
//  The program is an example of Timer0 to implement it.
//  you can select mode0~3 by "#define MODE Mode0~3"  
//    
//  Timer 0 function List:
//      1. Mode 0 : 13-bit timer
//      2. Mode 1 : 16-bit timer
//      3. Mode 2 : 8 -bit auto-reload Timer
//      4. Mode 3 : Timer 0 acts as two independent 8 bit Timers ¡V TL0, TH0. Timer0 and Timer1
// 
//  The system clock frequency is IHRC 32MHz
//
//  Device:     SN8F5708 
//  Tool chain: KEIL C51 V9.50a
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SN8F5702.h>
//-----------------------------------------------------------------------------
// Global Constants 
//-----------------------------------------------------------------------------
#define Mode0   1       				// 13-bit timer
#define Mode1   2       				// 16-bit timer
#define Mode2   3       				// 8 -bit auto-reload Timer
#define Mode3   4       				// Timer 0 acts as two independent 8 bit Timers
#define MODE    Mode3   				// select mode of timer0
//=============================================================================
//		T0=1MS  if(MODE = Mode3) T1=150US;
//=============================================================================
#define	TCON0_Value	0X00				//T0:Fclk = Fhosc/128 = 32M/128 = 250K
																//if MODE = Mode3,T1:Fclk = Fcpu/12 = Fhosc/2/12 = 16M/12
													
#if		MODE == Mode0
#define	TM0D_Value	0X04				//MODE0 13BIT-TIME,Fhosc
#define	TH0_Value		0XF8				//TH[7£º0]TL[4£º0]=1111 1000 0 0110 = 1F06   8192 - 7942 = 250     250 * 1/Fclk = 250 * 128/32M = 1MS
#define	TL0_Value		0X06	

#elif	MODE == Mode1
#define	TM0D_Value	0X05				//MODE1 16BIT-TIME,Fhosc
#define	TH0_Value		0XFF				//65536 - 65286 = 250    250 * 1/Fclk = 250 * 128/32M = 1MS
#define	TL0_Value		0X06	

#elif	MODE == Mode2				
#define	TM0D_Value	0X06				//MODE2 8BIT-TIME,Fhosc
#define	TH0_Value		0X06				//Auto-reload
#define	TL0_Value		0X06				//256 - 6 = 250    250 * 1/Fclk = 250 * 128/32M = 1MS

#elif	MODE == Mode3
#define	TM0D_Value	0X07				//MODE3,TWO 8BIT-TIME
#define	TH0_Value		0X38				//256 - 56 = 200,  200 * 1/Fclk = 200 * 12/16M = 150US
#define	TL0_Value		0X06				//256 - 250
#endif
//-----------------------------------------------------------------------------
// Global Variables 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function Prototypes 
//-----------------------------------------------------------------------------
void Timer0_Init(void);
//-----------------------------------------------------------------------------
// Main loop
//-----------------------------------------------------------------------------
void main(void)
{    
		CLKSEL = 0x06;							// Fcpu = 32M/2 = 16M
		CLKCMD = 0X69;
		CKCON = 0X10;								// Wait	2 Cycle From = 8M
	
	  WDTR = 0x5A;           			// clear watchdog if watchdog enable
	
		P0 = 0;
		P0M = 0;
		P0UR = 0xFF;
	
		P1 = 0;
		P1M = 0;
		P1UR = 0xFF;
	
		P2 = 0;
		P2M = 0x03;                // P2.0 & P2.1 output low
		P2UR = 0xFF;
    
		Timer0_Init();
    
    while (1) 
		{
        WDTR = 0x5A;            // clear watchdog if watchdog enable
    }
}
//-----------------------------------------------------------------------------
//  Function:   void Timer0_Init(void)
//  Description:
//          timer0's mode0 setting
//-----------------------------------------------------------------------------
void Timer0_Init(void)
{
    TMOD = TM0D_Value;          // Mode of Timer 0 
    TH0 = TH0_Value;            // Initial values
    TL0 = TL0_Value;
    TCON0 = TCON0_Value;        // T0EXT = Fhosc.
                                // T0RATE is Fhosc/128 = 250K

    ET0 = 1;                    // Timer 0 isr enable
    EAL = 1;                    // Golbal Interrupt enable
    TR0 = 1;                    // enable Timer 0 Run control
	#if	MODE == Mode3
		ET1 = 1; 										// Timer 1 isr enable
		TR1 = 1;										// enable Timer 1 Run control
	#endif
}
//-----------------------------------------------------------------------------
//  Timer0 ISR
//-----------------------------------------------------------------------------
void Timer0_ISR(void) interrupt ISRTimer0   // Vector @  0x0B
{
#if			MODE == Mode3    
		TL0 = TL0_Value;						// if Timer0 is Mode 3,The timer TL0 is controlled by T0						
#elif 	MODE != Mode2
	  TH0 = TH0_Value;            // if Timer0 is Mode 2, the TL0 will auto reload  
    TL0 = TL0_Value;            // and the reload value is kept in TH0 
#endif        
    P20 = ~ P20;                // P2.4 toggle
}
//-----------------------------------------------------------------------------
//  Timer1 ISR
//-----------------------------------------------------------------------------
#if MODE == Mode3
void Timer1_ISR(void) interrupt ISRTimer1   // Vector @  0x1B
{ 
    TH0 = TH0_Value;            // timer1 used
    P21 = ~ P21;                // P2.6 toggle
}
#endif
