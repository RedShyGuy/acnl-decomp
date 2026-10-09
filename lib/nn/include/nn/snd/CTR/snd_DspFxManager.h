#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_Types.h"

namespace nn {
namespace snd {
namespace CTR {
// Which DSP effect is attached to which aux bus. Member names are ours.
class DspFxManager
{
public:
    enum DspEffectType : u8
    {
        DSP_EFFECT_TYPE_DELAY = 0,
        DSP_EFFECT_TYPE_REVERB = 1,
        DSP_EFFECT_TYPE_COUNT = 2
    };

    static DspFxManager* GetInstance(); // 0x0046143C | nintendogs:callgraph [confirmed by fefates, mk7dlp] [tier A]
    u32 GetDspCycles(); // 0x00461448 | nintendogs:bytes [confirmed by fefates] [tier A]
    bool Attach(DspEffectType type, AuxBusId bus); // 0x004614BC | fefates:bytes [tier B]
    bool Detach(DspEffectType type, AuxBusId bus); // 0x004614DC | fefates:bytes [tier B]
    void Initialize(); // 0x00462A2C | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
    bool SetDspDelayEffect(AuxBusId bus, DspFxDelayParams& params); // 0x00462C48 | fefates:bytes [tier B]
    bool SetDspReverbEffect(AuxBusId bus, DspFxReverbParams& params); // 0x00462CE4 | fefates:bytes [tier B]
    void Finalize(); // 0x00462DE8 | nintendogs:callgraph [confirmed by fefates, mk7dlp] [tier A]

    bool m_IsAttached[DSP_EFFECT_TYPE_COUNT][AUX_BUS_COUNT];    // 0x0
    bool m_IsEnabled[DSP_EFFECT_TYPE_COUNT][AUX_BUS_COUNT];     // 0x4
    s8 m_ChannelCount[DSP_EFFECT_TYPE_COUNT][AUX_BUS_COUNT];    // 0x8
};
ASSERT_SIZE(DspFxManager, 0xC);
} // namespace CTR
} // namespace snd
} // namespace nn
