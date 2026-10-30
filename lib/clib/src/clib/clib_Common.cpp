// The common helpers of ARMCC: small copies, clears and fills of a fixed size that the compiler calls instead of
// writing the stores inline (__ARM_common_<memcpy|memclr|memset><alignment>_<size>). Names of the ones without a
// reference symbol are ours, after the same pattern.

#include "decomp.h"
#include "types.h"

extern "C" {

// 0x00134D28 | fefates:callgraph [tier C]
void __ARM_common_memclr4_12(void* destination)
{
    u32* words = static_cast<u32*>(destination);
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
}

// 0x0013D834 | nintendogs:bytes [tier A]
void __ARM_common_memcpy4_5(void* destination, const void* source)
{
    *static_cast<u32*>(destination) = *static_cast<const u32*>(source);
    static_cast<u8*>(destination)[4] = static_cast<const u8*>(source)[4];
}

// 0x00307F20 | nintendogs:bytes [tier A]
void __ARM_common_memcpy4_8(void* destination, const void* source)
{
    const u32* from = static_cast<const u32*>(source);
    u32* to = static_cast<u32*>(destination);
    u32 a = from[0];
    u32 b = from[1];
    to[0] = a;
    to[1] = b;
}

// 0x0077A564 | nintendogs:bytes [tier A]
void __ARM_common_memclr4_10(void* destination)
{
    u32* words = static_cast<u32*>(destination);
    words[0] = 0;
    words[1] = 0;
    static_cast<u16*>(destination)[4] = 0;
}

// 0x0077A578 (name is ours)
void __ARM_common_memclr4_5(void* destination)
{
    *static_cast<u32*>(destination) = 0;
    static_cast<u8*>(destination)[4] = 0;
}

// 0x0077A588 | fefates:callgraph [tier C]
void __ARM_common_memclr4_6(void* destination)
{
    *static_cast<u32*>(destination) = 0;
    static_cast<u16*>(destination)[2] = 0;
}

// 0x0077A5B4 | fefates:bytes [tier B]
DECOMP_NOINLINE void __ARM_common_memclr4_7(void* destination)
{
    *static_cast<u32*>(destination) = 0;
    static_cast<u16*>(destination)[2] = 0;
    static_cast<u8*>(destination)[6] = 0;
}

// (three words, then falls into __ARM_common_memclr4_7)
// 0x0077A598 (name is ours)
void __ARM_common_memclr4_19(void* destination)
{
    u32* words = static_cast<u32*>(destination);
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
    __ARM_common_memclr4_7(words + 3);
}

// 0x0077A5C8 | fefates:callgraph [tier C]
void __ARM_common_memclr4_8(void* destination)
{
    u32* words = static_cast<u32*>(destination);
    words[0] = 0;
    words[1] = 0;
}

// 0x0077A5D8 (name is ours)
void __ARM_common_memclr4_9(void* destination)
{
    u32* words = static_cast<u32*>(destination);
    words[0] = 0;
    words[1] = 0;
    static_cast<u8*>(destination)[8] = 0;
}

// 0x0077A5EC | nintendogs:bytes [tier A]
void __ARM_common_memcpy4_10(void* destination, const void* source)
{
    const u32* from = static_cast<const u32*>(source);
    u32* to = static_cast<u32*>(destination);
    u32 a = from[0];
    u32 b = from[1];
    to[0] = a;
    to[1] = b;
    static_cast<u16*>(destination)[4] = static_cast<const u16*>(source)[4];
}

// 0x0077A608 | nintendogs:bytes [tier A]
void __ARM_common_memcpy4_6(void* destination, const void* source)
{
    *static_cast<u32*>(destination) = *static_cast<const u32*>(source);
    static_cast<u16*>(destination)[2] = static_cast<const u16*>(source)[2];
}

// the byte of value in all four bytes of a word
static inline u32 FillWord(int value)
{
    u32 byte = value & 0xFF;
    u32 half = byte | byte << 8;
    return half | half << 16;
}

// 0x0077A61C | fefates:bytes [tier B]
void __ARM_common_memset4_12(void* destination, int value)
{
    u32 fill = FillWord(value);
    u32* words = static_cast<u32*>(destination);
    words[0] = fill;
    words[1] = fill;
    words[2] = fill;
}

// 0x0077A638 (name is ours)
void __ARM_common_memset2_3(void* destination, int value)
{
    u32 byte = value & 0xFF;
    u32 half = byte | byte << 8;
    *static_cast<u16*>(destination) = half;
    static_cast<u8*>(destination)[2] = half;
}

// 0x0077A654 (name is ours)
DECOMP_NOINLINE void __ARM_common_memset4_4(void* destination, int value)
{
    *static_cast<u32*>(destination) = FillWord(value);
}

// (falls into __ARM_common_memset4_4)
// 0x0077A64C (name is ours)
void __ARM_common_memset4_4_ff(void* destination)
{
    __ARM_common_memset4_4(destination, -1);
}

// 0x0077A668 | mk7dlp:bytes [tier B]
void __ARM_common_memset4_5(void* destination, int value)
{
    u32 fill = FillWord(value);
    *static_cast<u32*>(destination) = fill;
    static_cast<u8*>(destination)[4] = fill;
}

// 0x0077A6BC (name is ours)
void __ARM_common_memset4_8(void* destination, int value)
{
    u32 fill = FillWord(value);
    u32* words = static_cast<u32*>(destination);
    words[0] = fill;
    words[1] = fill;
}

} // extern "C"
