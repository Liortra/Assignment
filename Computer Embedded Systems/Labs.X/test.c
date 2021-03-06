#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include <string.h>
#include <p32xxxx.h>
#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF
#pragma config FNOSC = FRCPLL
#pragma config FSOSCEN = OFF
#pragma config POSCMOD = EC
#pragma config OSCIOFNC = ON
#pragma config FPBDIV =     DIV_1
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLODIV = DIV_1

#define SLOW_SPEED  86000
#define FAST_SPEED  32000

unsigned int counter = 0;
int last_mode = -1;

int check_keyboard(int mode_, int * speed, int * reverse);
void updateText(int current_mode);
void playSound(int mult);
void writeToLCD(char str[], int up);
void swing(int speed, int reverse);
void hexaCounter(int speed, int reverse);
void sideToSide(int speed, int reverse);
void delay(int speed);
void change_mode(int xy , int * mode, int * speed, int * reverse);
void stopLight(int * current_mode, int * speed , int * reverse);

int in_y(int a) {
    int j = 1, flag = 0;
    if (!PORTCbits.RC3) {
        flag = 1;
        j = 1;
    } else if (!PORTGbits.RG7) {
        flag = 1;
        j = 2;
    } else if (!PORTGbits.RG8) {
        flag = 1;
        j = 3;
    } else if (!PORTGbits.RG9) {
        flag = 1;
        j = 4;
    }
    if (flag == 0)
        return (0xff);
    else
        return (j | (a << 4));
}

void main(void) {
    TRISA &= 0xff00; //led
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
    PORTBbits.RB15 = 0; //rs=0
    PORTDbits.RD5 = 0; //w=0
    int current_mode = -1;
    int speed = 0;
    int reverse = 1;

    while (1) {
        current_mode = check_keyboard(current_mode, &speed, &reverse);
        if (current_mode == 7)
            break;
        if (current_mode == 6)
            playSound(5);
        if(current_mode == 5)
            stopLight(&current_mode,&speed, &reverse);
        if (current_mode == 2 || last_mode == 2)
            swing(speed, reverse);
        else if (current_mode == 1 || last_mode == 1)
            sideToSide(speed, reverse);
        else if (current_mode == 0 || last_mode == 0)
            hexaCounter(speed, reverse);  
    }
}

int check_keyboard(int mode_, int * speed, int * reverse) {
    static int i;
    int xy;
    int mode = mode_;
    i = 1;
    PORTCbits.RC2 = 1;
    PORTCbits.RC1 = 1;
    PORTCbits.RC4 = 1;
    PORTGbits.RG6 = 1;
    
    PORTCbits.RC2 = 0;
    xy = in_y(1);
    if (xy != 0xff){
        change_mode(xy,&mode,speed,reverse);
        return mode;
    }
    PORTCbits.RC2 = 1;
    i = 2;
    PORTCbits.RC1 = 0;
    xy = in_y(2);
    if (xy != 0xff){
        change_mode(xy,&mode,speed,reverse);
        return mode;
    }
    PORTCbits.RC1 = 1;
    i = 3;
    PORTCbits.RC4 = 0;
    xy = in_y(3);
    if (xy != 0xff){
        change_mode(xy,&mode,speed,reverse);
        return mode;
    }
    PORTCbits.RC4 = 1;
    i = 4;
    PORTGbits.RG6 = 0;
    xy = in_y(4);
    if (xy != 0xff){
        change_mode(xy,&mode,speed,reverse);
        return mode;
    }
    PORTGbits.RG6 = 1;
    PORTA = xy;
    change_mode(xy,&mode,speed,reverse);
    return mode;
}

void change_mode(int xy , int * mode, int * speed, int * reverse){
    switch (xy) {
        case 0x44://1
            playSound(10);
            writeToLCD("                ", 1);
            writeToLCD("     mode 1  -  ", 0);
            *mode = 1;
            last_mode = 1;
            break;
        case 0x34://2
            playSound(9);
            writeToLCD("                ", 1);
            writeToLCD("     mode 2  -  ", 0);
            *mode = 2;
            last_mode = 2;
            break;
        case 0x24://3
            playSound(8);
            writeToLCD("                ", 1);
            writeToLCD("     mode 3  -  ", 0);
            *reverse = !*reverse;
            break;
        case 0x43://4
            playSound(7);
            writeToLCD("                ", 1);
            writeToLCD("     mode 4  -  ", 0);
            *speed = !*speed;
            break;
        case 0x33://5
            playSound(6);
            writeToLCD("                ", 1);
            writeToLCD("     mode 5  -  ", 0);
            *mode = 5;
            break;
        case 0x23://6
            playSound(5);
            writeToLCD("                ", 1);
            writeToLCD("     mode 6  -  ", 0);
            *mode = 6;
            break;
        case 0x42://7
            playSound(4);
            writeToLCD("                ", 1);
            writeToLCD("     mode 7  -  ", 0);
            *mode = 7;
            break;
        case 0x32://8
            playSound(3);
            writeToLCD("                ", 1);
            writeToLCD("     mode 8  -  ", 0);
            *mode = 8;
            break;
        case 0x22://9
            playSound(2);
            writeToLCD("                ", 1);
            writeToLCD("     mode 9  -  ", 0);
            *mode = 9;
            break;
        case 0x41://0
            playSound(1);
            writeToLCD("                ", 1);
            writeToLCD("     mode 0  -  ", 0);
            *mode = 0;
            last_mode = 5;
            break;
    };
}

