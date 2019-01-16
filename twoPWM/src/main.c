#include "common.h"

//port
#define BOOST           P14         /* P14---->P05;�ɵ����ѹ������� */
#define LED_CON         P13         /* LED���� */
#define USB_INPUT       P11         /* USB+5V������ */
#define ADC_INPUT       P10         /* ��ص�ѹ���ADC */
#define CHARGE_LED      P02         /*������ */
#define GREEN_LED       P00         /* ������ָʾ�� */
#define RED_LED         P05        /* P05---P14;���״ָ̬ʾ�� + ������� */

#define S_PWM           P12       /* simulation PWM channel */
int led_mode=0;

//#define BAT_Propor      3.0f        /* ��ص�������ѹ���� */
#define ch_err          339        /* 0.4v */
#define BAT_Li_Max      2935       /* 4.3 ﮵������ѹֵ */
#define BAT_Li_Min      1706       /* (2.5f/3* 4096/2), ﮵����С��ѹֵ */
#define BAT_Li_ch_end   2935  // (4.3f/3* 4096/2)       /* ﮵��ͣ���ѹ */
#define BAT_Li_Pull     2901  // (4.25f/3* 4096/2)       /* ﮵�������ѹ */
#define BAT_Dry         1092    // (1.6f/3* 4096/2)     /* �ɵ�ص�ѹֵ */
#define BAT_Dry_LOW     546    // (1.6f/3* 4096/2)     /* �ɵ�ص�ѹֵ */
#define BAT_Dry_3       2935   //(4.3f/3* 4096/2)      /* 3�ڸɵ�ص�ѹֵ */
#define charge_off       100    //                    ͣ�����
bit ch_full=0;
bit incharging=0;
bit TRY=0;
bit is4v5=0;
bit slowcharge=0;
bit ischarge=0;

unsigned char cnt=0 ;

typedef enum {LED_OFF = 0, LED_ON} LED_Status;
typedef enum {S_PWM_OFF = 0, S_PWM_ON} S_PWM_Status;
typedef enum {BOOST_ON = 0, BOOST_OFF} BOOST_Status;
typedef enum {USB_OFF = 0, USB_ON} USB_Status;

BOOL BAT_Li_Flag = FALSE;       /* ﮵�ر�־ */

//adc--
uint16_t AD_Data = 0;
uint16_t AD_Data2 = 0;
uint16_t total = 0;
//uint16_t olddata=0;
//uint16_t wave_min = 4095;
uint16_t adc_max = 0;
//uint16_t adc_max2 = 0;
//uint16_t wave_peak = 0;
uint16_t	AD_mid=0;
//uint16_t wavelevel1 = 4095;
//uint16_t wavelevel2 = 0;

uint16_t AD_Buffer = 0;


int power=0;
static	uint16_t 		Get_AD();
uint16_t adva[8];

void InitADC(void);
CHARGE2=0;

uint8_t sample=0;
uint8_t flash_times=0;

unsigned char flag = 0;



