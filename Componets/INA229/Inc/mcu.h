/*
 *  ======== mcu.h ========
 *  MCU hardware abstraction used sensor API implementations
 */
#ifndef ti_sensors_MCU__include
#define ti_sensors_MCU__include 1

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "main_data.h"  // Include this to get NUM_INA229 definition


/* support C++ sources */
#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ======== mcu_spiInit ========
 *  Initialize the specified SPI bus for first use
 */
extern void mcu_spiInit(uint8_t busId);

/*
 * ======== mcu_spiTransfer ========
 * Transfer data to and from a SPI slave
 * 
 * @param busId id of an SPI bus to access for the transfer
 * @param count number of frames for this transaction
 * @param txBuf buffer with the data to be transmitted
 * @param rxBuf buffer to receive data
 * 
 * @return      0 if successful, otherwise non-zero
 */

extern uint8_t mcu_spiTransfer(uint8_t busId, uint8_t csGPIOId, uint8_t count, uint8_t *txBuf, uint8_t *rxBuf);


/*
 *  ======== mcu_msWait ========
 *  Delay CPU for at least the specified number of milliseconds
 *
 *  @param msWait - number of milliseconds to delay, a value of 0 causes
 *                  this function to return immediately.
 */
extern void mcu_msWait(uint16_t msWait);

/*
 *  ======== mcu_usWait ========
 *  Delay CPU for at least the specified number of microseconds
 *
 *  @param msWait - number of microseconds to delay, a value of 0 causes
 *                  this function to return immediately.
 */
extern void mcu_usWait(uint16_t usWait);


/*
 *  ======== INA229_Reading ========
 *  Structure to hold a single measurement set from an INA229 sensor
 *
 *  @param voltage_V    - Measured bus voltage in Volts
 *  @param current_A    - Measured current in Amperes
 *  @param temperature_C - Measured internal die temperature in Celsius
 */

typedef struct {
    float voltage_V;      // Bus voltage in Volts
    float current_mA;      // Current in milliAmperes
    float temperature_C;  // Internal die temperature in Celsius
    uint8_t cell_id;
    uint8_t reading_valid;
} INA229_Reading;



extern INA229_Reading INA229_Readings[NUM_INA229];


void init_ina229_devices(void);

/* support C++ sources */
#ifdef __cplusplus
}
#endif
#endif /* ti_sensors_MCU__include */

