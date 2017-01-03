/*	Here is some example code for interfacing with the LCD, 
	courtesy of Dave Bordoley.  Note that only a four bit data 
	bus was used, hence the use of lcd_send_nibble.  Also, not 
	all of the functions used are defined here; this bit of code
	is meant to give you an idea of where to go.
*/

#include "lcd.h"
#include "bus.h"

#define lcd_line_two 0x40    /* LCD RAM address for the second line */

/* You must set the data bus to write mode before calling this */
static void
lcd_send_nibble (BYTE n )
{
  /* Put lower four bits of data on data bus */
  output_bit (DATA_3, n & (0x08));
  output_bit (DATA_2, n & (0x04));
  output_bit (DATA_1, n & (0x02));
  output_bit (DATA_0, n & (0x01));

  bus_send_strobe_us (160);
}

static void
lcd_send_byte (BYTE address, BYTE n )
{
  output_low (LCD_RS);

  delay_us (160);

  set_tris_data_bus (WRITE_MODE);

  output_bit (LCD_RS, address);
  delay_us (160);

  lcd_send_nibble ((n & (0xF0)) >> 4);
  lcd_send_nibble (n & (0x0F));

  set_tris_data_bus (READ_MODE);
}

void
lcd_init ()
{
  int i;

  bus_select_device (LCD);

  output_low (LCD_RS);
  delay_ms (150);

  set_tris_data_bus (WRITE_MODE);

  for(i = 0; i < 2; i++)
  {
    lcd_send_nibble (0x02);
    delay_ms (20);
  }

  lcd_send_nibble (0x08);
  delay_ms (20);

  lcd_send_nibble (0x00);
  delay_ms (20);

  lcd_send_nibble (0x0F);
  delay_ms (20);

  lcd_send_nibble (0x08);
  lcd_send_nibble (0x00);

  set_tris_data_bus (READ_MODE);

  bus_select_device (NODEVICE);

  delay_ms (20);

  lcd_clear ();
}

void
lcd_gotoxy (BYTE x, BYTE y)
{
  BYTE address;

  bus_select_device (LCD);

  /* Choose the line of the lcd to address */
  if (y != 1)
  {
    address = lcd_line_two;
  }
  else
  {
    address = 0;
  }
  
  /* add the x-offset */
  address += x-1;

  lcd_send_byte (0, (0x80 | address));

  bus_select_device (NODEVICE);
}

void
lcd_putc (char c)
{
  bus_select_device (LCD);

  switch (c)
  {
    case '\f':
      lcd_send_byte (0, 1);
      break;
    case '\n':
      lcd_gotoxy (1, 2);
      break;
    case '\b':
      lcd_send_byte (0, (0x10));
      break;
    default:
      lcd_send_byte (1, c);
      break;
  }

  bus_select_device (NODEVICE);
}

void
lcd_clear ()
{
  lcd_clear_line (0);
  lcd_clear_line (1);
}

void
lcd_clear_line (BOOLEAN line)
{
  lcd_gotoxy (1, (line++));
  lcd_putc ("                ");
}

void
lcd_put_int (int num)
{
  lcd_putc ((num % 10)+ '0');  
}

void 
lcd_put_2_digit_int (int num)
{
	lcd_putc (((num / 10) % 10) + '0');
	lcd_putc ((num % 10) + '0');
}

void 
lcd_put_string (char *string)
{
  char value;
  int n = 0;

  value = string [n];
  while (value != NULL)
  {
    lcd_putc (value);
	n++;
	value = string [n];
  }
}