#define TCON_Value  0x40	       //TR1=Enable;
#define	TCON0_Value	0x00				// FOSC=torate  fosc/128=250k=0.004ms  *12 =0.048ms         
#define	TM0D_Value	0x60				//T1��01xxxxxx:fosc/t1rate;  mode xx10xxxx: 8-bit up counting timer with reload support    
#define	TH1_Value		0xc0				//Auto-reload
#define	TL1_Value		0xc0				//256 - 192(c0) = 64  256-252=4  62.5us 250 * 1/Fclk = 250 * 128/32M = 1MS   //85=0.5ms  250hz=4ms  �ɼ�8�� 5ms=
void Timer0_Init(void)
{
    TMOD = TM0D_Value;          // Mode of Timer 0 
    TH1 = TH1_Value;            // Initial values
    TL1 = TL1_Value;
    TCON0 = TCON0_Value; 	// T0EXT = Fhosc.
    TCON=    TCON_Value ;                       // T0RATE is Fhosc/128 = 250K

    ET1 = 1;                    // Timer 0 isr enable
    EAL = 1;                    // Golbal Interrupt enable
    TR1 = 1;                    // enable Timer 0 Run control

}
void initpwm()	

  {	
     PW1CH = 0x02;      //ѡ��PWCH11��ΪPWMͨ��
	
    PW1M |= 0x10;           	// PW1 rate is Fhosc/64 =500khz  2*12us    PWM���Ƶ��Σ(32/1)/256 = 125KHZ   PWM����Ϊ8uS

  //  PW1M |= 0x08;           	// enable PWM20/21 output
    PW1M |= 0x04;           	// enable PWM10/11 output

//    PW1M |= 0x01;        		// enable PW1 pulse output function (PW1PO)

    PW1YH = 0x01;//0x08;//0x01;
    PW1YL = 0x00;        			// set PW1 cycle control    0800   250hz  01==2khz

    PW1DH = 0x00;//0x07;//0x00;
    PW1DL = 0x00;           	// set PWM duty control   64    3f ռ�ձ�25��   13*16+9=85%

    // the dead band control : B point > A point (Must)
    PW1BH = PW1DH;
    PW1BL = PW1DL;           	// B point dead band control

    PW1A = 0x00;            	// A point dead band control

 //  PW1M |= 0x80;           	// Enable PWM output function (PW1EN)
  // IEN0=1;
	// Enable PWM1 interrupt & clear PWM1F 
	//IEN4 = 0x80;
	// Enable total interrupt 
//	IEN0 |= 0x80;
	
}

void SetPWM(int mode )
{			
    if(mode==1)
    {
        PW1DH = 0x00;
        PW1DL = 0xe6;  
    }  //low  10% --0x39=21.8  0x40=25%  0x4c=30%  0x59=35% 0x66=40%   0x73=45%
    //0x50=80% 0x88=55%  0x9a=60% 0xb3=70%
    if(mode==2)	 
    {
        PW1DH = 0x01;    //high   85%
        PW1DL = 0x00; 
    }  //96   0xe6=90% 9a=60%
    if(mode==3)	 
    {
        PW1DH = 0x00;    //flash off duty
        PW1DL = 0x00; 
    }  //

      if(mode==4)	 
    {
        PW1DH = 0x00;    //
        PW1DL = 0x73; 
    }  //
}

void InitIO(void)
{
    P1 = 0x10; P1UR = 0x18; P1M = 0x18;     /* 0B 0001 1000    output*/
    P0 = 0x00; P0UR = 0x25; P0M = 0x25;     /* 0B 0010 0101    output*/
    //add by lu 
    P1 |= 0x04;   P1UR |= 0x04;    P1M |= 0x04;  //config  for S_PWM; 
}

void Get_AD2(void)
{ 
    int i=0;
    total=0;
    AD_Data2=0;
    for(i=0;i<8;i++)
    {
        total +=adva[i];
    }
    AD_Data2=total>>3;

}
void judge(void)
{
        uint16_t x,max,min,mid;//,total,average
        int i;
        max=0;
        min=4096;   

        for(i=0;i<8;i++){

            if(adva[i]>max)
                {max=adva[i];}
            if(adva[i]<min)
                {min=adva[i];}
        }
        mid=(max+min)>>1; 
        x=0;		
        for(i=0;i<8;i++){
            if (adva[i]>mid)
                x++;
        }  //average
        if(x>4)
            {power=2;}
        else
            {power=1;}   
        flag = 0;
}




