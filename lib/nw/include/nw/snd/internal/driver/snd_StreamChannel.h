#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_Types.h"
#include "nw/snd/internal/driver/snd_MultiVoice.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// A channel of a stream: the voice and the wave buffers of the stream blocks (the class and function
// names are from the reference symbols, the members are ours).
class StreamChannel
{
public:
    static const int WAVE_BUFFER_COUNT = 32;

    StreamChannel(); // 0x004CCB24 | fefates:bytes [tier B]

    void AppendWaveBuffer(nn::snd::CTR::WaveBuffer* pBuffer, bool isLast); // 0x004CCB04 | fefates:bytes [tier B]

    u32 m_Unknown0;                                        // 0x000
    MultiVoice* m_pVoice;                                  // 0x004
    nn::snd::CTR::WaveBuffer m_WaveBuffers[WAVE_BUFFER_COUNT]; // 0x008
};
ASSERT_SIZE(StreamChannel, 0x308);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
