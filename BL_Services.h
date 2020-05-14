/* ///////////////////////////////////////////////////////////////////////////
 * BL_Servics.h
 *
 *
 *  Author: Eslam Aboutaleb
/////////////////////////////////////////////////////////////////////////// */ 

#ifndef BL_SERVICES
#define BL_SERVICES

                             /*BL commands*/
														 
/*This command is used to write data in to different memories of the MCU.*/
#define BL_MEM_WRITE			 '1'		

/*This command is used to mass erase or sector erase of the user flash .*/
#define BL_FLASH_ERASE     '2'

/*This command is used to read data from different memories of the microcontroller.*/
#define BL_MEM_READ				 '3'

/*This command is used to jump bootloader to specified address.*/
#define BL_GO_TO_ADDR			 '4'

/*This command is used to enable or disable read/write protect on different sectors of the user flash .*/
#define BL_EN_RW_PROTECT	 '5'

/*This command is used disable all sector read/write protection. */
#define BL_DIS_R_W_PROTECT '6'

/*This command is used to read the FLASH Read Protection level.*/
#define BL_GET_RDP_STATUS	 '7'

/*This command is used to read the bootloader version from the MCU.*/
#define BL_GET_VER				 '8'

/*This command is used to read the MCU chip identification number.*/
#define BL_GET_CID				 '9'

/*This command is used to read the MCU chip identification number.*/
#define BL_JMB_USR_APP		 'A'

/*This command is used to read the MCU chip identification number.*/
#define RSTR_SYS				   'B'



void BL_Handle_GetVer_CMD(void);
void BL_Handle_GetCid_CMD(void);
void BL_Handle_Go_CMD(void);
void BL_Handle_FlashErase_CMD(void);
void BL_Handle_MEM_Write_CMD(void);
void BL_Handle_En_RW_Potect_CMD(void);
void BL_Handle_MEM_Read_CMD(void);
void BL_Handle_ReadSectorProtect_CMD(void);
void BL_Handle_Dis_RW_protect_CMD(void);
uint8 BL_GET_Version(void);




#endif /*BL_SERVICES*/

