/**
  ******************************************************************************
  * @file    serialCommon.c
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
#include "serialCommon.h"
/* Private typedef------------------------------------------------------------------------------*/
/* FILE struct is used from __FILE */
struct __FILE {
    int dummy;
};
/* Private define------------------------------------------------------------------------------*/
/* Private macro------------------------------------------------------------------------------*/
/* Private variables------------------------------------------------------------------------------*/
UART_HandleTypeDef *huartCommon;
UART_HandleTypeDef *huartConsole;

bool txComplete;
bool rxComplete;

/* Struct FILE is implemented in stdio.h */
FILE __stdout;
/* Private function prototypes------------------------------------------------------------------------------*/
/* Private functions------------------------------------------------------------------------------*/

/** @group __SERIAL_COMMON_READ_AND_WRITE_DATA
    @{
*/#ifndef __SERIAL_COMMON_READ_AND_WRITE_DATA
#define __SERIAL_COMMON_READ_AND_WRITE_DATA

/** @brief HAL_UART_RxCpltCallback
    @return none
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == huartCommon->Instance)
    {
        rxComplete = true;
        
        __HAL_UART_FLUSH_DRREGISTER(huartCommon);
    }
}

/** @brief HAL_UART_TxCpltCallback
    @return none
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == huartCommon->Instance)
    {
        txComplete = true;
        
        __HAL_UART_FLUSH_DRREGISTER(huartCommon);
    }
}

/** @brief  serialCommon_sendData
    @return HAL_StatusTypeDef
*/
HAL_StatusTypeDef serialCommon_sendData(uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status = HAL_OK;
    
    status = HAL_UART_Transmit_DMA(huartCommon, data, len);
    while(txComplete == false);
    txComplete = false;
    
    return status;
}

/** @brief  serialCommon_readData
    @return HAL_StatusTypeDef
*/
HAL_StatusTypeDef serialCommon_readData(uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status = HAL_OK;
    
    status = HAL_UART_Receive_DMA(huartCommon, data, len);
    while(rxComplete == false);
    rxComplete = false;
    
    return status;
}

#endif
/**
    @}
*/

/** @group __SERIAL_CONSOLE_INIT
    @{
*/#ifndef __SERIAL_CONSOLE_INIT
#define __SERIAL_CONSOLE_INIT
/** @brief  serialConsole_init
    @return none
*/
void serialConsole_init(UART_HandleTypeDef *huart)
{
    huartConsole = huart;
}

/** @brief  fputc
    @return ch
*/
int fputc(int ch, FILE *f) 
{
    /* Do your stuff here */
    /* Send your custom byte */
    HAL_UART_Transmit(huartConsole, (uint8_t *)&ch, 1, 100);

    /* If everything is OK, you have to return character written */
    return ch;
    /* If character is not correct, you can return EOF (-1) to stop writing */
    //return -1;
}


#endif
/**
    @}
*/

/** @group __SERIAL_COMMON_INIT
    @{
*/#ifndef __SERIAL_COMMON_INIT
#define __SERIAL_COMMON_INIT
/** @brief  serialCommon_init
    @return none
*/
void serialCommon_init(UART_HandleTypeDef *huart)
{
    huartCommon = huart;
}

#endif
/**
    @}
*/

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/

