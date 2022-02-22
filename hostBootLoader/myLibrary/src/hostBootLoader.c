/**
  ******************************************************************************
  * @file    hostBootLoader.c
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
#include "hostBootLoader.h"
#include "serialCommon.h"
#include "BL_command.h"
/* Private typedef------------------------------------------------------------------------------*/
/* Private define------------------------------------------------------------------------------*/

/* Private macro------------------------------------------------------------------------------*/
/* Private variables------------------------------------------------------------------------------*/
BL_command_t command;
/* Private function prototypes------------------------------------------------------------------------------*/
/* Private functions------------------------------------------------------------------------------*/

/** @group __HOST_BOOTLOADER_DEVICE_COMMUNICATION
    @{
*/#ifndef __HOST_BOOTLOADER_DEVICE_COMMUNICATION
#define __HOST_BOOTLOADER_DEVICE_COMMUNICATION

void hostBootLoader_readCallBacl(void *arg)
{
    static uint32_t timeDebug = 0;
    
    if(HAL_GetTick() - timeDebug > 1000)
    {
        timeDebug = HAL_GetTick();
        
        printf("\n[hostBootLoader_readCallBack] waitting response from device\n");
    }
}

/** @brief hostBootLoader_deviceReady
    @return bool
*/
bool hostBootLoader_deviceReady(UART_HandleTypeDef *huart)
{
    bool ret = false;
    uint8_t data = 0;
    uint8_t send = STM32_CMD_INIT;
    
    /// set pin boot0 mcu logic 1 --> 
    RTS_HIGH;
    
    /// set pin rst mcu logic 0
    DTR_LOW;
    HAL_Delay(100);
    /// set pin rst mcu logic 1
    DTR_HIGH;
    HAL_Delay(100);
    
    serialCommon_sendData(&send, 1);
    serialCommon_readData(&data, 1, hostBootLoader_readCallBacl);
    
    if(data == STM32_ACK)
    {
        ret = true;
    }
    
    return ret;
}

#endif
/**
    @}
*/

/** @group __HOST_BOOTLOADER_INIT
    @{
*/#ifndef __HOST_BOOTLOADER_INIT
#define __HOST_BOOTLOADER_INIT
/** @brief hostBootLoader_init
    @return none
*/
void hostBootLoader_init(UART_HandleTypeDef *huart, UART_HandleTypeDef *huart1)
{
    serialCommon_init(huart);
    serialConsole_init(huart1);
    
    /// xoa bo dem uart truoc khi truyen
    __HAL_UART_FLUSH_DRREGISTER(huart);
    
    /// kiem tra device
    if(hostBootLoader_deviceReady(huart) == false)
    {
        HAL_GPIO_WritePin(red_GPIO_Port, red_Pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(blue_GPIO_Port, blue_Pin, GPIO_PIN_RESET);
    }
    
    BL_command_getInfomation(&command);
    
    BL_command_getVersion(&command);
    
    BL_command_getDeviceID(&command);
    
    BL_command_init(&command);
    
//    stm->dev = devices;
//    while(stm->dev->id != 0x00 && stm->dev->id != stm->pid)
//        ++stm->dev;
    
}

/** @brief hostBootLoader_process
    @return none
*/
void hostBootLoader_process(void)
{
    BL_command_process(&command);
}

#endif
/**
    @}
*/

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/

