/**
  ******************************************************************************
  * @file    BL_command.c
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
#include "BL_command.h"
#include "serialCommon.h"
/* Private typedef------------------------------------------------------------------------------*/

/* Private define------------------------------------------------------------------------------*/
#define STM32_MAX_FRAME  256	/* cmd read memory */
#define STM32_MAX_TX_FRAME  (1 + STM32_MAX_FRAME + 1)	/* cmd write memory */

#define STM32_MAX_PAGES     0x0000ffff
#define STM32_MASS_ERASE    0x00100000 /* > 2 x max_pages */

/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

/* Private macro------------------------------------------------------------------------------*/
/* Private variables------------------------------------------------------------------------------*/
static uint8_t flashState = 0;
/* Private function prototypes------------------------------------------------------------------------------*/
/* Private functions------------------------------------------------------------------------------*/

/** @group __BL_COMMAND_READ_DATA_FROM_FLASH
    @{
*/#ifndef __BL_COMMAND_READ_DATA_FROM_FLASH
#define __BL_COMMAND_READ_DATA_FROM_FLASH
/** @brief  BL_readDataFromFlash
    @return bool
*/
static void BL_readDataFromFlash(BL_command_t *BL, uint32_t flashStartAddress)
{
    uint32_t data = 0;
    
    /// clear buffer
    memset(BL->writeData, 0, STM32_MAX_FRAME);

    for(uint16_t i = 0; i < STM32_MAX_FRAME; i+=4)
    {
        /// read flash
        data = *(__IO uint32_t*)flashStartAddress;
        
        memcpy(&BL->writeData[i], (uint8_t *)&data, 4);
        
        printf("\n[%d][%s] data from flash address 0x%x value = 0x%x\n", __LINE__, __FUNCTION__, (int)flashStartAddress, (int)data);
        
        flashStartAddress += 4;
    }
}

#endif
/**
    @}
*/

/** @group __BL_COMMAND_READ_AND_WRITE
    @{
*/#ifndef __BL_COMMAND_READ_AND_WRITE
#define __BL_COMMAND_READ_AND_WRITE

/** @brief  BL_readAndWriteCallBack
    @return none
*/
void BL_readAndWriteCallBack(void *arg)
{
    static uint32_t time = 0;
    static uint32_t timeSet = 0;
    if(HAL_GetTick() - time > timeSet || time == 0)
    {
        time = HAL_GetTick();
        
        HAL_GPIO_TogglePin(blue_GPIO_Port, blue_Pin);
    }
    
    switch(flashState)
    {
        case 0 :
        {
            
        }break;
        case 1 :
        {
            timeSet = 100;
        }break;
        case 2 :
        {
            timeSet = 0;
        }break;
    }
}

/** @brief  BL_sendCommand
    @return stm32_err_t
*/
static stm32_err_t BL_sendCommand(uint8_t *command, uint16_t len, uint8_t *rData, uint16_t rLen, uint8_t ackByte)
{
    HAL_StatusTypeDef status = HAL_OK;
    
    status = serialCommon_sendData(command, len);
    
    /// read ack
    status = serialCommon_readData(rData, rLen, BL_readAndWriteCallBack);
    if(status == HAL_OK && ackByte == STM32_NACK)
    {
        printf("[BL_sendCommand] [ERROR] Command not found !\n");
        
        return STM32_ERR_NACK;
    }
    
    return STM32_ERR_OK;
}

/** @brief  BL_send_command
    @return stm32_err_t
*/
static stm32_err_t BL_get_command(BL_command_t *BL, uint8_t *data)
{
    stm32_err_t err = STM32_ERR_OK;
    uint8_t getCommand[2] = {0};
    
    getCommand[0] = STM32_CMD_GET;
    getCommand[1] = 0xff;
    
    err = BL_sendCommand(getCommand, 2, data, 15, data[0]);
    if(err != STM32_ERR_OK)
    {
        printf("[BL_get_command] [ERROR] %d\n", err);
        
        return err;
    }
    
    return STM32_ERR_OK;
}

/** @brief  BL_getVersionAndRPStatus_command
    @return stm32_err_t
*/
static stm32_err_t BL_getVersionAndRPStatus_command(BL_command_t *BL, uint8_t *data)
{
    stm32_err_t err = STM32_ERR_OK;
    uint8_t getVersionCommand[2] = {0};
    
    getVersionCommand[0] = BL->cmd.gvr;
    getVersionCommand[1] = 0xfe;
    
    err = BL_sendCommand(getVersionCommand, 2, data, 5, data[0]);
    if(err != STM32_ERR_OK)
    {
        printf("[BL_getVersionAndRPStatus_command] [ERROR] %d\n", err);
        
        return err;
    }
    
    return STM32_ERR_OK;
}

