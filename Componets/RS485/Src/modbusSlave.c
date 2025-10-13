/*
 * modbusSlave.c
 *
 *  Created on: Oct 27, 2022
 *      Author: controllerstech.com
 */

#include "modbusSlave.h"
#include "main.h"
#include "string.h"

// MODBUS database definitions
uint16_t Holding_Registers_Database[50]={
	0000,  1111,  2222,  3333,  4444,  5555,  6666,  7777,  8888,  9999,   // 0-9   40001-40010
	12345, 15432, 15535, 10234, 19876, 13579, 10293, 19827, 13456, 14567,  // 10-19 40011-40020
	21345, 22345, 24567, 25678, 26789, 24680, 20394, 29384, 26937, 27654,  // 20-29 40021-40030
	31245, 31456, 34567, 35678, 36789, 37890, 30948, 34958, 35867, 36092,  // 30-39 40031-40040
	45678, 46789, 47890, 41235, 42356, 43567, 40596, 49586, 48765, 41029,  // 40-49 40041-40050
};

uint8_t Coils_Database[25]={
	0b01001001, 0b10011100, 0b10101010, 0b01010101, 0b11001100,    // 0-39    1-40
	0b10100011, 0b01100110, 0b10101111, 0b01100000, 0b10111100,    // 40-79   41-80
	0b11001100, 0b01101100, 0b01010011, 0b11111111, 0b00000000,    // 80-119  81-120
	0b01010101, 0b00111100, 0b00001111, 0b11110000, 0b10001111,    // 120-159 121-160
	0b01010100, 0b10011001, 0b11111000, 0b00001101, 0b00101010,    // 160-199 161-200
};

extern uint8_t RxData_modbus_01[256];
extern uint8_t TxData_modbus_01[256];

extern uint8_t RxData_modbus_02[256];
extern int8_t TxData_modbus_02[256];

extern uint8_t RxData_modbus_03[256];
extern uint8_t TxData_modbus_03[256];

extern uint8_t RxData_modbus_04[256];
extern uint8_t TxData_modbus_04[256];

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;


void sendData (uint8_t *data, int size)
{



	// we will calculate the CRC in this function itself
	uint16_t crc = crc16(data, size);
	data[size] = crc&0xFF;   // CRC LOW
	data[size+1] = (crc>>8)&0xFF;  // CRC HIGH

	// Pull DE high to enable TX operation
	HAL_GPIO_WritePin(GPIOB, USART1_ENABLE_Pin, GPIO_PIN_SET);

	HAL_UART_Transmit(&huart1, data, size+2, 1000);

	// Pull RE Low to enable RX operation

	HAL_GPIO_WritePin(GPIOB, USART1_ENABLE_Pin, GPIO_PIN_RESET);
}

void modbusException (uint8_t exceptioncode)
{
	//| SLAVE_ID | FUNCTION_CODE | Exception code | CRC     |
	//| 1 BYTE   |  1 BYTE       |    1 BYTE      | 2 BYTES |

	TxData_modbus_01[0] = RxData_modbus_01[0];       // slave ID
	TxData_modbus_01[1] = RxData_modbus_01[1]|0x80;  // adding 1 to the MSB of the function code
	TxData_modbus_01[2] = exceptioncode;   // Load the Exception code
	sendData(TxData_modbus_01, 3);         // send Data... CRC will be calculated in the function
}


