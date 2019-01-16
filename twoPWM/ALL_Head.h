#ifndef _ALL_HEAD_H_
#define _ALL_HEAD_H_

#include <SN8F5702.h>
#include "include/GenericTypeDefs.h"


extern bit f_100us,f_key,f_power,f_buz,f_4ms,f_pwmout_chge;

extern bit	f_out_enb,f_chge_enb,f_vot_out_cycle,f_short,f_led1_on;


extern INT8 LED_MODE,Strength_pcl,Vot_table_pcl,table_pcl_count,vot_countbuf,led_mode;
extern INT  AD_Data,AD_Srteng;
extern INT8	led1_bright,led2_bright,led3_bright,led4_bright,sleep_delay;

//-----------------------------------------------------------------------------
//Global LED_DISPLAY Constants
                                                              
//-----------------------------------------------------------------------------


#define	Key		P00
#define	led1	P01
#define	led2	P02
#define	led3	P03

extern void AD_Mode(void);
extern void System_ini(void);
extern void KeyScan(void);
extern void LED_Display(void);
extern void	f_led2mode(void);
extern	void Scanbat2curr(void);
extern	void Bat1voltage(void);
extern	void f_led1_breathe_mode(void);
extern	void f_led3_breathe_mode(void);
//-----------------------------------------------------------------------------
//Global Time Constants
//-----------------------------------------------------------------------------
/*
#define Mode0   1       // 13-bit timer
#define Mode1   2       // 16-bit timer
#define Mode2   3       // 8 -bit auto-reload Timer
#define Mode3   4       // Timer 0 acts as two independent 8 bit Timers
#define MODE    Mode2   // select mode of timer0



#if		MODE == Mode0
#define	TM0D_Value	0X04	//MODE0 13BIT-TIME,Fhosc
#define	TH0_Value		0XF8	//8192 - 250
#define	TL0_Value		0X06	

#elif	MODE == Mode1
#define	TM0D_Value	0X05	//MODE1 16BIT-TIME,Fhosc
#define	TH0_Value		0XFF	//65536 - 250
#define	TL0_Value		0X06	

#elif	MODE == Mode2				
#define	TM0D_Value	0X06	//MODE2 8BIT-TIME,Fhosc
#define	TH0_Value		130//0X06	//Auto-reload
#define	TL0_Value		130//0X06	//256 - 250

#elif	MODE == Mode3
#define	TM0D_Value	0X07	//MODE3,TWO 8BIT-TIME
#define	TH0_Value		0X38	//256 - 200,Fcpu(0.75 * 200 = 150US)
#define	TL0_Value		0X06	//256 - 250

#endif

//-----time1------//
													
#if		MODE == Mode0
#define	TM1D_Value	0X40	//MODE0 13BIT-TIME,Fhosc
#define	TH1_Value		0XF8	//8192 - 250
#define	TL1_Value		0X06	

#elif	MODE == Mode1
#define	TM1D_Value	0X50	//MODE1 16BIT-TIME,Fhosc
#define	TH1_Value		0XFF	//65536 - 250
#define	TL1_Value		0X06	

#elif	MODE == Mode2				
#define	TM1D_Value	0X60	//MODE2 8BIT-TIME,Fhosc
#define	TH1_Value		0X06	//Auto-reload
#define	TL1_Value		0X06	//256 - 250
#endif

*/


#define	TM0D_Value	0X06	//MODE  8BIT-TIME,Fhosc

#define TCON0_Value 0x60		//[6:4]  000:	f/128				001(0x10):	f/64	010(0x20):	f/32	011(0x30):	f/16	
														//			 100(0x40):	f/8		101(0x50):	f/4		110(0x60):	f/2		111(0x70):	f/1

#define TH0_Value 128//128
#define TL0_Value 128//131//128			( t0c values 0  until count 256 intrupt)


#define	TM1D_Value	0X50	//MODE  16BIT-TIME,Fhosc

#define TCON1_Value 0x00		//[2:0]  000:	f/128		001:	f/64		010:	f/32	011:	f/16	
														//			 100:	f/8		  101:	f/4		  110:	f/2		111:	f/1


#define TH1_Value 0XFc//	4MS
#define TL1_Value 0X16//


//-----------------------------------------------------------------------------
// Global AD Constants 
//-----------------------------------------------------------------------------
#define _EOC                ADM & 0x20          // ADC status (EOC)
#define CLR_ADCCH          (ADM & 0xE0)        	// clear CHS[4:0]
#define CLR_ADCVH          (VREFH & 0xF8)       //
#define _ADENB              ADM |= 0x80         // ADC enable (ADENB)
#define EnADC_Coverting     ADM |= 0x40         // ADC converting start (ADS)
#define  CL_EOC							(ADM &=0xDF)

//ADR
#define	ADCChannelEn		0x40				//enable ADC channel
#define	ADCSpeedDiv16		0x00				//ADC clock = fosc/16
#define	ADCSpeedDiv8		0x10				//ADC clock = fosc/8
#define	ADCSpeedDiv1		0x20				//ADC clock = fosc/1
#define	ADCSpeedDiv2		0x30				//ADC clock = fosc/2
#define	ADCSpeed			ADCSpeedDiv1

//VREFH
#define	EvhenbInt			0x00				//Enable ADC internal VREFH function (EVHENB)
#define	EvhenbExt			0x80				//Disenable ADC internal VREFH function (EVHENB)
#define	VoltChannel			EvhenbInt 


#define	ADCAIN_2V			0x00				// reference voltage is 2V.  
#define	ADCAIN_3V			0x01				// reference voltage is 3V.  
#define	ADCAIN_4V			0x02				// reference voltage is 4V.  
#define	ADCInRefVDD			0x03				// reference voltage is VDD.  
#define	ADCAIN14_VDD		0x04				// ADC internal reference high voltage selects bit
#define	ReferVolt			ADCAIN_3V 




#endif