#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xc.h>
#include <sys/attribs.h>
#include <string.h>
#include <p32xxxx.h>
#include <math.h>

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

#define SIN_DATA_POINTS 180
#define COS_DATA_POINTS 180
#define DELAY 640000

//AD
#define MIC 4
#define VOL 2

//switches
#define SW0  PORTFbits.RF3
#define SW1  PORTFbits.RF5
#define SW2  PORTFbits.RF4
#define SW3  PORTDbits.RD15
#define SW4  PORTDbits.RD14
#define SW5  PORTBbits.RB11
#define SW6  PORTBbits.RB10
#define SW7  PORTBbits.RB9

//RGB
#define RGB_RED PORTDbits.RD2
#define RGB_GREEN PORTDbits.RD12
#define RGB_BLUE PORTDbits.RD3

//LED
#define BIT0 PORTCbits.RC2
#define BIT1 PORTCbits.RC1
#define BIT2 PORTCbits.RC4
#define BIT3 PORTGbits.RG6
#define BIT4 PORTCbits.RC3
#define BIT5 PORTGbits.RG7
#define BIT6 PORTGbits.RG8
#define BIT7 PORTGbits.RG9

void writeToLCD(char str[], int up);

void delay();
void Delay();

void playBeep();
void DAsquare();
void DAsaw();
void DAsin(int * data);
void DACos(int * data);
void DAtriangle();
void PORT(int j);
void PORT_B(int j);
void busy();
void ADC_Init();
void BITS_TO_BINARY(int *binaryNum);
void DECIMAL_TO_BINARY(int * binaryNum, int n);
int BINARY_TO_DECIMAL(int* binaryNum);
void UPDATE_PORT_BITS(int* binaryNum);
void READ_ANALOG(int analog);
unsigned int ADC_AnalogRead(unsigned char analogPIN);

