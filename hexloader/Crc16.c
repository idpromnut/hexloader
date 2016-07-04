/**
    @file crc16.c
    @brief CRC CCITT functions
    
    This file contains the functions used to calculate CRC
*/

#include "Crc16.h"

// ----------------------------------------------------------------------------
uint16_t Crc16Compute( uint16_t crc, uint8_t data )
{
    uint16_t     lcData;

    lcData = (data ^ (crc & 0x00FF));
    lcData = (lcData ^ (((lcData) & 0x000F) << 4));
    lcData = (((((lcData << 8) & 0xFFFF) | ((crc >> 8) & 0x00FF))) ^ (lcData >> 4) ^ ((lcData << 3) & 0xFFFF));

    return( lcData );
}

// ----------------------------------------------------------------------------
uint16_t Crc16ComputeBuffer( uint16_t crc, uint8_t *buffer, uint32_t size )
{
	uint16_t     lcCRC = crc;
    uint8_t     *lcPBuffer = buffer;

	while( size-- > 0 )
    {
		lcCRC = Crc16Compute( lcCRC, *lcPBuffer++ );
    }

    return( lcCRC );
}
