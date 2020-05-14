/* ///////////////////////////////////////////////////////////////////////////
 * BL_Servics.c
 *
 * This file has the implementation of bootloader commands
 *
 *  Author: Eslam Aboutaleb
/////////////////////////////////////////////////////////////////////////// */ 

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "Led_interface.h"
#include "COM.h" 
#include "Flash_interface.h"
#include "BL_interface.h"
#include "BL_Services.h"
#include "DBG_MCU_interface.h"


/* ***************************************************************************************
Function : BL_Handle_GetVer_CMD
Parameters : void 
Return : void
Job : Gets current bootloader version
**************************************************************************************** */
void BL_Handle_GetVer_CMD(void)
{
	uint8 Version = (uint8)BL_VERSION;
	
	Print_CMD("Bootloader version is %d  \r\n",Version );
}

	
/* ***************************************************************************************
Function : BL_Handle_GetCid_CMD
Parameters : void 
Return : void
Job : Gets micro-controller ID
**************************************************************************************** */	
void BL_Handle_GetCid_CMD(void)
{
	uint16 ChipID=0;
	ChipID = DBG_GetChipID();
	
	Print_CMD("Chip ID is : %#x  \r\n",ChipID);
}

/* ***************************************************************************************
Function : BL_Handle_ReadSectorProtect_CMD
Parameters : void 
Return : void
Job : Gets the current protection level of all the flash
**************************************************************************************** */	
void BL_Handle_ReadSectorProtect_CMD(void)
{
	uint8 ProtectionLvl=0;
	ProtectionLvl=Flash_GetProtectionLvl();
	Print_CMD("{Protection level is %#x \r\n",ProtectionLvl);
}


/* ***************************************************************************************
Function : BL_Handle_Go_CMD
Parameters : void 
Return : void
Job : jumps to to address that the user provids
**************************************************************************************** */
void BL_Handle_Go_CMD(void)
{
	uint8 Add_Buffer[5];
	uint32 Go_Address=0;
	
	Print_CMD("Enter 32 bit address \r\n");
	/*Recieve 32 bit address*/
	BL_Recieve(Add_Buffer,4);
	
	Go_Address = (uint32) &Add_Buffer[0];
	
	Print_Debug("Going to address %#x \r\n",Go_Address);
	Flash_JumbAddress(Go_Address);
}


/* ***************************************************************************************
Function : BL_Handle_FlashErase_CMD
Parameters : void 
Return : void
Job : Erase any number of sectors that the user enters but sector 0 & 1 because the boolo-
      -ader consumes them
**************************************************************************************** */	
void BL_Handle_FlashErase_CMD(void)
{
	uint8 SectorNumASCII = 0;
	uint8 SectorNum = 0;
	uint8 Index = 0;
	uint8 N_Sectors = 0;
	
	Print_CMD("Enter number of flash sectors \r\n");
	N_Sectors = BL_RecieveChar();
	
	for(Index=0;Index<N_Sectors;Index++)
	{
	  Print_CMD("Enter flash sector number %d \r\n",Index);
	  SectorNumASCII = BL_RecieveChar();
		/*Convert the ASCII value to a number*/
		SectorNum = (uint8)SectorNumASCII-'0';
		if(SectorNum != 0 || SectorNum !=1)
		{
			Flash_EraseSector(SectorNum);
			Print_Debug("Section %d is erased \r\n",SectorNum);
		}
		else
		{
			Print_Debug("Section %d cant be erased \r\n",SectorNum);
		}
	}
}

