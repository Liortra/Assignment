/* 
 * File:   lcd_fader.c
 * Author: MenachemE
 *
 * Created on July 11, 2019, 5:49 PM
 */

#include <xc.h>
#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF
#pragma config FNOSC =	FRCPLL
#pragma config FSOSCEN = OFF
#pragma config POSCMOD = EC
#pragma config OSCIOFNC = ON
#pragma config FPBDIV = DIV_1
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLODIV = DIV_1
#include <stdio.h>
#include <stdlib.h>
void busy(void);
void PlaySound();

void PlaySound() {

    PORTBbits.RB14 = 1;
    int i;
    for (i = 0; i <= 200; i++)
        PORTBbits.RB14 = 0;
    for (i = 0; i <= 4; i++)
        PORTBbits.RB14 = 1;
}

void main(void) {
    int j, i;

    char CG_father[8] = {0x00, 0x1b, 0x00, 0x04, 0x04, 0x11, 0x0e, 0x00}; //?
    char father[1] = {0}; // 
    char control[7] = {0x38, 0x38, 0x38, 0xe, 0x6, 0x1, 0x40};
    // set CGRAM=0x40

    TRISA &= 0xff00; //led
    TRISFbits.TRISF3 = 1; // RF3 (SW0) configured as input
    PORTAbits.RA0 = PORTFbits.RF3; // RF3 (SW0) configured as input
    TRISCbits.TRISC2 = 0; //RC2
    TRISCbits.TRISC1 = 0; //RC1
    TRISCbits.TRISC4 = 0; //RC4            
    TRISGbits.TRISG6 = 0; //RG6
    ANSELGbits.ANSG6 = 0; //???????
    TRISCbits.TRISC3 = 1; //RC3
    CNPUCbits.CNPUC3;
    TRISGbits.TRISG7 = 1; //RG7
    ANSELGbits.ANSG7 = 0;
    CNPUGbits.CNPUG7;
    TRISGbits.TRISG8 = 1; //RG8
    ANSELGbits.ANSG8 = 0; //???????
    CNPUGbits.CNPUG8; //???
    TRISGbits.TRISG9 = 1; //RG9
    ANSELGbits.ANSG9 = 0; //???????
    CNPUGbits.CNPUG9; //????? 
    TRISBbits.TRISB15 = 0; // RB15 (DISP_RS) set as an output
    ANSELBbits.ANSB15 = 0; // disable analog functionality on RB15 (DISP_RS)
    TRISDbits.TRISD5 = 0; // RD5 (DISP_RW) set as an output
    TRISDbits.TRISD4 = 0; // RD4 (DISP_EN) set as an output
    TRISE &= 0xff00;
    ANSELEbits.ANSE2 = 0;
    ANSELEbits.ANSE4 = 0;
    ANSELEbits.ANSE5 = 0;
    ANSELEbits.ANSE6 = 0;
    ANSELEbits.ANSE7 = 0;
    TRISBbits.TRISB9 = 1; // RB9 (SW7) configured as input
    ANSELBbits.ANSB9 = 0; // RB9 (SW7) disabled analog
    TRISBbits.TRISB14 = 0;
    ANSELBbits.ANSB14 = 0;

    PORTBbits.RB15 = 0; //rs=0
    PORTDbits.RD5 = 0; //w=0
    for (i = 0; i < 7; i++) {
        PORTE = control[i];
        PORTDbits.RD4 = 1;
        PORTDbits.RD4 = 0;
        busy();
    }
    PORTBbits.RB15 = 1; //rs
    for (i = 0; i < 8; i++) {
        PORTE = CG_father[i];
        PORTDbits.RD4 = 1; //enable=1
        PORTDbits.RD4 = 0; //enable=0
        busy();
    }
    PORTBbits.RB15 = 0; //rs control 
    PORTE = 0x80; //DDRAM
    PORTDbits.RD4 = 1; //enable=1
    PORTDbits.RD4 = 0; //enable=0
    //for(j=0;j<32000;j++);
    busy();
    PORTBbits.RB15 = 1; //rs 
    for (i = 0; i < 1; i++) {
        PORTE = father[i];
        PORTDbits.RD4 = 1; //enable=1
        PORTDbits.RD4 = 0; //enable=0
        busy();
    }
    int counter = 0;
    while (1) {
        if (PORTBbits.RB9) {
            PlaySound();
        }
        if (counter > 17) {
            PORTBbits.RB15 = 0; //rs 
            PORTE = 0x1;
            PORTDbits.RD4 = 1; //enable=1
            PORTDbits.RD4 = 0; //enable=0
            busy();

            counter = 0;
            PORTBbits.RB15 = 1; //rs 
            for (i = 0; i < 1; i++) {
                PORTE = father[i];
                PORTDbits.RD4 = 1; //enable=1
                PORTDbits.RD4 = 0; //enable=0
                busy();
            }
        } else if (counter < 0) {
            PORTBbits.RB15 = 0; //rs 
            PORTE = 0x1;
            PORTDbits.RD4 = 1; //enable=1
            PORTDbits.RD4 = 0; //enable=0
            busy();
            counter = 16;
            int k;
            PORTBbits.RB15 = 1;
            for (k = 0; k < 15; k++) {
                PORTE = 0x1c;
                PORTDbits.RD4 = 1; //enable=1
                PORTDbits.RD4 = 0; //enable=0
                busy();
            }
            PORTBbits.RB15 = 1; //rs 
            for (i = 0; i < 1; i++) {
                PORTE = father[i];
                PORTDbits.RD4 = 1; //enable=1
                PORTDbits.RD4 = 0; //enable=0
                busy();
            }

        }
        PORTBbits.RB15 = 0; //rs 
        if (PORTFbits.RF3) // RF3 (SW0) configured as input 
        {
            counter++;
            PORTE = 0x1c;
        } else {
            counter--;
            PORTE = 0x18;
        }
        PORTDbits.RD4 = 1;
        PORTDbits.RD4 = 0;
        busy();
        for (i = 0; i < 200000; i++);
    }
}

void busy(void) {
    char RD, RS;
    int STATUS_TRISE;
    int portMap;
    RD = PORTDbits.RD5;
    RS = PORTBbits.RB15;
    STATUS_TRISE = TRISE;
    PORTDbits.RD5 = 1; //w/r
    PORTBbits.RB15 = 0; //rs 
    portMap = TRISE;
    portMap |= 0x80;
    TRISE = portMap;
    do {
        PORTDbits.RD4 = 1; //enable=1 
        PORTDbits.RD4 = 0; //enable=0
    } while (PORTEbits.RE7); // BF ?????
    PORTDbits.RD5 = RD;
    PORTBbits.RB15 = RS;
    TRISE = STATUS_TRISE;
}


