//
//  serial.h
//
//  Created by Philippe Lavoie on 11-10-10.
//

#ifndef Blutooth_IMU_serial_h
#define Blutooth_IMU_serial_h

int openPort(char *portNAme);
short dataToShort(char *byte);
int dataToInt(char *byte);
float dataToFloat(char *byte);
unsigned int dataToUInt(char *byte);
void configPort(int fd, int baudrate, int vmin);


#endif
