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

#if OPENTRACKER_HW_REV >= 0x0200 && OPENTRACKER_HW_REV <= 0x02FF
// OpenTracker v2
#define FLASH_STORAGE_START (IFLASH1_ADDR)
#define FLASH_STORAGE_SIZE  131072u
#elif OPENTRACKER_HW_REV >= 0x0300 && OPENTRACKER_HW_REV <= 0x03FF
// OpenTracker v3
#define FLASH_STORAGE_START (FLASH_BASE + 131072u)
#define FLASH_STORAGE_SIZE  131072u
#else
#error "DueFlashStorage library not supported on this platform!"
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
