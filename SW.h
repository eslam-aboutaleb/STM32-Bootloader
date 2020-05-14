/* ////////////////////////////////////////////////////////////////////////////////////////////////////
Name: Eslam Aboutaleb
File: SW.h
Date: 30/3/2020
//////////////////////////////////////////////////////////////////////////////////////////////////// */
#ifndef __SW_H__
#define __SW_H__

#define N_SWITCHES      (1)

#define SW_Boot_PORT_DR        (GPIOC)
#define SW_Boot_PORT_CR        (GPIOC)
#define SW_Boot_PIN            (GPIO_PIN_13)

typedef enum
{
    SW_Boot

}tSW;

typedef enum
{
    SW_RELEASED,
    SW_PRE_PRESSED,
    SW_PRESSED,
    SW_PRE_RELEASED
}tSW_State;

void SW_Init(void);
tSW_State SW_GetState(tSW sw);
void SW_Update(void);

#endif // __SW_H__
