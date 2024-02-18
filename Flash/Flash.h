//
// Created by 小新 on 2024/2/3.
//

#ifndef STM32_DEMO_FLASH_H
#define STM32_DEMO_FLASH_H

#include "stdio.h"
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_flash_ex.h>
#include <memory.h>

#define ADDR_FLASH_SECTOR_0 ((uint32_t)0x08000000)  /* Base address of Sector 0, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_1 ((uint32_t)0x08004000)  /* Base address of Sector 1, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_2 ((uint32_t)0x08008000)  /* Base address of Sector 2, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_3 ((uint32_t)0x0800C000)  /* Base address of Sector 3, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_4 ((uint32_t)0x08010000)  /* Base address of Sector 4, 64 Kbytes   */
#define ADDR_FLASH_SECTOR_5 ((uint32_t)0x08020000)  /* Base address of Sector 5, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_6 ((uint32_t)0x08040000)  /* Base address of Sector 6, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_7 ((uint32_t)0x08060000)  /* Base address of Sector 7, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_8 ((uint32_t)0x08080000)  /* Base address of Sector 8, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_9 ((uint32_t)0x080A0000)  /* Base address of Sector 9, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_10 ((uint32_t)0x080C0000) /* Base address of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11 ((uint32_t)0x080E0000) /* Base address of Sector 11, 128 Kbytes */
#define ADDR_FLASH_SECTOR_12 ((uint32_t)0x08100000) /* Base address of Sector 12, 16 Kbytes  */
#define FLASH_END_ADDR ((uint32_t)0x08100000)       /* Base address of Sector 23, 128 Kbytes */


uint32_t get_sector(uint32_t address);               // 获取对应Flash扇区
uint32_t get_next_flash_address(uint32_t address);   // 获取下一个Flash扇区地址
int8_t Flash_Erase(uint32_t address, uint16_t len);    // 擦除Flash
int8_t Flash_Write(uint32_t address,uint32_t *data,uint32_t data_len); // 写入Flash数据
void Flash_Read(uint32_t address, uint32_t *buf, uint32_t len);

#endif //STM32_DEMO_FLASH_H
