#pragma once

#include "decomp.h"

namespace nn {
namespace snd {
namespace CTR {
// Records the output of the DSP into a ring buffer of stereo frames. Member names are ours.
class OutputCapture
{
public:
    void Write(s16* samples, int sampleCount); // 0x00461F20 | nintendogs:bytes [confirmed by fefates] [tier A]

    s16* m_Buffer;          // 0x00, left and right
    s32 m_SampleCount;      // 0x04
    u32 m_Unknown8;         // 0x08
    s32 m_Position;         // 0x0C
    bool m_IsEnabled;       // 0x10
};
ASSERT_SIZE(OutputCapture, 0x14);
} // namespace CTR
} // namespace snd
} // namespace nn
