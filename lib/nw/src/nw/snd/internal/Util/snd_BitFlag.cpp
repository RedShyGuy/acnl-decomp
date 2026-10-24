#include "nw/snd/internal/Util/snd_BitFlag.h"

namespace nw {
namespace snd {
namespace internal {
namespace Util {
// 0x00142108 | nintendogs:bytes [tier A]
bool nw::snd::internal::Util::BitFlag::GetValue(u32* pValue, u32 bit) const
{
    // the index of the value: the number of set flags up to the bit
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
    *pValue = (&m_Flags)[count];
    return true;
}

} // namespace Util
} // namespace internal
} // namespace snd
} // namespace nw
