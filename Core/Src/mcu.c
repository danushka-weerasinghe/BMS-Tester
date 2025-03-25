/*
 *  Include Generic Header Files Here
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "mcu.h"

/*
 *  Include MCU Specific Header Files Here
 */

#include "main.h"  // for SPI_HandleTypeDef definitions

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;



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



    // Perform SPI transaction using HAL
    HAL_SPI_TransmitReceive(&hspi1, txBuf, rxBuf, count, HAL_MAX_DELAY);



    return 0; // Success
}


/********* MCU SPECIFIC SPI CODE ENDS HERE **********/


/********* MCU SPECIFIC DELAY CODE STARTS HERE ************/
void mcu_msWait(uint16_t msWait)
{
    /*
     *  Add MCU specific wait loop for msWait. The unit is in milli-seconds
     */

	HAL_Delay(msWait);

}

void mcu_usWait(uint16_t usWait)
{
    /*
     *  Add MCU specific wait loop for usWait. The unit is in micro-seconds
     */

    uint32_t ticks = (SystemCoreClock / 1000000) * usWait / 5; // Approximation for STM32
    while (ticks--) { __NOP(); }

}
/********* MCU SPECIFIC DELAY CODE ENDS HERE ************/
