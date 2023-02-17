// main.c
#include <msp430g2553.h>
#include <LCD.h>

unsigned int  respuesta;
unsigned int regres;
unsigned int variante;
unsigned int tiempo;
unsigned char medida1  = 0;
unsigned char medida2  = 0;
unsigned char decenas  = 0x30;
unsigned char centenas = 0x30;
unsigned char unidades = 0x30;          // Definir las variables utilizadas.

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           // stop watchdog timer

    P1DIR |= BIT0|BIT6;                 //P1DIR = BIT0; P1.0 como salida
    P1OUT &= ~BIT0;                     // apagar LED
    P1DIR |= BIT1;
    P1OUT &= ~BIT1;                     // apagar

    P2SEL &= ~BIT6;                     //configura funcion periferica para los bit 6 y 7 del puerto 2
    P2SEL &= ~BIT7;
    P2DIR |= BIT0 + BIT1 + BIT4 + BIT5 + BIT6 + BIT7;  // configura al puerto 1 como salida
    P2OUT &= ~RS;                       // selecciona entrada de comandos
    P2OUT &= ~E;                // deshabilita al display

    displayinicio();                    // inicializo display
    renglon1();                         // envio letrero renglon superior
    SaltoRenglon();                     // coloco el cursor al inicio del renglon inferior
    DLCD(centenas);                     // envio centenas// envio unidades
    renglon2();                         // envio letrero renglon inferior

    WDTCTL = WDTPW|WDTTMSEL|WDTCNTCL;   // WDT:passw|modo intervalo|resetcontador para interrupcion cada 32 ms
    P1DIR |= 65;                        // configura al puerto1 como salida en los bits 0 y 6

    ADC10CTL0 = ADC10SHT_2 + ADC10ON;   // SHTime16xADC10CLKs, ADC10ON
    ADC10CTL1 = INCH_3;                 // input A3, PIN5
    ADC10AE0 |= 0x08;

    ADC10CTL0 |= ENC + ADC10SC;         // AQUI  SE LA HACE CONVERSION A DIGITAL
    ADC10AE0 = BIT3;

    variante = ADC10MEM / 60;

    while(1)
    {

        ADC10CTL0 |= ENC + ADC10SC;     // AQUI  SE LA HACE CONVERSION A DIGITAL
        respuesta = ADC10MEM;
        tiempo  = ADC10MEM;
        regres = ADC10MEM;
        medida1 = tiempo/4;
        medida2 = medida1;
        tiempo  = 0;
        P1OUT  |= 1;                    // led encendido del bit0
        while(tiempo != medida2)
        {
            tiempo++;
            _delay_cycles(1000);
        }

        tiempo = 0;
        P1OUT  &= ~ 1;                  // apago led del bit0
        while(tiempo != medida2)
        {
            tiempo++;
            _delay_cycles(1000);
        }
       regres = regres / 5;            //asignacion de la señal convertida a una variable
       regres = regres - 60;
       regres = regres * 3.5;

       if (regres <= 0 || regres > 330)
       {
           decenas  = (((regres % 100)/10) + 0x30);
           centenas = ((regres / 100)     + 0x30);
           unidades = ((regres % 10)      + 0x030);
       } else
       {
           decenas  = (((regres % 100)/10) + 0x30);
           centenas = ((regres / 100)     + 0x30);
           unidades = ((regres % 10)      + 0x030);
       }
         SaltoRenglon();                 // coloco el cursor al inicio del renglon inferior
         DLCD(decenas);                 // envio decenas
         DLCD(centenas);                // envio centenas
         DLCD(unidades);                // envio unidades

        _delay_cycles(150000);          //delay para actualizar LCD
    }
}
