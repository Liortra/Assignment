#include <xc.h>
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
void delay();
void delay2();

void PlaySound() {
    if (PORTBbits.RB14)
        PORTBbits.RB14--;
    else
        PORTBbits.RB14++;
    delay2();
}

void sideToSide() {
    int i;
    if (PORTDbits.RD15) // sw3
    {
        PORTA = 0x80;
        for (i = 0; i < 8; i++) {
            delay();
            if (PORTA == 0) {
                PORTA = 0x80;
            } else {
                PORTA /= 2;
            }
        }
    } else {
        PORTA = 0x1;
        for (i = 0; i < 8; i++) {
            delay();
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

void updateText() {
    if (checkStatus()) {
        if (PORTBbits.RB9) //sw 7
        {
            writeToLCD("Mode 7:         ", 1);
            writeToLCD("      Exit      ", 0);
        } else if (PORTBbits.RB11) {
            writeToLCD("Mode 5:         ", 1);
            writeToLCD("     Pause      ", 0);
        } else if (PORTBbits.RB10) // sw6
        {
            writeToLCD("Mode 6:         ", 1);
            if (PORTDbits.RD14) //fast                    
                writeToLCD("   High Beep    ", 0);
            else
                writeToLCD("   Low Beep   ", 0);
        } else if (PORTFbits.RF4) //switch 2
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

        } else if (PORTFbits.RF5)// switch 1
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
        } else if (PORTFbits.RF3) // RF3 (SW0) configured as input
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

void swing() {
    int i;
    int left, right;
    left = 0x1;
    right = 0x8;
    for (i = 0; i < 4; i++) {
        if (PORTBbits.RB9 || PORTBbits.RB10 || hasChangedSpeed) // sw 4,6,7 instant stops
        {
            break;
        }
        if (PORTDbits.RD15)
            PORTA = right * 16 | left;
        else// sw3
            PORTA = left * 16 | right;
        delay();
        left *= 2;
        right /= 2;
    }
}

void hexaCounter() {
    int n;
    if (PORTDbits.RD15)
        n = -1;
    else
        n = 1;
    PORTA = (PORTA + n) % 256;
    delay();
}

void delay2() {
    int j;
    if (PORTDbits.RD14) {
        for (j = 0; j < DELAY / 4000; j++);
    } else {
        for (j = 0; j < DELAY / 1000; j++);
    }
}

void delay() {
    int j;
    updateText();
    while (PORTBbits.RB11) // sw5
    {
        continue;
    }
    if (PORTDbits.RD14) {
        for (j = 0; j < DELAY / 2; j++);
    } else //speed off
    {
        for (j = 0; j < DELAY; j++);
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
    } while (PORTEbits.RE7); // BF רגסטר
    PORTDbits.RD5 = RD;
    PORTBbits.RB15 = RS;
    TRISE = STATUS_TRISE;
}

void writeToLCD2() {
    int j, i;
    char control[] = {0x38, 0x38, 0x38, 0xe, 0x6, 0x1, 0x0c};
    TRISBbits.TRISB15 = 0; // RB15 (DISP_RS) set as an output
    ANSELBbits.ANSB15 = 0; // disable analog functionality on RB15 (DISP_RS)
    TRISDbits.TRISD5 = 0; // RD5 (DISP_RW) set as an output
    TRISDbits.TRISD4 = 0; // RD4 (DISP_EN) set as an output
    TRISE &= 0xff00;
    PORTBbits.RB15 = 0; //rs=0
    PORTDbits.RD5 = 0; //w=0
    ANSELEbits.ANSE7 = 0;
    for (i = 0; i < 6; i++) {
        PORTE = control[i];
        PORTDbits.RD4 = 1;
        PORTDbits.RD4 = 0;
        busy();
    }
    PORTBbits.RB15 = 1; //rs=0
    PORTDbits.RD5 = 0; //w=0
    for (i = 0; i < 16; i++) {
        PORTE = mode[i];
        PORTDbits.RD4 = 1;
        PORTDbits.RD4 = 0;
        busy();
    }
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

void func() {
    int j, i;
    char CG_father[32] = {0x11, 0x0b, 0x07, 0x05, 0x09, 0x09, 0x11, 0x17, //מ
        0x23, 0x23, 0x21, 0x21, 0x21, 0x21, 0x3f, 0x3f, //נ
        0x5f, 0x5f, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, //ח
        0x7f, 0x7f, 0x71, 0x71, 0x71, 0x71, 0x7f, 0x7f,}; //ם
    char father[4] = {3, 2, 1, 0}; // מנחם
    char control[7] = {0x38, 0x38, 0x38, 0xe, 0x6, 0x1, 0x40};

    PORTBbits.RB15 = 0; //rs=0
    PORTDbits.RD5 = 0; //w=0
    for (i = 0; i < 7; i++) {
        PORTE = control[i];
        PORTDbits.RD4 = 1;
        PORTDbits.RD4 = 0;
        busy();
    }
    PORTBbits.RB15 = 1; //rs
    for (i = 0; i < 32; i++) {
        PORTE = CG_father[i];
        PORTDbits.RD4 = 1; //enable=1
        PORTDbits.RD4 = 0; //enable=0
        busy();
    }
    PORTBbits.RB15 = 0; //rs control
    if(PORTFbits.RF4)
        PORTE = 0x80; //DDRAM
    else
        PORTE = 0xc0; //DDRAM
    PORTDbits.RD4 = 1; //enable=1
    PORTDbits.RD4 = 0; //enable=0
    busy();
    PORTBbits.RB15 = 1; //rs 

    for (i = 0; i < 4; i++) {
        PORTE = father[i];
        PORTDbits.RD4 = 1; //enable=1
        PORTDbits.RD4 = 0; //enable=0
        busy();
    }
}

void main() {
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
    int str_len = 3;
    int counter = 0;
    int direction = 0;
    func();
    while (1) {
        int u;
        if (PORTBbits.RB9) //sw 7
            PlaySound();
        if (!PORTFbits.RF3) //sw0
        {
            direction = 1;
            int i;
            if (counter < (17) && counter >= 0) {
                for (i = 0; i < 320000; i++);
            } else {
                func();
                counter = 0;
                int j;
                for (j = 0; j < str_len + 3; j++) {
                    PORTBbits.RB15 = 0; //rs=0
                    PORTE = 0x18;
                    PORTDbits.RD4 = 1;
                    PORTDbits.RD4 = 0;
                }
            }
            PORTBbits.RB15 = 0; //rs=0
            if (direction > 0) {
                counter++;
                PORTE = 0x1c;
            } else {
                counter--;
                PORTE = 0x18;
            }
            PORTDbits.RD4 = 1;
            PORTDbits.RD4 = 0;
        } else {
            direction = -1;
            int i;
            if (counter < (17) && counter >= 0) {
                for (i = 0; i < 320000; i++);
            } else {
                func();
                counter = 17;
                int j;
                for (j = 0; j < str_len + 3; j++) {
                    PORTBbits.RB15 = 0; //rs=0
                    PORTE = 0x18;
                    PORTDbits.RD4 = 1;
                    PORTDbits.RD4 = 0;
                }
            }
            PORTBbits.RB15 = 0; //rs=0
            if (direction > 0) {
                counter++;
                PORTE = 0x1c;
            } else {
                counter--;
                PORTE = 0x18;
            }
            PORTDbits.RD4 = 1;
            PORTDbits.RD4 = 0;
        }
    }
}