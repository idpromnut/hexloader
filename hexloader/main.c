//
//  main.c
//  hexloader
//
//  Created by Chris Matthews on 2016-06-30.
//  Copyright © 2016 Unrecoverable.org. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <termios.h> // POSIX terminal control definitions
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include "serial.h"
#include "flashLoader.h"

#define DEBUG 1

int main(int argc, const char * argv[]) {
    
    int fd = -1; // File descriptor for the port
    char portName[512] = {};
    
//    strcpy(portName, "/dev/tty.usbserial-A403IF57");
    strcpy(portName, "/dev/tty.usbserial");

    printf("Opening serial port %s...", portName);
    fd = openPort(portName);
    configPort(fd,57600,10);
    
    if(fd!=-1)
    {
        printf("OK\n");
        printf("Connecting to STM32...");
        if (flashLoaderConnect(fd))
        {
            printf("OK\n");
            fetchAndPrintStatus(fd);
        }
        else
        {
            printf("FAIL\n");
        }
        printf("Closing serial port...");
        close(fd);
        printf("OK\n");
    }
    else {
        printf("FAILED\n");
    }
    
    return 0;
}


