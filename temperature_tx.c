#include<lpc21xx.h>

#include<stdint.h>

#include<stdio.h>

#include "Header.h"

#include"delay.h"

#include"lcd4bit.h"
#include"can1driv.c"





uint8_t dht11_data(void);
void dht11_request(void);
void dht11_response(void);

void us(uint16_t j)
{
    uint16_t x,i;
	for(i=0;i<j;i++)
	{
    for(x=0; x<7; x++);    /* loop to generate 1.04 microsecond delay with 60MHz Cclk */
	}
}

void ms(uint16_t j)
{
    uint16_t x,i;
	for(i=0;i<j;i++)
	{
    for(x=0; x<6000; x++);    /* loop to generate 1 milisecond delay with 60MHz Cclk */
	}
}


int main(){	

    CAN2_MSG m1;

	unsigned char ch,temp_int,temp_dec;
	can2_init();

	lcd_init();

	m1.id=0x000001AF;
	m1.rtr=0;//data frame
	m1.dlc=4;

	delay_ms(50);
	while(1)
	{	
	    dht11_request();
      	dht11_response();
		ch = dht11_data();
		ch = dht11_data();
		temp_int = dht11_data();
		temp_dec= dht11_data();
		ch = dht11_data();
		m1.byteA = temp_int;
        m1.byteB =(unsigned char)((temp_dec- m1.byteA)*100); ;
		lcd_cmd(0x80);
		str("TEMPERATURE:");
		lcd_integer(m1.byteA);
		str("."); 
		lcd_integer(m1.byteB);
		can2_tx(m1);//data-frame
		delay_us(1000);			
	}
}


void dht11_request(void)
{
	IO0DIR = IO0DIR | 0x00000010;	
	IO0PIN = IO0PIN & 0xFFFFFFEF; 
	ms(20);
	IO0PIN = IO0PIN | 0x00000010; 
}

void dht11_response(void)
{
	IO0DIR = IO0DIR & 0xFFFFFFEF;	
	while( IO0PIN & 0x00000010 );	
	while( (IO0PIN & 0x00000010) == 0 );	
	while( IO0PIN & 0x00000010 );	
}


uint8_t dht11_data(void)
{
	int8_t count;
	uint8_t data = 0;
	for(count = 0; count<8 ; count++)	
	{
		while( (IO0PIN & 0x00000010) == 0 );	
		us(30);
		if ( IO0PIN & 0x00000010 ) 
			data = ( (data<<1) | 0x01 );
		else	
			data = (data<<1);
		while( IO0PIN & 0x00000010 );	
	}
	return data;
}		  


