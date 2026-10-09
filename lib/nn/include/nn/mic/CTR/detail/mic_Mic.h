#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/mic/CTR/mic_Types.h"

namespace nn {
namespace mic {
namespace CTR {
namespace detail {
// The commands of mic:u (3dbrew "MIC Services"); all static. Method names from the symbols or
// after the commands.
class Mic
{
public:
    static nn::Result MapSharedMem(nn::Handle sharedMemory, size_t size); // 0x00131004 | tier B
    static nn::Result SetClientVersion(u32 version); // 0x00131048 | tier B
    static nn::Result GetPGAB(u8* pGain); // 0x00131084 | nintendogs:bytes [tier B]
    static nn::Result FreeBuffer(); // 0x00140AF0 | nintendogs:bytes [tier A]
    static nn::Result IsSampling(bool* pIsSampling); // 0x00140B20 | nintendogs:bytes [tier A]
    // (only a branch to SetMicBias)
    static nn::Result SetMicBiasEntry(bool enable); // 0x00140B5C | tier B
    static nn::Result SetMicBias(bool enable); // 0x00140B60 | nintendogs:bytes [tier A]
    static nn::Result StopSampling(); // 0x00140BA0 | nintendogs:bytes [tier A]
    static nn::Result StartSampling(SamplingType type, SamplingRate rate, s32 offset, size_t size, bool loop); // 0x003549B4 | tier B
    static nn::Result AdjustSampling(SamplingRate rate); // 0x00354A08 | tier B
    static nn::Result SetIirFilterMic(const void* pCoefficients, size_t size); // 0x00354A44 | tier B
    // (only a branch to SetPGAB)
    static nn::Result SetPGABEntry(u8 gain); // 0x00354A8C | tier B
    static nn::Result SetPGAB(u8 gain); // 0x00354A90 | nintendogs:bytes [tier A]
};

// the session of mic:u
extern nn::Handle s_Session;
} // namespace detail
} // namespace CTR
} // namespace mic
} // namespace nn
