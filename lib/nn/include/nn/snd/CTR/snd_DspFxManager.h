#pragma once

#include "decomp.h"

namespace nn {
namespace snd {
namespace CTR {
class DspFxManager
{
public:
    struct DspEffectType { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void GetInstance(); // 0x0046143C | nintendogs:callgraph [tier A]
    void GetDspCycles(); // 0x00461448 | nintendogs:bytes [tier A]
    void Attach(nn::snd::CTR::DspFxManager::DspEffectType, nn::snd::CTR::AuxBusId); // 0x004614BC | fefates:bytes [tier B]
    void Detach(nn::snd::CTR::DspFxManager::DspEffectType, nn::snd::CTR::AuxBusId); // 0x004614DC | fefates:bytes [tier B]
    void Initialize(); // 0x00462A2C | nintendogs:bytes [tier A]
    void SetDspDelayEffect(nn::snd::CTR::AuxBusId, nn::snd::CTR::DspFxDelayParams&); // 0x00462C48 | fefates:bytes [tier B]
    void SetDspReverbEffect(nn::snd::CTR::AuxBusId, nn::snd::CTR::DspFxReverbParams&); // 0x00462CE4 | fefates:bytes [tier B]
    void Finalize(); // 0x00462DE8 | nintendogs:callgraph [tier A]
};
} // namespace CTR
} // namespace snd
} // namespace nn