void main(void) { //configure switches to input
    TRISA &= 0xff00;
    TRISBbits.TRISB14 = 0;
    ANSELBbits.ANSB14 = 0;
    TRISFbits.TRISF3 = 1; // RF3 (SW0) configured as input
    TRISFbits.TRISF5 = 1; // RF5 (SW1) configured as input
    TRISFbits.TRISF4 = 1; // RF4 (SW2) configured as input
    TRISDbits.TRISD15 = 1; // RD15 (SW3) configured as input
    TRISDbits.TRISD14 = 1; // RD14 (SW4) configured as input
    TRISBbits.TRISB11 = 1; // RB11 (SW5) configured as input
    ANSELBbits.ANSB11 = 0; // RB11 (SW5) disabled analog
    TRISBbits.TRISB10 = 1; // RB10 (SW6) configured as input
    ANSELBbits.ANSB10 = 0; // RB10 (SW6) disabled analog
    TRISBbits.TRISB9 = 1; // RB9 (SW7) configured as input
    ANSELBbits.ANSB9 = 0; // RB9 (SW7) disabled analog

    //RGB
    TRISDbits.TRISD2 = 0; // R LED8_R AN25/RPD2/RD2 output
    TRISDbits.TRISD12 = 0; //G LED8_G RPD12/PMD12/RD12 output
    TRISDbits.TRISD3 = 0; // B LED8_B AN26/RPD3/RD3 output

    //conditions for ssd
    TRISBbits.TRISB1 = 1; // RB1 (BTNU) configured as input
    ANSELBbits.ANSB1 = 0; // RB1 (BTNU) disabled analog
    TRISBbits.TRISB0 = 1; // RB0 (BTNL) configured as input
    ANSELBbits.ANSB0 = 0; // RB0 (BTNL) disabled analog
    TRISFbits.TRISF0 = 1; // RF0 (BTNC) configured as input
    TRISBbits.TRISB8 = 1; // RB8 (BTNR) configured as input
    ANSELBbits.ANSB8 = 0; // RB8 (BTNR) disabled analog
    TRISAbits.TRISA15 = 1; // RA15 (BTND) configured as input 
    TRISA &= 0xff00;
    SSD_Init();

    //conditions for DAC
    TRISCbits.TRISC2 = 0; //RC2            
    TRISCbits.TRISC1 = 0; //RC1           
    TRISCbits.TRISC4 = 0; //RC4            
    TRISGbits.TRISG6 = 0; //RG6
    ANSELGbits.ANSG6 = 0; //???????
    TRISCbits.TRISC3 = 0; //RC3
    TRISGbits.TRISG7 = 0; //RG7
    ANSELGbits.ANSG7 = 0;
    TRISGbits.TRISG8 = 0; //RG8
    ANSELGbits.ANSG8 = 0; //???????
    TRISGbits.TRISG9 = 0; //RG9
    ANSELGbits.ANSG9 = 0; //???????

    //conditions for DAC B
    TRISDbits.TRISD9 = 0; //RD9            
    TRISDbits.TRISD11 = 0; //RD11          
    TRISDbits.TRISD10 = 0; //RD10          
    TRISDbits.TRISD8 = 0; //RG6
    //    ANSELDbits.ANSD8=0;//???????
    TRISCbits.TRISC14 = 0; //RC3
    TRISDbits.TRISD0 = 0; //RG7
    //    ANSELGbits.ANSG7=0;
    TRISDbits.TRISD1 = 0; //RG8
    ANSELDbits.ANSD1 = 0; //???????
    TRISCbits.TRISC13 = 0; //RG9
    //    ANSELGbits.ANSG9=0;//digital

    //conditions for AD
    TRISC = 0;
    TRISG = 0;
    int i, analog, x;
    int port_value;
    PORTC = 0;
    PORTG = 0;
    char vol[16];
    TRISA &= 0xff00;
    TRISBbits.TRISB2 = 1;
    ANSELBbits.ANSB2 = 1;
    int cur_x;
    int prev_x;
    ADC_Init();

    int k;
    int sin_data[SIN_DATA_POINTS];
    for (k = 0; k < SIN_DATA_POINTS; k++)// make data samples for sinus function
        sin_data[k] = (127 + 127 * sin((2 * 3.14 * k) / SIN_DATA_POINTS));

    int cos_data[COS_DATA_POINTS];
    for (k = 0; k < COS_DATA_POINTS; k++)// make data samples for sinus function
        cos_data[k] = (127 + 127 * cos((2 * 3.14 * k) / COS_DATA_POINTS));

    int sin_data_small[SIN_DATA_POINTS];
    for (k = 0; k < SIN_DATA_POINTS; k++)// make data samples for sinus function
    {
        sin_data_small[k] = (63 + 63 * sin((2 * 3.14 * k) / SIN_DATA_POINTS));
    }

    int cos_data_small[COS_DATA_POINTS];
    for (k = 0; k < COS_DATA_POINTS; k++)// make data samples for sinus function
    {
        cos_data_small[k] = (63 + 63 * sin((2 * 3.14 * k) / SIN_DATA_POINTS));
    }
    int mode = -1; // prevent writing to LCD in a loop
    writeToLCD("                        ", 1); // clear LCD
    while (1) {
        if (SW5)//ACD to DCA
        {
            cur_x = ADC_AnalogRead(2); //IN analog RB2
            PORTA = cur_x / 4;
            analog = cur_x / 100;
            READ_ANALOG(analog);
            PORT(cur_x / 4);
            for (i = 0; i < 6400; i++);
            if (cur_x / 4 > 250)
                RGB_GREEN = 1;
            else if (cur_x / 4 > 160)
                RGB_RED = 1;
            else if (cur_x / 4 > 80)
                RGB_BLUE = 1;
            else {
                RGB_GREEN = 0;
                RGB_RED = 0;
                RGB_BLUE = 0;
            }

            if (mode != 5) {
                playBeep();
                writeToLCD("Mode5-ACD to DCA        ", 0);
                SSD_WriteDigits(5, 0, 0, 0, 0, 0, 0, 0);
                mode = 5;
            }
        } else if (SW4) //DA triangle
        {
            DAtriangle();
            if (mode != 4) {
                playBeep();
                writeToLCD("Mode 4: Triangle        ", 0);
                SSD_WriteDigits(4, 0, 0, 0, 0, 0, 0, 0);
                mode = 4;
            }
        } else if (SW3) //saw wave
        {
            DAsaw();
            if (mode != 3) {
                playBeep();
                writeToLCD("Mode 3: Saw Wave        ", 0);
                SSD_WriteDigits(3, 0, 0, 0, 0, 0, 0, 0);
                mode = 3;
            }
        } else if (SW2) //DA square
        {
            DAsquare();
            if (mode != 2) {
                playBeep();
                writeToLCD("Mode 2: Square         ", 0);
                SSD_WriteDigits(2, 0, 0, 0, 0, 0, 0, 0);
                mode = 2;
            }
        } else if (SW1)//DA sinus
        {
            DAsin(sin_data);
            if (mode != 1) {
                playBeep();
                writeToLCD("Mode 1: Sinus         ", 0);
                SSD_WriteDigits(1, 0, 0, 0, 0, 0, 0, 0);
                mode = 1;
            }

        }
        else if (SW6)//DA cosinus
        {
            DASinCos(sin_data, cos_data, sin_data_small, cos_data_small);

            if (mode != 6) {
                playBeep();
                writeToLCD("Mode 6: SinCos        ", 0);
                SSD_WriteDigits(6, 0, 0, 0, 0, 0, 0, 0);
                mode = 6;
            }

        }
        else if (SW7)//DA cosinus
        {
            DACos(cos_data);

            if (mode != 7) {
                playBeep();
                writeToLCD("Mode 7: Cosinus       ", 0);
                SSD_WriteDigits(7, 0, 0, 0, 0, 0, 0, 0);
                mode = 7;
            }

        }
        else {
            if (mode != -2) {
                playBeep();
                writeToLCD("Select mode SW            ", 0);
                SSD_WriteDigits(0, 0, 0, 0, 0, 0, 0, 0);
                mode = -2;
            }
        }
    }
}

