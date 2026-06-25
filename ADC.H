#include<LPC21XX.h>

#include "lcd1.h"
unsigned int adc_value;
unsigned int temp;
   int Delay(unsigned int x)
{
 x=x*12000;
 while(x!=0)
 {
  x--;
 }
 return 0;
}
int ADC_Conversion()
{
 int ab;                                /* Variable to store ADC value         */
 Delay(1000);
 ADCR  = ADCR|0x01000000;               /* Start conversion                    */
 while((ADDR&0x80000000)!=0x80000000);  /* Wait here till conversion is over   */ 
 ab    = (ADDR&0x0000FFC0);             /* Extracting the result               */
 ab    = (ab>>6);                       /* Shift 6 bits right                  */
 return ab;                             /* Return the result                   */
}
void Int_ASCII(int value,char cnt)
{
 int i = 0;                             /* Local variables                     */
 char array[7];
 int values;
 values= value;
 
 for(i=1;i<=cnt;i++)                    /* Store the received value in array   */
 {
  array[i] = values%10;
  values   = values/10;
 }
 
 for(i=cnt;i>=1;i--)                    /* Display it on LCD                   */
 {
  lcd_data(array[i]+'0');
 }
}
 
/********************************************************************************
* Function    : Sensor_Check                                                    *
*                                                                               *
* Description : Function to take ADC reading & display it on LCD                *
*                                                                               *
* Parameters  : None                                                            *
********************************************************************************/
void Sensor_Check()
{

 ADCR=0x00200602;                       /* PDN=1,CLKDIV=6,channel=AD0.2        */
 
 lcd_cmd(0xC0);
 str("Temp:");
 adc_value=ADC_Conversion();            /* Get the result of conversion        */
 temp=((adc_value*3.3 * 100.0)/1023.0);
 lcd_cmd(0xC6);                    /* 2nd row, 5th location               */
 Int_ASCII(temp,2);  
lcd_cmd(0xC9); 	/* Display the result on LCD 	*/
lcd_data('C');
}
 