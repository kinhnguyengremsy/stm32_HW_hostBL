/**
  ******************************************************************************
  * @file    pwmCommon.c
  * @author  Gremsy Team
  * @version v100
  * @date    2021
  * @brief   This file contains all the functions prototypes for the  
  *          firmware library.
  *
  ************************************************************
  ******************
  * @par
  * COPYRIGHT NOTICE: (c) 2011 Gremsy.
  * All rights reserved.Firmware coding style V1.0.beta
  *
  * The information contained herein is confidential
  * property of Company. The use, copying, transfer or
  * disclosure of such information is prohibited except
  * by express written agreement with Company.
  *
  ******************************************************************************
*/
/* Includes------------------------------------------------------------------------------*/
#include "pwmCommon.h"
/* Private typedef------------------------------------------------------------------------------*/
/* Private define------------------------------------------------------------------------------*/
#define TIMER_IC_CLOCK         84000000
#define TIMER_IC_PRESCALAR     84
/* Private macro------------------------------------------------------------------------------*/
/* Private variables------------------------------------------------------------------------------*/
TIM_HandleTypeDef *htimPwmOut;
TIM_HandleTypeDef *htimPwmIn;

float frequency = 0;
bool caculatorFqComplete = false;
/* Private function prototypes------------------------------------------------------------------------------*/
/* Private functions------------------------------------------------------------------------------*/

/** @group __PWM_COMMON_TIMER_INTERRUPT_CALLBACK
    @{
*/#ifndef __PWM_COMMON_TIMER_INTERRUPT_CALLBACK
#define __PWM_COMMON_TIMER_INTERRUPT_CALLBACK
/** @brief  HAL_TIM_IC_CaptureCallback
    @return none
*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        uint32_t IC_Val1 = 0;
        uint32_t IC_Val2 = 0;
        uint32_t Difference = 0;
        bool Is_First_Captured = false;
        
        if (Is_First_Captured == false) // if the first rising edge is not captured
        {
            IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
            Is_First_Captured = true;  // set the first captured as true
        }

        else   // If the first rising edge is captured, now we will capture the second edge
        {
            IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value

            if (IC_Val2 > IC_Val1)
            {
                Difference = IC_Val2-IC_Val1;
            }

            else if (IC_Val1 > IC_Val2)
            {
                Difference = (0xffffffff - IC_Val1) + IC_Val2;
            }

            float refClock = TIMER_IC_CLOCK / TIMER_IC_PRESCALAR;

            frequency = refClock / Difference;

            __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
            Is_First_Captured = false; // set it back to false
            caculatorFqComplete = true;
        }
    }
}


#endif
/**
    @}
*/


/** @group __PWM_COMMON_READ_AND_WRITE
    @{
*/#ifndef __PWM_COMMON_READ_AND_WRITE
#define __PWM_COMMON_READ_AND_WRITE
/** @brief  pwmCommon_write
    @return none
*/
void pwmCommon_write(TIM_HandleTypeDef *htimOut, uint32_t timerChannel, uint32_t value)
{
    __HAL_TIM_SetCompare(htimOut, timerChannel, value);
}

/** @brief  pwmCommon_read
    @return value
*/
uint32_t pwmCommon_read(void)
{
    return frequency;
}

#endif
/**
    @}
*/


/** @group __PWM_COMMON_INIT
    @{
*/#ifndef __PWM_COMMON_INIT
#define __PWM_COMMON_INIT
/** @brief  pwmOutput_init
    @return none
*/
void pwmOutput_init(TIM_HandleTypeDef *htimOut, uint32_t timerChannel)
{
    htimPwmOut = htimOut;
    
    HAL_TIM_PWM_Start(htimOut, timerChannel);
}

/** @brief  pwmInput_init
    @return none
*/
void pwmInput_init(TIM_HandleTypeDef *htimIn, uint32_t timerChannel)
{
    htimPwmIn = htimIn;
    
    HAL_TIM_IC_Start_IT(htimIn, timerChannel);
}

/** @brief  pwmCommon_init
    @return none
*/
void pwmCommon_init(
    TIM_HandleTypeDef *htimOut1, uint32_t timerOutChannel
    , TIM_HandleTypeDef *htimIn1, uint32_t timerInChannel
)
{
    pwmOutput_init(htimOut1, timerOutChannel);
    pwmOutput_init(htimIn1, timerInChannel);
}

/** @brief  pwmCommon_process
    @return none
*/
void pwmCommon_process(void)
{

}

#endif
/**
    @}
*/

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/

