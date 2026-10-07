#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
class Time;
}
namespace transport {
// Notes the time of the last packet of each station (StationPacketHandler has one). The member
// name is ours.
class KeepAliveReceiver
{
public:
    KeepAliveReceiver(); // 0x00454210 | fefates:bytes [tier B]
    // receivedBitmap: the stations something was received from now
    void Update(unsigned int receivedBitmap, const nn::pia::common::Time& now); // 0x0045419C | fefates:bytes [tier B]

    bool m_IsEnabled; // 0x0
};
} // namespace transport
} // namespace pia
} // namespace nn
