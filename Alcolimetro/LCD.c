///LCD.c
#include <msp430g2553.h>
#include <LCD.h>

const unsigned char Comandos[5] =
{
    0x28, 0x0C, 0x01,  0x06, 0x80           // 0x80 direccion de escritura
};

const unsigned char Datos [16] =
{   "Nivel de Alcohol"  };

const unsigned char Datos1 [13] =
{   "    BAC      " };                      // función de escritura del LCD

void lcd(void)
{
    P2OUT |= E;                         // habilita al display
    _delay_cycles(2000);
    P2OUT &= ~E;                        // deshabilita al display
    _delay_cycles(2000);
}                                          // funcion principal de escritura

void displayinicio(void)
{
    P2OUT &= ~RS;                        // selecciona entrada de comandos

    DatosP = (unsigned char*) Comandos;    // apunta a la tabla
    Contador = sizeof Comandos;            // Carga al contador de la tabla

    Variable = *DatosP++;                  // envia el dato al puerto
    Variable2 = (Variable & 0xF0) ;        // proteje el nibble superior
    Variable3 = P2OUT;
    Variable3 = (Variable3 & 0x03);        // cuidar los bits RS y E
    Variable2 = Variable2 | Variable3;     // concatenar bits
    P2OUT = Variable2;
    lcd();                                 // escribe al lcd
    _delay_cycles(5000);                   // retardo de tiempo
    P2OUT = Variable2;
    lcd();                                 // escribe al lcd
    Variable = Variable << 4;              // preparaón del nibble bajo del comando corrimiento
    Variable2 = (Variable & 0xF0);
    Variable2 = Variable2 | Variable3;     // concatenar bits
    P2OUT = Variable2;                     // envía el comando
    lcd();                                 // escribe al display
    Contador--;                            // decrementa al contador de datos

    while(Contador !=0)                    // ejecuta mientras se cumpla
    {
        Variable = *DatosP++;              // envia el dato al puerto
        Variable2 = (Variable & 0xF0) ;    // proteje el nibble superior
        Variable3 = P2OUT;
        Variable3 = (Variable3 & 0x03);    // cuidar los bits RS y E
        Variable2 = Variable2 | Variable3; // concatenar bits
        P2OUT = Variable2;                 // envía el comando
        lcd();                             // escribe al display

        Variable = Variable << 4;          // preparaón del nibble bajo del comando corrimiento
        Variable2 = (Variable & 0xF0);
        Variable2 = Variable2 | Variable3; // concatenar bits
        P2OUT = Variable2;                 // envía el comando
        lcd();                             // escribe al display
        Contador--;                        // decrementa al contador de datos
    }
}

void renglon1(void)
{
    P2OUT |= RS;                         // seleciona entrada de datos

    DatosP = (unsigned char*) Datos;       //apunta a la tabla
    Contador = sizeof Datos;               // carga al contador de la tabla

    while(Contador !=0)                    // ejecuta mientas se cumpla
    {
        Variable = *DatosP++;              // envia el dato al puerto
        Variable2 = (Variable & 0xF0) ;    // proteje el nibble superior
        Variable3 = P2OUT;
        Variable3 = (Variable3 & 0x03);    // cuidar los bits RS y E
        Variable2 = Variable2 | Variable3; // concatenar bits
        P2OUT = Variable2;                 // envía el comando
        lcd();                             // escribe al display

        Variable = Variable << 4;          // preparaón del nibble bajo del comando corrimiento
        Variable2 = (Variable & 0xF0);
        Variable2 = Variable2 | Variable3; // concatenar bits
        P2OUT = Variable2;                 // envía el comando
        lcd();                             // escribe al display
        Contador--;                        // decrementa al contador de datos
    }
}                                          //Escritura Renglon inferior

void SaltoRenglon(void)
{
    P2OUT &= ~RS;                        // selecciona entrada de comandos
                                           // envio comando que coloca el cursor
                                           // en el inicio del 2o renglon
    Variable = 0xC0;                       // envia el dato al puerto  dirección
    Variable2 = (Variable & 0xF0) ;        // proteje el nibble superior
    Variable3 = P2OUT;
    Variable3 = (Variable3 & 0x03);        // cuidar los bits RS y E
    Variable2 = Variable2 | Variable3;     // concatenar bits
    P2OUT = Variable2;                     // envía el comando
    lcd();                                 // escribe al display

    Variable = Variable << 4;              // preparaón del nibble bajo del comando corrimiento
    Variable2 = (Variable & 0xF0);
    Variable2 = Variable2 | Variable3;     // concatenar bits
    P2OUT = Variable2;                     // envía el comando
    lcd();                                 // escribe al display
                                           // decrementa al contador de datos
}

void renglon2(void)
{
        P2OUT |= RS;                     // seleciona entrada de datos
        DatosP = (unsigned char*) Datos1;  //apunta a la tabla renglon inferior
        Contador = sizeof Datos1;          // carga al contador de la tabla

        while(Contador !=0)                // ejecuta mientas se cumpla
        {
            Variable = *DatosP++;          // envia el dato al puerto
            Variable2 = (Variable & 0xF0) ;// proteje el nibble superior
            Variable3 = P2OUT;
            Variable3 = (Variable3 & 0x03);// cuidar los bits RS y E
            Variable2 = Variable2 | Variable3; // concatenar bits
            P2OUT = Variable2;             // envía el comando
            lcd();                         // escribe al display

            Variable = Variable << 4;      // preparaón del nibble bajo del comando corrimiento
            Variable2 = (Variable & 0xF0);
            Variable2 = Variable2 | Variable3;// concatenar bits
            P2OUT = Variable2;             // envía el comando
            lcd();                         // escribe al display
            Contador--;                    // decrementa al contador de datos
        }
}

void DLCD(unsigned char dato)
{
        P2OUT |= RS;
        Variable = dato;                   // envia el dato al puerto
        Variable2 = (Variable & 0xF0) ;    // proteje el nibble superior
        Variable3 = P2OUT;
        Variable3 = (Variable3 & 0x03);    // cuidar los bits RS y E
        Variable2 = Variable2 | Variable3; // concatenar bits
        P2OUT = Variable2;                 // envía el comando
        lcd();                             // escribe al display

        Variable = Variable << 4;          // corrimiento
        Variable2 = (Variable & 0xF0);
        Variable2 = Variable2 | Variable3; // concatenar bits
        P2OUT = Variable2;                 // envía el comando
        lcd();                             // escribe al display
}
