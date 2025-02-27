/*
 * Temp_controller.h
 *
 *  Created on: Feb 26, 2025
 *      Author: Asus
 */

#ifndef INC_TEMP_CONTROLLER_H_
#define INC_TEMP_CONTROLLER_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "stdbool.h"
void intToBinary(int num, int *binaryArray);
void assignBytes(int *resArray, uint8_t *r);
bool arrayComparison (uint8_t *array1,uint8_t *array2);
void cell12Temp01Set(float resistance);
void cell12Temp02Set(float resistance);
void cell12Temp03Set(float resistance);
void cell11Temp01Set(float resistance);
void cell11Temp02Set(float resistance);
void cell11Temp03Set(float resistance);




#ifdef __cplusplus
}
#endif


#endif /* INC_TEMP_CONTROLLER_H_ */
