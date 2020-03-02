#include <xc.h>
#include <string.h>
#include <sys/attribs.h>
#include <stdio.h>
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

uint8_t arr[16] = {0}; // a b c d 3 6 9 e 2 5 8 f 1 4 7 0
uint8_t last_det = -1;
uint8_t last_mode = -1;
char *modes[] = {"Analog 1 ? %1.2fV","Analog 2 ? %1.2fV"};
float voltage = 0.0;

void setup()
{
//    TRISFbits.TRISF3 = 1; // RF3 (SW0) configured as input
//    TRISFbits.TRISF5 = 1; // RF5 (SW1) configured as input
//    TRISFbits.TRISF4 = 1; // RF4 (SW2) configured as input
//    TRISDbits.TRISD15 = 1; // RD15 (SW3) configured as input
//    TRISDbits.TRISD14 = 1; // RD14 (SW4) configured as input
//    TRISBbits.TRISB11 = 1; // RB11 (SW5) configured as input
//    ANSELBbits.ANSB11 = 0; // RB11 (SW5) disabled analog
//    TRISBbits.TRISB10 = 1; // RB10 (SW6) configured as input
//    ANSELBbits.ANSB10 = 0; // RB10 (SW6) disabled analog
//    TRISBbits.TRISB9 = 1; // RB9 (SW7) configured as input
//    ANSELBbits.ANSB9 = 0; // RB9 (SW7) disabled analog
//    TRISBbits.TRISB14 = 0; //speaker configured output
//    ANSELBbits.ANSB14 = 0; //speaker disabled analog
    
    TRISCbits.TRISC2=0;//RC2            
    TRISCbits.TRISC1=0;//RC1           
    TRISCbits.TRISC4=0;//RC4            
    TRISGbits.TRISG6=0;//RG6
    ANSELGbits.ANSG6=0;//???????
    TRISCbits.TRISC3 =1;//RC3
    CNPUCbits.CNPUC3;               
    TRISGbits.TRISG7=1;//RG7
    ANSELGbits.ANSG7=0;
    CNPUGbits.CNPUG7;                  
    TRISGbits.TRISG8=1;//RG8
    ANSELGbits.ANSG8=0;//???????
    CNPUGbits.CNPUG8;//?????
    TRISGbits.TRISG9=1;//RG9
    ANSELGbits.ANSG9=0;//???????
    CNPUGbits.CNPUG9;//????? 
    LATGSET=9;
    
    TRISA &= 0xff00; // define 8 LSB of port A as output 
    PORTA =0x00; // set all 8 leds (8 LSB) of port A to zero.
}

void setup_interrupts()
{
    PR4 = 0xffff;                        //             set period register, generates one interrupt every 1 ms
    TMR4 = 0;                           //             initialize count to 0

    T4CONbits.TCKPS0 = 1;                //            1:256 prescale value
    T4CONbits.TCKPS1 = 1; 
    T4CONbits.TCKPS2 = 1; 
    T4CONbits.TGATE = 0;                //             not gated input (the default)
    T4CONbits.TCS = 0;                  //             PCBLK input (the default)
    IPC4bits.T4IP = 2;                  //             priority
    IPC4bits.T4IS = 0;                  //             subpriority
    IFS0bits.T4IF = 0;                  //             clear interrupt flag
    IEC0bits.T4IE = 1;
    
    TRISCbits.TRISC3 = 1; /* Set RA15 for digital input*/
    CNPUCbits.CNPUC3 = 0; /* Disable RC3 pull up resistor */
    CNENCbits.CNIEC3 = 1; /* Enable RC3 change notification */
    IEC1bits.CNCIE = 1; /* Enable RC3 CN interrupts */
    IFS1bits.CNCIF=0 ;/* Check if CN on BTND - RC3 */
    
    TRISGbits.TRISG9 = 1; /* Set RG9 for digital input*/
    CNPUGbits.CNPUG9 = 0; /* Disable RG9 pull up resistor */
    CNENGbits.CNIEG9 = 1; /* Enable RG9 change notification */
    
    TRISGbits.TRISG8 = 1; /* Set RG8 for digital input*/
    CNPUGbits.CNPUG8 = 0; /* Disable RG8 pull up resistor */
    CNENGbits.CNIEG8 = 1; /* Enable RG8 change notification */
    
    TRISGbits.TRISG7 = 1; /* Set RG7 for digital input*/
    CNENGbits.CNIEG7 = 1; /* Enable RG7 change notification */
    CNPUGbits.CNPUG7 = 0; /* Disable RG8 pull up resistor */
    
    CNCONGbits.ON = 1; /* Set RA15 change notification on */
    IEC1bits.CNGIE = 1; /* Enable Rg CN interrupts */
    CNCONCbits.ON = 1; /* Set RA15 change notification on */
    IEC1bits.CNCIE = 1; /* Enable Rc CN interrupts */
    /* Set CN interrupt level */
    IPC8bits.CNIP = 4; /* Set CN interrupt group level 1 */
    IPC8bits.CNIS = 0; /* Set CN interrupt sub group level 0 */
    IFS1bits.CNGIF=0 ;/* Check if CN on BTND - Rg */
    IFS1bits.CNCIF=0 ;/* Check if CN on BTND - Rc */

    INTCONbits.MVEC=1;//vector interrupt
    IPTMR=0;//INTERRUPT PROXIMITY TIMER REGISTER
    T4CONbits.ON = 1;   //turn on Timer1
    asm("ei");//on interrupt    
}

