#pragma once

#include "decomp.h"

namespace sead {
// A fixed array whose index is checked: an index out of range gives the first element (ARMCC tests
// `cmp idx, #N; addcc`; the class and its name are ours).
template <typename T, s32 N>
class CheckedArray
{
public:
    T& operator[](s32 idx)
    {
        if (static_cast<u32>(idx) < static_cast<u32>(N)) {
            return mBuffer[idx];
        }
        return mBuffer[0];
    }
    const T& operator[](s32 idx) const
    {
        if (static_cast<u32>(idx) < static_cast<u32>(N)) {
            return mBuffer[idx];
        }
        return mBuffer[0];
    }

    T mBuffer[N];
};
} // namespace sead
