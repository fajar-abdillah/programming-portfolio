/*
Author: Abdillah Faisal Nur Fajar
Microcontroller: PIC16F877A
IDE: MPLAB  X
*/

#include <xc.h>

#define _XTAL_FREQ 20000000
#pragma config FOSC = HS     
#pragma config WDTE = OFF       
#pragma config PWRTE = OFF    
#pragma config BOREN = OFF
#pragma config LVP = OFF         
#pragma config CPD = OFF        
#pragma config WRT = OFF        
#pragma config CP = OFF          

int z;
short i;
char buffer;

void __interrupt() I2C_Slave()
{ 
    if(SSPIF == 1)
    {            
       SSPCONbits.CKP = 0;
       
       if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL))
       {
            z = SSPBUF;                                 
            SSPCONbits.SSPOV = 0;                       
            SSPCONbits.WCOL = 0;                           
            SSPCONbits.CKP = 1;
       }

       if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW)        
       {            
            while(!BF);
            
            buffer = SSPBUF;
            
            SSPCONbits.CKP = 1;
            
            z = SSPBUF;
       }
       
       else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW)          
       {                                   
            z = SSPBUF;
            BF = 0;       
            
            if(i == 0)
            {
                SSPBUF = '#';
            }
            else if(i == 6)
            {
                SSPBUF = 0x10;
            }
            else if(i == 20)
            {
                SSPBUF = 0x12;
            }
            else if(i == 34)
            {
                SSPBUF = 0x14;
            }
            else if(i == 86)
            {
                SSPBUF = 0x16;
            }
            else if(i == 19 || i == 33 || i == 85)
            {
                SSPBUF = ';';
            }
            else if(i>=99)
            {
                SSPBUF = '*';
                i = 0;
            }
            else
            {
                SSPBUF = 'd';
            }
            
            i++;
            SSPCONbits.CKP = 1;            
            while(SSPSTATbits.BF);
       }
       
       SSPIF = 0;
    }
}

void I2C_Slave_Init(short address)
{
    SSPSTAT = 0x00;
    SSPADD = address;
    SSPCON = 0b00110110;
    SSPCON2 = 0x00;
    
    GIE = 1;
    PEIE = 1;
    SSPIF = 0;
    SSPIE = 1;
    
    TRISC3 = 1;
    TRISC4 = 1;
}

void main()
{
    I2C_Slave_Init(0x10);
    while(1);
}
