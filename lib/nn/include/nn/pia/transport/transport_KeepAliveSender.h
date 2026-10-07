#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace pia {
namespace common {
class Time;
}
namespace transport {
// The stations to send a keep alive message to: the connected ones nothing was sent to for the
// interval (StationPacketHandler has one). Layout from the constructor; the member names are
// ours.
class KeepAliveSender
{
public:
    KeepAliveSender(); // 0x00450160 | fefates:bytes [tier B]
    nn::Result SetInterval(int intervalMSec); // 0x00450054 | fefates:bytes [tier B]
    // sentBitmap: the stations something was sent to now; the result: the stations to send a
    // keep alive message to
    u32 Update(unsigned int sentBitmap, const nn::pia::common::Time& now); // 0x0045006C | fefates:bytes [tier B]

    s32 m_IntervalMSec; // 0x0
    bool m_IsEnabled;   // 0x4
};
ASSERT_SIZE(KeepAliveSender, 0x8);
} // namespace transport
} // namespace pia
} // namespace nn
