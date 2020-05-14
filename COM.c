/* ///////////////////////////////////////////////////////////////////////////
 *  File : COM.c
 *  
 *  This file for abstraction the communication of bootloader
 *
 *  Author: Eslam Aboutaleb
/////////////////////////////////////////////////////////////////////////// */ 

#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "USART_interface.h"
#include "COM.h"

/*Static local functions*/
static void Debug_Transmit(uint8 *Data,uint32 DataSize);

/*USART Handlers*/
USART_Handle hUART_Com_Handle, hUART_Debug_Handle;


/* ***************************************************************************************
Function   : Com_Init
Parameters : void 
Return     : void
Job : Initialize the communication protocol of bootloader
**************************************************************************************** */
void Com_Init(void)
{
	/*Choose for sending commands USART2*/
	hUART_Com_Handle.Instance          = USART2;
	/*BaudRate =9600*/
	hUART_Com_Handle.Init.BaudRate     = USART_BAUD_9600;
	/*Data frame eight bits*/
	hUART_Com_Handle.Init.WordLength   = USART_EightBits;
	/*One stop bit*/
	hUART_Com_Handle.Init.StopBits     = USART_One_Stop_Bit;
	/*Parity off*/
	hUART_Com_Handle.Init.Parity       = USART_Parity_OFF;
	/*Tx Rx UART*/
	hUART_Com_Handle.Init.Mode         = Transmiter_Reciever;
	/*Over sampling 16*/
	hUART_Com_Handle.Init.OverSampling = USART_OV16;
	/*Initialize USART*/
	UART_Init(&hUART_Com_Handle);
	
	#ifdef BL_DEBUG_MSG_EN
	/*Choose for debugging USART3*/
	hUART_Debug_Handle.Instance          = USART3;
	/*BaudRate =9600*/
	hUART_Debug_Handle.Init.BaudRate     = USART_BAUD_115200;
	/*Data frame eight bits*/
	hUART_Debug_Handle.Init.WordLength   = USART_EightBits;
	/*One stop bit*/
	hUART_Debug_Handle.Init.StopBits     = USART_One_Stop_Bit;
	/*Parity off*/
	hUART_Debug_Handle.Init.Parity       = USART_Parity_OFF;
	/*Tx Rx UART*/
	hUART_Debug_Handle.Init.Mode         = Transmiter_Reciever;
	/*Over sampling 16*/
	hUART_Debug_Handle.Init.OverSampling = USART_OV16;
	/*Initialize USART*/
	UART_Init(&hUART_Debug_Handle);
	#endif
}


/* ***************************************************************************************
Function   : Read_CMD
Parameters : Pointer to buffer type uint8 
Return     : void
Job : Recieve strings by Command port
**************************************************************************************** */
void Read_CMD(uint8 *Data_Buffer)
{
	UART_ReadString(&hUART_Com_Handle,Data_Buffer);
}



/* ***************************************************************************************
Function   : BL_Transmit
Parameters : Pointer to buffer type uint8 , DataSize from type uint32 
Return     : void
Job : Send data by command port
**************************************************************************************** */
void BL_Transmit(uint8 *Data,uint32 DataSize)
{
	uint32 Index=0;
	for(Index=0;Index<DataSize+1;Index++)
	{
		UART_SendChar(&hUART_Com_Handle,*Data);
		Data++;
	}
}

/* ***************************************************************************************
Function   : BL_RecieveChar
Parameters : void 
Return     : uint8
Job : Recieve byte by command port
**************************************************************************************** */
uint8 BL_RecieveChar(void)
{
	uint8 Data;
	Data=UART_GetChar(&hUART_Com_Handle);
	return Data;
}

/* ***************************************************************************************
Function   : BL_Recieve
Parameters : Pointer to buffer type uint8 , DataSize from type uint32  
Return     : void
Job : Recieve data by size by command port 
**************************************************************************************** */
void BL_Recieve(uint8 *Data_Buffer,uint32 DataSize)
{
	UART_Recieve(&hUART_Com_Handle,Data_Buffer,DataSize,MAX_DELAY);
}


/* ***************************************************************************************
Function   : Debug_Transmit
Parameters : Pointer to buffer type uint8 , DataSize from type uint32  
Return     : void
Job : Recieve data by size by debugging port
**************************************************************************************** */
void Debug_Transmit(uint8 *Data,uint32 DataSize)
{
	uint32 Index=0;
	for(Index=0;Index<DataSize;Index++)
	{
		UART_SendChar(&hUART_Debug_Handle,*Data);
	}
}


/* ***************************************************************************************
Function   : Print_Debug
Parameters : Pointer to buffer type uint8 
Return     : void
Job : Print function for debugging port
**************************************************************************************** */
void Print_Debug(uint8 *Data,...)
{
	#ifdef BL_DEBUG_MSG_EN
	uint8 Str[80];
	/*Extract the the argument list using VA apis */
	va_list args;
	va_start(args, Data);
	vsprintf(Str, Data,args);
	Debug_Transmit(Str,strlen(Str));
	va_end(args);
	UART_WriteString(&hUART_Com_Handle,Data);
  #endif
}

/* ***************************************************************************************
Function   : Print_CMD
Parameters : Pointer to buffer type uint8 
Return     : void
Job : Print function for command port
**************************************************************************************** */
void Print_CMD(uint8 *Data, ...)
{
	uint8 Str[80];
	/*Extract the the argument list using VA apis */
	va_list args;
	va_start(args, Data);
	vsprintf(Str, Data,args);
	BL_Transmit((uint8 *)Str,strlen(Str));
	va_end(args);
}
