#pragma once

#include "decomp.h"

namespace nn {
namespace snd {
namespace CTR {
class VoiceManager
{
public:
    void Initialize(); // 0x00130820 | nintendogs:bytes [tier A]
    void SetPriority(nn::snd::CTR::Voice*, int); // 0x00461690 | nintendogs:bytes [tier A]
    void AdjustVoicePlayState(int, int); // 0x004617D4 | nintendogs:bytes [tier A]
    void Finalize(); // 0x00461980 | nintendogs:callgraph [tier A]
    void FreeVoice(nn::snd::CTR::Voice*); // 0x0046198C | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace snd
} // namespace nn
