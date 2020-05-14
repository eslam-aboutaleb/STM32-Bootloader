/* ///////////////////////////////////////////////////////////////////////////
 * BL_interface.h
 *
 *  Author: Eslam Aboutaleb
/////////////////////////////////////////////////////////////////////////// */ 

#ifndef BOOTLOADER_H
#define BOOTLOADER_H

/* Bootloader version 1.0*/
#define BL_VERSION     1

/*BL used addresses*/
#define USER_APP_BASE_ADDRESS               FLASH_SECTOR2_BASE_ADDRESS
#define USER_APP_USED_SECTOR_NUM            2
#define BL_BASE_ADDRESS                     FLASH_SECTOR0_BASE_ADDRESS

/*Max flash memory sectors , for STM32F446RE there are 7 sectors*/
#define MAX_SECTORS_NUM                     7
/*Hex file line won't be more than 13 - 50 character*/
#define BL_BUFFER_SIZE                      50

/*Time to wait on prssing boot button in ms*/
#define BL_T_BOOT_BUTTON_MS                 200
                                  

 
                                                   /*Bootloader APIs*/
void BL_Init(void);
void BL_Login(void);


#endif /*BOOTLOADER_H*/
