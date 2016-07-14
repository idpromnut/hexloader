//
//  flashLoader.c
//  hexloader
//
//  Created by Chris Matthews on 2016-07-04.
//  Copyright © 2016 Unrecoverable.org. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "serial.h"
#include "flashLoader.h"


int flashLoaderSendCommand(int fd, uint8_t command, uint8_t response);

int16_t readCharSerial(int fd, uint32_t timeout);
void writeCharSerial(int fd, uint8_t byteToWrite, uint16_t timeout);
ssize_t flashLoaderReadBytes(int fd, uint8_t* buffer, uint16_t size, uint32_t timeoutMillis);
ssize_t flashLoaderWriteBytes(int fd, uint8_t* buffer, uint16_t size, uint32_t timeoutMillis);
void printBytes(uint8_t* buffer, uint8_t length);

uint8_t txBuffer[TX_BUFFER_SIZE];
uint8_t rxBuffer[RX_BUFFER_SIZE];

void fetchAndPrintStatus(int fd)
{
    STM32Features features;
    if (flashLoaderGet(fd, &features))
    {
        printf("STM32 Flash Bootloader version:    0x%02X\n", features.bootLoaderVersion);
    }
    
    uint16_t id;
    if (flashLoaderGetId(fd, &id))
    {
        printf("CPU ID: 0x%04X\n", id);
    }
    
    if (flashLoaderGo(fd, 0x00800000))
    {
        printf("GO: Success\n");
    }
    
}


int flashLoaderConnect(int fd)
{
    int16_t readByte;
    usleep(100);
    writeCharSerial(fd, 0x7F, 0);
    readByte = readCharSerial(fd, 1000);
    if (readByte != ERROR_READ_TIMEOUT) {
        if ((uint8_t)readByte == FLASH_LOADER_ACK) {
            return 1;
        }
        else {
#if DEBUG
            printf("UNEXPECTED BYTE (%X)\n", (uint8_t)readByte);
#endif
        }
    }
    
    return 0;
}


int flashLoaderGet(int fd, STM32Features* features)
{
    uint8_t readBuffer[128];
    ssize_t bytesRead;
    
    if (flashLoaderSendCommand(fd, FLASH_LOADER_COMMAND_GET, FLASH_LOADER_ACK)) {
        bytesRead = flashLoaderReadBytes(fd, readBuffer, 128, 1000);
        if (bytesRead > 0)
        {
#if DEBUG
            printf("GET(): read %li bytes: ", bytesRead);
            printBytes(readBuffer, bytesRead);
            printf("\n");
#endif
            // decode features results
            uint8_t index = 0;
            features->bootLoaderVersion = readBuffer[index++];
            features->getCommand = readBuffer[index++] == FLASH_LOADER_COMMAND_GET;
            features->versionCommand = readBuffer[index++] == FLASH_LOADER_COMMAND_VERSION;
            features->idCommand = readBuffer[index++] == FLASH_LOADER_COMMAND_GET_ID;
            features->readCommand = readBuffer[index++] == FLASH_LOADER_COMMAND_READ_MEM;
            features->goCommand = readBuffer[index++] == FLASH_LOADER_COMMAND_GO;
            features->writeCommand = readBuffer[index++] == FLASH_LOADER_COMMAND_WRITE_MEM;
            features->eraseCommand = readBuffer[index++] == FLASH_LOADER_COMMAND_ERASE;
            features->writeProtCommand = readBuffer[index++] == FLASH_LOADER_COMMAND_WPROT;
            features->writeUnProtCommand = readBuffer[index++] == FLASH_LOADER_COMMAND_WUNPROT;
            features->readProtCommand = readBuffer[index++] == FLASH_LOADER_COMMAND_RPROT;
            features->readUnProtCommand = readBuffer[index++] == FLASH_LOADER_COMMAND_RUNPROT;
            return SUCCESS;
        }
        else
        {
            return ERROR_COMMAND_FAILED;
        }
    }
    
    return ERROR_COMMAND_FAILED;
}


int flashLoaderGetId(int fd, uint16_t* id)
{
    uint8_t readBuffer[16];
    ssize_t bytesRead;
    
    if (flashLoaderSendCommand(fd, FLASH_LOADER_COMMAND_GET_ID, FLASH_LOADER_ACK)) {
        bytesRead = flashLoaderReadBytes(fd, readBuffer, 16, 1000);
        if (bytesRead == 2)
        {
#if DEBUG
            printf("GET_ID(): read %li bytes: ", bytesRead);
            printBytes(readBuffer, bytesRead);
            printf("\n");
#endif
            *id = (readBuffer[0] << 8) | (readBuffer[1]);
            return SUCCESS;
        }
        else
        {
            return ERROR_COMMAND_FAILED;
        }
    }
    
    return ERROR_COMMAND_FAILED;
}


