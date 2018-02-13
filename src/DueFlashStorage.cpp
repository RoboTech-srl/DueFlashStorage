#include "DueFlashStorage.h"

#if defined(_SAM3XA_)

#include "flash_efc.h"

DueFlashStorage::DueFlashStorage()
{
  uint32_t retCode;

  /* Initialize flash: 6 wait states for flash writing. */
  retCode = flash_init(FLASH_ACCESS_MODE_128, 6);
}

boolean DueFlashStorage::write(uint32_t address, byte *data, uint32_t dataLength)
{
  uint32_t retCode;

  // write data
  retCode = flash_write((uint32_t)FLASH_STORAGE_START+address, data, dataLength, 1);

  if (retCode != FLASH_RC_OK) {
    return false;
  }

  return true;
}

bool DueFlashStorage::write(uint32_t address, byte value, uint32_t dataLength)
{
  uint32_t retCode;

  // write data
  retCode = flash_fill((uint32_t)FLASH_STORAGE_START+address, value, dataLength, 1);

  if (retCode != FLASH_RC_OK) {
    return false;
  }

  return true;
}

#elif defined(_STM32_DEF_)

#include <strings.h>

DueFlashStorage::DueFlashStorage()
{
}

// Flash page buffer (aligned to Flash program size)
static uint64_t page_buffer[FLASH_PAGE_SIZE / sizeof(uint64_t)];

bool DueFlashStorage::write(uint32_t address, byte *data, uint32_t dataLength)
{
  FLASH_EraseInitTypeDef EraseInitStruct;
  
  if (dataLength == 0)
    return true;
  
  HAL_FLASH_Unlock();

  uint32_t page = (FLASH_STORAGE_START + address - FLASH_BASE) / FLASH_PAGE_SIZE;
  uint32_t offs = (FLASH_STORAGE_START + address - FLASH_BASE) % FLASH_PAGE_SIZE;
  uint8_t* padr = (uint8_t*)(FLASH_BASE + page * FLASH_PAGE_SIZE);

  // before the loop, it can happen only the first time
  if (offs > 0)
    memcpy(page_buffer, padr, offs);
  while (dataLength > 0)
  {
    // copy a portion of the source buffer that fits in the current page
    uint32_t size = min(FLASH_PAGE_SIZE - offs,	dataLength);
    memcpy((uint8_t*)page_buffer + offs, data, size);
    // any padding at the end?
    if (offs + size < FLASH_PAGE_SIZE)
      memcpy((uint8_t*)page_buffer + offs + size, padr + offs + size, FLASH_PAGE_SIZE - (offs + size));

    // erase current page
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Banks       = FLASH_BANK_1;
    EraseInitStruct.Page        = page;
    EraseInitStruct.NbPages     = 1;

    uint32_t PAGEError = 0;
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
    {
      HAL_FLASH_Lock();
      return false;
    }
    
    // write back orginal data, overwritten by source buffer
    for (unsigned int i = 0; i < FLASH_PAGE_SIZE / sizeof(uint64_t); ++i)
    {
      if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t)padr, page_buffer[i]) != HAL_OK)
      {
        HAL_FLASH_Lock();
        return false;
      }
      padr += sizeof(uint64_t);
    }

    // advance buffer
    dataLength -= size;
    if (dataLength == 0)
      break;
    data += size;
    page++;
    offs = 0;
  }

  HAL_FLASH_Lock();
  return true;
}

bool DueFlashStorage::write(uint32_t address, byte value, uint32_t dataLength)
{
  FLASH_EraseInitTypeDef EraseInitStruct;
  
  if (dataLength == 0)
    return true;
  
  HAL_FLASH_Unlock();

  uint32_t page = (FLASH_STORAGE_START + address - FLASH_BASE) / FLASH_PAGE_SIZE;
  uint32_t offs = (FLASH_STORAGE_START + address - FLASH_BASE) % FLASH_PAGE_SIZE;
  uint8_t* padr = (uint8_t*)(FLASH_BASE + page * FLASH_PAGE_SIZE);

  // before the loop, it can happen only the first time
  if (offs > 0)
    memcpy(page_buffer, padr, offs);
  while (dataLength > 0)
  {
    // fill a portion of the destination that fits in the current page
    uint32_t size = min(FLASH_PAGE_SIZE - offs,	dataLength);
    memset((uint8_t*)page_buffer + offs, value, size);
    // any padding at the end?
    if (offs + size < FLASH_PAGE_SIZE)
      memcpy((uint8_t*)page_buffer + offs + size, padr + offs + size, FLASH_PAGE_SIZE - (offs + size));

    // erase current page
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Banks       = FLASH_BANK_1;
    EraseInitStruct.Page        = page;
    EraseInitStruct.NbPages     = 1;

    uint32_t PAGEError = 0;
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
    {
      HAL_FLASH_Lock();
      return false;
    }
    
    // write back orginal data, overwritten by source buffer
    for (unsigned int i = 0; i < FLASH_PAGE_SIZE / sizeof(uint64_t); ++i)
    {
      if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t)padr, page_buffer[i]) != HAL_OK)
      {
        HAL_FLASH_Lock();
        return false;
      }
      padr += sizeof(uint64_t);
    }

    // advance buffer
    dataLength -= size;
    if (dataLength == 0)
      break;
    page++;
    offs = 0;
  }

  HAL_FLASH_Lock();
  return true;
}

#endif