uint8_t readHoldingRegs (void)
{
	uint16_t startAddr = ((RxData_modbus_01[2]<<8)|RxData_modbus_01[3]);  // start Register Address

	uint16_t numRegs = ((RxData_modbus_01[4]<<8)|RxData_modbus_01[5]);   // number to registers master has requested
	if ((numRegs<1)||(numRegs>125))  // maximum no. of Registers as per the PDF
	{
		modbusException (ILLEGAL_DATA_VALUE);  // send an exception
		return 0;
	}

	uint16_t endAddr = startAddr+numRegs-1;  // end Register
	if (endAddr>49)  // end Register can not be more than 49 as we only have record of 50 Registers in total
	{
		modbusException(ILLEGAL_DATA_ADDRESS);   // send an exception
		return 0;
	}

	// Prepare TxData_modbus_01 buffer

	//| SLAVE_ID | FUNCTION_CODE | BYTE COUNT | DATA      | CRC     |
	//| 1 BYTE   |  1 BYTE       |  1 BYTE    | N*2 BYTES | 2 BYTES |

	TxData_modbus_01[0] = SLAVE_ID;  // slave ID
	TxData_modbus_01[1] = RxData_modbus_01[1];  // function code
	TxData_modbus_01[2] = numRegs*2;  // Byte count
	int indx = 3;  // we need to keep track of how many bytes has been stored in TxData_modbus_01 Buffer

	for (int i=0; i<numRegs; i++)   // Load the actual data into TxData_modbus_01 buffer
	{
		TxData_modbus_01[indx++] = (Holding_Registers_Database[startAddr]>>8)&0xFF;  // extract the higher byte
		TxData_modbus_01[indx++] = (Holding_Registers_Database[startAddr])&0xFF;   // extract the lower byte
		startAddr++;  // increment the register address
	}

	sendData(TxData_modbus_01, indx);  // send data... CRC will be calculated in the function itself
	return 1;   // success
}

uint8_t readInputRegs (void)
{
	uint16_t startAddr = ((RxData_modbus_01[2]<<8)|RxData_modbus_01[3]);  // start Register Address

	uint16_t numRegs = ((RxData_modbus_01[4]<<8)|RxData_modbus_01[5]);   // number to registers master has requested
	if ((numRegs<1)||(numRegs>125))  // maximum no. of Registers as per the PDF
	{
		modbusException (ILLEGAL_DATA_VALUE);  // send an exception
		return 0;
	}

	uint16_t endAddr = startAddr+numRegs-1;  // end Register
	if (endAddr>49)  // end Register can not be more than 49 as we only have record of 50 Registers in total
	{
		modbusException(ILLEGAL_DATA_ADDRESS);   // send an exception
		return 0;
	}

	// Prepare TxData_modbus_01 buffer

	//| SLAVE_ID | FUNCTION_CODE | BYTE COUNT | DATA      | CRC     |
	//| 1 BYTE   |  1 BYTE       |  1 BYTE    | N*2 BYTES | 2 BYTES |

	TxData_modbus_01[0] = SLAVE_ID;  // slave ID
	TxData_modbus_01[1] = RxData_modbus_01[1];  // function code
	TxData_modbus_01[2] = numRegs*2;  // Byte count
	int indx = 3;  // we need to keep track of how many bytes has been stored in TxData_modbus_01 Buffer

	for (int i=0; i<numRegs; i++)   // Load the actual data into TxData_modbus_01 buffer
	{
		TxData_modbus_01[indx++] = (Input_Registers_Database[startAddr]>>8)&0xFF;  // extract the higher byte
		TxData_modbus_01[indx++] = (Input_Registers_Database[startAddr])&0xFF;   // extract the lower byte
		startAddr++;  // increment the register address
	}

	sendData(TxData_modbus_01, indx);  // send data... CRC will be calculated in the function itself
	return 1;   // success
}

