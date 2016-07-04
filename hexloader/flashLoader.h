//
//  flashLoaderCommands.h
//  hexloader
//
//  Created by Chris Matthews on 2016-07-03.
//  Copyright © 2016 Unrecoverable.org. All rights reserved.
//

#ifndef flashLoaderCommands_h
#define flashLoaderCommands_h

#define TX_BUFFER_SIZE 4096
#define RX_BUFFER_SIZE 4096

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


void flashLoaderStateMachine(int fd);
uint8_t flashLoaderConnect(int fd);
uint8_t flashLoaderSendCommand(int fd, uint8_t command, uint8_t response);

#endif /* flashLoaderCommands_h */