/** @brief  BL_getDeviceID_command
    @return bool
*/
static stm32_err_t BL_getDeviceID_command(BL_command_t *BL, uint8_t *data)
{
    stm32_err_t err = STM32_ERR_OK;
    uint8_t getDeviceIdCommand[2] = {0};
    
    getDeviceIdCommand[0] = BL->cmd.gid;
    getDeviceIdCommand[1] = 0xfd;
    
    err = BL_sendCommand(getDeviceIdCommand, 2, data, 5, data[0]);
    if(err != STM32_ERR_OK)
    {
        printf("[BL_getDeviceID_command] [ERROR] %d\n", err);
        
        return err;
    }
    
    return STM32_ERR_OK;
}

/** @brief  BL_readMemory_command
    @return stm32_err_t
*/
static stm32_err_t BL_readMemory_command(uint32_t address, uint8_t *data, uint16_t len)
{
    stm32_err_t err = STM32_ERR_OK;
    uint8_t ackByte = 0;
    uint8_t buffer[2];
    uint8_t addressBuffer[5];
    buffer[0] = 0x11;
    buffer[1] = 0xEE;
    
    if(!len)
    {
        return STM32_ERR_OK;
    }
    
    if(len > STM32_MAX_FRAME)
    {
        printf("Error: READ length limit at STM32_MAX_FRAME bytes\n");
        
        return STM32_ERR_UNKNOWN;
    }
    
    printf("[BL_readMemory_command] Read memory ....\n");
    
    err = BL_sendCommand(buffer, 2, &ackByte, 1, ackByte);
    if(err != STM32_ERR_OK)
    {
        return STM32_ERR_NO_CMD;
    }
    
    addressBuffer[0] = address >> 24;
    addressBuffer[1] = (address >> 16) & 0xFF;
    addressBuffer[2] = (address >> 8) & 0xFF;
    addressBuffer[3] = address & 0xFF;
    addressBuffer[4] = addressBuffer[0] ^ addressBuffer[1] ^ addressBuffer[2] ^ addressBuffer[3];

    printf("[BL_readMemory_command] Read memory from address 0x%x\n", address);
    
    /// send address
    err = BL_sendCommand(addressBuffer, 5, &ackByte, 1, ackByte);
    if(err != STM32_ERR_OK)
    {
        return STM32_ERR_NO_CMD;
    }
    
    uint8_t bufferNbyte[2];
    bufferNbyte[0] = len - 1;
    bufferNbyte[1] = bufferNbyte[0] ^ 0xff;
    
    // send the number of bytes to be read (1 byte) and a crc (1 byte)
    err = BL_sendCommand(bufferNbyte, 2, data, len, data[0]);
    
    return STM32_ERR_OK;
}

/** @brief  BL_readOutProtect_command
    @return stm32_err_t
*/
static stm32_err_t BL_readOutProtect_command(const uint8_t command)
{
    stm32_err_t err = STM32_ERR_OK;
    uint8_t ackByte[2];
    uint8_t buffer[2];
    buffer[0] = command;
    buffer[1] = 0x7D;
    
    err = BL_sendCommand(buffer, 2, ackByte, 1, ackByte[1]);
    if(err != STM32_ERR_OK)
    {
        return err;
    }
    
    return STM32_ERR_OK;
}

/** @brief  BL_readOutUnProtect_command
    @return stm32_err_t
*/
static stm32_err_t BL_readOutUnProtect_command(const uint8_t command)
{
    stm32_err_t err = STM32_ERR_OK;
    uint8_t ackByte[2];
    uint8_t buffer[2];
    buffer[0] = command;
    buffer[1] = 0x6D;
    
    err = BL_sendCommand(buffer, 2, ackByte, 1, ackByte[1]);
    if(err != STM32_ERR_OK)
    {
        return err;
    }
    
    return STM32_ERR_OK;
}

