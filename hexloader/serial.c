//
//  serial.c
//
//  Created by Philippe Lavoie on 11-10-10.
//

#include <stdio.h>   // Standard input/output definitions 
#include <string.h>  // String function definitions 
#include <unistd.h>  // UNIX standard function definitions 
#include <fcntl.h>   // File control definitions 
#include <errno.h>   // Error number definitions 
#include <termios.h> // POSIX terminal control definitions 
#include <sys/ioctl.h>

#include "serial.h"

// 'openPort()' - Open serial port for BluetoothIMU1126 (/dev/cu.BluetoothIMU1126-Bluet.
// Returns the file descriptor on success or -1 on error.

int openPort(char *portNAme)
{
    int fd; /* File descriptor for the port */

    fd = open(portNAme, O_RDWR | O_NOCTTY | O_NONBLOCK);
    
    if (fd == -1)
    {
        /*
         * Could not open the port.
         */
        perror(portNAme);
    }
    
    return (fd);
}

short dataToShort(char *byte)
{
    short * p_short;
    char data[2];
    
    data[0] = byte[0];
    data[1] = byte[1];
    
    p_short = (short *)&data[0];
    
    return (*p_short);
}

int dataToInt(char *byte)
{
    int * p_int;
    char data[4];
    
    data[0] = byte[0];
    data[1] = byte[1];
    data[2] = byte[2];
    data[3] = byte[3];
    
    p_int = (int *)&data[0];
    return (*p_int);
}

float dataToFloat(char *byte)
{
    float * p_float;
    char data[4];
    
    data[0] = byte[0];
    data[1] = byte[1];
    data[2] = byte[2];
    data[3] = byte[3];
    
    p_float = (float *)&data[0];
    return (*p_float);
}

void configPort(int fd, int baudrate, int vmin)
{
    struct termios options;
    
//    fcntl(fd, F_SETFL, 0);
    
    // Get the current options for the port...
    tcgetattr(fd, &options);
    
    // Set the baud rates to baudrate...
    cfsetispeed(&options, baudrate);
    cfsetospeed(&options, baudrate);
    
    // Enable the receiver and set local mode...
    options.c_cflag |= (CLOCAL | CREAD);
    
    // 8 bits No Parity
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    
    // Disable hardware flow control
    options.c_cflag &= ~CRTSCTS;
    
    // Raw input
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    
    // One input byte is enough to return from read()
    // Inter-character timer off
    options.c_cc[VMIN]  = vmin;
    options.c_cc[VTIME] = 0;
    
    // Set the new options for the port...
    tcsetattr(fd, TCSANOW, &options);

    fcntl(fd, F_SETFL, O_ASYNC|O_NONBLOCK|fcntl(fd, F_GETFL));
    
    // Diacards unread data
    tcflush(fd, TCIFLUSH);
}


