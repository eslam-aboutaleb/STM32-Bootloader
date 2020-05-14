/* ///////////////////////////////////////////////////////////////////////////
 *  COM.h
 *
 *  Author: Eslam Aboutaleb
/////////////////////////////////////////////////////////////////////////// */ 

#ifndef COM_H
#define COM_H

void Com_Init(void);
void Print_CMD(uint8 *Data, ...);
void Read_CMD(uint8 *Data_Buffer);
void BL_Transmit(uint8 *Data,uint32 DataSize);
void BL_Recieve(uint8 *Data_Buffer,uint32 DataSize);
void Print_Debug(uint8 *Data,...);
uint8 BL_RecieveChar(void);


#endif /*COM_H*/

