#pragma once

// Exclusive access (ldrex / strex / clrex) and the "update if" pattern built on it, used by the
// lock and event primitives of nn::os (SimpleLock, LightEvent, ...).
//
// The binary always shows the same shape:
//     retry: ldrex  x, [p]
//            <condition on x>   -> fails: clrex, give up
//            <new value from x>
//            strex  status, x, [p]
//            cmp    status, #0
//            bne    retry
// which is what AtomicUpdateConditional produces once the update object is inlined.
// The names in this header are ours.

#include "types.h"

namespace nn {
namespace os {
namespace detail {


inline s32 LoadExclusive(volatile s32* p)
{
    s32 value;
    __asm__ __volatile__("ldrex %0, [%1]" : "=r"(value) : "r"(p) : "memory");
    return value;
}

inline bool StoreExclusive(volatile s32* p, s32 value)
{
    u32 failed;
    __asm__ __volatile__("strex %0, %2, [%1]" : "=&r"(failed) : "r"(p), "r"(value) : "memory");
    return failed != 0;
}

inline void ClearExclusive() { __asm__ __volatile__("clrex" ::: "memory"); }


// 16 bit variants (ldrexh / strexh)
inline s16 LoadExclusive(volatile s16* p)
{
    s16 value;
    __asm__ __volatile__("ldrexh %0, [%1]" : "=r"(value) : "r"(p) : "memory");
    return value;
}

inline bool StoreExclusive(volatile s16* p, s16 value)
{
    u32 failed;
    __asm__ __volatile__("strexh %0, %2, [%1]" : "=&r"(failed) : "r"(p), "r"(value) : "memory");
    return failed != 0;
}

// Atomically adds delta, returns the new value.
inline s16 AtomicAdd(volatile s16* p, s16 delta)
{
    s16 value;
    do {
        value = static_cast<s16>(LoadExclusive(p) + delta);
    } while (StoreExclusive(p, value));
    return value;
}

// Atomically stores value (16 bit).
inline void AtomicStore(volatile s16* p, s16 value)
{
    do {
        LoadExclusive(p);
    } while (StoreExclusive(p, value));
}

// Atomically adds delta, returns the new value.
inline s32 AtomicAdd(volatile s32* p, s32 delta)
{
    s32 value;
    do {
        value = LoadExclusive(p) + delta;
    } while (StoreExclusive(p, value));
    return value;
}

// If *p == compare, atomically sets it to value. Returns the old contents either way.
inline s32 AtomicCompareAndSwap(volatile s32* p, s32 compare, s32 value)
{
    s32 old;
    do {
        old = LoadExclusive(p);
        if (old != compare) {
            ClearExclusive();
            break;
        }
    } while (StoreExclusive(p, value));
    return old;
}

// Atomically stores value (a plain store would not cancel another core's exclusive access).
inline void AtomicStore(volatile s32* p, s32 value)
{
    do {
        LoadExclusive(p);
    } while (StoreExclusive(p, value));
}

// Atomically replaces *p by an updated value if the update object agrees.
// Update: bool operator()(s32& value) - returns false to leave *p unchanged, otherwise
// changes value to the new contents of *p.
// Returns whether *p was updated.
template <typename Update>
inline bool AtomicUpdateConditional(volatile s32* p, Update update)
{
    s32 value;
    do {
        value = LoadExclusive(p);
        if (!update(value)) {
            ClearExclusive();
            return false;
        }
    } while (StoreExclusive(p, value));
    return true;
}

} // namespace detail
} // namespace os
} // namespace nn
