#include <xc.h>
#include <string.h>
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

#define LCD_CLEAR           0x01
#define LCD_HOME            0x02
#define ENTRY_MODE(INC,S)   (0x04 | (INC << 2) | (S))

//define helper variables:

unsigned int menifa[] = {0x18,0x24,0x42,0x81};
unsigned int menifa_index = 0;
unsigned char v_shift = 1;
unsigned int counter = 0;
unsigned int speaker_counter = 0;
unsigned int check_var = 128;

unsigned int last_mode = -1;
unsigned int last_det = -1;
unsigned int played = 0;

char *modes[] = {"Mode 0:","Mode 1:","Mode 2:","Mode 5:","Mode 6:","Mode 7:"};

char *details[] = {"Counter Up Slow","Counter Up Fast","Shift Right Slow","Swing Up Slow","Counter Down Slow","Shift Left Slow","Swing Down Slow","Counter Down Fast","Shift Left Fast","Shift Left Fast","Halt","Beep Mode","Exit"};

void write_lcd_data(unsigned int data, unsigned int line, unsigned int offset);
void write_to_lcd(unsigned int idx, unsigned int line);
void busy(void);
void write_lcd_control(unsigned int data);
void clear_lcd();

void play_sound()
{
    if (PORTBbits.RB10) //SW6
    {
        if(!played)
        {
            write_to_lcd(4,0);
            write_to_lcd(12,1);
        }
        played = 1;
        if(!speaker_counter++%check_var)
            PORTBbits.RB14 = !PORTBbits.RB14; //change RB14
    }
    else
    {
        if(played)
            clear_lcd();
        played = 0;
    }
    check_var = check_var<<1;    
}

//void clear_lcd()
//{
//    write_lcd_control(0x01);
//    busy();
//}

void delay(unsigned int time)
{
    unsigned int i;
    for(i = 0 ; i < time && !PORTBbits.RB11 ; i++)
        play_sound();
}

void write_lcd_control(unsigned int data)
{
    PORTBbits.RB15=0;//rs=0
    PORTDbits.RD5=0;//w=0
    PORTE=data;
    PORTDbits.RD4=1;
    PORTDbits.RD4=0;
}

void write_lcd_data(unsigned int data, unsigned int line, unsigned int offset)
{
    if(data == last_mode)
        return;
    last_mode = data;
    unsigned int to = 0x80;
    if(line)
        to = 0xc0;
    to += offset;
    write_lcd_control(to);
    delay(3200);
    PORTBbits.RB15=1;//rs=1
    PORTDbits.RD5=0;//w=0
    PORTE=data;
    PORTDbits.RD4=1;
    PORTDbits.RD4=0;
    delay(3200);
}

void write_to_lcd(unsigned int idx, unsigned int line)
{
    int i;
    
    if(line) // line == 1
    {
        if(last_det == idx)
            return;
        last_det = idx;
        write_lcd_control(0xc0);
        busy();
    }
    else // line == 0
    {
        if(last_mode == idx)
            return;
        last_mode = idx;
        write_lcd_control(0x80);
        busy();
    }
    
    PORTBbits.RB15=1;//rs=0
    PORTDbits.RD5=0;//w=0
    
    char* str = (line == 0) ? modes[idx] : details[idx];
    
    for (i = 0 ; i < strlen(str) ; i++)
    {
        PORTE=str[i];
        PORTDbits.RD4=1;
        PORTDbits.RD4=0;
        busy();
    }
}

void lcd_setup()
{
    TRISBbits.TRISB15 = 0; // RB15 (DISP_RS) set as an output
    ANSELBbits.ANSB15 = 0; // disable analog functionality on RB15 (DISP_RS)
    TRISDbits.TRISD5 = 0; // RD5 (DISP_RW) set as an output
    TRISDbits.TRISD4 = 0; // RD4 (DISP_EN) set as an output
    TRISE&=0xff00;
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
        delay(32000);
    }
    write_to_lcd(0, 0);
}

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
    lcd_setup();
    
    while(1)
    {
        play_sound();
        
        if(PORTFbits.RF4) //SW2
        {
            write_to_lcd(2,0);
            write_to_lcd(3,1);
            while(PORTBbits.RB11) //sw5
            {
                write_to_lcd(3,0);
                write_to_lcd(11,1);
            }  
            if (PORTDbits.RD15)  //sw3
            {
                write_to_lcd(6,1);
                PORTA = menifa[(menifa_index--)%4];
            }
            else
                PORTA = menifa[(menifa_index++)%4];
            delay(PORTDbits.RD14 ? FAST_SPEED : SLOW_SPEED); //sw4
        }
        
        else if(PORTFbits.RF5) //SW1
        {
            write_to_lcd(1,0);
            write_to_lcd(2,1);
            while(PORTBbits.RB11) //sw5
            {
                write_to_lcd(3,0);
                write_to_lcd(11,1);
            } 
            PORTA = v_shift;
            if (!PORTDbits.RD15) //SW3
            {
                write_to_lcd(5,1);
                v_shift <<= 1;
                v_shift = v_shift == 0 ? 1 : v_shift;
            }
            else
            {
                v_shift >>= 1;
                v_shift = v_shift == 0 ? 0x80 : v_shift;
            }
            delay(PORTDbits.RD14 ? FAST_SPEED : SLOW_SPEED); //sw4
        }
        
        else if(PORTFbits.RF3) //SW0
        {
            write_to_lcd(0,0);
            while(PORTBbits.RB11) //sw5
            {
                write_to_lcd(3,0);
                write_to_lcd(11,1);
            } 
            PORTA = counter;
            if (!PORTDbits.RD15){ //SW3
                counter++;
                write_to_lcd(PORTDbits.RD14 ? 1 : 0,1);
            }
            else
            {
                counter--;
                write_to_lcd(PORTDbits.RD14 ? 7 : 4,1);
            }
            delay(PORTDbits.RD14 ? FAST_SPEED : SLOW_SPEED); //sw4
        }
        
        else
            reset();
        
        if (PORTBbits.RB9) //sw7
        {
            clear_lcd();
            write_to_lcd(5,0);
            reset();
            exit(0);
        }
    }
}

void clear_lcd(void)
{
    char control = 0x01;
    PORTBbits.RB15 = 0;
    PORTE = control;
    PORTDbits.RD4 = 1;
    PORTDbits.RD4 = 0;
    busy(); 
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