/** @brief  BL_writeUnProtect_command
    @return stm32_err_t
*/
static stm32_err_t BL_writeUnProtect_command(const uint8_t command)
{
    stm32_err_t err = STM32_ERR_OK;
    uint8_t ackByte[2] = {0};
    uint8_t buffer[2];
    buffer[0] = command;
    buffer[1] = 0x8C;
    
    err = BL_sendCommand(buffer, 2, ackByte, 1, ackByte[1]);
    if(err != STM32_ERR_OK)
    {
        return err;
    }
    
    return STM32_ERR_OK;
}


/** @brief  BL_writeMemory_command
    @return stm32_err_t
*/
static stm32_err_t BL_writeMemory_command(BL_command_t *BL, uint32_t address, uint8_t *data, uint16_t len)
{
    stm32_err_t err = STM32_ERR_OK;
    uint8_t ackByte = 0;
    uint8_t buffer[2] = {0};
    uint8_t addressBuffer[5] = {0};
    uint8_t cs = 0;
    uint8_t dataBuffer[STM32_MAX_FRAME + 2] = {0};
    uint16_t i, aligned_len;
    
    buffer[0] = BL->cmd.wm;
    buffer[1] = 0xCE;
    
    if(!len)
    {
        return STM32_ERR_OK;
    }
    
    if(len > STM32_MAX_FRAME)
    {
        return STM32_ERR_UNKNOWN;
    }
    uint32_t check = address & 0x30;
    if(check)
    {
        return STM32_ERR_NO_CMD;
    }
    
    err = BL_sendCommand(buffer, 2, &ackByte, 1, ackByte);
    if(err != STM32_ERR_OK)
    {
        return err;
    }
    
    addressBuffer[0] = address >> 24;
    addressBuffer[1] = (address >> 16) & 0xFF;
    addressBuffer[2] = (address >> 8) & 0xFF;
    addressBuffer[3] = address & 0xFF;
    addressBuffer[4] = addressBuffer[0] ^ addressBuffer[1] ^ addressBuffer[2] ^ addressBuffer[3];
    
    err = BL_sendCommand(addressBuffer, 5, &ackByte, 1, ackByte);
    if(err != STM32_ERR_OK)
    {
        return err;
    }
    
    aligned_len = (len + 3) & ~3;
    cs = aligned_len - 1;
    dataBuffer[0] = aligned_len - 1;
    for (i = 0; i < len; i++) 
    {
        cs ^= data[i];
        dataBuffer[i + 1] = data[i];
    }
    /* padding data */
    for (i = len; i < aligned_len; i++) 
    {
        cs ^= 0xFF;
        dataBuffer[i + 1] = 0xFF;
    }
    dataBuffer[aligned_len + 1] = cs;
    
    err = BL_sendCommand(dataBuffer, aligned_len + 2, &ackByte, 1, ackByte);
    if(err != STM32_ERR_OK)
    {
        return err;
    }
    
    return STM32_ERR_OK;
}

/** @brief  BL_eraseMemory_command
    @return stm32_err_t
*/
static stm32_err_t BL_eraseMemory_command(uint8_t command)//(uint32_t fPage, uint32_t nPage)
{
//    HAL_StatusTypeDef status = HAL_OK;
    stm32_err_t err = STM32_ERR_OK;
    uint8_t ackByte = 0;
    uint8_t buffer[2];
    buffer[0] = command;
    buffer[1] = 0xBB;
    
    printf("[BL_eraseMemory_command] Erase memory ....\n");
    
    err = BL_sendCommand(buffer, 2, &ackByte, 1, ackByte);
    if(err != STM32_ERR_OK)
    {
        return err;
    }
    
    printf("[BL_eraseMemory_command] Erase memory running ....\n");
    
    uint8_t eBuffer[3];
    eBuffer[0] = 0xff;
    eBuffer[1] = 0xff;
    eBuffer[2] = 0x00;
    
    err = BL_sendCommand(eBuffer, 3, &ackByte, 1, ackByte);
    if(err != STM32_ERR_OK)
    {
        printf("[BL_eraseMemory_command] Erase memory fail ....\n");
        
        return err;
    }
    
    printf("[BL_eraseMemory_command] Erase memory successfull ....\n");
    
    return STM32_ERR_OK;
}

