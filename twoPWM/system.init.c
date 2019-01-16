#include <include/All_Head.h>


void	System_ini(void);
//											SN8F57021S
//			VSS					VSS		1			14	VDD			VDD
//								  P00		2			13	P10			
//									P01		3			12	P11			
//									P02		4			11	P12			
//									P05		5			10	P13			
//									P06		6			9		P14			
//									P07		7			8		P20			

/*****************************************

*****************************************/
void	System_ini(void)
{
	
//-----IO init -----//

		P0M	= 0xFE;		//1111 1011
		P0	= 0x00;
		P0UR	= 0x01;
	
	
		P1M	= 0xFF;//0xf3;		//1111 0011
		P1	= 0x00;//0x00;
		P1UR	= 0x0c;//0x0c;	//0000 1100
		P1W = 0x00;
	
		P2M	= 0xFF;		//0000 0010
		P2	= 0x00;
		P2UR	=0x00;
	
		_nop_();
//-----Time init -----//

		//TMOD = TM0D_Value | TM1D_Value;          // Mode of Timer 0 		
		//TCON0 = TCON0_Value|TCON1_Value;     // T0EXT = Fhosc.
		
		TMOD =  0X06   |	0X50 ;  // = 01010110	//T1RATE is Fhosc/128 = 250K
		TCON0=	0X00   |	0X00 ;	// = 00000000										  // T0RATE is Fhosc/128 = 250K
										 	
		TH0	= 231;	//TH0_Value;	  =  (256- 231 ) *4US =100US 
		TL0	= 231;	//TL0_Value;
		
		TH1 = 0XFc;	//TH1_Value;      =  (FFFF- FC16 ) *4US =4000US = 4MS	
		TL1 = 0X16;	//lTL1_Value;
	 
		TF0 = 0;     //  T0定时器 溢出标志      // Clear Timer0 overflow flag
		TR0	= 1;	// T0定时器 打开控制位	  // enable Timer 0 Run control
		ET0	= 1;	// T0定时器中断控制位	// Timer 0 isr enable

		TF1 = 0;        	  // Clear Timer1 overflow flag
		TR1	= 1;			// enable Timer 1 Run control
		ET1	= 1;			// Timer 1 isr enable	
							// T0RATE is Fhosc/1
//----- GIE -----			//			
		EAL		=1; 			 // Golbal Interrupt enable
		
	// pwm  int
		
		// PW1M |= 0x70;           // PW1 rate is Fhosc/1
	//PW1M |= 0x20;           // PW1 rate is Fhosc/32 = 1m
		// PW1M |= 0x01;  				//PW1 PW1P0(1/ENABLE)
		// PW1M |= 0x04;           // PWM11 outputs is an inverse signal of pwm
		 

	
							// enable PWM11 output (PWCH11)
    PW1CH = 0x00;           // enable PWM10 output (PWCH10) 
	PW1YH = 0X00;			// 周期设置，。8BIT 设定255，
    PW1YL = 0X64;           // set PW1 cycle control 

    PW1DH = 0x00;			// 占空比 初始化给0
    PW1DL = 0x00;           // set PWM duty control 

							// the dead band control : B point > A point (Must)
    PW1BH = PW1DH;
    PW1BL = PW1DL;           // B point dead band control
  	PW1A = 0x00;            // A point dead band control
	
	 //PW1M |= 0X80;		  // Enable PWM output function (PW1EN)

	IEN4 = 0x00;           // PWM1 interrupt enable (EPWM1) 		 
	PW1M = 0x80;			//  频率设定，正反向设定，打开关闭 BIT0关闭
	
}