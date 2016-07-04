//
//  flashLoader.c
//  hexloader
//
//  Created by Chris Matthews on 2016-07-04.
//  Copyright © 2016 Unrecoverable.org. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include "serial.h"
#include "flashLoader.h"

int16_t readCharSerial(int fd, uint32_t timeout);
void writeCharSerial(int fd, uint8_t byteToWrite, uint16_t timeout);

uint8_t txBuffer[TX_BUFFER_SIZE];
uint8_t rxBuffer[RX_BUFFER_SIZE];

void flashLoaderStateMachine(int fd)
{
    //    int16_t readByte;
    
    // send start to device and wait for ACK
    while(1)
    {
    }
}


uint8_t flashLoaderGet(int fd)
{
//    flashLoaderSendCommand(fd, ,);
    return 0;
}


uint8_t flashLoaderConnect(int fd)
{
    int16_t readByte;
    usleep(100);
    writeCharSerial(fd, 0x7F, 0);
    readByte = readCharSerial(fd, 1000);
    if (readByte != -1) {
        if ((uint8_t)readByte == 0x79) {
            return 1;
        }
        else {
            printf("UNEXPECTED BYTE (%X)\n", (uint8_t)readByte);
        }
    }
    
    return 0;
}


uint8_t flashLoaderSendCommand(int fd, uint8_t command, uint8_t response)
{
    int16_t readByte;
    usleep(100);
    writeCharSerial(fd, command, 0);
    readByte = readCharSerial(fd, 1000);
    if (readByte != -1) {
        if ((uint8_t)readByte == response) {
            return 1;
        }
        else {
            printf("UNEXPECTED BYTE (%X)\n", (uint8_t)readByte);
        }
    }
    
    return 0;
}


int16_t readCharSerial(int fd, uint32_t timeoutMillis)
{
    const uint32_t loopDelay = 500;
    ssize_t bytesRead = 0;
    uint8_t input = -1;
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
    write(fd,&byteToWrite,1);
}
