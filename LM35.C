
     /* main_nodeA_TX.c */
#include<lpc21xx.h>
#include "header1.h"
#include "can_driver.c"
//#include "lcd1.h"
#include "uart.c"
 #include "ADC.H"
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned short int u16;

typedef   signed short int s16;

 
	void delay_us(unsigned int s){

	T0PR=15-1;

	//T0TC=T0PC=0;

	T0TCR=0x1;

	while(T0TC<s);

	T0TCR=0x03;

	T0TCR = 0x00;

}
 




 int main(){	
     		

	CAN2_MSG m2;

	//m2;
	lcd_init();
	//uart0_init(115200);
	can2_init();
	 //PORT_Initial();   
	/*sending data frame*/

	m2.id=0x000002AF;

	m2.rtr=0;//data frame

	m2.dlc=4;

	m2.byteB=0;

	

	/*m2.id=0x5;

	m2.dlc=7;

	m2.rtr=1;*/
	str("hi");
	while(1)

		{
		str("temperature");
		Sensor_Check();
		//uart0_tx_string("temperature");
	
		m2.id=0x000002AF;
		m2.rtr=0;
		m2.dlc=4;
		m2.byteA=(unsigned char)temp;
		//m1.byteA=24;
		//m1.byteA=24;
		//str("transmit"); 
		//uart0_tx_string("transmit");
	    lcd_integer(m2.byteA);
	    can2_tx(m2);//data-frame0
		delay_us(100);

		/*can2_tx(m2);//remote frame

		delay_ms(2000);*/

		//can2_rx(&m2);

		}
		}