uint8_t readCoils (void)
{
	uint16_t startAddr = ((RxData_modbus_01[2]<<8)|RxData_modbus_01[3]);  // start Coil Address

	uint16_t numCoils = ((RxData_modbus_01[4]<<8)|RxData_modbus_01[5]);   // number to coils master has requested
	if ((numCoils<1)||(numCoils>2000))  // maximum no. of coils as per the PDF
	{
		modbusException (ILLEGAL_DATA_VALUE);  // send an exception
		return 0;
	}

	uint16_t endAddr = startAddr+numCoils-1;  // Last coils address
	if (endAddr>199)  // end coil can not be more than 199 as we only have record of 200 (0-199) coils in total
	{
		modbusException(ILLEGAL_DATA_ADDRESS);   // send an exception
		return 0;
	}


	//reset TxData_modbus_01 buffer
	memset (TxData_modbus_01, '\0', 256);

	// Prepare TxData_modbus_01 buffer

	//| SLAVE_ID | FUNCTION_CODE | BYTE COUNT | DATA      | CRC     |
	//| 1 BYTE   |  1 BYTE       |  1 BYTE    | N*2 BYTES | 2 BYTES |

	TxData_modbus_01[0] = SLAVE_ID;  // slave ID
	TxData_modbus_01[1] = RxData_modbus_01[1];  // function code
	TxData_modbus_01[2] = (numCoils/8) + ((numCoils%8)>0 ? 1:0);  // Byte count
	int indx = 3;  // we need to keep track of how many bytes has been stored in TxData_modbus_01 Buffer

	/* The approach is simple. We will read 1 bit at a time and store them in the TxData_modbus_01 buffer.
	 * First find the offset in the first byte we read from, for eg- if the start coil is 13,
	 * we will read from database[1] with an offset of 5. This bit will be stored in the TxData_modbus_01[0] at 0th position.
	 * Then we will keep shifting the database[1] to the right and read the bits.
	 * Once the bitposition has crossed the value 7, we will increment the startbyte
	 * When the indxposition exceeds 7, we increment the indx variable, so to copy into the next byte of the TxData_modbus_01
	 * This keeps going until the number of coils required have been copied
	 */
	int startByte = startAddr/8;  // which byte we have to start extracting the data from
	uint16_t bitPosition = startAddr%8;  // The shift position in the first byte
	int indxPosition = 0;  // The shift position in the current indx of the TxData_modbus_01 buffer

	// Load the actual data into TxData_modbus_01 buffer
	for (int i=0; i<numCoils; i++)
	{
		TxData_modbus_01[indx] |= ((Coils_Database[startByte] >> bitPosition) &0x01) << indxPosition;
		indxPosition++; bitPosition++;
		if (indxPosition>7)  // if the indxposition exceeds 7, we have to copy the data into the next byte position
		{
			indxPosition = 0;
			indx++;
		}
		if (bitPosition>7)  // if the bitposition exceeds 7, we have to increment the startbyte
		{
			bitPosition=0;
			startByte++;
		}
	}

	if (numCoils%8 != 0)indx++;  // increment the indx variable, only if the numcoils is not a multiple of 8
	sendData(TxData_modbus_01, indx);  // send data... CRC will be calculated in the function itself
	return 1;   // success
}

uint8_t readInputs (void)
{
	uint16_t startAddr = ((RxData_modbus_01[2]<<8)|RxData_modbus_01[3]);  // start Register Address

	uint16_t numCoils = ((RxData_modbus_01[4]<<8)|RxData_modbus_01[5]);   // number to coils master has requested
	if ((numCoils<1)||(numCoils>2000))  // maximum no. of coils as per the PDF
	{
		modbusException (ILLEGAL_DATA_VALUE);  // send an exception
		return 0;
	}

	uint16_t endAddr = startAddr+numCoils-1;  // Last coils address
	if (endAddr>199)  // end coil can not be more than 199 as we only have record of 200 (0-199) coils in total
	{
		modbusException(ILLEGAL_DATA_ADDRESS);   // send an exception
		return 0;
	}


	//reset TxData_modbus_01 buffer
	memset (TxData_modbus_01, '\0', 256);

	// Prepare TxData_modbus_01 buffer

	//| SLAVE_ID | FUNCTION_CODE | BYTE COUNT | DATA      | CRC     |
	//| 1 BYTE   |  1 BYTE       |  1 BYTE    | N*2 BYTES | 2 BYTES |

	TxData_modbus_01[0] = SLAVE_ID;  // slave ID
	TxData_modbus_01[1] = RxData_modbus_01[1];  // function code
	TxData_modbus_01[2] = (numCoils/8) + ((numCoils%8)>0 ? 1:0);  // Byte count
	int indx = 3;  // we need to keep track of how many bytes has been stored in TxData_modbus_01 Buffer

	/* The approach is simple. We will read 1 bit at a time and store them in the TxData_modbus_01 buffer.
	 * First find the offset in the first byte we read from, for eg- if the start coil is 13,
	 * we will read from database[1] with an offset of 5. This bit will be stored in the TxData_modbus_01[0] at 0th position.
	 * Then we will keep shifting the database[1] to the right and read the bits.
	 * Once the bitposition has crossed the value 7, we will increment the startbyte
	 * When the indxposition exceeds 7, we increment the indx variable, so to copy into the next byte of the TxData_modbus_01
	 * This keeps going until the number of coils required have been copied
	 */
	int startByte = startAddr/8;  // which byte we have to start extracting the data from
	uint16_t bitPosition = startAddr%8;  // The shift position in the first byte
	int indxPosition = 0;  // The shift position in the current indx of the TxData_modbus_01 buffer

	// Load the actual data into TxData_modbus_01 buffer
	for (int i=0; i<numCoils; i++)
	{
		TxData_modbus_01[indx] |= ((Inputs_Database[startByte] >> bitPosition) &0x01) << indxPosition;
		indxPosition++; bitPosition++;
		if (indxPosition>7)  // if the indxposition exceeds 7, we have to copy the data into the next byte position
		{
			indxPosition = 0;
			indx++;
		}
		if (bitPosition>7)  // if the bitposition exceeds 7, we have to increment the startbyte
		{
			bitPosition=0;
			startByte++;
		}
	}

	if (numCoils%8 != 0)indx++;  // increment the indx variable, only if the numcoils is not a multiple of 8
	sendData(TxData_modbus_01, indx);  // send data... CRC will be calculated in the function itself
	return 1;   // success
}

