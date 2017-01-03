

#include <avr/io.h>
#include <avr/interrupt.h>
 #include <stdlib.h>
 //http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega128&LOW=FF&HIGH=CD&EXTENDED=FF&LOCKBIT=FF
#define F_CPU 16000000UL //Fuse bit is LOW=FF&HIGH=CD
//#define F_CPU 8000000
#include <util/delay.h>

#include "adc_new.h"
#define D4 eS_PORTC4
#define D5 eS_PORTC5
#define D6 eS_PORTC6
#define D7 eS_PORTC7
#define RS eS_PORTC2
#define EN eS_PORTC3
#include "lcd.h" //Can be download from the bottom of this article
//	Constants and variables
//*****************************************************************************
// Input: Square wave on ICP PIN
// This program determines the pulse width of a square wave and if the pulse width is greater than 40 us
//than PD4 goes higher ,if its smaller than PD4 is low.



unsigned int overflow_counter, rising_edge, falling_edge, pulse_width;
unsigned int TOP = 65535;
unsigned char *txt,str1;
int pulse_width2;
char str3[16];

#define ICP PIND4
#define LCD_port DDRC
//---------------------------------------------------------//

/*
#define ICP PIND4
#define LCD_port DDRC
#define LCD_IO_MODE 0  // in  4-bit IO port mode
//---------------------------------------------------------//
//------------------<pick of lcd_lib.h>
#include "lcd_lib.h"
#define LCD_RS 2
#define LCD_RW	1 	//define MCU pin connected to LCD R/W
#define LCD_E 3
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define LDP PORTC
#define LCP PORTC
#define LDDR DDRC
#define LCDR DDRC

//------------------<E>
//---------------------------------------------------------//
//Strings stored in AVR Flash memory
const uint8_t LCDwelcomeln1[] PROGMEM="AVR LCD DEMO\0";
const uint8_t LCDprogress[] PROGMEM="Loading...\0";
const uint8_t LCDanimation[] PROGMEM=" LCD animation \0";

// additional custom LCD characters
const uint8_t backslash[8] PROGMEM= 
{
0b00000000,//back slash
0b00010000,
0b00001000,
0b00000100,
0b00000010,
0b00000001,
0b00000000,
0b00000000
};
//delay 1s
void delay1s(void)
{
	uint8_t i;
	for(i=0;i<100;i++)
	{
		_delay_ms(10);
	}
}
//demonstration of progress bar
void progress(void)
{
	LCDclr();
	CopyStringtoLCD(LCDwelcomeln1, 3, 0);	
	delay1s();
	LCDclr();
	CopyStringtoLCD(LCDprogress, 3, 0);
	for(uint8_t i=0;i<255;i++)
		{
			_delay_ms(10);
			LCDGotoXY(0, 1);
			LCDprogressBar(i, 255, 16);
		}
}
//demonstration of animation
void demoanimation(void)
{
	LCDclr();
	LCDdefinechar(backslash,0);
	CopyStringtoLCD(LCDanimation, 0, 0);
	for(uint8_t i=0;i<3;i++)
		{
			LCDGotoXY(8, 1);
			LCDsendChar(0);
			delay1s();
			LCDGotoXY(8, 1);
			LCDsendChar('-');
			delay1s();
			LCDGotoXY(8, 1);
			LCDsendChar('/');
			delay1s();
			LCDGotoXY(8, 1);
			LCDsendChar('|');
			delay1s();
			LCDGotoXY(8, 1);
			LCDsendChar(8);//backslash
			delay1s();
			LCDGotoXY(8, 1);
			LCDsendChar('-');
			delay1s();
			LCDGotoXY(8, 1);
			LCDsendChar('/');
			delay1s();
			LCDGotoXY(8, 1);
			LCDsendChar('|');
			delay1s();
		}	
}

int main(void)
{	DDRF = 0XFF;     // put PortB bit 5 as input
	LCDinit();//init LCD bit, dual line, cursor right
	LCDclr();//clears LCD
	while(1)//loop demos
	{
		progress();
		delay1s();
		demoanimation();
	}
	return 0;
}

*/
//---------------------------------------------------------//
void LCD_0(void)
{
	/*
' Lcd module connections
dim LCD_RS as sbit at PORTc2_bit
dim LCD_EN as sbit at PORTc3_bit
dim LCD_D4 as sbit at PORTc4_bit
dim LCD_D5 as sbit at PORTc5_bit
dim LCD_D6 as sbit at PORTc6_bit
dim LCD_D7 as sbit at PORTc7_bit

dim LCD_RS_Direction as sbit at DDc2_bit
dim LCD_EN_Direction as sbit at DDc3_bit
dim LCD_D4_Direction as sbit at DDc4_bit
dim LCD_D5_Direction as sbit at DDc5_bit
dim LCD_D6_Direction as sbit at DDc6_bit
dim LCD_D7_Direction as sbit at DDc7_bit

*/

//DDRD = 0xFF;
LCD_port = 0xFF;

Lcd4_Init();
Lcd4_Set_Cursor(1,1);
Lcd4_Write_String("Elasa.ir Test");	
}	

