/*
Author: Abdillah Fajar
Microcontroller: PIC16F877A
IDE: MPLAB X
*/

#include <xc.h>
#include "uart.h"
#include "adc.h"

#define _XTAL_FREQ 20000000
#pragma config FOSC = HS     
#pragma config WDTE = OFF       
#pragma config PWRTE = OFF      
#pragma config BOREN = OFF       
#pragma config LVP = OFF        
#pragma config CPD = OFF        
#pragma config WRT = OFF       
#pragma config CP = OFF          

uint8_t adcT, adcV, adcI;    
unsigned char RcvChar, dummy = 141, ack;
int dat = 0x00;
short chk_com;
int i = 0, j = 0;
float val_T, val_V, val_I;              
    
void I2C_Master_Init(const unsigned long c)
{
    SSPCON = 0b00101000;
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ/(4*c))-1;
    SSPSTAT = 0;
    
    GIE = 1;
    PEIE = 1;
    INTE = 1;
    SSPIF = 0;
    
    TRISC3 = 1;
    TRISC4 = 1;
}

void I2C_Wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2C_Start()
{
    I2C_Wait();
    SEN = 1;
}

void I2C_RepeatedStart()
{
    I2C_Wait();
    RSEN = 1;
}

void I2C_Stop()
{
    I2C_Wait();
    PEN = 1;
}

void I2C_Write(unsigned d)
{
    I2C_Wait();
    SSPBUF = d;
}

int I2C_Read(unsigned short a)
{
    unsigned short temp;
    I2C_Wait();
    RCEN = 1;
    I2C_Wait();
    while(!SSPIF);
    temp = SSPBUF;
    I2C_Wait();
    ACKDT = (a)?0:1;
    ACKEN = 1;
    SSPIF = 0;
    return temp;
}

int Temperature()
{
    adcT = ADC_GetAdcValue(0);          
    val_T = adcT / 1023.0;
    return(val_T);
}

int Voltage()
{
    adcV = ADC_GetAdcValue(1);          
    val_V = ((adcV / 1023.0) * 9000.0);    //V = ((adcV / ADCbit) * Vmax)
    return(val_V);
}

int Current()
{
    adcI = ADC_GetAdcValue(4);         
    val_I = (((adcI / 1023.0) * 5000.0) - (5000.0/2) / 185.0);  //I = ((((adcI / ADCbit) * Vreff) - ACSoffset) / mVperAmp)
    return(val_I);
}

void Beacon()
{
    UART_TxChar('P');
    __delay_ms(5);
}

void sendAck(char ack)
{
    UART_TxChar(ack);
    __delay_ms(500);
}

void forwardCommand(char cmd)
{    
    I2C_Start();
    I2C_Write(0x10);
    I2C_Write(cmd);
    I2C_Stop();
}

void flowData()
{
    while(1)
    {
        I2C_Start();
        I2C_Write(0x11);
        dat = I2C_Read(0);
        __delay_ms(50);
        I2C_Stop();
        UART_TxChar(dat);
        
        j++;
        
        if(dat == '*' && j >= 99)                       
        {
            j = 0;
            
            for(int t = 0; t < 10; t++)                 
            {           
                if(UART_Data_Ready())
                {
                    ack = UART_RxChar();
                }
                
                __delay_ms(100);
            }
            
            if(ack == 'K')
            {
            forwardCommand(ack);
            ack = 0;
            break;
            }
        }                    
    }
}

void main()
{
    nRBPU = 0;
    
    UART_Init(19200);
    I2C_Master_Init(400000);
    
    while(1)
    {   
        chk_com = 0;
        
        if(UART_Data_Ready())
        {
            char check = UART_RxChar();    
            
            while(check == 'C')                                    
            {
                if(chk_com < 1)
                {         
                    sendAck('R');                                  
                }
                
                if(UART_Data_Ready())
                {
                    char cmd = UART_RxChar();
                    
                    if(cmd == 'H')                                  
                    {
                        chk_com = 0;
                        sendAck('r');                               
						            forwardCommand('H');                        
						            __delay_ms(500);
						            flowData();                                 
                        
                        break;
                    }
                }
                else                                                
				        {
					          chk_com++;
				        }
                
				        if(chk_com > 100)                                    
				        {
                    UART_Printf("\r\nCommunication Timeout\r\n");
					          break;
				        }
				
				        __delay_ms(10);
            }
        }
        else
        {
            Beacon();
        }    
    }
}
