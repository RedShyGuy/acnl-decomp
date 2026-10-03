#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace ptm {
namespace CTR {
namespace detail {
// The commands of the ptm:u service (3dbrew "PTM Services") on the session detail::s_Session.
class PtmIpc
{
public:
    // the steps of `hours` hours from `start` (milliseconds since 2000), one u16 per hour
    static nn::Result GetStepHistory(u16* steps, u32 hours, s64 start); // 0x0012A768 (name after 3dbrew)
    static nn::Result GetTotalStepCount(u32* count); // 0x0012A7B4 | nintendogs:bytes [tier B]
};
} // namespace detail
} // namespace CTR
} // namespace ptm
} // namespace nn