uint8_t writeHoldingRegs (void)
{
	uint16_t startAddr = ((RxData_modbus_01[2]<<8)|RxData_modbus_01[3]);  // start Register Address

	uint16_t numRegs = ((RxData_modbus_01[4]<<8)|RxData_modbus_01[5]);   // number to registers master has requested
	if ((numRegs<1)||(numRegs>123))  // maximum no. of Registers as per the PDF
	{
		modbusException (ILLEGAL_DATA_VALUE);  // send an exception
		return 0;
	}

	uint16_t endAddr = startAddr+numRegs-1;  // end Register
	if (endAddr>49)  // end Register can not be more than 49 as we only have record of 50 Registers in total
	{
		modbusException(ILLEGAL_DATA_ADDRESS);   // send an exception
		return 0;
	}

	/* start saving 16 bit data
	 * Data starts from RxData_modbus_01[7] and we need to combine 2 bytes together
	 * 16 bit Data = firstByte<<8|secondByte
	 */
	int indx = 7;  // we need to keep track of index in RxData_modbus_01
	for (int i=0; i<numRegs; i++)
	{
		Holding_Registers_Database[startAddr++] = (RxData_modbus_01[indx]<<8)|RxData_modbus_01[indx+1];
		indx += 2;  // Increment by 2 since we read 2 bytes per register
	}

	// Prepare Response

	//| SLAVE_ID | FUNCTION_CODE | Start Addr | num of Regs    | CRC     |
	//| 1 BYTE   |  1 BYTE       |  2 BYTE    | 2 BYTES      | 2 BYTES |

	TxData_modbus_01[0] = SLAVE_ID;    // slave ID
	TxData_modbus_01[1] = RxData_modbus_01[1];   // function code
	TxData_modbus_01[2] = RxData_modbus_01[2];   // Start Addr HIGH Byte
	TxData_modbus_01[3] = RxData_modbus_01[3];   // Start Addr LOW Byte
	TxData_modbus_01[4] = RxData_modbus_01[4];   // num of Regs HIGH Byte
	TxData_modbus_01[5] = RxData_modbus_01[5];   // num of Regs LOW Byte

	sendData(TxData_modbus_01, 6);  // send data... CRC will be calculated in the function itself
	return 1;   // success
}

