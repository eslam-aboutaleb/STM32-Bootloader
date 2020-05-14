/* ///////////////////////////////////////////////////////////////////////////
 * 
 *
 *  Author: Eslam Aboutaleb
/////////////////////////////////////////////////////////////////////////// */ 

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "main_SupportFuncs.h"
#include "SysTick_interface.h"

void SystemTick_Init(void)
{
	SysTick_Init(AHB_DIVIDED_BY_8,ENABLE_SYSTICK_EXCEPTION);
	SysTick_Start(2000,0);
}


