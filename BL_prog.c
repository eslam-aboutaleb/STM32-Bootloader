/* ///////////////////////////////////////////////////////////////////////////
 * BL_Prog.c
 *
 *
 * This file contains the manager of bootloader and manages which path is the
 * MCU going to take bootloader program or the user app
 *
 *  Author: Eslam Aboutaleb
/////////////////////////////////////////////////////////////////////////// */ 

/*Includes*/
#include <stdint.h>
#include <string.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "USART_interface.h"
#include "SysTick_interface.h"
#include "SW.h"
#include "COM.h" 
#include "BL_Services.h"
#include "cmsis_armcc.h"
#include "Flash_interface.h"
#include "BL_interface.h"


/*Static global APIs*/
static void BL_StartBootLoader(void);
static void BL_WelcomeMSG(void);
static void CMD_MSG(void);
static void BL_RSTR_SYS(void);
static void BL_StartUserApp(void);
static void BL_ChipReset(void);

/*Extern variables*/
extern uint32 SysTickCounter;

/* ***************************************************************************************
Function   : BL_Init
Parameters : void 
Return     : void
Job : Initialize the flash
**************************************************************************************** */
void BL_Init(void)
{
	/*Flash latency : one wait state*/
	Flash->ACR |=(0x1<<0);
	/*Flash word size : Half word - 16 bit*/
	Flash_SetWordSize(FLASH_PSIZE_HALF_WORD);
	
}

/* ***************************************************************************************
Function   : BL_Login
Parameters : void 
Return     : void
Job : Welcome message at the begining of the program
**************************************************************************************** */
static void BL_WelcomeMSG(void)
{
	Print_CMD("Welcome : Press boot button to start bootloader \r\n");
}

/* ***************************************************************************************
Function   : BL_Login
Parameters : void 
Return     : void
Job : Check if the boot button pressed or not if it is pressed will jump to the bootloader
program if it isn't it will jump to the user app
**************************************************************************************** */
static void CMD_MSG(void)
{
  uint8 MEM_WR[]      = "1 - Memory write program \r\n";
  uint8 Flash_Erase[] = "2 - Flash Erase sectors \r\n";
  uint8 MEM_R[]       = "3 - Read memory \r\n";
  uint8 GO_Add[]      = "4 - Go to address \r\n";
  uint8 EN_P[]        = "5 - Enable protection for sector \r\n";
  uint8 DIS_P[]       = "6 - Disable all sectors protection \r\n";
  uint8 Read_P[]      = "7 - Show the current protection level \r\n";
  uint8 BL_VR[]       = "8 - Get Bootloader version \r\n";
  uint8 CH_ID[]       = "9 - Get chip ID \r\n";
	uint8 JMP_APP[]     = "A - Jump to user App \r\n";
	uint8 RSTR_S[]      = "B - Restart the system \r\n";
	
	/*Print on screen for the user*/
	Print_CMD("           BL Commands        \r\n");
	Print_CMD("           ===========        \r\n");
	Print_CMD(MEM_WR);
	Print_CMD(Flash_Erase);
	Print_CMD(MEM_R);
	Print_CMD(GO_Add);
	Print_CMD(EN_P);
	Print_CMD(DIS_P);
	Print_CMD(Read_P);
	Print_CMD(BL_VR);
	Print_CMD(CH_ID);
	Print_CMD(JMP_APP);
	Print_CMD(RSTR_S);
}

/* ***************************************************************************************
Function   : BL_Login
Parameters : void 
Return     : void
Job : Check if the boot button pressed or not if it is pressed will jump to the bootloader
program if it isn't it will jump to the user app
**************************************************************************************** */
void BL_Login(void)
{
	uint32 Delay_Index=0;
	/*Welcome Msg*/
	BL_WelcomeMSG();
	/*loop on switch*/
	for(Delay_Index=0;Delay_Index<BL_T_BOOT_BUTTON_MS*LOOP_1_MS;Delay_Index++)
	{
	  SW_Update();
	}
	/*Check if the boot button has been pressed*/
	if(SW_GetState(SW_Boot)== SW_PRESSED)
	{
		Print_Debug("Boot button has beeen pressed \r\n Going to bootloader mode ... \r\n");
		/*Jump to bootloader */
		BL_StartBootLoader();
	}
	else
	{
		Print_Debug("Going to user app \r\n");
		/*Jump to user app*/
		BL_StartUserApp();
	}
}

