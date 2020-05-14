/* ///////////////////////////////////////////////////////////////////////////
*  Project : Bootloader for Stm32 MCUs
 *
 *  Author: Eslam Aboutaleb
/////////////////////////////////////////////////////////////////////////// */ 

/*Includes*/
#include <stdio.h>
#include <stdlib.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "main_SupportFuncs.h"
#include "COM.h"
#include "SW.h"
#include "Led_interface.h"
#include "BL_interface.h"


/* *************************************************************************************************

                                       *  STM32 Bootloader  *
																						
   BootLoader program compatible with all STM32 MCUs just change private files over any protcol.
	 
	 To use the bootloader just attach the board to any sender device and keep in mind adjust the
	 COM files to be compatible with the sender protcol, in case of using computers just open any
	 terminal and follow the directions and copy-paste hex file in terminal window with out any 
	 need of python scribts so it can be used with any protcol.
	 
	 For protcol adjustment : Adjust COM.c & COM.h files.
	 
	 For starting bootloader press boot button at the first five minutes after turning on the MCU
	 otherwise the last user app in the memory will start.
	 
	 To check commands of bootloader check its entry sequence in BL_program.c file and then the 
   handling sequence in BL_Sercices.c file.

   The bootloader consumes the first sector of flash memory (Flash sector 0) & left the second
	 one (Flash sector 1) for future updates.
	 
	 The bootloader has two messages ports one is for debugging messages and the other one is for
	 mandatory messages which directs the user and for sending and recieving commands.
	 
	 For turning on debugging messages port ,must define its macro:
	 #define BL_DEBUG_MSG_EN in COM files, otherwise it is turned off and won't cause any problems.
   
   For a secure system choose low speed to transfere Hex files to let the processor do its work
	 ex: UART baudrate 9600.
	 
	 Speed of debugging port can be any speed it has no control over flash interface.
	 
	 *********************
	 Supported commands :
	 *********************
	 1-Memory write program
	 2-Flash Erase sectors
	 3-Read memory
	 4-Go to address
	 5-Enable protection for sector
	 6-Disable all sectors protection
	 7-Show the current protection level
	 8-Get Bootloader version
	 9-Get chip ID
	 A-Jumb to user app
	 B-Restart the system
	  
		*******************************************************************************************
		email: eslamaboutaleb@yahoo.com                 
		*******************************************************************************************

**************************************************************************************************** */


int main()
{
	/*Initialize system tick*/
	SystemTick_Init();
	/*Initialize communication*/
	Com_Init();
	/*Initialize Bootloader*/
	BL_Init();
	/*Initialize Leds*/
	LED_Init(LED_1,LED_OFF);
	/*Initialize Switches*/
	SW_Init();
	/*Initialize bootloader*/
	BL_Login();
	
	while(1)
	{
		/*No Action*/
	}
	
}
