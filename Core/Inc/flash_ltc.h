/*
 * flash_ltc.h
 *
 *  Created on: Oct 7, 2019
 *      Author: Harshana Rathnayake
 */

#ifndef SOURCECODE_ANALOG_FLASH_LTC_H_
#define SOURCECODE_ANALOG_FLASH_LTC_H_

//Battery pack definition
#define NUMBER_OF_LTC_DEVICES
#define NUMBER_OF_CELLS
#define MAX_CELLS_NUMBER_IN_LTC

#define HV_MSP
//#define AUX_MSP
#define TOTAL_IC 2          //Number of ICs in the daisy chain
#define TEMP_PER_IC 3
#define CELLS_PER_IC 12


#define AUX_REGISTERS 6
#define ST_REGISTERS 4
#define PWM_REGISTERS 6
#define SCTRL_REGISTERS 6

/*
 * IC arrangement
 */

#define CURRENT_SENSOR_IC 0
/*
 * Cell balancing
 */

//#define CELL_BALANCING_EN
#define MINUTE  2

//Battery pack information and threshold values
#define dCOV_THRESHOLD                 3600  //COV_THRESHOLD           [mV]
#define dCUV_THRESHOLD                 2500  //CUV_THRESHOLD           [mV]

#define dPACK_OVER_TEMP               60   // PACK_OVER_TEMP1   [C]

#define dDELTA_CHARGE_V                30   //DELTA_CHARGE_V               [mv]
#define dDELTA_DISCHARGE_V             200   //DELTA_DISCHARGE_V            [mV]

#define dCELL_BALANCING_THRESHOLD      20

#endif /* SOURCECODE_ANALOG_FLASH_LTC_H_ */
