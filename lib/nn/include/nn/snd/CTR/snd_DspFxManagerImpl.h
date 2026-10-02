#pragma once

#include "decomp.h"

namespace nn {
namespace snd {
namespace CTR {
class DspFxManagerImpl
{
public:
    void Initialize(); // 0x00462A70 | nintendogs:bytes [tier A]
    void GetInstance(); // 0x00462BDC | nintendogs:callgraph [tier A]
    void ForceUpdateParams(); // 0x00462BE8 | nintendogs:bytes [tier A]
    void SetDspDelayEffect(nn::snd::CTR::AuxBusId, nn::snd::CTR::DspFxDelayParams&); // 0x00462C7C | fefates:bytes [tier B]
    void SetDspReverbEffect(nn::snd::CTR::AuxBusId, nn::snd::CTR::DspFxReverbParams&); // 0x00462D18 | fefates:bytes [tier B]
};
} // namespace CTR
} // namespace snd
} // namespace nn
