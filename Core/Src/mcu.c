/*
 *  Include Generic Header Files Here
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "mcu.h"
#include "main.h"


/*
 *  Include MCU Specific Header Files Here
 */
#include "stm32f4xx_hal.h"

extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi1;


/********* MCU SPECIFIC SPI CODE STARTS HERE **********/
void mcu_spiInit(uint8_t busId)
{
    /* Add MCU specific init necessary for I2C to be used */




}

uint8_t mcu_spiTransfer(uint8_t busId, uint8_t csGPIOId, uint8_t count, uint8_t* txBuf, uint8_t* rxBuf)
{
    /*
     *  Add MCU specific SPI read/write code here.
     */




//    HAL_SPI_TransmitReceive(&hspi2, txBuf, rxBuf, count, HAL_MAX_DELAY);

    HAL_SPI_TransmitReceive(&hspi1, txBuf, rxBuf, count, HAL_MAX_DELAY);



    /*
     *  Add MCU specific return code for error handling
     */

    return (0);
}


/********* MCU SPECIFIC SPI CODE ENDS HERE **********/


/********* MCU SPECIFIC DELAY CODE STARTS HERE ************/
void mcu_msWait(uint16_t msWait)
{

	HAL_Delay(msWait); // Millisecond delay
    /*
     *  Add MCU specific wait loop for msWait. The unit is in milli-seconds
     */
}

void mcu_usWait(uint16_t usWait)
{
    /*
     *  Add MCU specific wait loop for usWait. The unit is in micro-seconds
     */

    uint32_t ticks = (SystemCoreClock / 1000000) * usWait;
    while (ticks--) {
        __NOP(); // No-operation instruction
    }
}
/********* MCU SPECIFIC DELAY CODE ENDS HERE ************/
