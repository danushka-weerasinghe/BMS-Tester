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
	  uint8_t tx_data[3] = {0xBE, 0x00, 0x00}; // Command to read register 0x3E
	  uint8_t rx_data[3];

//    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
   // HAL_GPIO_WritePin(GPIOE, CS3_Pin, GPIO_PIN_RESET);
   // HAL_GPIO_WritePin(GPIOE, cs4_Pin, GPIO_PIN_RESET);

//    HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, GPIO_PIN_RESET);// CS Low
//    HAL_GPIO_WritePin(CS2_GPIO_Port, CS2_Pin, GPIO_PIN_RESET);// CS Low

//    HAL_SPI_TransmitReceive(&hspi2, tx_data, rx_data, 3, HAL_MAX_DELAY);

//    HAL_SPI_TransmitReceive(&hspi2, txBuf, rxBuf, count, HAL_MAX_DELAY);

   // HAL_SPI_TransmitReceive(&hspi1, txBuf, rxBuf, count, HAL_MAX_DELAY);
//        HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET); // CS High
   // HAL_GPIO_WritePin(GPIOE, CS3_Pin, GPIO_PIN_SET);
   // HAL_GPIO_WritePin(GPIOE, cs4_Pin, GPIO_PIN_SET);
//

//        HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, GPIO_PIN_SET);; // CS High
//        HAL_GPIO_WritePin(CS2_GPIO_Port, CS2_Pin, GPIO_PIN_SET);// CS Low


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
