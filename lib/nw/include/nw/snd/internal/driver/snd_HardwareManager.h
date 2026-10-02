#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_DspFxDelay.h"
#include "nn/snd/CTR/snd_DspFxReverb.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class HardwareManager
{
public:
    void ClearEffect(nw::snd::AuxBus, int); // 0x004CCE78 | nintendogs:bytes-fuzzy [tier A]
    void AppendEffect(nw::snd::AuxBus, nn::snd::CTR::DspFxDelay*, const nn::snd::CTR::DspFxDelay::Param&); // 0x004CCEEC | fefates:bytes [tier B]
    void AppendEffect(nw::snd::AuxBus, nn::snd::CTR::DspFxReverb*, const nn::snd::CTR::DspFxReverb::Param&); // 0x004CCFBC | fefates:bytes [tier B]
    void AppendEffect(nw::snd::AuxBus, nw::snd::FxBase*); // 0x004CD154 | nintendogs:callseq [tier A]
    void SetOutputMode(nw::snd::OutputMode); // 0x004CD204 | nintendogs:bytes-fuzzy [tier B]
    void FinalizeEffect(nw::snd::AuxBus); // 0x004CD2D0 | nintendogs:callseq-callee [tier A]
    void AuxCallbackFunc(nn::snd::CTR::AuxBusData*, int, unsigned int); // 0x004CD3C0 | fefates:bytes [tier B]
    void SetMasterVolume(float, int); // 0x004CD480 | nintendogs:bytes-fuzzy [tier B]
    void Update(); // 0x004CD50C | fefates:bytes [tier B]
    void Finalize(); // 0x004CD710 | nintendogs:bytes-fuzzy [tier A]
    HardwareManager(); // 0x004CD768 | fefates:bytes [tier B]
    void GetOutputVolume() const; // 0x00741458 | fefates:bytes [tier B]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
