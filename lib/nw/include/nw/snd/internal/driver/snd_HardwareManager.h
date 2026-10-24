#pragma once

#include "decomp.h"
#include "nn/os/os_Tick.h"
#include "nn/snd/CTR/CTR_Api.h"
#include "nn/snd/CTR/snd_DspFxDelay.h"
#include "nn/snd/CTR/snd_DspFxReverb.h"
#include "nw/snd/internal/snd_MoveValue.h"
#include "nw/snd/snd_Global.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace snd {
class BiquadFilterCallback;
class FxBase;

namespace internal {
namespace driver {
// The output of the sound library: the master volume, the effects of the two aux buses (DSP
// effects of nn::snd or FxBase objects run in the aux callback) and the biquad filters (the class
// and function names are from the reference symbols, the rest is ours). There is one instance.
class HardwareManager
{
public:
    static const int AUX_BUS_COUNT = 2;
    static const int BIQUAD_FILTER_TYPE_COUNT = 128;

    // a DSP effect of an aux bus (0 delay, 1 reverb)
    struct DspEffect
    {
        // (inline)
        DspEffect() : pEffect(nullptr), state(STATE_NONE) {}

        enum State : u8
        {
            STATE_NONE = 0,
            STATE_ENABLED = 1,
            STATE_FINALIZING = 2, // until the DSP no longer uses the buffer
        };

        void* pEffect; // 0x0 nn::snd::CTR::DspFxDelay or DspFxReverb
        State state;   // 0x4
    };

    HardwareManager(); // 0x004CD768 | fefates:bytes [tier B]

    // (inline)
    static HardwareManager& GetInstance() { return s_Instance; }
    OutputMode GetOutputMode() const { return m_OutputMode; }

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
    void GetOutputVolume() const; // 0x00741458 | fefates:bytes [tier B]

    static HardwareManager s_Instance;

    bool m_IsInitialized;                                       // 0x000
    OutputMode m_OutputMode;                                    // 0x001
    u8 m_Unknown0x2;                                            // 0x002 (2)
    MoveValue<f32, int> m_MasterVolume;                         // 0x004
    MoveValue<f32, int> m_AuxFadeVolumes[AUX_BUS_COUNT];        // 0x014 (fading out before the effects are finalized)
    MoveValue<f32, int> m_AuxUserVolumes[AUX_BUS_COUNT];        // 0x034
    ut::LinkList<FxBase, 4> m_FxLists[AUX_BUS_COUNT];           // 0x054
    nn::snd::CTR::AuxCallback m_OldAuxCallbacks[AUX_BUS_COUNT]; // 0x06C the callbacks before Initialize
    BiquadFilterCallback* m_BiquadFilterCallbacks[BIQUAD_FILTER_TYPE_COUNT]; // 0x074 by filter type (1-5: the built-in ones)
    uptr m_OldAuxCallbackArgs[AUX_BUS_COUNT];                   // 0x274
    nn::os::Tick m_AuxCallbackTicks[AUX_BUS_COUNT];             // 0x280 the time the last aux callback took
    DspEffect m_DspEffects[AUX_BUS_COUNT][2];                   // 0x290
};
ASSERT_SIZE(HardwareManager, 0x2B0);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
