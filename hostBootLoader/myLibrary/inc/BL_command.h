/** 
  ******************************************************************************
  * @file    BL_command.h
  * @author  Gremsy Team
  * @version v1.0.0
  * @date    2021
  * @brief   This file contains all the functions prototypes for the  
  *          firmware library.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2011 Gremsy. All rights reserved.
  *
  * The information contained herein is confidential
  * property of Company. The use, copying, transfer or 
  * disclosure of such information is prohibited except
  * by express written agreement with Company.
  *
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __BL_COMMAND_H
#define __BL_COMMAND_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
/* Exported define ------------------------------------------------------------*/
//#define DTR_HIGH        HAL_GPIO_WritePin(DTR_GPIO_Port, DTR_Pin, GPIO_PIN_SET) // reset pin in mcu
//#define DTR_LOW         HAL_GPIO_WritePin(DTR_GPIO_Port, DTR_Pin, GPIO_PIN_RESET)
//#define RTS_HIGH        HAL_GPIO_WritePin(RTS_GPIO_Port, RTS_Pin, GPIO_PIN_SET) /// boot0 pin in mcu
//#define RTS_LOW         HAL_GPIO_WritePin(RTS_GPIO_Port, RTS_Pin, GPIO_PIN_RESET)

#define DTR_HIGH        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET) // reset pin in mcu
#define DTR_LOW         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET)
#define RTS_HIGH        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET) /// boot0 pin in mcu
#define RTS_LOW         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET)

#define STM32_ACK       0x79
#define STM32_NACK      0x1F
#define STM32_CMD_INIT  0x7F
#define STM32_CMD_GET   0x00    /* get the version and command supported */
/* Exported types ------------------------------------------------------------*/
typedef enum {
    STM32_ERR_OK = 0,
    STM32_ERR_UNKNOWN,	/* Generic error */
    STM32_ERR_NACK,
    STM32_ERR_NO_CMD,	/* Command not available in bootloader */
} stm32_err_t;
typedef struct __stm32_cmd
{
    uint8_t get;
    uint8_t gvr; // Get Version and Read Protection Status
    uint8_t gid; // Get ID
    uint8_t rm; // Read Memory command
    uint8_t go; // Go command
    uint8_t wm; // Write Memory command
    uint8_t er; /* this may be extended erase */
    //      uint8_t ee; // Erase command or Extended Erase command
    uint8_t wp; // Write Protect command
    uint8_t uw; // Write Unprotect command
    uint8_t rp; // Readout Protect command
    uint8_t ur;
    uint8_t crcCmd;
}stm32_cmd_t;

typedef struct
{
    uint8_t         bl_version;
    uint8_t         version;
    uint8_t         option1, option2;
    uint16_t        pid;
    stm32_cmd_t     cmd;
    uint8_t         writeData[256];
}BL_command_t;
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @brief  BL_command_init
    @return none
*/
void BL_command_init(BL_command_t *BL);

/** @brief  BL_command_process
    @return none
*/
void BL_command_process(BL_command_t *BL);

/** @brief  BL_command_getInfomation
    @return bool
*/
bool BL_command_getInfomation(BL_command_t *BL);

/** @brief  BL_command_getVersion
    @return bool
*/
bool BL_command_getVersion(BL_command_t *BL);

/** @brief  BL_command_getDeviceID
    @return bool
*/
bool BL_command_getDeviceID(BL_command_t *BL);

#endif /* __BL_COMMAND_H */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/

