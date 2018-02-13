/* 
DueFlashStorage saves non-volatile data for Arduino Due.
The library is made to be similar to EEPROM library
Uses flash block 1 per default.

Note: uploading new software will erase all flash so data written to flash
using this library will not survive a new software upload. 

Inspiration from Pansenti at https://github.com/Pansenti/DueFlash
Rewritten and modified by Sebastian Nilsson
*/


#ifndef DUEFLASHSTORAGE_H
#define DUEFLASHSTORAGE_H

#include <Arduino.h>

#if defined(_SAM3XA_)
// On SAM3X/SAM3A default to using the second flash bank

#ifndef FLASH_STORAGE_START
#define FLASH_STORAGE_START IFLASH1_ADDR
#endif

#ifndef FLASH_STORAGE_SIZE
#define FLASH_STORAGE_SIZE  IFLASH1_SIZE
#endif

#elif defined(_STM32_DEF_)
// On STM32 default to using second flash bank or half the available flash

#ifndef FLASH_STORAGE_START
#ifndef FLASH_BANK_2
#define FLASH_STORAGE_START (FLASH_BASE + (FLASH_SIZE/2))
#else
#define FLASH_STORAGE_START (FLASH_BASE + FLASH_BANK_SIZE)
#endif
#endif

#ifndef FLASH_STORAGE_SIZE
#ifndef FLASH_BANK_2
#define FLASH_STORAGE_SIZE  (FLASH_SIZE/2)
#else
#define FLASH_STORAGE_SIZE  FLASH_BANK_SIZE
#endif
#endif

#else
#error "DueFlashStorage library not supported on this hardware platform!"
#endif

//  DueFlash is the main class for flash functions
class DueFlashStorage
{
public:
  DueFlashStorage();

  // write() writes the specified amount of data into flash.
  // flashStart is the address in memory where the write should start
  // data is a pointer to the data to be written
  // dataLength is length of data in bytes
  
  byte read(uint32_t address) { return *(byte*)(FLASH_STORAGE_START + address); }
  byte* readAddress(uint32_t address) { return (byte*)(FLASH_STORAGE_START + address); }
  bool write(uint32_t address, byte *data, uint32_t dataLength);
  bool write(uint32_t address, byte value, uint32_t dataLength = 1);
};

#endif
