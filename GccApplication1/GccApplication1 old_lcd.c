

#include <avr/io.h>
#include <avr/interrupt.h>
 #include <stdlib.h>
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
int pulse_width2;
char str3[16];
#define ICP PIND4
#define LCD_port DDRC
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


// ADC Configuration Function
//www.noise.blog.ir/post/%DA%A9%D8%A7%D8%B1%DA%AF%D8%A7%D9%87-%D8%B9%D9%85%D9%84%DB%8C-%D8%B4%D9%85%D8%A7%D8%B1%D9%87-2-%DA%A9%D8%A7%D8%B1-%D8%A8%D8%A7-%D9%88%D8%A7%D8%AD%D8%AF-LM35-ADC-%D8%B3%D9%88%D9%86-%D8%B3%DA%AF%D9%85%D9%86%D8%AA-18
void adc_configuration(){
    ADMUX |= (1<<REFS1)|(1<<REFS0);
	ADMUX=0x00; // ADC input channel set to PF0
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADEN);
}

// ADC Temprature Reading Function
unsigned int temp(){
    ADCSRA |=(1<<ADSC);
    while ((ADCSRA & (1<<ADIF))==0);
    ADCSRA |=(1<<ADIF);
    return ADCW;
}


void ADC_Init() 
{
 
  uint16_t ergebnis;
    ADMUX |= (1<<REFS1) | (1<<REFS0);              // interne Referenzspannung nutzen
	ADMUX=0x00; // ADC input channel set to PF0
    ADCSRA |= (1<ADPS1) | (1<<ADPS2) | (1<<ADPS0);       // Frequenzvorteiler 
    ADCSRA = (1<<ADEN);                         // ADC aktivieren
 
     //----------Dummylauf---------------------
  
  //  ADMUX |= (1<<ADLAR);////////////neu/////
    ADCSRA |= (1<<ADSC);                          // eine ADC-Wandlung 
    while (ADCSRA & (1<<ADSC) ) {}                // auf Abschluss der Konvertierung warten
    ergebnis = ADCL;////////////////////////neu/
  ergebnis |= (ADCH<<8);
  
}


/*------------ADC Einzelmessung----------------------- */
float ADC_Read0()
{
	//ADC_Init() ;
  uint16_t ergebnis ;
//uint16_t ergebnis2;
	
  ADCSRA |= (1<<ADSC);                     // eine Wandlung "single modus"
    while (ADCSRA & (1<<ADSC) ) {}           // auf Abschluss der Konvertierung warten
    ergebnis = ADCL;/////////////neu 
  ergebnis |= (ADCH<<8);////////////////neu; 

  return ergebnis;  ////////////neu                    // ADC auslesen und zurückgeben
}

float ADC_Read()
{
		char templ,temph;
		uint8_t adresult ;
       ADCSRA|=(1<<ADSC); //¿ªÊ¼×ª»»   
	   
       while(!(ADCSRA & (1 << ADIF)));    /*µÈ´ý*/    
       //ADCSRA&=~(1<<ADEN);   
       ADCSRA|=(1<<ADIF);   
       templ=ADCL;   
       temph=ADCH; //ÏÈµÍÎ» ºó¸ßÎ»   
       adresult = ((temph)<<8) + templ; //10 bit 
	   
	   adresult= (ADCH*256+ADCL)*2.56/1024;
	   return adresult;
}
//-------------Umrechnen in Temperatur-----------------------------------------------

//MinT.  0°C   = 100 Ohm 
//MaxT.219°C  = 182,5 Ohm
// Bei 1°C unterschied = 82,5/219 = 0,3783105 Ohm


//--------------Berechnung pt100 Wiederstand----------------
 int Temperatur(uint16_t t) 
{
  int sensor  = t;                // Wert zwischenspeichern
  int U_PT100   = sensor * 2.56 /1023.0;      // Spannung vom PT100 errechnen
  int U250     = 4.79 - U_PT100 ;          // I = U / R
  int I_PT100   = U250 / 2300;       
  int R_PT100   = U_PT100 / I_PT100;        // Wiederstand vom PT100  
   R_PT100   = R_PT100 - 1.0;            // Leitungswiderstand

//--------------Berechnung Temperatur-----------------------
  int grad ;
  if (R_PT100 < 182.5)
  {
    int tmp =  182.5 - R_PT100;
    tmp = tmp / 0.3783105; 
    grad = 219.0 - tmp;
  }  
  int int_grad = (int) grad;

  return sensor;

}


uint16_t adc_read_new(uint8_t ch)
{
  // select the corresponding channel 0~7
  // ANDing with ’7′ will always keep the value
  // of ‘ch’ between 0 and 7
  ch &= 0b00000111;  // AND operation with 7
  ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
   ADMUX |= (1<<REFS1)|(1<<REFS0);
  //ADMUX = (1<<REFS0); 
  // start single convertion
  // write ’1′ to ADSC
  ADCSRA |= (1<<ADSC);
 
  // wait for conversion to complete
  // ADSC becomes ’0′ again
  // till then, run loop continuously
  while(ADCSRA & (1<<ADSC));
 
  return (ADCW);
}
unsigned int i,T,T2;
float T3;
int tt;

uint16_t adc_value;            //Variable used to store the value read from the ADC
char buffer[5];                //Output of the itoa function


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

Lcd4_Init();
pulse_width=pulse_width2=00;
Lcd4_Set_Cursor(1,1);	
Lcd4_Write_String("Pulse width measuring:");
Lcd4_Set_Cursor(2,1);	
inttostr(pulse_width);
//itoa(pulse_width2, str3, 10);
Lcd4_Write_String(str3);
_delay_ms(300);
    
   
              
	while(1)
    {
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
		 
		 
		 tt = adc_read_new(0);
		 //tt = temp();
         T2=Temperatur(tt);
		 Lcd4_Clear();
		 Lcd4_Set_Cursor(1,1);	
         Lcd4_Write_String("Temp3:");
		 inttostr(T2);
		 Lcd4_Set_Cursor(2,1);	
         Lcd4_Write_String(str3);
         _delay_ms(300); 
		  
         
		 tt = adc_read_new(0);
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
		 
		 itoa(volts, buffer, 10);
		  Lcd4_Set_Cursor(1,1);	
         Lcd4_Write_String("Temp8:");
		 Lcd4_Set_Cursor(2,1);	
         Lcd4_Write_String(buffer);
         _delay_ms(300);
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

 //--------------Berechnung pt100 Wiederstand----------------
 int Temperatur2(uint16_t t) 
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

  return I_PT100;

}