uint8_t writeSingleReg (void)
{
	uint16_t startAddr = ((RxData_modbus_01[2]<<8)|RxData_modbus_01[3]);  // start Register Address

	if (startAddr>49)  // The Register Address can not be more than 49 as we only have record of 50 Registers in total
	{
		modbusException(ILLEGAL_DATA_ADDRESS);   // send an exception
		return 0;
	}

	/* Save the 16 bit data
	 * Data is the combination of 2 bytes, RxData_modbus_01[4] and RxData_modbus_01[5]
	 */

	Holding_Registers_Database[startAddr] = (RxData_modbus_01[4]<<8)|RxData_modbus_01[5];

	// Prepare Response

	//| SLAVE_ID | FUNCTION_CODE | Start Addr | Data     | CRC     |
	//| 1 BYTE   |  1 BYTE       |  2 BYTE    | 2 BYTES  | 2 BYTES |

	TxData_modbus_01[0] = SLAVE_ID;    // slave ID
	TxData_modbus_01[1] = RxData_modbus_01[1];   // function code
	TxData_modbus_01[2] = RxData_modbus_01[2];   // Start Addr HIGH Byte
	TxData_modbus_01[3] = RxData_modbus_01[3];   // Start Addr LOW Byte
	TxData_modbus_01[4] = RxData_modbus_01[4];   // Reg Data HIGH Byte
	TxData_modbus_01[5] = RxData_modbus_01[5];   // Reg Data LOW  Byte

	sendData(TxData_modbus_01, 6);  // send data... CRC will be calculated in the function itself
	return 1;   // success
}

uint8_t writeSingleCoil (void)
{
	uint16_t startAddr = ((RxData_modbus_01[2]<<8)|RxData_modbus_01[3]);  // start Coil Address

	if (startAddr>199)  // The Coil Address can not be more than 199 as we only have record of 200 Coils in total
	{
		modbusException(ILLEGAL_DATA_ADDRESS);   // send an exception
		return 0;
	}

	/* Calculation for the bit in the database, where the modification will be done */
	int startByte = startAddr/8;  // which byte we have to start writing the data into
	uint16_t bitPosition = startAddr%8;  // The shift position in the first byte


	/* The next 2 bytes in the RxData_modbus_01 determines the state of the coil
	 * A value of FF 00 hex requests the coil to be ON.
	 * A value of 00 00 requests it to be OFF.
	 * All other values are illegal and will not affect the coil.
	 */

	if ((RxData_modbus_01[4] == 0xFF) && (RxData_modbus_01[5] == 0x00))
	{
		Coils_Database[startByte] |= 1<<bitPosition; // Replace that bit with 1
	}

	else if ((RxData_modbus_01[4] == 0x00) && (RxData_modbus_01[5] == 0x00))
	{
		Coils_Database[startByte] &= ~(1<<bitPosition); // Replace that bit with 0
	}

	// Prepare Response

	//| SLAVE_ID | FUNCTION_CODE | Start Addr | Data     | CRC     |
	//| 1 BYTE   |  1 BYTE       |  2 BYTE    | 2 BYTES  | 2 BYTES |

	TxData_modbus_01[0] = SLAVE_ID;    // slave ID
	TxData_modbus_01[1] = RxData_modbus_01[1];   // function code
	TxData_modbus_01[2] = RxData_modbus_01[2];   // Start Addr HIGH Byte
	TxData_modbus_01[3] = RxData_modbus_01[3];   // Start Addr LOW Byte
	TxData_modbus_01[4] = RxData_modbus_01[4];   // Coil Data HIGH Byte
	TxData_modbus_01[5] = RxData_modbus_01[5];   // Coil Data LOW  Byte

	sendData(TxData_modbus_01, 6);  // send data... CRC will be calculated in the function itself
	return 1;   // success
}

