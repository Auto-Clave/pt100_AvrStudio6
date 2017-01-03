/*
 * GccApplication1.c
 *
 * Created: 12/01/2016 01:17:00 ب.ظ
 *  Author: Hamed
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#define F_CPU 8000000
#include <util/delay.h>

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
//---------------------------------------------------------//
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
DDRC = 0xFF;

Lcd4_Init();
Lcd4_Set_Cursor(1,1);
Lcd4_Write_String("Elasa.ir Test");	
}	

unsigned char inttostr(unsigned int mode)
{
char *str;  
itoa(mode, str1, 10);
//itoa();	
return(str);

}

/*
//This interrupt service routine calculates the pulse width of a square wave on the ICP pin 
//and displays that in a certain form on PORT D

ISR(TIMER1_CAPT_vect)
{
	Lcd4_Init();
	Lcd4_Set_Cursor(1,1);
	Lcd4_Write_String("Pulse width measuring");  
	_delay_ms(3000);
	
	// If ICP pin is set, there was a rising edge else if its low there must have been a falling edge /
	if (bit_is_set(PINB,0))
	{
		rising_edge = ICR1;
		TCCR1B &= ~(1<<ICES1); //Capture now on falling edge
		overflow_counter = 0;
		Lcd4_Write_String("falling edge");_delay_ms(1);
	}
	else
	{
		falling_edge = ICR1;
		TCCR1B |= (1<<ICES1);//Capture now on rising edge
		pulse_width =falling_edge - rising_edge + TOP*overflow_counter;
		
		//Test to see if correct pulse width being calculated
		//if pulse width is greater than 40us than PD4 goes high otherwise PD5 IS HIGH
		if (pulse_width>40)
		{			
			PORTD |= (1<<PD4);
			PORTD &= ~(1<<PD5);
		}
		else
		{
			PORTD |= (1<<PD5);
			PORTD &= ~(1<<PD4);
		}
	}
	
	inttostr(pulse_width);
	Lcd4_Set_Cursor(1,1);	
	Lcd4_Write_String("Pulse width measuring:");
	Lcd4_Set_Cursor(2,1);	
	Lcd4_Write_String(txt);
	_delay_ms(3000);
	 
}

*/
ISR(TIMER1_CAPT_vect)
{
	Lcd4_Init();
	Lcd4_Set_Cursor(1,1);
	Lcd4_Write_String("Pulse width measuring");  
	_delay_ms(1);
	// If ICP pin is set, there was a rising edge else if its low there must have been a falling edge /
	if (bit_is_set(PIND,4))
		{
		rising_edge = ICR1;
		TCCR1B &= ~(1<<ICES1); //Capture now on falling edge
		overflow_counter = 0;
		Lcd4_Set_Cursor(1,1);	
	    Lcd4_Write_String("falling edge");_delay_ms(1);
		}
	else
		{
		falling_edge = ICR1;
		TCCR1B |= (1<<ICES1);//Capture now on rising edge
		pulse_width =falling_edge - rising_edge + TOP*overflow_counter;
		Lcd4_Set_Cursor(1,1);	
	    Lcd4_Write_String("rising edge");_delay_ms(1);
	
		//Test to see if correct pulse width being calculated
		//if pulse width is greater than 35 than PD2 goes high otherwise PD3 IS HIGH
		if (pulse_width>35)
			{
			PORTA |= (1<<PA2);
			PORTA &= ~(1<<PA3);
			}
		else
			{
			PORTA |= (1<<PA3);
			PORTA &= ~(1<<PA2);
			}
		}
		inttostr(pulse_width);
	Lcd4_Set_Cursor(1,1);	
	Lcd4_Write_String("Pulse width measuring:");
	Lcd4_Set_Cursor(2,1);	
	Lcd4_Write_String(txt);
	_delay_ms(3);
}

ISR(TIMER1_OVF_vect)
{

	overflow_counter++; //increment counter when overflow occurs
	
}


int main(void)
{	
	
	
	DDRD  = 0x00;// ICP pin as input which is pin 0 on port B/
	//DDRD = 5;// ICP pin as input which is pin 0 on port B/
	DDRA = 0XFF; // ALL PINS on port A are set as output.
	
	sei();//enable global interrupt
	
	//setting up Timer control register 1
	
	//TOP VALUE IS 0XFFFF; (NORMAL MODE)
	TCCR1A = 0;
	TCCR1B = 0;
	
	
	//SETS PRESCALER ON 1
	TCCR1B |= (1<<CS10);
	TCCR1B &= ~(1<<CS11) & ~(1<<CS12);
	
	// Enable Input noise canceller and capture time on rising edge
	TCCR1B |=  (1<<ICES1) | (1<<ICNC1) ; 
	
	TIMSK = 0;
	TIMSK |= (1<<TICIE1); //Enable Input Capture Interrupt
	TIMSK |= (1<<TOIE1); //Enable timer overflow interrupt
	
	
	while(1)
	{
       //LCD_0();
	
	}

}