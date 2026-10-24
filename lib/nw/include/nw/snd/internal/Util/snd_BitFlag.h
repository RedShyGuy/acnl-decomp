#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace Util {
// Optional values of a resource: a word of flags, then one word per set flag in the order of the
// bits (the class and function names are from the reference symbols, the rest is ours).
class BitFlag
{
public:
    // the value of the flag; false if it is not set
    bool GetValue(u32* pValue, u32 bit) const; // 0x00142108 | nintendogs:bytes [tier A]
    // (inline) a float value (name is ours)
    bool GetValueF32(f32* pValue, u32 bit) const
    {
        u32 count = 0;
        bool isSet = false;
        for (u32 i = 0; i < bit + 1; ++i) {
            if (m_Flags & (1 << i)) {
                ++count;
                if (i == bit) {
                    isSet = true;
                }
            }
        }
        if (!isSet || count == 0) {
            return false;
        }
        *pValue = reinterpret_cast<const f32*>(&m_Flags)[count];
        return true;
    }

    u32 m_Flags; // 0x0 (the values follow)
};
} // namespace Util
} // namespace internal
} // namespace snd
} // namespace nw