uint8_t writeMultiCoils (void)
{
	uint16_t startAddr = ((RxData_modbus_01[2]<<8)|RxData_modbus_01[3]);  // start Coil Address

	uint16_t numCoils = ((RxData_modbus_01[4]<<8)|RxData_modbus_01[5]);   // number to coils master has requested
	if ((numCoils<1)||(numCoils>1968))  // maximum no. of coils as per the PDF
	{
		modbusException (ILLEGAL_DATA_VALUE);  // send an exception
		return 0;
	}

	uint16_t endAddr = startAddr+numCoils-1;  // Last coils address
	if (endAddr>199)  // end coil can not be more than 199 as we only have record of 200 (0-199) coils in total
	{
		modbusException(ILLEGAL_DATA_ADDRESS);   // send an exception
		return 0;
	}

	/* Calculation for the bit in the database, where the modification will be done */
	int startByte = startAddr/8;  // which byte we have to start writing the data into
	uint16_t bitPosition = startAddr%8;  // The shift position in the first byte
	int indxPosition = 0;  // The shift position in the current indx of the RxData_modbus_01 buffer

	int indx = 7;  // we need to keep track of index in RxData_modbus_01

	/* The approach is simple. We will read 1 bit (starting from the very first bit in the RxData_modbus_01 Buffer)
	 * at a time and store them in the Database.
	 * First find the offset in the first byte we write into, for eg- if the start coil is 13,
	 * we will Write into database[1] with an offset of 5. This bit is read from the RxData_modbus_01[indx] at 0th indxposition.
	 * Then we will keep shifting the RxData_modbus_01[indx] to the right and read the bits.
	 * Once the bitposition has crossed the value 7, we will increment the startbyte and start modifying the next byte in the database
	 * When the indxposition exceeds 7, we increment the indx variable, so to copy from the next byte of the RxData_modbus_01
	 * This keeps going until the number of coils required have been modified
	 */

	// Modify the bits as per the Byte received
	for (int i=0; i<numCoils; i++)
	{
		if (((RxData_modbus_01[indx]>>indxPosition)&0x01) == 1)
		{
			Coils_Database[startByte] |= 1<<bitPosition;  // replace that bit with 1
		}
		else
		{
			Coils_Database[startByte] &= ~(1<<bitPosition);  // replace that bit with 0
		}

		bitPosition++; indxPosition++;

		if (indxPosition>7)  // if the indxposition exceeds 7, we have to copy the data into the next byte position
		{
			indxPosition = 0;
			indx++;
		}
		if (bitPosition>7)  // if the bitposition exceeds 7, we have to increment the startbyte
		{
			bitPosition=0;
			startByte++;
		}
	}

	// Prepare Response

	//| SLAVE_ID | FUNCTION_CODE | Start Addr | Data     | CRC     |
	//| 1 BYTE   |  1 BYTE       |  2 BYTE    | 2 BYTES  | 2 BYTES |

	TxData_modbus_01[0] = SLAVE_ID;    // slave ID
	TxData_modbus_01[1] = RxData_modbus_01[1];   // function code
	TxData_modbus_01[2] = RxData_modbus_01[2];   // Start Addr HIGH Byte
	TxData_modbus_01[3] = RxData_modbus_01[3];   // Start Addr LOW Byte
	TxData_modbus_01[4] = RxData_modbus_01[4];   // num of coils HIGH Byte
	TxData_modbus_01[5] = RxData_modbus_01[5];   // num of coils LOW  Byte

	sendData(TxData_modbus_01, 6);  // send data... CRC will be calculated in the function itself
	return 1;   // success
}

// Helper function to parse command from RxData_modbus_01 buffer
uint32_t parse_command_from_buffer(uint16_t Size)
{
	// Check if 'm' command was received (Size can be 1 or 2 due to null terminator)
	if ((Size == 1 || Size == 2) && RxData_modbus_01[0] == 'm')
	{
		return 'm';
	}

	if (Size >= 1)
	{
		// Check if it's a two-digit command (10-31) FIRST
		if (Size >= 2 && RxData_modbus_01[0] >= '1' && RxData_modbus_01[0] <= '3' &&
			RxData_modbus_01[1] >= '0' && RxData_modbus_01[1] <= '9')
		{
			return (RxData_modbus_01[0] - '0') * 10 + (RxData_modbus_01[1] - '0');
		}

		// Check if it's a single digit command (1-9) ONLY if not two-digit
		if (RxData_modbus_01[0] >= '1' && RxData_modbus_01[0] <= '9')
		{
			return RxData_modbus_01[0] - '0';
		}
	}

	return 0; // Invalid command
}
