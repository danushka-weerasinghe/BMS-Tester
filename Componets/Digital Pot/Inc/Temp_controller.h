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
void cell12_Temp_01_Set(float resistance);
void cell12_Temp_02_Set(float resistance);
void cell12_Temp_03_Set(float resistance);
void cell11_Temp_01_Set(float resistance);
void cell11_Temp_02_Set(float resistance);
void cell11_Temp_03_Set(float resistance);
void cell12_Temp_01_startup(float resistance);
void cell12_Temp_02_startup(float resistance);
void cell12_Temp_03_startup(float resistance);
void cell11_Temp_01_startup(float resistance);
void cell11_Temp_02_startup(float resistance);
void cell11_Temp_03_startup(float resistance);




#ifdef __cplusplus
}
#endif


#endif /* INC_TEMP_CONTROLLER_H_ */