void playBeep() {
    int i, j;
    for (i = 0; i < 1300; i++) {
        PORTBbits.RB14--;
        for (j = 0; j < 100; j++);
        PORTBbits.RB14++;
    }
}

void DAsaw() {
    int j;
    for (j = 0; j < 0xff; j++)
        PORT(j);

}

void DAsin(int * data) {
    int x;
    for (x = 0; x < SIN_DATA_POINTS; x++)
        PORT(data[x]);
}

void DACos(int * data) {

    int x;
    for (x = 0; x < COS_DATA_POINTS; x++)
        PORT_B(data[x]);
}

void DASinCos(int * data, int * data2, int * data3, int * data4) {

    int x;
    for (x = 0; x < COS_DATA_POINTS; x++) {
        PORT(data[x]);
        PORT_B(data2[x]);
        PORT(data3[x]);
        PORT_B(data4[x]);
    }

}

void DAtriangle() {
    int j;
    for (j = 0; j < 0xff; j++)
        PORT(j);
    for (j = 0xff; j > 0; j--)
        PORT(j);

}

void DAsquare() {
    int x;

    PORT(0xff);
    for (x = 0; x < 1000; x++);

    PORT(0);
    for (x = 0; x < 1000; x++);

}

void PORT(int j) {
    PORTCbits.RC2 = j & 1;
    Delay();
    PORTCbits.RC1 = (j >> 1)&1;
    Delay();
    PORTCbits.RC4 = (j >> 2)&1;
    Delay();
    PORTGbits.RG6 = (j >> 3)&1;
    Delay();
    PORTCbits.RC3 = (j >> 4)&1;
    Delay();
    PORTGbits.RG7 = (j >> 5)&1;
    Delay();
    PORTGbits.RG8 = (j >> 6)&1;
    Delay();
    PORTGbits.RG9 = (j >> 7)&1;
}

void PORT_B(int j) {
    PORTDbits.RD9 = j & 1;
    Delay();
    PORTDbits.RD11 = (j >> 1)&1;
    Delay();
    PORTDbits.RD10 = (j >> 2)&1;
    Delay();
    PORTDbits.RD8 = (j >> 3)&1;
    Delay();
    PORTCbits.RC14 = (j >> 4)&1;
    Delay();
    PORTDbits.RD0 = (j >> 5)&1;
    Delay();
    PORTDbits.RD1 = (j >> 6)&1;
    Delay();
    PORTCbits.RC13 = (j >> 7)&1;
}

void Delay() {
    int i;
    for (i = 0; i < 1; i++);
}

void READ_ANALOG(int analog) {
    BIT0 = analog & 1;
    BIT1 = (analog >> 1)&0x01;
    BIT2 = (analog >> 2)&0x01;
    BIT3 = (analog >> 3)&0x01;
    BIT4 = (analog >> 4)&0x01;
    BIT5 = (analog >> 5)&0x01;
    BIT6 = (analog >> 6)&0x01;
    BIT7 = (analog >> 7)&0x01;
}

