#include "nn/snd/CTR/snd_OutputCapture.h"
#include <string.h>

namespace nn {
namespace snd {
namespace CTR {
// 0x00461F20 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::OutputCapture::Write(s16* samples, int sampleCount)
{
    memcpy(m_Buffer + m_Position * 2, samples, sampleCount * 4);
    m_Position += sampleCount;
    if (m_Position >= m_SampleCount) {
        m_Position = 0;
    }
}

} // namespace CTR
} // namespace snd
} // namespace nn
