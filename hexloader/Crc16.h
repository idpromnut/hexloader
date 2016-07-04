#ifndef crc16_h
#define crc16_h

#include <stdint.h>

// -----------------------------------------------------------------------------
/**
    @brief Crc16Compute

    This function computes the CRC CCITT 16 bits of the given data

    @param  [in] crc CRC seed
    @param  [in] data Data

    @return The CRC CCITT value
*/    
uint16_t Crc16Compute( uint16_t crc, uint8_t data );

// -----------------------------------------------------------------------------
/**
    @brief Crc16ComputeBuffer

    This function computes the CRC CCITT 16 bits of a given buffer

    @param  [in] crc CRC seed
    @param  [in] buffer Pointer to buffer
    @param  [in] size Buffer size

    @return The CRC CCITT value
*/    
uint16_t Crc16ComputeBuffer( uint16_t crc, uint8_t *buffer, uint32_t size );

#endif /* crc16_h */
