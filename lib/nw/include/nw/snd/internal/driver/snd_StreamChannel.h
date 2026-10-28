#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_Types.h"
#include "nw/snd/internal/driver/snd_MultiVoice.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// A channel of a stream: the buffer, the voice and the wave buffers of the stream blocks (the class and function
// names are from the reference symbols, the members are ours).
class StreamChannel
{
public:
    static const int WAVE_BUFFER_COUNT = 32;

    StreamChannel(); // 0x004CCB24 | fefates:bytes [tier B]

    void AppendWaveBuffer(nn::snd::CTR::WaveBuffer* pBuffer, bool isLast); // 0x004CCB04 | fefates:bytes [tier B]

    void* m_pBuffer;                                       // 0x000 the buffer blocks (StreamBufferPool)
    MultiVoice* m_pVoice;                                  // 0x004
    nn::snd::CTR::WaveBuffer m_WaveBuffers[WAVE_BUFFER_COUNT]; // 0x008
    nn::snd::CTR::AdpcmContext m_AdpcmContexts[WAVE_BUFFER_COUNT]; // 0x308 the state at the start of the blocks
};
ASSERT_SIZE(StreamChannel, 0x3C8);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
