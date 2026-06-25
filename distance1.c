 #include<lpc21xx.h>
#include "header1.h"
#include "lcd1.h"
#include "can_driver.c"
//#include "uart0_driver.c"
#define TRIG  (1<<3)  
#define ECHO  (1<<4)
//#define LED   (1<<2)

unsigned int d;
unsigned int dist;
unsigned int timeout;
char buf[10];
int i;

void distance_init(void);
unsigned int distance(void);


void usdelay(unsigned int n){
    T0PR  = 15 - 1;
    T0TCR = 0x03;
    T0TCR = 0x01;
    while(T0TC < n);
    T0TCR = 0x00;
}

void msdelay(unsigned int n){
    unsigned int i;
    for(i = 0; i < n; i++)
        usdelay(1000);
}
/***********************************/

int main(){
CAN2_MSG m1;
can2_init();
lcd_init();
distance_init();
msdelay(500);
//lcd_string("Tx Ready");
while(1)
{
d=distance();
if(d!=1000)
{
str("Dist: ");
lcd_integer(d);
str("cm");
delay_ms(1000);
}
else
{
lcd_init();
str("Dist:-");
}
msdelay(500);
m1.id=0x000001AF;
     m1.rtr=0;
     m1.dlc=2;
m1.byteA=d;
m1.byteB=0;
can2_tx(m1);
usdelay(1);
}
return 0;
}
/****************************************/

void distance_init(void){
    IODIR0 |=  TRIG;
    IODIR0 &= ~ECHO;
    IOCLR0  =  TRIG;
}

unsigned int distance(void){
    timeout = 0;

    IOCLR0 = TRIG;
    usdelay(5);
    IOSET0 = TRIG;
    usdelay(15);
    IOCLR0 = TRIG;

    timeout = 0;
    while(!(IOPIN0 & ECHO)){
        timeout++;
        if(timeout > 50000) return 1000;
    }

    T1PR  = 15 - 1;
    T1TCR = 0x03;
    T1TCR = 0x01;

    timeout = 0;
    while(IOPIN0 & ECHO){
        timeout++;
        if(timeout > 50000){
            T1TCR = 0x00;
            return 1000;
        }
    }

    T1TCR = 0x00;

    dist = T1TC / 58;
    if(dist < 2 || dist > 400) return 1000;
    return dist;
}

