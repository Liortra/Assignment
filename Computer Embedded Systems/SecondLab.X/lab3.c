#include <xc.h>
#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF
#pragma config FNOSC =	FRCPLL
#pragma config FSOSCEN =	OFF
#pragma config POSCMOD =	EC
#pragma config OSCIOFNC =	ON
#pragma config FPBDIV =     DIV_1
#pragma config FPLLIDIV =	DIV_2
#pragma config FPLLMUL =	MUL_20
#pragma config FPLLODIV =	DIV_1
#include <stdio.h>
#include <stdlib.h>

void busy(void);
void main (void)
{
    int j,i;
    char  CG_father[16]={0x09,0x0a,0x0c,0x0c,0x0c,0x0a,0x09,0x09,//?
                                       0x26,0x29,0x31,0x21,0x29,0x2b,0x36,0x34};//?
    char father[3]={0,1,0};// ???
    char control[7]={0x38,0x38,0x38,0xe,0x6,0x1,0x40};

    TRISBbits.TRISB15 = 0; // RB15 (DISP_RS) set as an output
    ANSELBbits.ANSB15 = 0; // disable analog functionality on RB15 (DISP_RS)
    TRISDbits.TRISD5 = 0; // RD5 (DISP_RW) set as an output
    TRISDbits.TRISD4 = 0; // RD4 (DISP_EN) set as an output
    TRISE&=0xff00;
    ANSELEbits.ANSE2 = 0;
    ANSELEbits.ANSE4 = 0;
    ANSELEbits.ANSE5 = 0;
    ANSELEbits.ANSE6 = 0;
    ANSELEbits.ANSE7 = 0;
    PORTBbits.RB15=0;//rs=0
    PORTDbits.RD5=0;//w=0
    
    for(i=0;i<7;i++)
    {
        PORTE=control[i];
        PORTDbits.RD4=1;
        PORTDbits.RD4=0;
        busy();
    }
    PORTBbits.RB15 = 1;//rs
    for(i = 0;i < 16;i++)
    {
        PORTE=CG_father[i];
        PORTDbits.RD4=1;//enable=1
        PORTDbits.RD4=0;//enable=0
        busy();
    } 
    PORTBbits.RB15 = 0;//rs control 
    PORTE=0x80;//DDRAM
    PORTDbits.RD4=1;//enable=1
    PORTDbits.RD4=0;//enable=0
    busy();
    PORTBbits.RB15 = 1;//rs 

    for(i = 0;i < 3;i++)
    {
        PORTE=father[i];
        PORTDbits.RD4=1;//enable=1
        PORTDbits.RD4=0;//enable=0
        busy();
    } 
}

void busy(void)
{
    char RD,RS;
    int STATUS_TRISE;
    int portMap;
    RD=PORTDbits.RD5;
    RS=PORTBbits.RB15;
    STATUS_TRISE=TRISE;
    PORTDbits.RD5 = 1;//w/r
    PORTBbits.RB15 = 0;//rs 
    portMap = TRISE;
    portMap |= 0x80;
    TRISE = portMap;
    do
    {
    PORTDbits.RD4=1;//enable=1
    PORTDbits.RD4=0;//enable=0
    }
    while(PORTEbits.RE7); // BF ?????
    PORTDbits.RD5=RD; 
    PORTBbits.RB15=RS;
    TRISE=STATUS_TRISE;   
}
