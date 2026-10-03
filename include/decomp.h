#pragma once

// Common header of the ACNL decompilation. Every generated header includes it.

#include "types.h"

#ifdef __cplusplus
#include "forward.h"
#endif

// The project builds with GCC (devkitARM) only. Nintendo built the game with ARMCC 4.1, so the
// code is compared by structure, not byte for byte (tools/decomp/check.py, tools/decomp/fuzzy.py).

// Layout checks for structures whose size / member offsets are known.
#ifdef __cplusplus
#define ASSERT_SIZE(type, size) \
    static_assert(sizeof(type) == (size), #type " has the wrong size")
#define ASSERT_OFFSET(type, member, offset) \
    static_assert(offsetof(type, member) == (offset), #type "::" #member " is at the wrong offset")
#else
#define ASSERT_SIZE(type, size) _Static_assert(sizeof(type) == (size), #type " has the wrong size")
#define ASSERT_OFFSET(type, member, offset) \
    _Static_assert(offsetof(type, member) == (offset), #type "." #member " is at the wrong offset")
#endif

#define DECOMP_PACKED __attribute__((packed))
#define DECOMP_ALIGN(n) __attribute__((aligned(n)))
#define DECOMP_NORETURN __attribute__((noreturn))
#define DECOMP_NOINLINE __attribute__((noinline))   // the original calls it, GCC would inline it
// the original calls it with the arguments of its only caller; GCC would also specialize it for them
#define DECOMP_NOIPA __attribute__((noipa))
#define DECOMP_ALWAYS_INLINE inline __attribute__((always_inline))   // the original inlines it, GCC would call it