void main()
{  
		 
			//Set clk
    CKCON = 0X70;     
    CLKSEL = 0x04;          /* fcpu32M/0-7==/128-64-32-16-8-4-2-1//02=1m;03:fcpu32M=fosc/16=2M; 04:fcpu=fosc/8=4M; 05:fcpu=fosc/4=8M; */  //ԭΪ05
    CLKCMD = 0x69;          /* Apply CLKSELs setting */
    CKCON = 0X10;         /* ���Ӷ�ȡ����洢�������ڡ�000���ޣ�001�� 1 ������ */

    WDTR = 0x5A;          /* clear watchdog if watchdog enable */
    InitIO();

    BOOST = BOOST_OFF;
    RED_LED = LED_OFF;
    GREEN_LED = LED_OFF;
    InitADC();
    Timer0_Init();
    initpwm();
    //PW1M |= 0x00; //�ر�pwm      
    
    //just for test 
  //  PW1DH = 0x00;    //
   // PW1DL = 0x73; 
    //PW1M |= 0x80;     

    AD_Data = 0;
    Get_AD();
//			wave_peak=AD_Data;
//			wave_min=AD_Data;
       
			 
  while(1)  {		
    WDTR = 0x5A;          /* clear watchdog if watchdog enable */  
    if(USB_INPUT == USB_OFF )      
    { ///////////////////* �ж�Ϊ���ڸɵ��*/		
         if(TRY==1)
        {
            power=0;
            TRY=0; //  ����ǲ���Ҫ��0	 	//
            LED_CON = LED_OFF;
            BOOST = BOOST_OFF;			
            ch_full=0;
            ischarge=0;
            Delay1ms(500); 
            AD_Data2=0;
        } 	/* ������				*/
        RED_LED = LED_OFF;
        GREEN_LED = LED_OFF;
        CHARGE_LED = LED_OFF; 

        if (AD_Data>adc_max) adc_max=AD_Data;
        if(flag)               //0.5ms * 8 = 4ms
        {
            judge();          //����AD�м�ֵ�����Ĵ�ʱ  power=2;  else power=1;
        }
        if(adc_max <= BAT_Li_Min)  
        {     
            if (power==2)   
                LED_CON = LED_ON; 		
            SetPWM(power);//���
            PW1M |= 0x80;           	// Enable PWM output function (PW1EN)               
        }     
        else  
        { //////////////////���ǵ��ڸɵ��  ֱ�ӹر�		 
            PW1M=0x00;		// PW1M &= (~0x80); //�ر�pwm            //cut by lu
            BOOST = BOOST_OFF;		 
            if (power==2)	
            {	LED_CON = LED_ON;}
        /* Ĭ������´�LED���� */  
        }

    }// if(USB_INPUT == USB_OFF )   

    if(USB_INPUT == USB_ON )         /* ��磬����4.5v���� USB���� */
    {   // �iint i=0; 
        BOOST = BOOST_OFF;  
        LED_CON = LED_OFF;
        power=3;						
        /* �ر����� */
        Delay1ms(100);
        //		GREEN_LED = LED_ON;  

        if((ischarge == 0)&&(ch_full==0))  //û���ʱ���Ѿ���ʱ���ж�.
        {	 
            /*			Get_AD2();
            if((AD_Data2 < BAT_Li_Min)||(AD_Data2>=BAT_Dry_3))
            //�ж�Ϊ�ɵ�ز��ɳ�����3���ɵ��Ҳ���ɳ��;Ϊ﮵�ز����̵�
            {
            BAT_Li_Flag = FALSE;
            }   */  

            if(TRY==0 && (slowcharge==0))//�򿪼�����Ƿ�﮵��
            {
                RED_LED = LED_ON;
                Delay1ms(500); 
                Get_AD2();
                TRY=1;	
                ch_full=0;
                RED_LED = LED_OFF;
                if((AD_Data2 >= BAT_Li_Min) && (AD_Data2<=(BAT_Li_Pull+ch_err))) 
                    {BAT_Li_Flag = TRUE;}
                //�������Ƿ񱣻���﮵��
                else
                {
                    BAT_Li_Flag = FALSE; 
                }

            }
        }
        if(BAT_Li_Flag == FALSE )
        {
            GREEN_LED = LED_ON;  
            Delay1ms(500);
            GREEN_LED = LED_OFF;
            Delay1ms(500);
        }

        ////////////////////////////////////////////��û��������
        if((ch_full==0) )   //   (BAT_Li_Flag == TRUE)&&
        {
            Get_AD2();
            ischarge=1;                                           //change by lu
            if( (AD_Data2 >= (BAT_Li_Min+ch_err)) && (AD_Data2 <= (BAT_Li_ch_end+ch_err-34)) && (slowcharge==0))
            {  
                RED_LED = LED_ON;
                CHARGE_LED = LED_ON;
                //		GREEN_LED = LED_OFF; 
                                //���
            }
            ///////////////////////////////////////////�л�������										
            if((AD_Data2>(BAT_Li_ch_end+ch_err-0)) && (slowcharge==0))   
            { 
                Delay1ms(100); 
                Get_AD2();
                if((AD_Data2>(BAT_Li_ch_end+ch_err-0)) && (slowcharge==0))  //����                                                                                     
                {
                    WDTR = 0x5A;
                    RED_LED = LED_OFF;
                    CHARGE_LED = LED_OFF;
                    GREEN_LED = LED_OFF;
                    Delay1ms(500);
                    /*��������*/
//                    GREEN_LED = LED_ON;  
                    RED_LED = LED_ON;
                    Delay1ms(500);                                                                
                    CHARGE_LED = LED_OFF;
                    GREEN_LED = LED_OFF; 
                    slowcharge=1;

//                    Delay1ms(100); //������
                     RED_LED = LED_ON;    //change by lu
                     Delay1ms(200); 

                }
            }
            //////////////////////////////////////////////����----����
            if(slowcharge==1) 
            {
                Get_AD2();//������ͣ��ȡadc
                if(AD_Data2>(BAT_Li_ch_end+ch_err+0))            //change by lu
                {  	
									  GREEN_LED = LED_OFF;  
                    RED_LED = LED_ON;
                    Delay1ms(100); 
                    Get_AD2();

                    if(AD_Data2>(BAT_Li_ch_end+ch_err+0))    //����
                    {
                        RED_LED = LED_OFF;
                        CHARGE_LED = LED_OFF;        //����
                        GREEN_LED = LED_ON;
                        ch_full=1;
                        slowcharge=0;                                                     
                        ischarge=0;
                    //  �˳����ʱ�ĵ�ѹ����
                    }  
                }
            }// slow charge
        } //usb���
        /////////////////////////////////////////// �����			      

    }//usb�е�����
  } //while
}//main
void InitADC(void)
{  
    P1M   &= ~(1<<0);   /* set P10 Input(ADC_INPUT) */
    P1CON |= 0x01;      /* �ر�P10��ʩ���ش��� */
    
    ADM = 0x80;         /* ʹ��ADC */
    ADM |= 0x00;        /* ѡ��P10ͨ��0 */
    ADR = 0x40;
    ADR |= 0x00;        /* Fosc/16 */
  //  VREFH = 0x00;
    VREFH = 0x00;       /* 4V --->2v*/
    
   // IEN2 |= 0x02;       /* ADC �ж� */
}

uint16_t Get_AD()
{ 	

    ADM |= 0x00;        /* ѡ��P10ͨ��0 */ 
    AD_Data = 0;   
    ADM |= 0x40;													//Start ADC
    while((ADM & 0x20) != 0X20);					//EOC
    ADM &= 0xdf;

    AD_Buffer = ADB;
    AD_Buffer = (AD_Buffer << 4) + (ADR & 0X0F);
    AD_Data = AD_Buffer;  
        return	AD_Data;
}

unsigned char k=0;y=0;

void Timer0_ISR(void) interrupt ISRTimer1   // Vector @  0x0B
{  
    WDTR = 0x5A;          /* clear watchdog if watchdog enable */  
    Get_AD();                //    ÿ0.5ms�ɼ�һ��ad
    adva[k]=AD_Data;
    if(k<8) 
    {
      k++;
     
    }         
    else
    { 
        k=0; 
        flag = 1; 
        if(y<4)
            {y++;}
        else
            {y=0;}
    } 
   // S_PWM = ~S_PWM;
   
    //   P00= ~ P00;  
}


