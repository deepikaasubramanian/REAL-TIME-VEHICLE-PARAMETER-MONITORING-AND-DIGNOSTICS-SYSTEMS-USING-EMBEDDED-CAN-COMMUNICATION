   #include <lpc21xx.h>
#include <stdint.h>
#include "lcd1.h"
//E:\V25CEC1D3\DOCUMENTS\MAIN PROJECT\adc1.c

#include <stdio.h>
#include <string.h>
#include  "can_driver.c"
#define LED 1<<3
#define LDR 1<<4
#define LED1 1<<5
#define MOTOR 1<<6
unsigned int d,R;

int main(void)
{

CAN2_MSG m1;
CAN2_MSG m2;
uint32_t result;
float voltage;
float percent;
PINSEL1 = 0x01000000; /* P0.28 as AD0.1 */
IODIR0|=LED|LED1|MOTOR;
IOSET0=LED|LED1;
ADCR = 0x00200402; /* ADC operational, 10-bits, 11 clocks for conversion */
can2_init();
lcd_init();
//can2_init();
lcd_cmd(0x80);
str("real time vechile detectiom");
delay_ms(50);


//ADCR = 0x00200402; /* ADC operational, 10-bits, 11 clocks for conversion */

while(1)
{ 
    
   //lcd_init();
   //can2_init();
   can2_rx(&m1);
   //lcd_integer(m1.rtr);
   delay_ms(10);
   can2_rx(&m2);
	//lcd_integer(m2.rtr);
   //str("enter");
   //delay_ms(10);
 //Convert ADC value to equivalent voltage
if(m1.id==0x000001AF)
{
//can2_rx(&m1);
//str("hey");
lcd_cmd(0x01);
lcd_cmd(0x80);
str("DIST : ");
//delay_ms(50);
        d=m1.byteA;
        if(d!=1000){
		lcd_cmd(0x80);
            str("DIST : ");
            lcd_integer(d);
            str(" cm");
			if(d>35)
			IOCLR0=LED1;
        }
        else{
            //IOCLR0 = LED;
			lcd_cmd(0x80);
            str("DIST : -");
        }
    delay_ms(300);
 }
if(m2.id==0x000002AF ){
         lcd_cmd(0x01);
         lcd_cmd(0xc0);
         str("temp");
         R=m2.byteA;
		//lcd_cmd(0x01);
		//lcd_cmd(0xc0);
		//str("Temp:");
		lcd_cmd(0xc5);
		lcd_integer(R);
		lcd_cmd(0xc7);
		str("c");
		if(R>36)
		IOSET0=MOTOR;
		}
ADCR = ADCR | (1<<24); /* Start Conversion */
while ( !(ADDR & 0x80000000) ); /* Wait till DONE */
result = ADDR;
result = (result>>6);
result = (result & 0x000003FF);
voltage = ( (result/1023.0) * 3.3 );
lcd_cmd(0x94);
str("Fuel:");

percent=(voltage/3.3)*100;
lcd_cmd(0x98);
float_display(percent);
lcd_cmd(0x9D);
str("%");

delay_ms(500);
 if(IOPIN0 & LDR)
   {
   IOCLR0=LED;
 lcd_cmd(0xD4);
 str("turn oN light");
 delay_ms(200);
 }
else
{
   
  lcd_cmd(0xD4);
  IOSET0=LED;
  str("turn oFF light");
  delay_ms(200);
  }
    }
}