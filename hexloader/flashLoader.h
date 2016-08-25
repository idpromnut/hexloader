//
//  flashLoaderCommands.h
//  hexloader
//
//  Created by Chris Matthews on 2016-07-03.
//  Copyright © 2016 Unrecoverable.org. All rights reserved.
//

#ifndef flashLoaderCommands_h
#define flashLoaderCommands_h

#define TX_BUFFER_SIZE 1024
#define RX_BUFFER_SIZE 1024

#define FLASH_LOADER_START              0x7F
#define FLASH_LOADER_ACK                0x79
#define FLASH_LOADER_NACK               0x1F
#define FLASH_LOADER_COMMAND_GET        0x00
#define FLASH_LOADER_COMMAND_VERSION    0x01
#define FLASH_LOADER_COMMAND_GET_ID     0x02
#define FLASH_LOADER_COMMAND_READ_MEM   0x11
#define FLASH_LOADER_COMMAND_GO         0x21
#define FLASH_LOADER_COMMAND_WRITE_MEM  0x31
#define FLASH_LOADER_COMMAND_ERASE      0x43
#define FLASH_LOADER_COMMAND_EXT_ERASE  0x44
#define FLASH_LOADER_COMMAND_WPROT      0x63
#define FLASH_LOADER_COMMAND_WUNPROT    0x73
#define FLASH_LOADER_COMMAND_RPROT      0x82
#define FLASH_LOADER_COMMAND_RUNPROT    0x92

#define SUCCESS                         1
#define ERROR                           0
#define ERROR_READ_TIMEOUT              -2
#define ERROR_BAD_READ                  -3
#define ERROR_BAD_LENGTH                -4
#define ERROR_COMMAND_FAILED            -5


struct STM32Features_struct
{
    uint8_t bootLoaderVersion;
    uint8_t getCommand:1;
    uint8_t versionCommand:1;
    uint8_t idCommand:1;
    uint8_t readCommand:1;
    uint8_t goCommand:1;
    uint8_t writeCommand:1;
    uint8_t eraseCommand:1;
    uint8_t writeProtCommand:1;
    uint8_t writeUnProtCommand:1;
    uint8_t readProtCommand:1;
    uint8_t readUnProtCommand:1;
};

typedef struct STM32Features_struct STM32Features;

void fetchAndPrintStatus(int fd);
int flashLoaderConnect(int fd);
int flashLoaderGet(int fd, STM32Features* features);
int flashLoaderGetId(int fd, uint16_t* id);
int flashLoaderGo(int fd, uint32_t address);
int flashLoaderWriteProt(int fd);
int flashLoaderWriteUnProt(int fd);
int flashLoaderReadoutProt(int fd);
int flashLoaderReadoutUnProt(int fd);
int flashLoaderRead(int fd);

#endif /* flashLoaderCommands_h */
