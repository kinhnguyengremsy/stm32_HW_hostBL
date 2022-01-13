/** 
  ******************************************************************************
  * @file    serialCommon.h
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

#ifndef __SERIAL_COMMON_H
#define __SERIAL_COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
/* Exported define ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @brief  serialCommon_init
    @return none
*/
void serialCommon_init(UART_HandleTypeDef *huart);

/** @brief  serialCommon_sendData
    @return HAL_StatusTypeDef
*/
HAL_StatusTypeDef serialCommon_sendData(uint8_t *data, uint16_t len);

/** @brief  serialCommon_readData
    @return HAL_StatusTypeDef
*/
HAL_StatusTypeDef serialCommon_readData(uint8_t *data, uint16_t len);

/** @brief  serialConsole_init
    @return none
*/
void serialConsole_init(UART_HandleTypeDef *huart);

#endif /* __SERIAL_COMMON_H */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/