void UPDATE_PORT_BITS(int* binaryNum) {
    BIT0 = binaryNum[0];
    BIT1 = binaryNum[1];
    BIT2 = binaryNum[2];
    BIT3 = binaryNum[3];
    BIT4 = binaryNum[4];
    BIT5 = binaryNum[5];
    BIT6 = binaryNum[6];
    BIT7 = binaryNum[7];
}

void DECIMAL_TO_BINARY(int * binaryNum, int n) {
    // binarynum is an array to store binary number size 32
    // counter for binary array 
    int i = 0;
    while (n > 0) {
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
}

int BINARY_TO_DECIMAL(int* binaryNum) {
    // [0,0,0,1,0,1,1]
    int dec_value = 0;
    int mult = 1;
    int i;
    int current_digit;
    for (i = 0; i < 8; i++) {
        current_digit = binaryNum[i];
        dec_value += current_digit * mult;
        mult = mult * 2;
    }
    return dec_value;
}

void BITS_TO_BINARY(int *binaryNum) {
    //binaryNum is an array size 8 to store the bits
    binaryNum[0] = BIT0;
    binaryNum[1] = BIT1;
    binaryNum[2] = BIT2;
    binaryNum[3] = BIT3;
    binaryNum[4] = BIT4;
    binaryNum[5] = BIT5;
    binaryNum[6] = BIT6;
    binaryNum[7] = BIT7;
}

void ADC_Init() {
    AD1CON1 = 0;
    AD1CON1bits.SSRC = 7; // Internal counter ends sampling and starts conversion (auto convert)
    AD1CON1bits.FORM = 0; // Integer 16-bit
    // Setup for manual sampling
    AD1CSSL = 0;
    AD1CON3 = 0x0002; // ADC Conversion Clock Select bits: TAD = 6 TPB
    AD1CON2 = 0;
    AD1CON2bits.VCFG = 0; // Voltage Reference Configuration bits: VREFH = AVDD and VREFL = AVSS
    // Turn on ADC
    AD1CON1bits.ON = 1;
}

unsigned int ADC_AnalogRead(unsigned char analogPIN) {
    int adc_val = 0;
    IEC0bits.T2IE = 0;
    AD1CHS = analogPIN << 16; // AD1CHS<16:19> controls which analog pin goes to the ADC
    AD1CON1bits.SAMP = 1; // Begin sampling
    while (AD1CON1bits.SAMP); // wait until acquisition is done
    while (!AD1CON1bits.DONE); // wait until conversion is done
    adc_val = ADC1BUF0;
    IEC0bits.T2IE = 1;
    return adc_val;
}

void playSound(int mult) {
    int i;
    PORTBbits.RB14--;
    for (i = 0; i < 500 + mult * 100; i++);
    PORTBbits.RB14++;
}

void writeToLCD(char str[], int up) {
    int j, i, size = 6;
    char control[6] = {0x38, 0x38, 0x38, 0xe, 0x6, 0x1};
    char control2 = 0xc0;
    TRISBbits.TRISB15 = 0; // RB15 (DISP_RS) set as an output
    ANSELBbits.ANSB15 = 0; // disable analog functionality on RB15 (DISP_RS)
    TRISDbits.TRISD5 = 0; // RD5 (DISP_RW) set as an output
    TRISDbits.TRISD4 = 0; // RD4 (DISP_EN) set as an output
    //TRISEbits.TRISE0 = 1; // RE0 (DB0) set as input (change 1 to 0 for
    TRISE &= 0xff00;
    PORTBbits.RB15 = 0; //rs=0
    PORTDbits.RD5 = 0; //w=0
    ANSELEbits.ANSE7 = 0;
    if (!up)
        size = 1;
    for (i = 0; i < size; i++) {
        if (up)
            PORTE = control[i];
        else
            PORTE = control2;
        PORTDbits.RD4 = 1;
        PORTDbits.RD4 = 0;
        busy();
    }
    PORTBbits.RB15 = 1; //rs=0
    PORTDbits.RD5 = 0; //w=0
    for (i = 0; i < 16; i++) {
        PORTE = str[i];
        PORTDbits.RD4 = 1;
        PORTDbits.RD4 = 0;
        busy();
    }
}

void delay() {
    int j;
    for (j = 0; j < DELAY; j++);
}

void busy() {
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
    }    while (PORTEbits.RE7); // BF ?????
    PORTDbits.RD5 = RD;
    PORTBbits.RB15 = RS;
    TRISE = STATUS_TRISE;
}