int flashLoaderGo(int fd, uint32_t address)
{
    uint8_t jumpBuffer[16];
    uint16_t len = 0;
    uint8_t checksum = 0;
    
    if (flashLoaderSendCommand(fd, FLASH_LOADER_COMMAND_GO, FLASH_LOADER_ACK)) {
        
        // copy and calculate checksum
        for(; len < 4; ++len)
        {
            jumpBuffer[len] = (uint8_t)( address >> ((3 - len) * 8) );
            checksum ^= jumpBuffer[len];
        }
        jumpBuffer[++len] = checksum;
        
#if DEBUG
        printf("GO(): writing address: ");
        printBytes(jumpBuffer, len);
        printf("\n");
#endif
        
//        bytesWritten = flashLoaderWriteBytes(fd, jumpBuffer, len, 1000);
        return SUCCESS;
    }
    
    return ERROR_COMMAND_FAILED;
}


int flashLoaderSendCommand(int fd, uint8_t command, uint8_t response)
{
    int16_t readByte;
    usleep(100);
    writeCharSerial(fd, command, 0);
    writeCharSerial(fd, ~command, 0);
    readByte = readCharSerial(fd, 1000);
    if (readByte != ERROR_READ_TIMEOUT) {
        if ((uint8_t)readByte == response) {
            return 1;
        }
        else {
#if DEBUG
            printf("UNEXPECTED BYTE (%X)\n", (uint8_t)readByte);
#endif
        }
    }
    
    return 0;
}


ssize_t flashLoaderReadBytes(int fd, uint8_t* buffer, uint16_t size, uint32_t timeoutMillis)
{
    const uint32_t loopDelay = 500;
    ssize_t bytesRead = 0;
    ssize_t bytesExpected = 0;
    ssize_t dataBytesRead = 0;
    uint32_t waitedTime = 0;

    // read the "length" byte first:  N = number of bytes - 1
    while(bytesRead < 1 && waitedTime < (timeoutMillis*1000))
    {
        bytesRead = read(fd,&rxBuffer, 1);
        usleep(loopDelay);
        waitedTime += loopDelay;
    }
    
    // if no length was read, return with an error
    if (bytesRead < 1)
    {
        return ERROR_READ_TIMEOUT;
    }
    
    if (bytesExpected > size)
    {
        return ERROR_BAD_LENGTH;
    }
    
    // read the number of expected bytes
    bytesRead = 0;
    bytesExpected = (ssize_t)(rxBuffer[0] + 1);
    dataBytesRead = 0;
    while(dataBytesRead < bytesExpected && waitedTime < (timeoutMillis*1000))
    {
        bytesRead = read(fd, &rxBuffer, bytesExpected - dataBytesRead);
        usleep(loopDelay);
        waitedTime += loopDelay;
        if (bytesRead > 0)
        {
            if (bytesRead == bytesExpected)
            {
                memcpy((buffer + dataBytesRead), &rxBuffer, bytesExpected);
                dataBytesRead += bytesExpected;
            }
            else
            {
                memcpy((buffer + dataBytesRead), &rxBuffer, bytesRead);
                dataBytesRead += bytesRead;

            }
        }
    }
    
    if (dataBytesRead == 0)
    {
        return ERROR_READ_TIMEOUT;
    }
    else
    {
        // read ack that is expected
        if((uint8_t)readCharSerial(fd, 100) == FLASH_LOADER_ACK)
        {
            return dataBytesRead;
        }
        else
        {
            return ERROR_BAD_READ;
        }
    }
}


ssize_t flashLoaderWriteBytes(int fd, uint8_t* buffer, uint16_t size, uint32_t timeoutMillis)
{
    const uint32_t loopDelay = 500;
    ssize_t bytesWritten = 0;
    ssize_t totalBytesWritten = 0;
    uint32_t waitedTime = 0;
    
    while(totalBytesWritten < size && waitedTime < (timeoutMillis*1000))
    {
        bytesWritten = write(fd, (buffer + totalBytesWritten), (size - totalBytesWritten));
        totalBytesWritten += bytesWritten;
        if (totalBytesWritten > 0)
        {
            if (totalBytesWritten == size)
            {
                // we're done, return
            }
            else
            {
                usleep(loopDelay);
                waitedTime += loopDelay;
            }
        }
    }

    return totalBytesWritten;
}


int16_t readCharSerial(int fd, uint32_t timeoutMillis)
{
    const uint32_t loopDelay = 500;
    ssize_t bytesRead = 0;
    int16_t input = ERROR_READ_TIMEOUT;
    uint32_t waitedTime = 0;
    while(bytesRead < 1 && waitedTime < (timeoutMillis*1000))
    {
        bytesRead = read(fd,&input, 1);
        usleep(loopDelay);
        waitedTime += loopDelay;
    }
    return input;
}


void writeCharSerial(int fd, uint8_t byteToWrite, uint16_t timeout)
{
    write(fd, &byteToWrite, 1);
}


void printBytes(uint8_t* buffer, uint8_t length)
{
    for(uint8_t i = 0; i < length; ++i)
    {
        printf("%02X ", buffer[i]);
    }
}
