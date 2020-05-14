/* ///////////////////////////////////////////////////////////////////////////
 * STD_TYPES.h
 *
 *  Author: Eslam Aboutaleb
/////////////////////////////////////////////////////////////////////////// */ 

#ifndef STD_TYPES_H
#define STD_TYPES_H

#define STD_HIGH 1
#define STD_LOW  0
#define OUTPUT   1
#define INPUT    0

#define True     1
#define False    0

#define MAX_DELAY  0xFFFFFFFFU
#define LOOP_1_MS  1000

typedef enum 
{
  OK             = 0x00U,
  StatusERROR    = 0x01U,
  BUSY           = 0x02U,
  TIMEOUT        = 0x03U
} StatusTypeDef;

typedef enum 
{
  FlagRESET = 0U, 
  FlagSET   =!0U
} FlagState;

typedef enum 
{
  UNLOCKED = 0x00U,
  LOCKED   = 0x01U  
} LockTypeDef;

#define _LOCK(__HANDLE__)                                           \
                                do{                                        \
                                    if((__HANDLE__)->Lock == LOCKED)   \
                                    {                                      \
                                       return BUSY;                    \
                                    }                                      \
                                    else                                   \
                                    {                                      \
                                       (__HANDLE__)->Lock = LOCKED;    \
                                    }                                      \
                                  }while (0U)

  #define _UNLOCK(__HANDLE__)                                          \
                                  do{                                       \
                                      (__HANDLE__)->Lock = UNLOCKED;    \
                                    }while (0U)


typedef unsigned char      uint8;
typedef unsigned short int uint16;
typedef unsigned long int  uint32;

typedef char               sint8;
typedef int                sint16;
typedef long int           sint32;

typedef float              float32;
typedef double             float64;

typedef uint8              tByte;
typedef uint16             tWord;

#endif