unsigned char inttostr0( uint32_t mode)
{
char *str;  
itoa(mode, str1, 10);
//itoa();	
return(str1);

}
char inttostr( int mode)
{
char *str;  
itoa(mode, str3, 10);
//itoa();	
return(str3);

}
#define ICP PIND4
#define LCD_port DDRC
#define Puls_Port  DDRD
//define ovrflow counter

uint16_t ov_counter;

//define times for start and end of signal

uint16_t rising, falling;

//define overall counts

uint32_t counts;


//overflow counter interrupts service routine

ISR(TIMER1_OVF_vect){

  ov_counter++;

}

//Timer1 capture interrupt service subroutine

ISR(TIMER1_CAPT_vect){

//This subroutine checks was it start of pulse (rising edge)

//or was it end (fallingedge)and performs required operations
    Lcd4_Init();
	Lcd4_Set_Cursor(1,1);
	Lcd4_Write_String("Pulse width measuring");  
	_delay_ms(1);
    if (ICP) //if high level

            {

            //save start time

            rising=ICR1;

            //set to trigger on falling edge

            TCCR1B=TCCR1B&0xBF;

            //reset overflow counter

            ov_counter=0;

   }

else

            {

            //save falling time

            falling=ICR1;

            //rising edge triggers next

            TCCR1B=TCCR1B|0x40;

            counts=(uint32_t)falling-(uint32_t)rising+(uint32_t)ov_counter;
			pulse_width=(int)counts;

            //you can convert coutns to seconds and send to LCD

            }
			inttostr(pulse_width);
	        Lcd4_Set_Cursor(1,1);	
			Lcd4_Write_String("Pulse width measuring:");
			Lcd4_Set_Cursor(2,1);	
			Lcd4_Write_String(str3);
			_delay_ms(3);

}


