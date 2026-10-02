#pragma once

// Basic integer types, named like the game itself (u8, s32, f32, bit32, ...).
//
// They are spelled out instead of using <stdint.h>: devkitARM's uint32_t is
// "unsigned long", ARMCC's is "unsigned int". The game's mangled names use
// "unsigned int" (j) / "int" (i), so these typedefs keep the symbols identical
// with both compilers.

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;

typedef float f32;
typedef double f64;

// "bit" types are raw bit patterns (flags, handles, ...)
typedef u8 bit8;
typedef u16 bit16;
typedef u32 bit32;
typedef u64 bit64;

typedef u32 uptr;
typedef s32 sptr;

#ifdef __cplusplus
typedef char16_t char16;
#else
typedef u16 char16;
#endif
