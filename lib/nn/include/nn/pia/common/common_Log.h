#pragma once

#include "decomp.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace common {
// The log of pia (common::Initialize makes one in a static buffer). Layout from the constructor;
// the member names are ours.
class Log
{
public:
    Log(); // 0x00428BAC | fefates:bytes [tier B]

    u32 m_Unknown0x0;     // 0x00 (all bits set)
    Time m_StartTime;     // 0x08, when the log was made
    bool m_Unknown0x10;   // 0x10

    static Log* s_pInstance; // 0x0097E3CC
};
ASSERT_SIZE(Log, 0x18);
} // namespace common
} // namespace pia
} // namespace nn
