/*
 * calibration.c
 *
 *  Created on: Mar 3, 2025
 *      Author: Asus
 */


#include <math.h>
#include "config.h"
#include "INA229.h"
#include "main.h"
#include "main_data.h"
#include "mcu.h"


double shuntResistor = 1;
double maxCurrent = 0.1;


double correct_Current_LSB(double maxCurrent){

	double currentLSB = (maxCurrent)/(pow(2,19));
	int exponent = (int) floor(log10(fabs(currentLSB)));
	double mantissa = currentLSB / pow(10, exponent);
	mantissa = ceil(mantissa);
	double correctLSB = mantissa*(pow(10,exponent));
	return correctLSB;

}

uint16_t calibrationValue (double shuntResistor){
	double currentLSB = correct_Current_LSB(maxCurrent);
	int calibrationConstant = 13107.2 * (pow(10,6))*shuntResistor*currentLSB;
	return((uint16_t)calibrationConstant);


}





