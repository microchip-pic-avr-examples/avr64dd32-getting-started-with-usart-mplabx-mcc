 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
*/

/*
� [2022] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/


#include "mcc_generated_files/system/system.h"

#include <string.h>
#include <util/delay.h>

#define MAX_COMMAND_LEN		8


void USART0_sendChar(char c)
{
    while(!USART0_IsTxReady());    
    return USART0_Write(c);
}

void SendString(const char* pStr)
{     
    while(*pStr)
        USART0_sendChar(*pStr++); 
}

char USART0_getChar(void)
{
    while(!USART0_IsRxReady());        
    return USART0_Read();
}

void getCommand(char *command)
{
    uint8_t index = 0;

    if(command == NULL)
        return;

    while(1)
    {
        char c = USART0_getChar();
        USART0_sendChar(c); /* perform echo */
        if(c != '\n' && c != '\r')
            command[index++] = c;
        else
        {
            SendString("\n\r");
            break;
        }
        if(index >= MAX_COMMAND_LEN)
        {
            index = 0;
            SendString("\n\r");
            break;
        }
    }
    command[index] = '\0';
}

void executeCommand(const char *command)
{
    if(command != NULL)
    {
        if(strcmp(command, "ON") == 0)
        {
            LED_SetLow();
            SendString("OK, LED ON.\n\r");
            return;
        }
        else if (strcmp(command, "OFF") == 0)
        {
            LED_SetHigh();
            SendString("OK, LED OFF.\n\r");
            return;
        }
        else if (strcmp(command, "TOG") == 0)
        {
            LED_Toggle();
            SendString("OK, LED TOGGLE.\n\r");
            return;
        } 
    }
    SendString("Type ON/OFF/TOG to control the LED.\n\r");
}

int main(void)
{    
    SYSTEM_Initialize();
	
	/* This delay allows USB CDC device to connect */
    _delay_ms(2000);
	
    /* this will printout: "Type ON/OFF/TOG to control the LED.\n\r"  */
    executeCommand("");
    
    while (1)
    {
        char command[MAX_COMMAND_LEN];
        getCommand(command);
        executeCommand(command);
    }
}
