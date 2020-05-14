/* ///////////////////////////////////////////////////////////////////////////
 * 
 *
 *  Author: Eslam Aboutaleb
/////////////////////////////////////////////////////////////////////////// */ 

#ifndef DBG_MCU_PRIVATE_H
#define DBG_MCU_PRIVATE_H

#define DBG_MCU_BASE_ADDRESS               0xE0042000U

typedef struct
{
  volatile uint32 IDCODE;  /*!< MCU device ID code,               Address offset: 0x00 */
  volatile uint32 CR;      /*!< Debug MCU configuration register, Address offset: 0x04 */
  volatile uint32 APB1FZ;  /*!< Debug MCU APB1 freeze register,   Address offset: 0x08 */
  volatile uint32 APB2FZ;  /*!< Debug MCU APB2 freeze register,   Address offset: 0x0C */
}DBGMCU_TypeDef;


#define DBG_MCU            ((DBGMCU_TypeDef*) DBG_MCU_BASE_ADDRESS)

#endif /*DBG_MCU_PRIVATE_H*/


