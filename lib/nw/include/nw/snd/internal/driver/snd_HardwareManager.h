#pragma once

#include "decomp.h"
#include "nn/os/os_Tick.h"
#include "nn/snd/CTR/CTR_Api.h"
#include "nn/snd/CTR/snd_DspFxDelay.h"
#include "nn/snd/CTR/snd_DspFxReverb.h"
#include "nw/snd/internal/snd_MoveValue.h"
#include "nw/snd/snd_FxBase.h"
#include "nw/snd/snd_Global.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace snd {
class BiquadFilterCallback;

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
    // the interval of Update (ms); fade times are rounded up to it
    static const u32 UPDATE_INTERVAL_MSEC = 5;

    // a DSP effect of an aux bus (0 delay, 1 reverb)
    struct DspEffect
    {
        enum State : u8
        {
            STATE_NONE = 0,
            STATE_ENABLED = 1,
            STATE_FINALIZING = 2, // until the DSP no longer uses the buffer
        };

        // (inline)
        DECOMP_EMIT_COPY DspEffect() : pEffect(nullptr), state(STATE_NONE) {} // 0x004CD1F4

        void* pEffect; // 0x0 nn::snd::CTR::DspFxDelay or DspFxReverb
        State state;   // 0x4
    };
    enum DspEffectType
    {
        DSP_EFFECT_DELAY = 0,
        DSP_EFFECT_REVERB = 1,
        DSP_EFFECT_TYPE_COUNT = 2,
    };

    HardwareManager(); // 0x004CD768 | fefates:bytes [tier B]

    // (inline)
    static HardwareManager& GetInstance() { return s_Instance; }
    OutputMode GetOutputMode() const { return m_OutputMode; }
    u8 GetInterpolationType() const { return m_InterpolationType; }
    BiquadFilterCallback* GetBiquadFilterCallback(int type) const { return m_BiquadFilterCallbacks[type]; }

    // keeps the old aux callbacks, installs the built-in biquad filters and reads the output mode
    void Initialize(); // 0x004CCD8C (name is ours)
    void Finalize(); // 0x004CD710 | nintendogs:bytes-fuzzy [tier A]
    void Update(); // 0x004CD50C | fefates:bytes [tier B]

    // fades the effects of a bus out (fadeTimes in ms; 0: at once) and finalizes them
    void ClearEffect(AuxBus bus, int fadeTimes); // 0x004CCE78 | nintendogs:bytes-fuzzy [tier A]
    bool AppendEffect(AuxBus bus, nn::snd::CTR::DspFxDelay* pEffect, const nn::snd::CTR::DspFxDelay::Param& param); // 0x004CCEEC | fefates:bytes [tier B]
    bool AppendEffect(AuxBus bus, nn::snd::CTR::DspFxReverb* pEffect, const nn::snd::CTR::DspFxReverb::Param& param); // 0x004CCFBC | fefates:bytes [tier B]
    bool AppendEffect(AuxBus bus, nn::snd::CTR::FxDelay* pEffect); // 0x004CD08C (name after the other AppendEffect)
    bool AppendEffect(AuxBus bus, nn::snd::CTR::FxReverb* pEffect); // 0x004CD0F0 (name after the other AppendEffect)
    bool AppendEffect(AuxBus bus, FxBase* pEffect); // 0x004CD154 | nintendogs:callseq [tier A]
    void FinalizeEffect(AuxBus bus); // 0x004CD2D0 | nintendogs:callseq-callee [tier A]
    // whether a bus has no effects left (also none finalizing)
    bool IsEffectFinalized(AuxBus bus) const; // 0x00741494 (name is ours)
    void SetOutputMode(OutputMode mode); // 0x004CD204 | nintendogs:bytes-fuzzy [tier B]
    // fadeTimes in ms
    void SetMasterVolume(f32 volume, int fadeTimes); // 0x004CD480 | nintendogs:bytes-fuzzy [tier B]
    f32 GetOutputVolume() const; // 0x00741458 | fefates:bytes [tier B]
    // the filter of a type (above 0)
    void SetBiquadFilterCallback(int type, BiquadFilterCallback* pCallback); // 0x004CD4FC (name is ours)

    // runs the FxBase effects of the bus arg
    static void AuxCallbackFunc(nn::snd::CTR::AuxBusData* pData, s32 sampleLength, uptr arg); // 0x004CD3C0 | fefates:bytes [tier B]

    static HardwareManager s_Instance;

    bool m_IsInitialized;                                       // 0x000
    OutputMode m_OutputMode;                                    // 0x001
    u8 m_InterpolationType;                                     // 0x002 of new voices (0 4-point, 1 linear, 2 none; names are ours)
    MoveValue<f32, int> m_MasterVolume;                         // 0x004
    MoveValue<f32, int> m_AuxFadeVolumes[AUX_BUS_COUNT];        // 0x014 (fading out before the effects are finalized)
    MoveValue<f32, int> m_AuxUserVolumes[AUX_BUS_COUNT];        // 0x034
    ut::LinkList<FxBase, 4> m_FxLists[AUX_BUS_COUNT];           // 0x054
    nn::snd::CTR::AuxCallback m_OldAuxCallbacks[AUX_BUS_COUNT]; // 0x06C the callbacks before Initialize
    BiquadFilterCallback* m_BiquadFilterCallbacks[BIQUAD_FILTER_TYPE_COUNT]; // 0x074 by filter type (1-5: the built-in ones)
    uptr m_OldAuxCallbackArgs[AUX_BUS_COUNT];                   // 0x274
    nn::os::Tick m_AuxCallbackTicks[AUX_BUS_COUNT];             // 0x280 the time the last aux callback took
    DspEffect m_DspEffects[AUX_BUS_COUNT][DSP_EFFECT_TYPE_COUNT]; // 0x290
};
ASSERT_SIZE(HardwareManager, 0x2B0);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
