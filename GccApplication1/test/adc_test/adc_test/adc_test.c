//**************************************************************//
//Microcontroller			:ATmega32
//System Clock				:1MHz 
//Project					:Analog to Digital Converter of ATmega32 with LCD Display
//Software					:AVR Studio 4 
//LCD Data Interfacing		:8-Bit  
//Author					:Arun Kumar Garg 
//							:ABLab Solutions
//							:www.ablab.in
//							:info@ablab.in
//Date						:1st January 2012
//**************************************************************//

#include <avr/io.h>
#include <avr/interrupt.h>
 #include <stdlib.h>
#define F_CPU 1000000
#include <util/delay.h>
/*Includes io.h header file where all the Input/Output Registers and its Bits are defined for all AVR microcontrollers*/

#define	F_CPU	1000000
/*Defines a macro for the delay.h header file. F_CPU is the microcontroller frequency value for the delay.h header file. Default value of F_CPU in delay.h header file is 1000000(1MHz)*/

#include<util/delay.h>
/*Includes delay.h header file which defines two functions, _delay_ms (millisecond delay) and _delay_us (microsecond delay)*/

#define		LCD_DATA_PORT		PORTB
/*Defines a macro for the lcd.h header File. LCD_DATA_PORT is the microcontroller PORT Register to which the data pins of the LCD are connected. Default PORT Register for data pins in lcd.h header file is PORTA*/

#define 	LCD_CONT_PORT		PORTD
/*Defines a macro for the lcd.h header File. LCD_CONT_PORT is the microcontroller PORT Register to which the control pins of the LCD are connected. Default PORT Register for control pins in lcd.h header file is PORTB*/

#define 	LCD_RS 		PD0
/*Defines a macro for the lcd.h header file. LCD_RS is the microcontroller Port pin to which the RS pin of the LCD is connected. Default Port pin for RS pin in lcd.h header file is PB0*/

#define 	LCD_RW 		PD1
/*Defines a macro for the lcd.h header file. LCD_RW is the microcontroller Port pin to which the RW pin of the LCD is connected. Default Port pin for RW pin in lcd.h header file is PB1*/

#define 	LCD_EN 		PD2
/*Defines a macro for the lcd.h header file. LCD_EN is the microcontroller Port pin to which the EN pin of the LCD is connected. Default Port pin for EN pin in lcd.h header file is PB2*/

#include "lcd.h"
/*Includes lcd.h header file which defines different functions for all Alphanumeric LCD(8-Bit Interfacing Method). LCD header file version is 1.1*/

#include "adc.h"
/*Includes adc.h header file which defines different functions for Analog to Digital Converter. ADC header file version is 1.1*/

int main(void)
{
	DDRB=0xff;DDRD=0xff;
	/*All the 8 pins of PortB are declared output (data pins of LCD are connected)*/
	//DDRA=0xff;
	DDRC=0x07;
	/*PD0, PD1 and PD2 pins of PortD are declared output (control pins of LCD are connected)*/

	int adc_output;
	/*Variable declarations*/

	lcd_init();
	/*LCD initialization*/

	adc_init();
	/*ADC initialization*/
	
	lcd_string_write("ABLab Solutions");
	/*String display in 1st row of LCD*/

	lcd_command_write(0xc0);
	/*Cursor moves to 2nd row 1st column of LCD*/

	lcd_string_write("www.ablab.in");
	/*String display in 2nd row of LCD*/
	
	_delay_ms(10);
	
	/*Display stays for 2 second*/

	/*Start of infinite loop*/
	while(1)
	{
		lcd_command_write(0x01);
		/*Clear screen*/

		lcd_string_write("ADC Value1:");
		/*String display in 1st row of LCD*/

		lcd_command_write(0xc0);
		/*Cursor moves to 2nd row 1st column of LCD*/

		adc_output=read_adc_channel(0);
		/*Reading adc output of channel 0*/ 
		int Temp;
		Temp=Temperatur_end(adc_output);
		lcd_number_write(Temp,10);
		/*adc output value is displayed in 2nd row of LCD*/ 

		_delay_ms(300);
		
		
		lcd_init();
	/*LCD initialization*/
	//	sprintf(temp3, "Voltage = %f", ((float)ADCW *2.56)/1023.0);
		adc_output=read_adc_channel(0);
		/*Reading adc output of channel 0*/
	     lcd_string_write("ADC Value2:");
		 lcd_command_write(0xc0);
		/*Cursor moves to 2nd row 1st column of LCD*/
		int t4=(((int)adc_output *2.56)/1023.0);
		float t5=((adc_output *2.56)/10.0);
		 lcd_number_write(t4,10);
		/*adc output value is displayed in 2nd row of LCD*/ 

		_delay_ms(300);
		/*Display stays for 500ms*/
		
		lcd_init();
	/*LCD initialization*/
	//	sprintf(temp3, "Voltage = %f", ((float)ADCW *2.56)/1023.0);
		
		adc_output=read_adc_channel(0);
		/*Reading adc output of channel 0*/
	     lcd_string_write("ADC Value3:");
		 lcd_command_write(0xc0);
		/*Cursor moves to 2nd row 1st column of LCD*/
		float t6=(((float)ADCW *2.56)/1023.0);
		 lcd_number_write(adc_output,10);
		/*adc output value is displayed in 2nd row of LCD*/ 

		_delay_ms(300);
		/*Display stays for 500ms*/
		
		
		float my_custom_float = 2.54; int Lux = 2.54;
		char buffer[200],buffer2[200];
		char buf[8];    // declare array
		sprintf(buffer,"%f",my_custom_float);
		sprintf(buffer2,"L1:%d%d%d%d",Lux/1000,(Lux%1000)/100,(Lux%1000%100)/10,Lux%1000%100%10);
		
		sprintf (buf, "float=%f", 1.156);
		lcd_init();
		/*LCD initialization*/
		//	sprintf(temp3, "Voltage = %f", ((float)ADCW *2.56)/1023.0);
		lcd_string_write("ADC float:");
		 lcd_command_write(0xc0);
		 lcd_string_write(buf);
		/*adc output value is displayed in 2nd row of LCD*/ 

		_delay_ms(300);
	}
}

 //--------------Berechnung pt100 Wiederstand----------------
 int Temperatur3(int t) 
{
  float sensor  = t;                // Wert zwischenspeichern
  float U_PT100   = sensor * 2.56 /1023.0;      // Spannung vom PT100 errechnen
  float U250     = 5 - U_PT100 ;          // I = U / R
  float I_PT100   = U250 / 2300.0;       
  float R_PT100   = U_PT100 / I_PT100;        // Wiederstand vom PT100  
   R_PT100   = R_PT100 - 1.0;            // Leitungswiderstand

//--------------Berechnung Temperatur-----------------------
  float grad ;
  if (R_PT100 < 182.5)
  {
    float tmp =  182.5 - R_PT100;
    tmp = tmp / 0.3783105; 
    grad = 219.0 - tmp;
  }  
  int int_grad = (int) grad;

  return U_PT100;

}

int Temperatur_end (int adc_data2) 		{
         char temp3[17],buffer[10];int volts3;
		 volts3=Temperatur3(adc_data2);
		 sprintf(temp3, "Voltage = %f", ((float)ADCW *2.56)/1023.0);
		 
		 sprintf(temp3, "Voltage = %i", volts3);
		  itoa(adc_data2, buffer, 10);
return volts3;
}
/*End of Program*/