/* ***************************************************************************************
Function : BL_Handle_MEM_Write_CMD
Parameters : void 
Return : void
Job : Reads hex file that is sent through communication protcol and translates it to 
      digits and writes them to flash
**************************************************************************************** */
void BL_Handle_MEM_Write_CMD(void)
{
	uint16 Index=0;
	uint16 Data_Index=0;
	uint8 N_Data=0;
	sint16 Data=0;
	uint8 Data_RX=0;
	uint8 RX_Buffer[BL_BUFFER_SIZE]={0};
	sint16 Address = 0;
	
	/*Clear flash sectors that are present after bootloader to make sure there is no other images in the flash memory*/
	for(Index=USER_APP_USED_SECTOR_NUM;Index<=MAX_SECTORS_NUM;Index++)
	{
		Flash_EraseSector(Index);
	}
	
	Print_CMD("Type the hex file \r\n"); 
	
	/*Critical section, disable interrupts*/
	__disable_irq();
	
	/*Turn on Led to indicate of flashing the program*/
	LED_SetState(LED_1,LED_ON);
	
	while(1)
	{
		  /*Recieve data until it is start of new hex line*/
	    Data_RX=BL_RecieveChar();
	    if(Data_RX == ':')
	    {
	    	/*Loop until there is new line to get all data after ':' into buffer, 13 = Ascii for enter button */
	    	for(Index=0;Data_RX!= 13;Index++)	
	    	{
	    		 Data_RX=BL_RecieveChar();
	    		 RX_Buffer[Index]=Data_RX;
	      }
	    	
	    	/* First & second in the array determines the length of data */
	    	N_Data=(( ( RX_Buffer[0]-'0')*16 )+( (RX_Buffer[1]-'0')*1) ); 
				
				/* If there is no data */
				if(N_Data == 0)
	    		{
          /*Terminate the while loop*/						
	    	    break;
	    		}
	    	
	    	/* Third & 4th & 5th & 6th are the address */
				/* Convert them from ASCII to digits */
	    	Address=     (
					           (   (RX_Buffer[2]>'9')       ?   ((RX_Buffer[2]-55)*4096)     :   ((RX_Buffer[2]-'0')*4096))   + 
	    					     (   (RX_Buffer[3]>'9')       ?   ((RX_Buffer[3]-55)*256)      :   ((RX_Buffer[3]-'0')*256 ))   +
	    					     (   (RX_Buffer[4]>'9')       ?   ((RX_Buffer[4]-55)*16)       :   ((RX_Buffer[4]-'0')*16  ))   +
	    					     (   (RX_Buffer[5]>'9')       ?   ((RX_Buffer[5]-55)*1)        :   ((RX_Buffer[5]-'0')*1   ))
					           )                                                                                              +
	    						       BL_BASE_ADDRESS;
					
	    	
				/* Data is in little endian order */
	    	for(Data_Index=0,Index=0;Index<N_Data;Data_Index+=4,Index+=2)
						{
							Data=  (
							       ( (RX_Buffer[10+Data_Index]>'9')      ?   ((RX_Buffer[10+Data_Index]-55)*4096)      :   ((RX_Buffer[10+Data_Index]-'0')*4096))+
									   ( (RX_Buffer[11+Data_Index]>'9')      ?   ((RX_Buffer[11+Data_Index]-55)*256)     	:   ((RX_Buffer[11+Data_Index]-'0')*256  ))+
									   ( (RX_Buffer[8 +Data_Index]>'9')      ?   ((RX_Buffer[8 +Data_Index]-55)*16)      	:   ((RX_Buffer[8 +Data_Index]-'0')*16   ))+
									   ( (RX_Buffer[9 +Data_Index]>'9')      ?   ((RX_Buffer[9 +Data_Index]-55)*1)       	:   ((RX_Buffer[9 +Data_Index]-'0')*1    ))
							       );
						
							/*Write data to the address*/
							Flash_Write(Address+Index,Data); 
	         	}
		}
   }
	/*Enable interrupts*/
	__enable_irq();
	 
	/*Turn off Led to indicate the end of flashing the program*/
	LED_SetState(LED_1,LED_OFF);

	 
	 Print_Debug("Writing on memory is done \r\n");
	 
}


/* ***************************************************************************************
Function   : BL_Handle_MEM_Write_CMD
Parameters : void 
Return     : void
Job : Enabls protection for flash sectors that the user decides
**************************************************************************************** */
void BL_Handle_En_RW_Potect_CMD(void)
{
	uint8 Sector;
	uint8 N_sectors = 0;
	uint8 Index=0;
	uint8 Status=0;
	Print_CMD("Enter number of sectors \r\n");
	N_sectors = BL_RecieveChar();
	
	for(Index=0;Index<N_sectors;Index++)
	{
		Print_CMD("Enter sector number %d \r\n",Index);
		Sector = BL_RecieveChar();
		
		/*For user app always can use just level 1 protection
		  because level 2 is irreversable                   */
		Status = Flash_SetProtection(Flash_Protect_Disable,Flash_Protect_Lvl_1,Sector);
		if(Status == True)
		{
			Print_Debug("Success");
		}
	}
}

/* ***************************************************************************************
Function   : BL_Handle_Dis_RW_protect_CMD
Parameters : void 
Return     : void
Job : Disabls protection for all flash sectors
**************************************************************************************** */
void BL_Handle_Dis_RW_protect_CMD(void)
{
	Print_CMD("All sectors protection is going to be disabled \r\n");
	Flash_SetProtection(Flash_Protect_Disable,Flash_Protect_Lvl_0,0);
	Print_Debug("Success \r\n");
}

/* ***************************************************************************************
Function   : BL_Handle_MEM_Read_CMD
Parameters : void 
Return     : void
Job : Reads the data in the address that the user provides
**************************************************************************************** */
void BL_Handle_MEM_Read_CMD(void)
{
	uint32 R_Address=0;
	uint8 Buffer[5];
	uint32 Read;
	
	Print_CMD("Enter address \r\n");
	BL_Recieve(Buffer,4);
	
	R_Address = (uint32)&Buffer[0];
	
	Read = Flash_Read(R_Address);
	
	Print_CMD("Value of address %#x is %#x \r\n",R_Address,Read);
	Print_Debug("Success \r\n");
}

	
