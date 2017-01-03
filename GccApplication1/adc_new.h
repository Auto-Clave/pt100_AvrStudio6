/*
 * IncFile1.h
 *
 * Created: 12/06/2016 06:36:47 ب.ظ
 *  Author: Hamed
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_


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
	_delay_us(30);
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
	_delay_us(30);
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
	_delay_us(30);
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
	   _delay_us(30);
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
   //ADMUX |= (1<<REFS1)|(1<<REFS0);_delay_us(30); //vreff internal with 2.56 v 
   ADMUX |= (0<<REFS1)|(1<<REFS0);_delay_us(30); //AVCC with external capacitor at AREF pin 
  //ADMUX = (1<<REFS0); 
  // start single convertion
  // write ’1′ to ADSC
  ADCSRA |= (1<<ADSC);
 
  // wait for conversion to complete
  // ADSC becomes ’0′ again
  // till then, run loop continuously
  while(ADCSRA & (1<<ADSC));
 
  return (ADC);
}
unsigned int i,T,T2;
float T3;
int tt;

uint16_t adc_value;            //Variable used to store the value read from the ADC
char buffer[5];                //Output of the itoa function

//#define ADC_VREF_TYPE 0xC0
#define ADC_VREF_TYPE 0x40
//#define ADC_VREF_TYPE 0xE0
// Read the AD conversion result
unsigned int read_adc_codevision(unsigned char adc_input)
{
ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
// Delay needed for the stabilization of the ADC input voltage
_delay_us(10);
// Start the AD conversion
ADCSRA|=0x40;
// Wait for the AD conversion to complete
while ((ADCSRA & 0x10)==0);
ADCSRA|=0x10;
return ADCW;
}



unsigned char adc_data33;
//#define ADC_VREF_TYPE 0xE0

// ADC interrupt service routine
ISR(ADC_vect)
{
// Read the 8 most significant bits
// of the AD conversion result
adc_data33=ADCH;
}

// Read the AD conversion result
unsigned int read_adc_codevision2(unsigned char adc_input)
{
ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
// Delay needed for the stabilization of the ADC input voltage
_delay_us(10);
// Start the AD conversion
ADCSRA|=0x40;
// Wait for the AD conversion to complete
while ((ADCSRA & 0x10)==0);
ADCSRA|=0x10;
unsigned int iDelay;

// Get ADC the Result
iDelay = ADCL;
iDelay += (ADCH << 8);

//return ADCW;
return iDelay;
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

  return sensor;

}
 //--------------Berechnung pt100 Wiederstand----------------
 float Temperatur3(int t) 
{
  float sensor  = t;                // Wert zwischenspeichern
  float Op_amp_gain=10.044;//33.5;//54;//40.08591;//33;//34.28017;//33.7328;//35.0688;//44.9858;//47.52;//22.760180;
  float V_reff = 5;//2.56;
  float Op_amp_V_out   = sensor * V_reff /(1023.0);      // Spannung vom PT100 errechnen
  float U_PT100_div   =Op_amp_V_out/Op_amp_gain;
  float R_pt=87.9002;//86.3306;//96.2126;//94.7441;//92.25;//87;
  float VCC_pt100=7;//5;
  float pt_res=1000.0;//2200.0;
  float I_pt_vetson= VCC_pt100/(pt_res+R_pt);//0.0021881818181818
  float V_pt_vetson=VCC_pt100-I_pt_vetson*pt_res;
  float U_PT100   = U_PT100_div + V_pt_vetson ;      // Spannung vom PT100 errechnen
  float U250     = VCC_pt100 - U_PT100 ;          // I = U / R
  float I_PT100   = U250 / pt_res-0.00006;//U250 / 2200.0;       
  float R_PT100   = U_PT100 / I_PT100;        // Wiederstand vom PT100  
   //R_PT100   = R_PT100 - 1.0;            // Leitungswiderstand

//--------------Berechnung Temperatur-----------------------
  float grad ;
  /*
  if (R_PT100 < 182.5)
  {
    float tmp1 =  182.5 - R_PT100;
    tmp1 = tmp1 / 0.3783105; 
    grad = 219.0 - tmp1;
  }  
  else
  {
  float tmp1 ;//=  182.5 - R_PT100;
    tmp1 = tmp1 / 0.3783105; 
    grad = 219.0 - tmp1;
  }	
  */
    // R_PT100 = 	0.389*temp	 + 	100.01    
    float tmp1=R_PT100 ;//=  182.5 - R_PT100;
    //tmp1 = tmp1 /0.385;// 0.3783105; 
    //grad = tmp1-259.6728;//257.09511;//217.0951 ;
	grad = 2.622*tmp1- 262.8; //Y = 2.622*X - 262.8 
	//grad=0.001014*tmp1*tmp1 + 2.3549*tmp1 - 245.6359;// y=1.014285456·10-3 x2 + 2.354970691 x - 245.6359419//http://www.xuru.org/rt/NLR.asp#CopyPaste
  //int int_grad = (int) grad;

  return grad;

}


 //--------------Berechnung pt100 Wiederstand----------------
 //http://www.noandishaan.com/forums/thread117675.html
 char *Temperatur_codevision(void) 
{
char str_CV[20];
char lcd[36];
float temp;
float min=0;
char i;

				_delay_ms(50);
				temp=read_adc_codevision2(0);
                //temp=read_adc(0);
                temp=(temp*256.0/1024.0);
                min=temp+min;
              
      min=min/3.0;
      //ftoa(min,1,str_CV);
      sprintf(lcd,"temp=%4s",str_CV);
	  Lcd4_Clear();
	  Lcd4_Set_Cursor(1,1);	
         Lcd4_Write_String("Temp_CV:");
		 Lcd4_Set_Cursor(2,1);	
         Lcd4_Write_String(lcd);
		 
		 float myFloat = 1.23456;
		char myFloatStr0[8];
		char *myFloatStr;

		//ftoa2(myFloatStr, myFloat, 10);
		//myFloatStr=floatToString(myFloat);
		char res[20];
		float n = 233.007;
		ftoa(n, res, 4);
		
		char *lcd3="2.12";
		//ftoa3(myFloatStr, myFloat, 10);
		//Lcd4_Write_String(myFloatStr);
		Lcd4_Set_Cursor(2,1);
		Lcd4_Write_String(res);
		_delay_ms(300);
		
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
		
		 return lcd;
		 
      
      };

/*	  
//http://www.avrfreaks.net/forum/convert-float-string-and-put-lcd	  
char ftoa2(char buffer, double d, int precision) {

	long wholePart = (long) d;

	// Deposit the whole part of the number.

	itoa(wholePart,buffer,10);

	// Now work on the faction if we need one.

	if (precision > 0) {

		// We do, so locate the end of the string and insert
		// a decimal point.

		char *endOfString = buffer;
		while (*endOfString != '\0') endOfString++;
		*endOfString++ = '.';

		// Now work on the fraction, be sure to turn any negative
		// values positive.

		if (d < 0) {
			d *= -1;
			wholePart *= -1;
		}
		
		double fraction = d - wholePart;
		while (precision > 0) {

			// Multipleby ten and pull out the digit.

			fraction *= 10;
			wholePart = (long) fraction;
			*endOfString++ = '0' + wholePart;

			// Update the fraction and move on to the
			// next digit.

			fraction -= wholePart;
			precision--;
		}

		// Terminate the string.

		*endOfString = '\0';
	}

   return buffer;
}
*/
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


//http://www.edaboard.com/thread317159.html
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



#endif /* INCFILE1_H_ */