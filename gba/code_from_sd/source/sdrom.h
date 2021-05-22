#ifndef __GBA_SDROM_H
#define __GBA_SDROM_H

#if __ARM_EABI__
#include <gba_types.h>
#else

#include <stdint.h>
#include <stdbool.h>

typedef	uint8_t		u8;	/**< Unsigned 8 bit value	*/
typedef	uint16_t	u16;	/**< Unsigned 16 bit value	*/
typedef	uint32_t	u32;	/**< Unsigned 32 bit value	*/
typedef	int8_t		s8;	/**< Signed 8 bit value	*/
typedef	int16_t		s16;	/**< Signed 16 bit value	*/
typedef	int32_t		s32;	/**< Signed 32 bit value	*/
typedef	volatile u8	vu8;	/**< volatile Unsigned 8 bit value	*/
typedef	volatile u16	vu16;	/**< volatile Unigned 16 bit value	*/
typedef	volatile u32	vu32;	/**< volatile Unsigned 32 bit value	*/
typedef	volatile s8	vs8;	/**< volatile Signed 8 bit value	*/
typedef	volatile s16	vs16;	/**< volatile Signed 8 bit value	*/
typedef	volatile s32	vs32;	/**< volatile Signed 8 bit value	*/

#endif

#define SDROM_MAGIC    0xCAFEBABE
#define SDROM_VER_01   0x00000001

#define SDROM_TYPE_GBA 0x00000001

typedef struct {
    u32 magic;
    u32 version;
    
    u8 reserved[3];
    u8 type;

    u8 description[32];

    u32 reserved2;
    u32 reserved3[16];

    u32 start_block;
    u32 num_blocks;
    u32 entry;
} SDROM_hdr;

#endif
