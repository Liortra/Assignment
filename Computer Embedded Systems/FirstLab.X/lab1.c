#include <xc.h>
#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF
#pragma config FNOSC =FRCPLL
#pragma config FSOSCEN =	OFF
#pragma config POSCMOD =	EC
#pragma config OSCIOFNC =	ON
#pragma config FPBDIV =     DIV_1
#pragma config FPLLIDIV =	DIV_2
#pragma config FPLLMUL =	MUL_20
#pragma config FPLLODIV =	DIV_1

#define SLOW_SPEED  86000
#define FAST_SPEED  32000
#define ENTRY_MODE(INC,S)   (0x04 | (INC << 2) | (S))

void setup()
{
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
    TRISBbits.TRISB14 = 0; //speaker configured output
    ANSELBbits.ANSB14 = 0; //speaker disabled analog
}

//define helper variables:

unsigned int menifa[] = {0x18,0x24,0x42,0x81};
unsigned int menifa_index = 0;
unsigned char v_shift = 1;
unsigned int counter=0;
unsigned int speaker_counter = 0;
unsigned int check_var = 1;

void play_sound()
{
    if (PORTBbits.RB10) //SW6
        if(speaker_counter++%check_var)
            PORTBbits.RB14 = !PORTBbits.RB14; //change RB14
    check_var = check_var<<1;
}

void delay(unsigned int time)
{
    unsigned int i;
    for(i = 0 ; i < time && !PORTBbits.RB11 ; i++)
        play_sound();
}

void reset()
{
    menifa_index = 0;
    v_shift = 1;
    PORTA &= 0xff00;
    counter = 0;
}

void main()
{
    TRISA &= 0xff00;
    PORTA =0x00;
    setup();
    
    while(1)
    {
        play_sound();
        if(PORTFbits.RF4) //SW2
        {
            while(PORTBbits.RB11);
            if (PORTDbits.RD15)
                PORTA = menifa[(menifa_index--)%4];
            else
                PORTA = menifa[(menifa_index++)%4];
            delay(PORTDbits.RD14 ? FAST_SPEED : SLOW_SPEED);
        }
        else if(PORTFbits.RF5) //SW1
        {
            while(PORTBbits.RB11);
            PORTA = v_shift;
            if (!PORTDbits.RD15) //SW3
            {
                v_shift <<= 1;
                v_shift = v_shift == 0 ? 1 : v_shift;
            }
            else
            {
                v_shift >>= 1;
                v_shift = v_shift == 0 ? 0x80 : v_shift;
            }
            delay(PORTDbits.RD14 ? FAST_SPEED : SLOW_SPEED);
        }
        else if(PORTFbits.RF3) //SW0
        {
            while(PORTBbits.RB11);
            PORTA = counter;
            if (!PORTDbits.RD15) //SW3
                counter++;
            else
                counter--;
            delay(PORTDbits.RD14 ? FAST_SPEED : SLOW_SPEED);
        }
        else
            reset();
        
        if (PORTBbits.RB9)
        {
            reset();
            exit(0);
        }
    }
}