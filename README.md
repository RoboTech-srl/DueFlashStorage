# DueFlashStorage
DueFlashStorage saves non-volatile data for Arduino Due. The library is made to be similar to the EEPROM library. Uses flash block 1 per default.

### Features
- Non-volatile data storage. Resetting or loss of power to the Arduino will not affect the data.
- Similar to the standard EEPROM library
- Write and read byte by byte
- Write and read byte arrays to store arbitrary variable types (strings, structs, integers, floats)

Note: The flash storage is reset every time you upload a new sketch to your Arduino.

Inspiration and some code from Pansenti at https://github.com/Pansenti/DueFlash

## Install
Create a new folder in your Arduino sketch folder named DueFlashStorage. 
Download and put all files from this repository into the folder. 

## Use
### Basic use
```cpp
// write the value 123 to address 0
dueFlashStorage.write(0,123);

// read byte at address 0
byte b = dueFlashStorage.read(0);
```

### Advanced use to store configuration parameters
```cpp
// say you want to store a struct with parameters:
struct Configuration {
  uint8_t a;
  uint8_t b;
  int32_t bigInteger;
  char* message;
  char c;
};
Configuration configuration;

// then write it to flash like this:
dueFlashStorage.write(4, (byte*)&configuration, sizeof(Configuration)); // write to flash at address 4

// and read from flash like this:
memcpy(&configuration, dueFlashStorage.readAddress(4), sizeof(Configuration)); // read from flash at address 4

/* see example code for a working example */
```

## Examples
### DueFlashStorageExample.cpp
This example will write 3 bytes to 3 different addresses and print them to the serial monitor.
Try resetting the Arduino Due or unplug the power to it. The values will stay stored.
   
### DueFlashStorageStructExample.cpp
This example will write a struct to memory which is a very convinient way of storing configuration parameters.
Try resetting the Arduino Due or unplug the power to it. The values will stay stored.
