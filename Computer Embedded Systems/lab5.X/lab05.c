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

#include <xc.h>  
#include <sys/attribs.h>
#include <string.h>
#include <stdlib.h>
int psika = 0;
int jj;
int xy;

void __ISR(_CHANGE_NOTICE_VECTOR, IPL4) PinChangeHandler(void) {
    //PORTAbits.RA4^=1;
    psika = 1;

    PORTCbits.RC2 = 1;
    PORTCbits.RC1 = 1;
    PORTCbits.RC4 = 1;
    PORTGbits.RG6 = 1;
    PORTCbits.RC2 = 0;
    xy = in_y(1);
    if (xy != 0xff);
    else {
        PORTCbits.RC2 = 1;
        PORTCbits.RC1 = 0;
        xy = in_y(2);
        if (xy != 0xff);
        else {
            PORTCbits.RC1 = 1;
            PORTCbits.RC4 = 0;
            xy = in_y(3);
            if (xy != 0xff);
            else {
                PORTCbits.RC4 = 1;
                PORTGbits.RG6 = 0;
                xy = in_y(4);
                PORTGbits.RG6 = 1;
            }
        }
    }
    PORTCbits.RC2 = 0;
    PORTCbits.RC1 = 0;
    PORTCbits.RC4 = 0;
    PORTGbits.RG6 = 0;
    PORTCbits.RC2 = 0;

    for (jj = 0; jj < 64000; jj++);
    IFS1bits.CNGIF = 0; /* Check if CN on BTND - RA15 */
    IFS1bits.CNCIF = 0; /* Check if CN on BTND - RA15 */
    // psika =1;
}

void __ISR(_TIMER_1_VECTOR, ipl7) timer1_interrupt(void) {
    //PORTAbits.RA7^=1;

    IFS0bits.T1IF = 0;
}

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

int check_keyboard(int mode_, int * speed, int * reverse) {
    static int i = 0;
    int mode = mode_;
    i = 1;
    int j;
    
    switch (xy) {
        case 0x44://1
            //  playSound(10);
            if (mode != 1) {
                writeToLCD("                ", 1);
                writeToLCD("     mode 1  -  ", 0);
                mode = 1;
            }
            break;
        case 0x34://2
            // playSound(9);
            if (mode != 2) {
                writeToLCD("                ", 1);
                writeToLCD("     mode 2  -  ", 0);
                mode = 2;
            }
            break;
        case 0x24://3
            playSound(8);
            writeToLCD("                ", 1);
            writeToLCD("     mode 3  -  ", 0);
            *reverse = !*reverse;
            break;
        case 0x43://4
            for (j = 0; j < 10000; j++);
            *speed = !*speed;
            break;
        case 0x33://5
            // playSound(6);
            if (mode != 5) {
                writeToLCD("                ", 1);
                writeToLCD("     mode 5  -  ", 0);
                mode = 5;
            }
            break;
        case 0x23://6
            if (mode != 6) {
                writeToLCD("                ", 1);
                writeToLCD("     mode 6  -  ", 0);
                mode = 6;
            }
            break;
        case 0x42://7
            // playSound(4);
            writeToLCD("                ", 1);
            writeToLCD("     mode 7  -  ", 0);
            mode = 7;
            break;
        case 0x32://8
            // playSound(3);
            writeToLCD("                ", 1);
            writeToLCD("     mode 8  -  ", 0);
            mode = 8;
            break;
        case 0x22://9
            //playSound(2);
            writeToLCD("                ", 1);
            writeToLCD("     mode 9  -  ", 0);
            mode = 9;
            break;
        case 0x41://0
            //playSound(1);
            writeToLCD("                ", 1);
            writeToLCD("     mode 0  -  ", 0);
            mode = 0;
            break;
    };
    return mode;
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

#define DELAY 160000
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
    if (!reverse) // sw3
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
    int n;
    if (reverse > 0)
        n = -1;
    else
        n = 1;
    PORTA = (PORTA + n) % 256;
    delay(speed);

}

void delay2() {
    int j;
    while (PORTBbits.RB11) {
        continue;
    }
    if (PORTDbits.RD14) {
        for (j = 0; j < DELAY / 400; j++);
    } else {
        for (j = 0; j < DELAY / 100; j++);
    }
}

void delay(int speed) {
    int j;
    if (speed > 0) {
        for (j = 0; j < DELAY / 2; j++);
    } else //speed off
    {
        for (j = 0; j < DELAY; j++);
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

void main() {
    int j;

    TRISA &= 0xff00; //led
    TRISCbits.TRISC2 = 0; //RC2            
    TRISCbits.TRISC1 = 0; //RC1           
    TRISCbits.TRISC4 = 0; //RC4            
    TRISGbits.TRISG6 = 0; //RG6
    ANSELGbits.ANSG6 = 0; //???????
    TRISCbits.TRISC3 = 1; //RC3
    CNPUCbits.CNPUC3; //???               
    TRISGbits.TRISG7 = 1; //RG7
    ANSELGbits.ANSG7 = 0; //???????
    CNPUGbits.CNPUG7; //???                  
    TRISGbits.TRISG8 = 1; //RG8
    ANSELGbits.ANSG8 = 0; //???????
    CNPUGbits.CNPUG8; //?????
    TRISGbits.TRISG9 = 1; //RG9
    ANSELGbits.ANSG9 = 0; //???????
    CNPUGbits.CNPUG9; //?????
    LATGSET = 9;

    PORTCbits.RC2 = 0;
    PORTCbits.RC1 = 0;
    PORTCbits.RC4 = 0;
    PORTGbits.RG6 = 0;

    T1CONbits.ON = 0;

    T1CONbits.TGATE = 0;
    T1CONbits.TCS = 0;
    T1CONbits.TCKPS0 = 1;
    T1CONbits.TCKPS1 = 1;
    T1CONbits.TSYNC = 1;
    TMR1 = 0;
    PR1 = 0Xffff;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    IPC1bits.T1IP = 7; //             priority
    IPC1bits.T1IS = 0;
    //================key

    TRISCbits.TRISC3 = 1; /* Set RA15 for digital input*/
    CNPUCbits.CNPUC3 = 0; /* Disable RC3 pull up resistor */
    CNENCbits.CNIEC3 = 1; /* Enable RC3 change notification */
    IEC1bits.CNCIE = 1; /* Enable RC3 CN interrupts */
    IFS1bits.CNCIF = 0; /* Check if CN on BTND - RC3 */

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
    IFS1bits.CNGIF = 0; /* Check if CN on BTND - Rg */
    IFS1bits.CNCIF = 0; /* Check if CN on BTND - Rc */
    //------
    INTCONbits.MVEC = 1; //vector interrupt
    IPTMR = 0; //INTERRUPT PROXIMITY TIMER REGISTER
    T1CONbits.ON = 1;
    asm("ei"); //on interrupt

    int current_mode = -1;
    int speed = 0;
    int reverse = 0;
    while (1) {
        if (psika) {
            current_mode = check_keyboard(current_mode, &speed, &reverse);
            psika = 0;
        }
        if (current_mode == 7)
            ; //  break;
        if (current_mode == 5)
            ;
        if (current_mode == 6)
            playSound(6);
        if (current_mode == 2)
            swing(speed, reverse);
        else if (current_mode == 1)
            sideToSide(speed, reverse);
        else if (current_mode == 0)
            hexaCounter(speed, reverse);
    }
} 