void get_key_pressed()
{
    
//    char chars[16] = {'A','B','C','D','3','6','9','E','2','5','8','F','1','4','7','0'};
    uint8_t i;
    PORTCbits.RC2=0;
    PORTCbits.RC1=1;
    PORTCbits.RC4=1;
    PORTGbits.RG6=1;
    arr[0] = !PORTGbits.RG9;
    arr[1] = !PORTGbits.RG8;
    arr[2] = !PORTGbits.RG7;
    arr[3] = !PORTCbits.RC3;

    PORTCbits.RC2=1;
    PORTCbits.RC1=0;
    PORTCbits.RC4=1;
    PORTGbits.RG6=1;
    arr[4] = !PORTGbits.RG9;
    arr[5] = !PORTGbits.RG8;
    arr[6] = !PORTGbits.RG7;
    arr[7] = !PORTCbits.RC3;

    PORTCbits.RC2=1;
    PORTCbits.RC1=1;
    PORTCbits.RC4=0;
    PORTGbits.RG6=1;
    arr[8] = !PORTGbits.RG9;
    arr[9] = !PORTGbits.RG8;
    arr[10] = !PORTGbits.RG7;
    arr[11] = !PORTCbits.RC3;

    PORTCbits.RC2=1;
    PORTCbits.RC1=1;
    PORTCbits.RC4=1;
    PORTGbits.RG6=0;
    arr[12] = !PORTGbits.RG9;
    arr[13] = !PORTGbits.RG8;
    arr[14] = !PORTGbits.RG7;
    arr[15] = !PORTCbits.RC3;
    
    PORTCbits.RC2=0;
    PORTCbits.RC1=0;
    PORTCbits.RC4=0;
    PORTGbits.RG6=0;
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
    while(PORTEbits.RE7); // BF 
    PORTDbits.RD5=RD; 
    PORTBbits.RB15=RS;
    TRISE=STATUS_TRISE;   
}

void write_lcd_control(uint8_t data)
{
    PORTBbits.RB15=0;//rs=0
    PORTDbits.RD5=0;//w=0
    PORTE=data;
    PORTDbits.RD4=1;
    PORTDbits.RD4=0;
}

void write_empty_to_lcd()
{
    int i;
    PORTBbits.RB15=1;//rs=0
    PORTDbits.RD5=0;//w=0
    for( i = 0 ; i < 16 ; i++ )
    {
        PORTE=' ';
        PORTDbits.RD4=1;
        PORTDbits.RD4=0;
        busy();
    }
}

void write_to_lcd(uint8_t idx, float voltage)
{
    int i;    
        
//    if(last_det == idx)
//        return;
//
//    last_det = idx;
    write_lcd_control(0xc0);
    busy();
    write_empty_to_lcd();
    write_lcd_control(0xc0);
    busy();

    PORTBbits.RB15=1;//rs=0
    PORTDbits.RD5=0;//w=0
    
    char str[64] =  {0};
    sprintf(str,modes[idx],voltage);
    for ( i = 0 ; i < strlen(str) ; i++ )
    {
        PORTE=str[i];
        PORTDbits.RD4=1;
        PORTDbits.RD4=0;
        busy();
    }
}