/** @brief  BL_verifyMemory
    @return stm32_err_t
*/
static stm32_err_t BL_verifyMemory(uint32_t addr, uint8_t *dataVerify, uint16_t len)
{
    uint8_t compareBuffer[STM32_MAX_FRAME];
    uint16_t offset = 0;
    uint16_t rlen = 0;
//    uint8_t retry = 10;
//    uint8_t failed = 0;
    stm32_err_t err = STM32_ERR_OK;
    
    printf("[BL_verifyMemory] Verify memory address 0x%x\n", addr);
    
    while(offset < len)
    {
        rlen = len - offset;
        rlen = rlen < STM32_MAX_FRAME ? rlen : STM32_MAX_FRAME;
        
        err = BL_readMemory_command(addr + offset, compareBuffer + offset, rlen);
        if(err != STM32_ERR_OK)
        {
            return STM32_ERR_UNKNOWN;
        }
        
        offset += rlen;
    }
    
    /// compare data
    for(uint8_t i = 0; i < len - 2; i++)
    {
        if(dataVerify[i] != compareBuffer[i + 1])
        {
//            if(failed == retry)
//            {
                printf("Failed to verify at address 0x%08x, expected 0x%02x and found 0x%02x\n", (uint32_t)(addr + i), dataVerify[i], compareBuffer[i]);
                
                return STM32_ERR_UNKNOWN;
//            }
//            ++failed;
        }
    }
    printf("[BL_verifyMemory] Verify memory successfull\n");
//    failed = 0;
    
    return STM32_ERR_OK;
}

/** @brief  BL_verifyMemory
    @return stm32_err_t
*/
static stm32_err_t BL_go_command(uint8_t command, uint32_t address)
{
    stm32_err_t err = STM32_ERR_OK;
    uint8_t ackByte = 0;
    uint8_t buffer[2];
    buffer[0] = command;
    buffer[1] = 0xDE;
    
    printf("[BL_eraseMemory_command] send go command ....\n");
    
    err = BL_sendCommand(buffer, 2, &ackByte, 1, ackByte);
    if(err != STM32_ERR_OK)
    {
        return err;
    }
    
    printf("[BL_eraseMemory_command] send address and checksum ....\n");
    
    uint8_t goBuffer[5];
    goBuffer[0] = address >> 24;
    goBuffer[1] = (address >> 16) & 0xFF;
    goBuffer[2] = (address >> 8) & 0xFF;
    goBuffer[3] = address & 0xFF;
    goBuffer[4] = goBuffer[0] ^ goBuffer[1] ^ goBuffer[2] ^ goBuffer[3];
    
    err = BL_sendCommand(goBuffer, 5, &ackByte, 1, ackByte);
    if(err != STM32_ERR_OK)
    {
        printf("[BL_eraseMemory_command] send address and checksum fail ....\n");
        
        return err;
    }
    
    printf("[BL_eraseMemory_command] go address successfull ....\n");
    
    return STM32_ERR_OK;
}

#endif
/**
    @}
*/

/** @group __BL_COMMAND_INFOMATION
    @{
*/#ifndef __BL_COMMAND_INFOMATION
#define __BL_COMMAND_INFOMATION
/** @brief  BL_command_getInfomation
    @return bool
*/
bool BL_command_getInfomation(BL_command_t *BL)
{
    bool ret = false;
    stm32_err_t err = STM32_ERR_OK;
    uint8_t data[15];
    
    err = BL_get_command(BL, data);
    if( err != STM32_ERR_OK)
    {
        return false;
    }
    
//    len = data[1];
    BL->bl_version = data[2];
    BL->cmd.get = data[3];
    BL->cmd.gvr = data[4];
    BL->cmd.gid = data[5];
    BL->cmd.rm = data[6];
    BL->cmd.go = data[7];
    BL->cmd.wm = data[8];
    BL->cmd.er = data[9];
    BL->cmd.wp = data[10]; 
    BL->cmd.uw = data[11];
    BL->cmd.rp = data[12];
    BL->cmd.ur = data[13];
    
    if(data[14] == STM32_ACK)
    {
        printf("[BL_command_getInfomation]\nbl_version : 0x%x\nget : 0x%x\ngvr : 0x%x\ngid : 0x%x\nrm : 0x%x\ngo : 0x%x\nwm : 0x%x\ner : 0x%x\nwp : 0x%x\nuw : 0x%x\nrp : 0x%x\nur : 0x%x\n"
        , BL->bl_version, BL->cmd.get, BL->cmd.gvr, BL->cmd.gid, BL->cmd.rm, BL->cmd.go, BL->cmd.wm, BL->cmd.er, BL->cmd.wp, BL->cmd.uw, BL->cmd.rp, BL->cmd.ur);
        
        ret = true;
    }

    return ret;
}

