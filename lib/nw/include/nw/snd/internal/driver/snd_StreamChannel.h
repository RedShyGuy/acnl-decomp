#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class StreamChannel
{
public:
    void AppendWaveBuffer(nn::snd::CTR::WaveBuffer*, bool); // 0x004CCB04 | fefates:bytes [tier B]
    StreamChannel(); // 0x004CCB24 | fefates:bytes [tier B]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
