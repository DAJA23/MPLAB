/* Main.c file generated by New Project wizard
 *
 * Created:   mar. d�c. 22 2020
 * Processor: PIC16F887
 * Compiler:  MPLAB XC8
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#define A1 RB0
#define A2 RB1
#define A3 RB2
#define A4 RB3
#define B1 RB4
#define B2 RB5
#define B3 RB6
#define B4 RB7
#define LA1 RD0
#define LA2 RD1
#define LA3 RD2
#define LA4 RD3
#define LB1 RD4
#define LB2 RD5
#define LB3 RD6
#define LB4 RD7


#define _XTAL_FREQ  20e6
#include <string.h>
#include <xc.h>
int phase = 3;
char t;
int etape = 0;
short cnt1, cnt2, cnt3;
// Fr�quence de transmission 9600 Baud
//Mode 8bits

void uart_unit(void) {
    T0CS = 0;
    PSA = 0;
    PS0 = 1;
    PS1 = 1;
    T0IE = 1;
    PS2 = 1;
    TXEN = 1;
    SYNC = 0;
    BRGH = 1;
    BRG16 = 0;
    SPBRG = 129;
    SPEN = 1;
    CREN = 1; //activation de la reception de donn�es
    GIE = 1; //interruption globale 
    PEIE = 1; //interruprion des p�riphiques 
    INTE = 1;
    RCIE = 1; //interruption pour la recption de donn�es
    RBIE = 1;
    IOCB = 0xFF; //Activation des interruptions sur les ports Bzoom 
}

void uart_txChar(unsigned char ch) {
    while (!TRMT);
    TXREG = ch;
}

void uart_txStr(unsigned const char *str) {
    while (*str != '\0') {
        uart_txChar(*str);
        str++;
    }
    uart_txChar('\r');
    uart_txChar('\n');
}
//Commentaire pour mieux comprendre le programme
//phase=0 veut dire aucune �quipe n'a la main

void interrupt jdk1() {
    if (RCIF) {
        RCIF = 0;
        switch (RCREG) {
            case 'A':
                    LB1 = 0;
                    LB2 = 0;
                    LB3 = 0;
                    LB4 = 0;
                    phase = 1;
                    uart_txStr("EA");
                break;
            case 'B':
                    LA1 = 0;
                    LA2 = 0;
                    LA3 = 0;
                    LA4 = 0;
                    phase = 2;
                    uart_txStr("EB");
                break;
            case '1':
                if (phase == 3 || phase == 1 || phase == 2 || phase == 4 || phase == 5 || phase == 6 || phase == 7) {
                    etape = 1; // ETAPE 1 du jeu
                    phase = 0; // phase d'initialisation
                }
                break;
            case '2':
                if (phase == 3 || phase == 1 || phase == 2 || phase == 4 || phase == 5 || phase == 6 || phase == 7) {
                    etape = 2; // ETAPE 2 du jeu
                    phase = 0;
                }
                break;
            case '3':
                if (phase == 3 || phase == 1 || phase == 2 || phase == 4 || phase == 5 || phase == 6 || phase == 7) {
                    etape = 3; // Etape 3 du jeu
                    phase = 0;
                }
                break;
             case '4':
                if (phase == 3 || phase == 1 || phase == 2 || phase == 4 || phase == 5 || phase == 6 || phase == 7) {
                    etape = 4; // Etape 4 du jeu
                    phase = 0;
                }
                break;
            case 'I':
                phase = 3; //phase ou A et B n'ont pas la main
                break;
            case 'V':
                if (etape == 1 || etape == 2 || etape == 3 || etape == 4) {
                    if (phase == 1 || phase == 2) {
                        LA1 = 0;
                        LA2 = 0;
                        LA3 = 0;
                        LA4 = 0;
                        LB1 = 0;
                        LB2 = 0;
                        LB3 = 0;
                        LB4 = 0;
                        phase = 3;
                    }
                }
                break;
            case 'F':
                  if (etape == 1) {
                    if (phase == 1) {
                        LA1 = 0;
                        LA2 = 0;
                        LA3 = 0;
                        LA4 = 0;
                        phase = 10;
                    } else if (phase == 2) {
                        LB1 = 0;
                        LB2 = 0;
                        LB3 = 0;
                        LB4 = 0;
                        phase = 11;
                    } else if (phase == 8 || phase == 9) {
                        phase = 3;
                    }
                }
                if (etape == 2) {
                    if (phase == 1) {
                        phase = 2; // phase ou equipe B a la main
                        uart_txStr("EB");
                        LA1 = 0;
                        LA2 = 0;
                        LA3 = 0;
                        LA4 = 0;
                        LB1 = 0;
                        LB2 = 0;
                        LB3 = 0;
                        LB4 = 0;
                    } else if (phase == 2) {
                        phase = 1; // phase ou equipe A a la main
                        uart_txStr("EA");
                        LA1 = 0;
                        LA2 = 0;
                        LA3 = 0;
                        LA4 = 0;
                        LB1 = 0;
                        LB2 = 0;
                        LB3 = 0;
                        LB4 = 0;
                    }
                }
                if (etape == 3) {
                    if (phase == 1) {
                        LA1 = 0;
                        LA2 = 0;
                        LA3 = 0;
                        LA4 = 0;
                        phase = 6;
                    } else if (phase == 2) {
                        LB1 = 0;
                        LB2 = 0;
                        LB3 = 0;
                        LB4 = 0;
                        phase = 7;
                    } else if (phase == 4 || phase == 5) {
                        phase = 3;
                    }
                }
                if (etape == 4) {
                    if (phase == 1 || phase == 2) {
                        LA1 = 0;
                        LA2 = 0;
                        LA3 = 0;
                        LA4 = 0;
                        LB1 = 0;
                        LB2 = 0;
                        LB3 = 0;
                        LB4 = 0;
                        phase = 3;
                    }
                }
                break;
            case 'Q':
                if (etape == 2) {
                    if (phase == 1 || phase == 2 || phase == 3) {
                        LA1 = 0;
                        LA2 = 0;
                        LA3 = 0;
                        LA4 = 0;
                        LB1 = 0;
                        LB2 = 0;
                        LB3 = 0;
                        LB4 = 0;
                        phase = 0;
                    }
                } else if (etape == 3 || etape == 4) {
                    if (phase == 1 || phase == 2 || phase == 3) {
                        LA1 = 0;
                        LA2 = 0;
                        LA3 = 0;
                        LA4 = 0;
                        LB1 = 0;
                        LB2 = 0;
                        LB3 = 0;
                        LB4 = 0;
                        phase = 0;
                    }
                }
                break;
        }
    }
    // timer
    if (T0IF) {
        T0IF = 0;
        TMR0 = 60;

        if (cnt2 > 400) {
            cnt2 = 0;
            LA1 = 0;
            LA2 = 0;
            LA3 = 0;
            LA4 = 0;
            LB1 = 0;
            LB2 = 0;
            LB3 = 0;
            LB4 = 0;
        } else if (cnt2 > 0) {
            cnt2++;
        }

        if (cnt3 > 400) {
            cnt3 = 0;
            LA1 = 0;
            LA2 = 0;
            LA3 = 0;
            LA4 = 0;
            LB1 = 0;
            LB2 = 0;
            LB3 = 0;
            LB4 = 0;
        } else if (cnt3 > 0) {
            cnt3++;
        }

    }



    //Envoi des donn�es au logiciel 
    if (RBIF) {
        RBIF = 0;
        
             //Etape 1 du jeu
              if (etape == 1) {
            if (A1 == 0 && phase == 1) {
                LA1 = 1;
           
                cnt3 = 1;
            } else if (A2 == 0 && phase == 1) {
                LA2 = 1;
               
                cnt3 = 1;
            } else if (A3 == 0 && phase == 1) {
                LA3 = 1;
                
                cnt3 = 1;
            } else if (A4 == 0 && phase == 1) {
                LA4 = 1;
               
                cnt3 = 1;
            } else if (B1 == 0 && phase == 2) {
                LB1 = 1;
               
                cnt3 = 1;
            } else if (B2 == 0 && phase == 2) {
                LB2 = 1;
                
                cnt3 = 1;
            } else if (B3 == 0 && phase == 2) {
                LB3 = 1;
                
                cnt3 = 1;
            } else if (B4 == 0 && phase == 2) {
                LB4 = 1;
               
                cnt3 = 1;
            }else if (A1 == 0 && phase == 11) {
                LA1 = 1;
                uart_txStr("EA");
                cnt2 = 1;
                phase = 8;
            } else if (A2 == 0 && phase == 11) {
                LA2 = 1;
                uart_txStr("EA");
                cnt2 = 1;
                phase = 8;
            } else if (A3 == 0 && phase == 11) {
                LA3 = 1;
                uart_txStr("EA");
                cnt2 = 1;
                phase = 8;
            } else if (A4 == 0 && phase == 11) {
                LA4 = 1;
                uart_txStr("EA");
                cnt2 = 1;
                phase = 8;
            } else if (B1 == 0 && phase == 10) {
                LB1 = 1;
                uart_txStr("EB");
                cnt2 = 1;
                phase = 9;
            } else if (B2 == 0 && phase == 10) {
                LB2 = 1;
                uart_txStr("EB");
                cnt2 = 1;
                phase = 9;
            } else if (B3 == 0 && phase == 10) {
                LB3 = 1;
                uart_txStr("EB");
                cnt2 = 1;
                phase = 9;
            } else if (B4 == 0 && phase == 10) {
                LB4 = 1;
                uart_txStr("EB");
                cnt2 = 1;
                phase = 9;
            }
        }
        
        
        
        
        
        



        //Etape 2
        if (etape == 2) {
            if (A1 == 0 && phase == 0) {
                LB1 = 0;
                LB2 = 0;
                LB3 = 0;
                LB4 = 0;
                phase = 1;
                LA1 = 1;
                uart_txStr("EA");
            } else if (A2 == 0 && phase == 0) {
                LB1 = 0;
                LB2 = 0;
                LB3 = 0;
                LB4 = 0;
                phase = 1;
                LA2 = 1;
                uart_txStr("EA");
                
            } else if (A3 == 0 && phase == 0) {
                LB1 = 0;
                LB2 = 0;
                LB3 = 0;
                LB4 = 0;
                phase = 1;
                LA3 = 1;
                uart_txStr("EA");
                
            } else if (A4 == 0 && phase == 0) {
                LB1 = 0;
                LB2 = 0;
                LB3 = 0;
                LB4 = 0;
                phase = 1;
                LA4 = 1;
                uart_txStr("EA");
               
            } else if (B1 == 0 && phase == 0) {
                LA1 = 0;
                LA2 = 0;
                LA3 = 0;
                LA4 = 0;
                phase = 2;
                LB1 = 1;
                uart_txStr("EB");
                
            } else if (B2 == 0 && phase == 0) {
                LA1 = 0;
                LA2 = 0;
                LA3 = 0;
                LA4 = 0;
                phase = 2;
                LB2 = 1;
                uart_txStr("EB");
                
            } else if (B3 == 0 && phase == 0) {
                LA1 = 0;
                LA2 = 0;
                LA3 = 0;
                LA4 = 0;
                phase = 2;
                LB3 = 1;
                uart_txStr("EB");
                
            } else if (B4 == 0 && phase == 0) {
                LA1 = 0;
                LA2 = 0;
                LA3 = 0;
                LA4 = 0;
                phase = 2;
                LB4 = 1;
                uart_txStr("EB");
                
            }
        }










        //Etape 3
        if (etape == 3) {
            if (A1 == 0 && phase == 0) {
                phase = 1;
                LA1 = 1;
                uart_txStr("EA");
                cnt2 = 1;
            } else if (A2 == 0 && phase == 0) {
                phase = 1;
                LA2 = 1;
                uart_txStr("EA");
                cnt2 = 1;
            } else if (A3 == 0 && phase == 0) {
                phase = 1;
                LA3 = 1;
                uart_txStr("EA");
                cnt2 = 1;
            } else if (A4 == 0 && phase == 0) {
                phase = 1;
                LA4 = 1;
                uart_txStr("EA");
                cnt2 = 1;
            } else if (B1 == 0 && phase == 0) {
                phase = 2;
                LB1 = 1;
                uart_txStr("EB");
                cnt2 = 1;
            } else if (B2 == 0 && phase == 0) {
                phase = 2;
                LB2 = 1;
                uart_txStr("EB");
                cnt2 = 1;
            } else if (B3 == 0 && phase == 0) {
                phase = 2;
                LB3 = 1;
                uart_txStr("EB");
                cnt2 = 1;
            } else if (B4 == 0 && phase == 0) {
                phase = 2;
                LB4 = 1;
                uart_txStr("EB");
                cnt2 = 1;
            } else if (A1 == 0 && phase == 7) {
                LA1 = 1;
                uart_txStr("EA");
                cnt2 = 1;
                phase = 4;
            } else if (A2 == 0 && phase == 7) {
                LA2 = 1;
                uart_txStr("EA");
                cnt2 = 1;
                phase = 4;
            } else if (A3 == 0 && phase == 7) {
                LA3 = 1;
                uart_txStr("EA");
                cnt2 = 1;
                phase = 4;
            } else if (A4 == 0 && phase == 7) {
                LA4 = 1;
                uart_txStr("EA");
                cnt2 = 1;
                phase = 4;
            } else if (B1 == 0 && phase == 6) {
                LB1 = 1;
                uart_txStr("EB");
                cnt2 = 1;
                phase = 5;
            } else if (B2 == 0 && phase == 6) {
                LB2 = 1;
                uart_txStr("EB");
                cnt2 = 1;
                phase = 5;
            } else if (B3 == 0 && phase == 6) {
                LB3 = 1;
                uart_txStr("EB");
                cnt2 = 1;
                phase = 5;
            } else if (B4 == 0 && phase == 6) {
                LB4 = 1;
                uart_txStr("EB");
                cnt2 = 1;
                phase = 5;
            }
        }








        //Etape 4
        if (etape == 4) {
            if (A1 == 0 && phase == 1) {
                LA1 = 1;
                cnt3 = 1;
            } else if (A2 == 0 && phase == 1) {
                LA2 = 1;
                cnt3 = 1;
            } else if (A3 == 0 && phase == 1) {
                LA3 = 1;
                
                cnt3 = 1;
            } else if (A4 == 0 && phase == 1) {
                LA4 = 1;
                
                cnt3 = 1;
            } else if (B1 == 0 && phase == 2) {
                LB1 = 1;
                
                cnt3 = 1;
            } else if (B2 == 0 && phase == 2) {
                LB2 = 1;
                
                cnt3 = 1;
            } else if (B3 == 0 && phase == 2) {
                LB3 = 1;
              
                cnt3 = 1;
            } else if (B4 == 0 && phase == 2) {
                LB4 = 1;
                
                cnt3 = 1;
            }
        }
    }

}

void main(void) {
    ANSEL = 0;
    ANSELH = 0;
    TRISD0 = 0;
    TRISD1 = 0;
    TRISD2 = 0;
    TRISD3 = 0;
    TRISD4 = 0;
    TRISD5 = 0;
    TRISD6 = 0;
    TRISD7 = 0;
    RD0 = 0;
    RD1 = 0;
    RD2 = 0;
    RD3 = 0;
    RD4 = 0;
    RD5 = 0;
    RD6 = 0;
    RD7 = 0;
    uart_unit();
    while (1);
}