int main(void) {
DDRD = (0<<PD4);     // put PortB bit 5 as input
//PORTD = 0<<PD4;		// Enable PE4 pull-up resistor
Puls_Port=0X00;
//enable overflow and input capture interrupts

TIMSK=0x24;

/*Noise canceller, without prescaler, rising edge*/

TCCR1B=0xC1;

//sei();
LCD_port = 0xFF;

// ADC initialization
// ADC Clock frequency: 1000.000 kHz
// ADC Voltage Reference: Int., cap. on AREF
ADMUX=ADC_VREF_TYPE & 0xff;
ADCSRA=0x83;_delay_ms(30);


Lcd4_Init();
pulse_width=pulse_width2=00;
Lcd4_Set_Cursor(1,1);	
Lcd4_Write_String("Pulse width measuring:");
Lcd4_Set_Cursor(2,1);	
inttostr(pulse_width);
//itoa(pulse_width2, str3, 10);
Lcd4_Write_String(str3);
_delay_ms(3);
    
   
              
	while(1)
    {
		
		
		 
		char CRam1[26];
		int adc_data3;
		float temp_n3;float volts33,volts333;
		//adc_data3=read_adc_codevision2(0);
		adc_data3=adc_read_new(0);
		temp_n3=(adc_data3*2.56)/1023.0;
		volts33=((float)adc_data3*2.56f)/(1024.0f); 
		volts333=Temperatur3(adc_data3);
		
		Float2Ascii (volts333, CRam1,7);  // my function .. keep same width
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,1);	
        Lcd4_Write_String("Temp_new:");
		Lcd4_Set_Cursor(2,1);
		Lcd4_Write_String(CRam1);
		_delay_ms(300);
		
		/*
		
		Temperatur_codevision();
		 _delay_ms(300);
		 
		 
         Lcd4_Clear();
         Lcd4_Set_Cursor(1,1);	
         Lcd4_Write_String("Temp:");
         // Temprature Calculating
		 
		 
		 adc_configuration();
         T = temp();
         //T=(T*256/1023);
		 inttostr(T);
		 Lcd4_Set_Cursor(2,1);	
         Lcd4_Write_String(str3);
         _delay_ms(300);
		 
		 //ADC_Init(); 
		 tt = ADC_Read();
		 //tt = temp();
         T2=Temperatur(tt);
          
		  
		 Lcd4_Clear();
		 Lcd4_Set_Cursor(1,1);	
         Lcd4_Write_String("Temp2:");
		 inttostr(T2);
		 Lcd4_Set_Cursor(2,1);	
         Lcd4_Write_String(str3);
         _delay_ms(300);
		 
		 
		 tt = adc_read_new(1);
		 //tt = temp();
         T2=Temperatur(tt);
		 Lcd4_Clear();
		 Lcd4_Set_Cursor(1,1);	
         Lcd4_Write_String("Temp3:");
		 inttostr(T2);
		 Lcd4_Set_Cursor(2,1);	
         Lcd4_Write_String(str3);
         _delay_ms(300); 
		  
         
		 tt = adc_read_new(1);
		 //tt = temp();
         //T2=Temperatur2(tt);
		 
		 adc_value = adc_read_new(0);        //Read one ADC channel 
		 itoa(adc_value, buffer, 10);        //Convert the read value to an ascii string   
		 
		 
		 Lcd4_Clear();
		 Lcd4_Set_Cursor(1,1);	
         Lcd4_Write_String("Temp4:");
		 Lcd4_Set_Cursor(2,1);	
         Lcd4_Write_String(buffer);
		 _delay_ms(300);
		 
		 Lcd4_Clear();
		 T3=0.13;
		 
		 
 
		 inttostr(T3);
		 //itoa(T3, str3, 10);        //Convert the read value to an ascii string 
		 Lcd4_Set_Cursor(1,1);	
         Lcd4_Write_String("Temp6:");
		 Lcd4_Set_Cursor(2,1);	
         Lcd4_Write_String(str3);
         _delay_ms(300); 
		 
		 
		 // ADC initialization
		// ADC Clock frequency: 1000.000 kHz
		// ADC Voltage Reference: Int., cap. on AREF
		ADMUX=ADC_VREF_TYPE & 0xff;
		ADCSRA=0x83;
		
		_delay_ms(30);
		 uint16_t adc_data;
		 float temp_n,volts;
		adc_data=ADCW;
		temp_n=(adc_data*2.56)/1023.0;
		volts=((float)adc_data*2.56f)/(1024.0f); 
		temp_n=temp_n;
		itoa(temp_n, buffer, 10);
		Lcd4_Clear();
		 Lcd4_Set_Cursor(1,1);	
         Lcd4_Write_String("Temp7:");
		 Lcd4_Set_Cursor(2,1);	
         Lcd4_Write_String(buffer);
         _delay_ms(300); 
		 
		 
		 // ADC initialization
		// ADC Clock frequency: 1000.000 kHz
		// ADC Voltage Reference: Int., cap. on AREF
		ADMUX=ADC_VREF_TYPE & 0xff;
		ADCSRA=0x83;_delay_ms(30);
		
		 int adc_data2;char temp3_0[17];
		 float temp_n2,volts2;
		adc_data2=read_adc_codevision2(0);
		temp_n2=(adc_data2*2.56)/1023.0;
		volts2=((float)adc_data2*2.56f)/(1024.0f); 
		 
		 itoa(volts2, buffer, 10);
		 sprintf(temp3_0, "Voltage = %i", adc_data2);
		  Lcd4_Set_Cursor(1,1);	
         Lcd4_Write_String("Temp8:");
		 Lcd4_Set_Cursor(2,1);	
         Lcd4_Write_String(temp3_0);
         _delay_ms(300);
		 
		 char temp3[17];int volts3;
		 volts3=Temperatur3(adc_data2);
		 sprintf(temp3, "Voltage = %f", ((float)ADCW *2.56)/1023.0);
		 
		 sprintf(temp3, "Voltage = %i", volts3);
		  itoa(adc_data2, buffer, 10);
		  Lcd4_Set_Cursor(1,1);	
         Lcd4_Write_String("Temp9:");
		 Lcd4_Set_Cursor(2,1);	
         Lcd4_Write_String(temp3);
         _delay_ms(300);
		 
		 
		*/
		 
		 
    }

/*

	for (;;) {
		
		T = temp();
        T = (((T*2.56)*100)/1023);



		//LCD_0();

loop forever timer does the job
			

    }
	*/

}

