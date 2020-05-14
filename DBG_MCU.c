/* ///////////////////////////////////////////////////////////////////////////
 * 
 *
 *  Author: Eslam Aboutaleb
/////////////////////////////////////////////////////////////////////////// */ 

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DBG_MCU_interface.h"


uint16 DBG_GetChipID(void)
{
	uint16 ChipID=0;
	
	/*Get the chip ID from first 11 bits*/
	ChipID=(uint16)(DBG_MCU->IDCODE) & 0x0FFF;
	
	return ChipID;
}