/** @brief  BL_command_getVersion
    @return bool
*/
bool BL_command_getVersion(BL_command_t *BL)
{
    bool ret = false;
    stm32_err_t err = STM32_ERR_OK;
    uint8_t data[5];
    
    err = BL_getVersionAndRPStatus_command(BL, data);
    if( err != STM32_ERR_OK)
    {
        return false;
    }
    
    BL->version = data[1];
    BL->option1 = data[2];
    BL->option2 = data[3];
    
    if(data[4] == STM32_ACK)
    {
        printf("[BL_command_getVersion] version : 0x%x\n option1 : 0x%x\n option2 : 0x%x\n ", BL->version, BL->option1, BL->option2);
        
        ret = true;
    }
    
    return ret;
}

/** @brief  BL_command_getDeviceID
    @return bool
*/
bool BL_command_getDeviceID(BL_command_t *BL)
{
    bool ret = false;
    stm32_err_t err = STM32_ERR_OK;
    uint16_t len = 0;
    uint8_t data[5];
    
    err = BL_getDeviceID_command(BL, data);
    if( err != STM32_ERR_OK)
    {
        return false;
    }
    
    len = data[1] + 1;
    if(len != 2)
    {
        printf("More then two bytes sent in the PID, unknown/unsupported device\n");
    }
    
    BL->pid = (data[2] << 8 | data[3]);
    
    if(data[4] == STM32_ACK)
    {
        printf("[BL_command_getDeviceID] Product Id : 0x%x\n", BL->pid);
        
        ret = true;
    }
    
    return ret;
}

/** @brief  BL_command_readMemory
    @return bool
*/
void BL_command_readAllMemory(BL_command_t *BL)
{
    uint8_t buff[STM32_MAX_FRAME];
    uint32_t start = 0x08000000;
    uint32_t end = 0x08004C70;
    uint32_t left;
    uint16_t len = 0;
    uint32_t addr = start;
    float readPerCent = 0;
    while(addr < end)
    {
        left = end - addr;
        len = STM32_MAX_FRAME > left ? left : STM32_MAX_FRAME;
        BL_readMemory_command(addr, buff, len);
        
        addr += len;
        readPerCent = 100.0f / ((float)(end - start) * (float)(addr - start));
        printf("Read address 0x%08x (%.2f%%) \n", addr, readPerCent);
    }
}

/** @brief  
    @return 
*/

#endif
/**
    @}
*/

/** @group __BL_COMMAND_INIT_
    @{
*/#ifndef __BL_COMMAND_INIT_
#define __BL_COMMAND_INIT_
/** @brief  BL_command_init
    @return none
*/
void BL_command_init(BL_command_t *BL)
{

}

/** @brief  BL_command_process
    @return none
*/
void BL_command_process(BL_command_t *BL)
{
    stm32_err_t err ;
    uint32_t flashAddress = ADDR_FLASH_SECTOR_4;
    uint32_t maxDataLength = 0x17270;
    uint32_t start = 0x08000000;
    uint32_t end = start + maxDataLength;
    uint32_t left;
    uint16_t len = 0;
    uint32_t addr = start;
    uint32_t offset = 0;
    uint16_t size = end - start;
    
    flashState = 1;
    err = BL_eraseMemory_command(BL->cmd.er);
    
    flashState = 2;
    while(addr < end && offset < size)
    {
        BL_readDataFromFlash(BL, flashAddress);
        
        left = end - addr;
        len = STM32_MAX_FRAME > left ? left : STM32_MAX_FRAME;
        len = len > (size - offset) ? (size - offset) : len;
        
        err = BL_writeMemory_command(BL, addr, BL->writeData, STM32_MAX_FRAME);
        
        err = BL_verifyMemory(addr, BL->writeData, STM32_MAX_FRAME);
        
        addr += len;
        offset += len;
        flashAddress += len;
        printf("\r[BL_command_process] Wrote address 0x%08x (%.2f%%) ", addr, (100.0f / size) * offset);
    }
    
    /// release pin boot0
    RTS_LOW;
    
    /// realease pin reset
    DTR_LOW;
    HAL_Delay(1000);
    DTR_HIGH;
    
//    BL_go_command(0x21, 0x08000000);
    
    printf("[BL_command_process] Programming successfull .... reset and run\n");
    
    while(1)
    {
        HAL_GPIO_WritePin(blue_GPIO_Port, blue_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(green_GPIO_Port, green_Pin, GPIO_PIN_RESET);
    }
}

#endif
/**
    @}
*/

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/

