/** 
  ******************************************************************************
  * @file    pwmCommon.h
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

#ifndef __PWMCOMMON_H
#define __PWMCOMMON_H

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
/** @brief  pwmCommon_init
    @return none
*/
/** @brief  pwmCommon_init
    @return none
*/
void pwmCommon_init(
    TIM_HandleTypeDef *htimOut1, uint32_t timerOutChannel
    , TIM_HandleTypeDef *htimIn1, uint32_t timerInChannel
);

/** @brief  pwmCommon_process
    @return none
*/
void pwmCommon_process(void);

/** @brief  pwmCommon_write
    @return none
*/
void pwmCommon_write(TIM_HandleTypeDef *htimOut, uint32_t timerChannel, uint32_t value);

/** @brief  pwmCommon_read
    @return value
*/
uint32_t pwmCommon_read(void);
#endif /* __PWMCOMMON_H */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/

