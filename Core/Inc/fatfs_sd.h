#ifndef __FATFS_SD_H
#define __FATFS_SD_H

#include "fatfs.h"
#include <string.h>
#include <stdio.h>
#include "stdbool.h"

/* Definitions for MMC/SDC command */
#define CMD0     (0x40+0)     	/* GO_IDLE_STATE */
#define CMD1     (0x40+1)     	/* SEND_OP_COND */
#define CMD8     (0x40+8)     	/* SEND_IF_COND */
#define CMD9     (0x40+9)     	/* SEND_CSD */
#define CMD10    (0x40+10)    	/* SEND_CID */
#define CMD12    (0x40+12)    	/* STOP_TRANSMISSION */
#define CMD16    (0x40+16)    	/* SET_BLOCKLEN */
#define CMD17    (0x40+17)    	/* READ_SINGLE_BLOCK */
#define CMD18    (0x40+18)    	/* READ_MULTIPLE_BLOCK */
#define CMD23    (0x40+23)    	/* SET_BLOCK_COUNT */
#define CMD24    (0x40+24)    	/* WRITE_BLOCK */
#define CMD25    (0x40+25)    	/* WRITE_MULTIPLE_BLOCK */
#define CMD41    (0x40+41)    	/* SEND_OP_COND (ACMD) */
#define CMD55    (0x40+55)    	/* APP_CMD */
#define CMD58    (0x40+58)    	/* READ_OCR */

/* MMC card type flags (MMC_GET_TYPE) */
#define CT_MMC		0x01		/* MMC ver 3 */
#define CT_SD1		0x02		/* SD ver 1 */
#define CT_SD2		0x04		/* SD ver 2 */
#define CT_SDC		0x06		/* SD */
#define CT_BLOCK	0x08		/* Block addressing */

//typedef enum {
//    SD_STATE_REMOVED,
//    SD_STATE_INSERTED,
//    SD_STATE_MOUNTED,
////    SD_STATE_ERROR
//} SD_CardState;
//
//typedef struct {
//    SD_CardState state;
//    FATFS fs;
//    uint32_t total_space;
//    uint32_t free_space;
//    uint32_t last_check;
//    char status_message[64];
//} SD_CardInfo;

extern bool sd_ready;

typedef enum {
    SD_OK = 0,
    SD_MOUNT_ERROR,
    SD_FILE_ERROR,
    SD_WRITE_ERROR,
    SD_READ_ERROR
} SD_Status;

//SD_CardState SD_GetState(void);
//const char* SD_GetStatusMessage(void);
void SD_Handler(void);
void Check_SDCard(void);
/* Function prototypes */

static void SD_PowerOff(void);
SD_Status SD_Init(void);
SD_Status SD_DeInit(void);
SD_Status SD_Get_Space(char* buffer, uint32_t* total, uint32_t* free_space);
SD_Status SD_Write_File(const char* filename, const char* data);
SD_Status SD_Read_File(const char* filename, char* buffer);
SD_Status SD_Append_File(const char* filename, const char* data);
SD_Status SD_Delete_File(const char* path);

/* Functions */
DSTATUS SD_disk_initialize (BYTE pdrv);
DSTATUS SD_disk_status (BYTE pdrv);
DRESULT SD_disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT SD_disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT SD_disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);

#define SPI_TIMEOUT 100

#endif

/* Example use */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/*
#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE];
uint32_t Total, Free;
*/
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
/*
int bufsize (char *buf)
{
	int i=0;
	while (*buf++ != '\0') i++;
	return i;
}

void clear_buffer (void)
{
	for (int i=0; i<BUFFER_SIZE; i++) buffer[i] = '\0';
}
*/
/* USER CODE END 0 */

/* USER CODE BEGIN 0 */
/*
  // Initialize SD card and check status
  if(SD_Init() == SD_OK) {
      display_popup("SD Card Ready");

      // Get and display SD card space
      if(SD_Get_Space(buffer, &Total, &Free) == SD_OK) {
          display_lcd(buffer);
          HAL_Delay(1000);
          clear_buffer();
      }

//      SD_Delete_File("test5.txt");

      // Write test file
//      SD_Write_File("test5.txt", "B");

      // Append to file
      SD_Append_File("test5.txt", "A");

      // Read file back
      memset(buffer, 0, sizeof(buffer));
      if(SD_Read_File("test5.txt", buffer) == SD_OK) {
          display_lcd(buffer);
          HAL_Delay(1000);
          clear_buffer();
      }
  } else {
      display_lcd("SD Init Failed");
  }
*/
/* USER CODE END 0 */

/*
  while (1)
  {
	  SD_Handler();
	  HAL_Delay(10);
*/
