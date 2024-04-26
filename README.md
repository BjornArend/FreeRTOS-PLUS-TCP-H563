# FreeRTOS-PLUS-TCP-H563
Attempt at porting Eth drivers for FreeRTOS-plus to work on STM32H563ZI

I am new to embedded and the code is a mess.

Things still needing to be added for full functionality
  -xApplicationGetRandomNumber( uint32_t * pulNumber) this needs actuall psuedo-random number generation as it currently just returns a static 32 bit number
  -Registers within the EXTI struct need to be tested if they are correct
  -certain functions within stm32h5xx_hal_eth.c/stm32h5xx_hal_eth.h need to be removed as they are duplicates due to porting.
