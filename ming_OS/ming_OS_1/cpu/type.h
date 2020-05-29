#ifndef TYPE_H
#define TYPE_H

#include <stdint.h>

// typedef unsigned short	uint16_t;
// typedef short		int16_t;
//
// typedef unsigned char	uint8_t;
// typedef char		int8_t;
//
// typedef unsigned int	uint32_t;
// typedef int		int32_t;
//
typedef unsigned long	size_t;

// typedef unsigned int   u32;
// typedef          int   s32;
// typedef unsigned short u16;
// typedef          short s16;
// typedef unsigned char  u8;
// typedef          char  s8;

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)


#endif