void setup_keyboard()
{
    PORTCbits.RC2=0;
    PORTCbits.RC1=0;
    PORTCbits.RC4=0;
    PORTGbits.RG6=0;
}

void lcd_setup()
{
    TRISBbits.TRISB15 = 0; // RB15 (DISP_RS) set as an output
    ANSELBbits.ANSB15 = 0; // disable analog functionality on RB15 (DISP_RS)
    TRISDbits.TRISD5 = 0; // RD5 (DISP_RW) set as an output
    TRISDbits.TRISD4 = 0; // RD4 (DISP_EN) set as an output
    //TRISEbits.TRISE0 = 1; // RE0 (DB0) set as input (change 1 to 0 for
    TRISE &= 0xff00;
    PORTBbits.RB15=0;//rs=0
    PORTDbits.RD5=0;//w=0
    ANSELEbits.ANSE7 = 0;
    
    char control[]={0x38,0x38,0x38,0xe,0x6,0x1};
    int i;
    for (i=0;i<6;i++)
    {
        PORTE=control[i];   
        PORTDbits.RD4=1;
        PORTDbits.RD4=0;
        busy();
    }
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL4) PinChangeHandler(void)
{
    //get_key_pressed();
    PORTA = 0xff;
    IFS1bits.CNGIF=0 ;/* Check if CN on BTND - RA15 */
    IFS1bits.CNCIF=0 ;/* Check if CN on BTND - RA15 */
}

void __ISR(_TIMER_4_VECTOR, ipl2auto) Timer4SR(void); 
    
void __ISR(_TIMER_4_VECTOR, ipl2) Timer4SR(void) 
{   
    if(arr[12])
        write_to_lcd(1,voltage);
    else if(arr[8])
        write_to_lcd(0,voltage);
    IFS0bits.T4IF = 0;
}

void ADC_Init()
{
    AD1CON1	=0; 
    AD1CON1bits.SSRC = 7;   // Internal counter ends sampling and starts conversion (auto convert)
    AD1CON1bits.FORM = 0;   // Integer 16-bit
	// Setup for manual sampling
    AD1CSSL	=0;
    AD1CON3	=0x0002;     // ADC Conversion Clock Select bits: TAD = 6 TPB
    AD1CON2	=0;
    AD1CON2bits.VCFG = 0;   // Voltage Reference Configuration bits: VREFH = AVDD and VREFL = AVSS
	// Turn on ADC
    AD1CON1bits.ON = 1;
} 

unsigned int ADC_AnalogRead(unsigned char analogPIN)
{
    int adc_val = 0;
    
    IEC0bits.T2IE = 0;
    AD1CHS = analogPIN << 16;       // AD1CHS<16:19> controls which analog pin goes to the ADC
 
    AD1CON1bits.SAMP = 1;           // Begin sampling
    while( AD1CON1bits.SAMP );      // wait until acquisition is done
    while( ! AD1CON1bits.DONE );    // wait until conversion is done
 
    adc_val = ADC1BUF0;
    IEC0bits.T2IE = 1;
    return adc_val;
}

void main(void)
{    
    int x,mode = 0 , y = 0;
    setup();
    lcd_setup();
    setup_keyboard();
//    setup_interrupts();
    TRISA &= 0xff00;
    TRISBbits.TRISB2 = 1;
    ANSELBbits.ANSB2 = 1;
    ADC_Init();
    while(1)
    {
        get_key_pressed();
        if (arr[8]) //btn2
            mode = 1;
        else if (arr[12]) //btn1
            mode = 0;
        x = ADC_AnalogRead(mode ? 2 : 4);
        voltage = (x * (3.3/1023.0)); //IN analog RB2
        write_to_lcd(mode,voltage);
        //PORTA= x/4;
        
        y = (int)(x * (8.0/1023.0));
        PORTA = (1 << y) - 1 ;
        for(x = 0 ; x < 64000 ; x++);
    }
}
