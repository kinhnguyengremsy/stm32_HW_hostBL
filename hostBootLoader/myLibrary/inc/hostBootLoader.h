/** 
  ******************************************************************************
  * @file    hostBootLoader.h
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

#ifndef __HOST_BOOTLOADER_H
#define __HOST_BOOTLOADER_H

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
/** @brief hostBootLoader_init
    @return none
*/
void hostBootLoader_init(UART_HandleTypeDef *huart, UART_HandleTypeDef *huart1);

/** @brief hostBootLoader_process
    @return none
*/
void hostBootLoader_process(void);

#endif /* __HOST_BOOTLOADER_H */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/