void stopLight(int * current_mode, int * speed , int * reverse){
    while(check_keyboard(current_mode, &speed, &reverse) == 5){ 
    };
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
        busy(86000);
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

#define DELAY 80000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char mode[16];
char modeDescription[16];
int currentSwitch = -1;
int currentSpeed = -1;
int SpeedLastFrame = -2;
int hasChangedSpeed = 1;
int status = -1;

void sideToSide(int speed, int reverse) {
    int i;
    if (reverse) // sw3
    {
        PORTA = 0x80;
        for (i = 0; i < 8; i++) {
            delay(speed);
            if (PORTA == 0) {
                PORTA = 0x80;
            } else {
                PORTA /= 2;
            }
        }
    } else {
        PORTA = 0x1;
        for (i = 0; i < 8; i++) {
            delay(speed);
            if (PORTA % 0x100 == 0) {
                PORTA = 0x1;
            } else {
                PORTA *= 2;
            }
        }
    }
}

int checkStatus() {
    int check2 = PORTBbits.RB9 * 128 + PORTBbits.RB10 * 64 + PORTBbits.RB11 * 32 +
            PORTDbits.RD14 * 16 + PORTDbits.RD15 * 8
            + PORTFbits.RF4 * 4 + PORTFbits.RF5 * 2 + PORTFbits.RF3;
    if (check2 != status) {
        status = check2;
        return 1;
    }
    return 0;
}

void updateText(int current_mode) {
    if (checkStatus()) {
        if (current_mode == 7) //sw 7
        {
            writeToLCD("Mode 7:         ", 1);
            writeToLCD("      Exit      ", 0);
        } else if (current_mode == 5) {
            writeToLCD("Mode 5:         ", 1);
            writeToLCD("     Pause      ", 0);
        } else if (current_mode == 6) // sw6
        {
            writeToLCD("Mode 6:         ", 1);
            if (PORTDbits.RD14) //fast                    
                writeToLCD("   High Beep    ", 0);
            else
                writeToLCD("   Low Beep   ", 0);
        } else if (current_mode == 2) //switch 2
        {
            writeToLCD("Mode 2:         ", 1);
            if (PORTDbits.RD15) // reverse
            {
                if (PORTDbits.RD14) //fast                    
                    writeToLCD("Swing Down Fast ", 0);
                else
                    writeToLCD("Swing Down Slow ", 0);
            } else //normal
            {
                if (PORTDbits.RD14) //fast                    
                    writeToLCD(" Swing Up Fast  ", 0);
                else
                    writeToLCD(" Swing Up Slow  ", 0);
            }
        } else if (current_mode == 1)// switch 1
        {
            writeToLCD("Mode 1:         ", 1);
            if (PORTDbits.RD15) // reverse
            {
                if (PORTDbits.RD14) //fast                    
                    writeToLCD("Shift Right Fast ", 0);
                else
                    writeToLCD("Shift Right Slow ", 0);
            } else //normal
            {
                if (PORTDbits.RD14) //fast                    
                    writeToLCD("Shift Left Fast ", 0);
                else
                    writeToLCD("Shift Left Slow ", 0);
            }

        } else if (current_mode == 0) // RF3 (SW0) configured as input
        {
            writeToLCD("Mode 0:         ", 1);
            if (PORTDbits.RD15) // reverse
            {
                if (PORTDbits.RD14) //fast                    
                    writeToLCD("Count Down Fast ", 0);
                else
                    writeToLCD("Count Down Slow ", 0);
            } else //normal
            {
                if (PORTDbits.RD14) //fast                    
                    writeToLCD("  Count Up Fast ", 0);
                else
                    writeToLCD("  Count Up Slow ", 0);
            }
            printf("counter == %d",counter);
        }
    }
}

void updateSpeed() {
    if (PORTDbits.RD14 != currentSpeed) {
        currentSpeed = PORTDbits.RD14;
        hasChangedSpeed = 1;
    } else {
        hasChangedSpeed = 0;
    }
}

void swing(int speed, int reverse) {
    int i;
    int left, right;
    left = 0x1;
    right = 0x8;

    for (i = 0; i < 4; i++) {
        if (reverse)
            PORTA = right * 16 | left;
        else// sw3
            PORTA = left * 16 | right;
        delay(speed);
        left *= 2;
        right /= 2;
    }
}

void hexaCounter(int speed, int reverse) {
    PORTA = counter;
    if (!reverse) { //SW3
        counter++;
    } else {
        counter--;
    }
    delay(speed);
}

void delay(int speed) {
    int j;
    if (speed) {
        for (j = 0; j < FAST_SPEED; j++);
    } else //speed off
    {
        for (j = 0; j < SLOW_SPEED; j++);
    }
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
    } while (PORTEbits.RE7); // BF ?????
    PORTDbits.RD5 = RD;
    PORTBbits.RB15 = RS;
    TRISE = STATUS_TRISE;
}