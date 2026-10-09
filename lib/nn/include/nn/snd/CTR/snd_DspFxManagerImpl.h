#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_Types.h"

namespace nn {
namespace snd {
namespace CTR {
// The parameters of the DSP effects (sent to Dspsnd, and sent again after a sleep). Member
// names are ours.
class DspFxManagerImpl
{
public:
    void Initialize(); // 0x00462A70 | nintendogs:bytes [confirmed by fefates] [tier A]
    static DspFxManagerImpl* GetInstance(); // 0x00462BDC | nintendogs:callgraph [confirmed by fefates] [tier A]
    void ForceUpdateParams(); // 0x00462BE8 | nintendogs:bytes [confirmed by fefates] [tier A]
    bool SetDspDelayEffect(AuxBusId bus, DspFxDelayParams& params); // 0x00462C7C | fefates:bytes [tier B]
    bool SetDspReverbEffect(AuxBusId bus, DspFxReverbParams& params); // 0x00462D18 | fefates:bytes [tier B]
    void Finalize() {}

    DspFxDelayParams m_Delay[AUX_BUS_COUNT];    // 0x00
    DspFxReverbParams m_Reverb[AUX_BUS_COUNT];  // 0x28
};
ASSERT_SIZE(DspFxManagerImpl, 0x90);
} // namespace CTR
} // namespace snd
} // namespace nn