/* ***************************************************************************************
Function   : BL_StartBootLoader
Parameters : void 
Return     : void
Job : Starts the bootloader. It gets command from the user and upon it decides the right
function to excute
**************************************************************************************** */
static void BL_StartBootLoader(void)
{
	uint8 Data=0;
	
	while(1)
	{	
    /*MSG to the user to tell the user to enter command*/
    Print_CMD("Enter the command \r\n");
		/*View bootloader commands*/
		CMD_MSG();
		/*Recieve first byte of BL command*/
		Data = BL_RecieveChar();
		
		                                               /**** Decode BL commands ****/
		
		/*Check the coming command*/
		switch(Data)
		{
			      case BL_GET_VER:
                BL_Handle_GetVer_CMD();
                break;
            case BL_GET_CID:
                BL_Handle_GetCid_CMD();
                break;
            case BL_GO_TO_ADDR:
                BL_Handle_Go_CMD();
                break;
            case BL_FLASH_ERASE:
                BL_Handle_FlashErase_CMD();
                break;
            case BL_MEM_WRITE:
							  BL_Handle_MEM_Write_CMD();
                break;
            case BL_EN_RW_PROTECT:
                BL_Handle_En_RW_Potect_CMD();
                break;
            case BL_MEM_READ:
                BL_Handle_MEM_Read_CMD();
                break;
            case BL_GET_RDP_STATUS:
                BL_Handle_ReadSectorProtect_CMD();
                break;
						case BL_DIS_R_W_PROTECT:
                BL_Handle_Dis_RW_protect_CMD();
                break;
						case BL_JMB_USR_APP:
							  BL_StartUserApp();
						    break;
						case RSTR_SYS:
                BL_RSTR_SYS();
							  break;
            default:
                Print_Debug("Invalid command code received from host \r\n");
                break;
		}
	}
}



/* ***************************************************************************************
Function   : BL_StartUserApp
Parameters : void 
Return     : void
Job : Starts the user app, Change the place of main stack pointer to the the address of
choosen sector , as the first address is the MSP and then assign the second address 
which is the reset handler to function pointer and resets the system to its initial 
state then calls that function pointer which that starts tha app.

Note that: The program that the user fetch must change its flash start address in linker
script and change the offset of address of vector table in the start up fuile.
**************************************************************************************** */
static void BL_StartUserApp(void)
{
		Print_Debug("Start user app \r\n");
	
	/*Pointer to function to hold address of reset handler of user app*/
	void(*UserApp_ResetHandler)(void);
	
	/*First byte in memory is MSP following by Reset handler*/
	/*Configure main stack pointer by reading value after where the bootloader kept*/
	uint32 UserApp_MSP_Value = (*(volatile uint32*)USER_APP_BASE_ADDRESS);
	
	/* Check if it has valid stack pointer in the RAM */
  if(0x20000000 != (UserApp_MSP_Value & 0x20000000))
	{
		Print_Debug("User app MSP is not valid \r\n");
		return;
	}
	
	Print_Debug("User app MSP value: %#x \r\n",UserApp_MSP_Value);
	
	/*Set main stack pointer where the user app is */
	__set_MSP(UserApp_MSP_Value);
	
	
	/*Get user app reset handler*/
	uint32 UserApp_ResetHandler_Value = (*(volatile uint32*)(USER_APP_BASE_ADDRESS + 4));
	
	/*Send reset handler on debug port*/
	Print_Debug("User app reset handler is %#x \r\n",UserApp_ResetHandler_Value);
	
	/*Initialize function pointer with the reset handler of user app*/
	UserApp_ResetHandler = (void*)UserApp_ResetHandler_Value;
	
	/*Reset MCU status*/
	BL_ChipReset();
	
	/*Start the User app*/
  UserApp_ResetHandler();
}



/* ***********************************************************
Function   : BL_ChipReset
Parameters : void
return     : void
job        : This function is portable, 
             the bootloader uses GPIOA for UART2 & LED
             and uses GPIOB used in UART3 if Debuging is enabled
             and uses GPIOC for boot switch and used too in UART3
*************************************************************** */  
static void BL_ChipReset(void)
{
	/*Choose system clock as HSI*/
	RCC_ClockInit(HSI);
	
	/*Disable used GPIO ports clocks*/
	RCC_GPIOA_CLK_DISABLE();
	#ifdef BL_DEBUG_MSG_EN
	RCC_GPIOB_CLK_DISABLE();
	#endif
	RCC_GPIOC_CLK_DISABLE();
	
	
	/*Disable USART clocks*/
	RCC_USART2_CLK_DISABLE();
	#ifdef BL_DEBUG_MSG_EN
	RCC_USART3_CLK_DISABLE();
	#endif
	
	/*Release reset*/
	RCC->AHB1RSTR=0;
	RCC->APB1RSTR=0;
	RCC->APB2RSTR=0;
	RCC->CFGR=0;
	
	/*Disable Systick exception*/
	SysTick_Init(AHB_DIVIDED_BY_8,DISABLE_SYSTICK_EXCEPTION);
	/*Clear Load and value of systick*/
	SysTick_Start(0,0);
	/*Clear system counter*/
	SysTickCounter=0;
}


/* ***************************************************************************************
Function   : BL_RSTR_SYS
Parameters : void 
Return     : void
Job : Calls rsest handler of the program
**************************************************************************************** */
static void BL_RSTR_SYS(void)
{
	/*Pointer to function to hold address of reset handler of program*/
	void(*Program_ResetHandler)(void);
	
	/*First byte in memory is MSP following by Reset handler*/
	uint32 Program_MSP_Value = (*(volatile uint32*)BL_BASE_ADDRESS);
	
	/*Set main stack pointer where the user app is */
	__set_MSP(Program_MSP_Value);
	
	/*Get user app reset handler*/
	uint32 Program_ResetHandler_Value = (*(volatile uint32*)(BL_BASE_ADDRESS + 4));
	
	/*Initialize function pointer with the reset handler of program*/
	Program_ResetHandler = (void*)Program_ResetHandler_Value;
	
	/*Reset MCU status*/
	BL_ChipReset();
	
	/*Start the User app*/
  Program_ResetHandler();	
	
}

