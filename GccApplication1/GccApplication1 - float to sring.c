

#include <avr/io.h>
#include <avr/interrupt.h>
 #include <stdlib.h>
//#define F_CPU 16000000UL
#define F_CPU 8000000
#include <util/delay.h>
#define ADC_VREF_TYPE 0x40
//#include "adc_new.h"
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

DDRC = 0xFF;


Lcd4_Init();
Lcd4_Set_Cursor(1,1);
Lcd4_Write_String("Elasa.ir Test");	
}	

//http://www.geeksforgeeks.org/convert-floating-point-number-string/
// Converts a floating point number to string.
void ftoa(float n, char *res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;
 
    // Extract floating part
    float fpart = n - (float)ipart;
 
    // convert integer part to string
    int i = itoa(ipart, res, 0);
 
    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot
 
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
 
        itoa((int)fpart, res + i + 1, afterpoint);
    }
}


 unsigned char *Float2Ascii (float x, unsigned char *str,char precision)
{
 /* converts a floating point number to an ascii string */
 /* x is stored into str, which should be at least 30 chars long */
 unsigned char *adpt;
 int ie, i, k, ndig;
 double y;
 adpt=str;
 ndig = ( precision<=0) ? 7 : (precision > 22 ? 23 : precision+1);
 ie = 0;
 /* if x negative, write minus and reverse */
 if ( x < 0)
 {
   *str++ = '-';
   x = -x;
 }
 /* put x in range 1 <= x < 10 */
 if (x > 0.0) while (x < 1.0)
 {
   x *= 10.0;                // a la place de =*
   ie--;
 }
 while (x >= 10.0)
 {
   x = x/10.0;
   ie++;
 }
 // in f format, number of digits is related to size
 ndig += ie;                                // a la place de =+
 //round. x is between 1 and 10 and ndig will be printed to
 // right of decimal point so rounding is ...
 for (y = i = 1; i < ndig; i++)
 y = y/10.;
 x += y/2.;
 if (x >= 10.0) {x = 1.0; ie++;}
 if (ie<0)
 {
   *str++ = '0'; *str++ = '.';
   if (ndig < 0) ie = ie-ndig;
   for (i = -1; i > ie; i--)  *str++ = '0';
 }
 for (i=0; i < ndig; i++)
 {
   k = x;
   *str++ = k + '0';
   if (i ==  ie ) *str++ = '.';
   x -= (y=k);
   x *= 10.0;
  }
 *str = '\0';
return (adpt);
}

int main(void) {
DDRD = (0<<PD4);     // put PortB bit 5 as input
//PORTD = 0<<PD4;		// Enable PE4 pull-up resistor
DDRC = 0xFF;
//enable overflow and input capture interrupts

TIMSK=0x24;

/*Noise canceller, without prescaler, rising edge*/

TCCR1B=0xC1;



// ADC initialization
// ADC Clock frequency: 1000.000 kHz
// ADC Voltage Reference: Int., cap. on AREF
ADMUX=ADC_VREF_TYPE & 0xff;
ADCSRA=0x83;_delay_ms(30);


Lcd4_Init();

Lcd4_Set_Cursor(1,1);	
Lcd4_Write_String("Pulse width measuring:");
//Lcd4_Set_Cursor(2,1);	

//itoa(pulse_width2, str3, 10);

_delay_ms(3);
    
   
              
	while(1)
    {
		//http://www.edaboard.com/thread63677.html
		char buffer22[24];
		float x = 1.5;
		
		sprintf(buffer22, "Flo %f", x);
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,1);	
        Lcd4_Write_String("Temp_eda:");
		Lcd4_Set_Cursor(2,1);
		Lcd4_Write_String(buffer22);
		_delay_ms(300);
		
		
		char res[20];
		float n = 233.007;
		ftoa(n, res, 4);
		
		
		//ftoa3(myFloatStr, myFloat, 10);
		//Lcd4_Write_String(myFloatStr);
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,1);	
        Lcd4_Write_String("Temp_PT100:");
		Lcd4_Set_Cursor(2,1);
		Lcd4_Write_String(res);
		_delay_ms(300);
		
		
		float F1 = 233.007;
		char CRam1[26];

		Float2Ascii (F1, CRam1,3);  // my function .. keep same width
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,1);	
        Lcd4_Write_String("Temp_new:");
		Lcd4_Set_Cursor(2,1);
		Lcd4_Write_String(CRam1);
		_delay_ms(300);
	}
}
	
		