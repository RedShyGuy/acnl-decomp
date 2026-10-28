#include "nw/snd/internal/driver/snd_HardwareManager.h"

#include "nn/svc/svc_Api.h"
#include "nw/snd/internal/driver/snd_VoiceManager.h"
#include "nw/snd/internal/snd_BiquadFilterBpf1024.h"
#include "nw/snd/internal/snd_BiquadFilterBpf2048.h"
#include "nw/snd/internal/snd_BiquadFilterBpf512.h"
#include "nw/snd/internal/snd_BiquadFilterHpf.h"
#include "nw/snd/internal/snd_BiquadFilterLpf.h"
#include "nw/snd/internal/snd_DriverCommandManager.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
namespace {
// the built-in biquad filters (types 1 to 5)
// 0x00976178
BiquadFilterLpf s_BiquadFilterLpf;
// 0x0097617C
BiquadFilterHpf s_BiquadFilterHpf;
// 0x00976180
BiquadFilterBpf512 s_BiquadFilterBpf512;
// 0x00976184
BiquadFilterBpf1024 s_BiquadFilterBpf1024;
// 0x00976188
BiquadFilterBpf2048 s_BiquadFilterBpf2048;

// seven ticks and two words that only the static initializer touches (names are ours)
struct UnusedTicks
{
    UnusedTicks() : unknown38(0), unknown40(0) {}

    nn::os::Tick ticks[7]; // 0x00
    s64 unknown38;         // 0x38
    u32 unknown40;         // 0x40
};
// 0x00AE5270
UnusedTicks s_UnusedTicks;

// the sample rate of the aux buffers (the DSP rate, rounded)
const f32 AUX_SAMPLE_RATE = 32728.0f;
// what the voices update after a change of the output (VoiceManager::UpdateAllVoicesSync)
const u32 VOICE_SYNC_OUTPUT_MODE = Voice::UPDATE_MIX;
const u32 VOICE_SYNC_MASTER_VOLUME = Voice::UPDATE_VOLUME;

// (inline) asks the sound thread to update the voices (name is ours)
void PushVoiceSyncCommand(u32 syncFlag)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandVoiceSync* pCommand = manager.AllocCommand<DriverCommandVoiceSync>();
    pCommand->id = DriverCommand::ID_VOICE_SYNC;
    pCommand->syncFlag = syncFlag;
    manager.PushCommand(pCommand);
}

// (inline) the frames of a fade time in ms
u32 GetFadeFrames(int fadeTimes)
{
    return (fadeTimes + HardwareManager::UPDATE_INTERVAL_MSEC - 1) / HardwareManager::UPDATE_INTERVAL_MSEC;
}

// (inline)
f32 Clamp01(f32 value)
{
    if (value > 1.0f) {
        return 1.0f;
    }
    if (value < 0.0f) {
        return 0.0f;
    }
    return value;
}
} // namespace

// builds the built-in filters, s_UnusedTicks and s_Instance
// 0x0079D2A0 (static initializer)

// 0x00AE4FC0
HardwareManager HardwareManager::s_Instance;

// 0x004CCD8C (name is ours)
void nw::snd::internal::driver::HardwareManager::Initialize()
{
    if (m_IsInitialized) {
        return;
    }
    for (int i = 0; i < AUX_BUS_COUNT; ++i) {
        const nn::snd::CTR::AuxBusId bus = static_cast<nn::snd::CTR::AuxBusId>(i);
        nn::snd::CTR::GetAuxCallback(bus, &m_OldAuxCallbacks[i], &m_OldAuxCallbackArgs[i]);
        nn::snd::CTR::RegisterAuxCallback(bus, nullptr, 0);
    }
    for (int i = 0; i < BIQUAD_FILTER_TYPE_COUNT; ++i) {
        m_BiquadFilterCallbacks[i] = nullptr;
    }
    m_BiquadFilterCallbacks[1] = &s_BiquadFilterLpf;
    m_BiquadFilterCallbacks[2] = &s_BiquadFilterHpf;
    m_BiquadFilterCallbacks[3] = &s_BiquadFilterBpf512;
    m_BiquadFilterCallbacks[4] = &s_BiquadFilterBpf1024;
    m_BiquadFilterCallbacks[5] = &s_BiquadFilterBpf2048;
    nn::snd::CTR::SetMasterVolume(1.0f);
    switch (nn::snd::CTR::GetSoundOutputMode()) {
    case nn::snd::CTR::OUTPUT_MODE_MONO:
        m_OutputMode = OUTPUT_MODE_MONO;
        break;
    case nn::snd::CTR::OUTPUT_MODE_STEREO:
        m_OutputMode = OUTPUT_MODE_STEREO;
        break;
    case nn::snd::CTR::OUTPUT_MODE_SURROUND:
        m_OutputMode = OUTPUT_MODE_SURROUND;
        break;
    }
    m_IsInitialized = true;
}

// 0x004CCE78 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::driver::HardwareManager::ClearEffect(AuxBus bus, int fadeTimes)
{
    if (fadeTimes != 0) {
        m_AuxFadeVolumes[bus].SetTarget(0.0f, GetFadeFrames(fadeTimes));
        return;
    }
    FinalizeEffect(bus);
    if (m_AuxFadeVolumes[bus].IsFinished()) {
        m_AuxFadeVolumes[bus].SetTarget(0.0f, 0);
    }
}

// 0x004CCEEC | fefates:bytes [tier B]
bool nw::snd::internal::driver::HardwareManager::AppendEffect(AuxBus bus, nn::snd::CTR::DspFxDelay* pEffect, const nn::snd::CTR::DspFxDelay::Param& param)
{
    DspEffect& effect = m_DspEffects[bus][DSP_EFFECT_DELAY];
    if (effect.state != DspEffect::STATE_NONE) {
        return false;
    }
    if (!m_AuxFadeVolumes[bus].IsFinished()) {
        FinalizeEffect(bus);
    }
    m_AuxFadeVolumes[bus].SetTarget(1.0f, 0);
    const nn::snd::CTR::AuxBusId busId = static_cast<nn::snd::CTR::AuxBusId>(bus);
    nn::snd::CTR::SetAuxReturnVolume(busId, 1.0f);
    if (!pEffect->Attach(busId)) {
        return false;
    }
    if (!pEffect->SetParam(param)) {
        return false;
    }
    if (!pEffect->Enable(true)) {
        return false;
    }
    effect.pEffect = pEffect;
    effect.state = DspEffect::STATE_ENABLED;
    return true;
}

// 0x004CCFBC | fefates:bytes [tier B]
bool nw::snd::internal::driver::HardwareManager::AppendEffect(AuxBus bus, nn::snd::CTR::DspFxReverb* pEffect, const nn::snd::CTR::DspFxReverb::Param& param)
{
    DspEffect& effect = m_DspEffects[bus][DSP_EFFECT_REVERB];
    if (effect.state != DspEffect::STATE_NONE) {
        return false;
    }
    if (!m_AuxFadeVolumes[bus].IsFinished()) {
        FinalizeEffect(bus);
    }
    m_AuxFadeVolumes[bus].SetTarget(1.0f, 0);
    const nn::snd::CTR::AuxBusId busId = static_cast<nn::snd::CTR::AuxBusId>(bus);
    nn::snd::CTR::SetAuxReturnVolume(busId, 1.0f);
    if (!pEffect->Attach(busId)) {
        return false;
    }
    if (!pEffect->SetParam(param)) {
        return false;
    }
    if (!pEffect->Enable(true)) {
        return false;
    }
    effect.pEffect = pEffect;
    effect.state = DspEffect::STATE_ENABLED;
    return true;
}

// 0x004CD08C (name after the other AppendEffect)
bool nw::snd::internal::driver::HardwareManager::AppendEffect(AuxBus bus, nn::snd::CTR::FxDelay* pEffect)
{
    if (!m_AuxFadeVolumes[bus].IsFinished()) {
        FinalizeEffect(bus);
    }
    m_AuxFadeVolumes[bus].SetTarget(1.0f, 0);
    const nn::snd::CTR::AuxBusId busId = static_cast<nn::snd::CTR::AuxBusId>(bus);
    nn::snd::CTR::SetAuxReturnVolume(busId, 1.0f);
    return nn::snd::CTR::SetEffect(busId, pEffect);
}

// 0x004CD0F0 (name after the other AppendEffect)
bool nw::snd::internal::driver::HardwareManager::AppendEffect(AuxBus bus, nn::snd::CTR::FxReverb* pEffect)
{
    if (!m_AuxFadeVolumes[bus].IsFinished()) {
        FinalizeEffect(bus);
    }
    m_AuxFadeVolumes[bus].SetTarget(1.0f, 0);
    const nn::snd::CTR::AuxBusId busId = static_cast<nn::snd::CTR::AuxBusId>(bus);
    nn::snd::CTR::SetAuxReturnVolume(busId, 1.0f);
    return nn::snd::CTR::SetEffect(busId, pEffect);
}

// 0x004CD154 | nintendogs:callseq [tier A]
bool nw::snd::internal::driver::HardwareManager::AppendEffect(AuxBus bus, FxBase* pEffect)
{
    if (!m_AuxFadeVolumes[bus].IsFinished()) {
        FinalizeEffect(bus);
    }
    m_AuxFadeVolumes[bus].SetTarget(1.0f, 0);
    const nn::snd::CTR::AuxBusId busId = static_cast<nn::snd::CTR::AuxBusId>(bus);
    nn::snd::CTR::SetAuxReturnVolume(busId, 1.0f);
    if (m_FxLists[bus].empty()) {
        nn::snd::CTR::RegisterAuxCallback(busId, AuxCallbackFunc, bus);
    }
    m_FxLists[bus].PushBack(pEffect);
    return true;
}

// 0x004CD204 | nintendogs:bytes-fuzzy [tier B]
void nw::snd::internal::driver::HardwareManager::SetOutputMode(OutputMode mode)
{
    if (m_OutputMode == mode) {
        return;
    }
    m_OutputMode = mode;
    switch (mode) {
    case OUTPUT_MODE_MONO:
        nn::snd::CTR::SetSoundOutputMode(nn::snd::CTR::OUTPUT_MODE_MONO);
        break;
    case OUTPUT_MODE_STEREO:
        nn::snd::CTR::SetSoundOutputMode(nn::snd::CTR::OUTPUT_MODE_STEREO);
        break;
    case OUTPUT_MODE_SURROUND:
        nn::snd::CTR::SetSoundOutputMode(nn::snd::CTR::OUTPUT_MODE_SURROUND);
        break;
    }
    PushVoiceSyncCommand(VOICE_SYNC_OUTPUT_MODE);
    for (int i = 0; i < AUX_BUS_COUNT; ++i) {
        ut::LinkList<FxBase, 4>& list = m_FxLists[static_cast<u8>(i)];
        for (ut::internal::LinkListImpl::iterator it = list.begin(); it != list.end(); ++it) {
            ut::LinkList<FxBase, 4>::GetPointerFromNode(it.GetNode())->OnChangeOutputMode();
        }
    }
}

// 0x004CD2D0 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::HardwareManager::FinalizeEffect(AuxBus bus)
{
    const nn::snd::CTR::AuxBusId busId = static_cast<nn::snd::CTR::AuxBusId>(bus);
    nn::snd::CTR::ClearEffect(busId);
    for (int i = 0; i < DSP_EFFECT_TYPE_COUNT; ++i) {
        DspEffect& effect = m_DspEffects[bus][i];
        if (effect.state != DspEffect::STATE_ENABLED) {
            continue;
        }
        switch (i) {
        case DSP_EFFECT_DELAY:
            static_cast<nn::snd::CTR::DspFxDelay*>(effect.pEffect)->Finalize();
            effect.state = DspEffect::STATE_FINALIZING;
            break;
        case DSP_EFFECT_REVERB:
            static_cast<nn::snd::CTR::DspFxReverb*>(effect.pEffect)->Finalize();
            effect.state = DspEffect::STATE_FINALIZING;
            break;
        }
    }

    ut::LinkList<FxBase, 4>& list = m_FxLists[bus];
    if (list.empty()) {
        return;
    }
    nn::snd::CTR::ClearAuxCallback(busId);
    m_AuxCallbackTicks[bus] = nn::os::Tick(0);
    for (ut::internal::LinkListImpl::iterator it = list.begin(); it != list.end(); ++it) {
        ut::LinkList<FxBase, 4>::GetPointerFromNode(it.GetNode())->Finalize();
    }
    list.erase(list.begin(), list.end());
}

// 0x004CD3C0 | fefates:bytes [tier B]
void nw::snd::internal::driver::HardwareManager::AuxCallbackFunc(nn::snd::CTR::AuxBusData* pData, s32 sampleLength, uptr arg)
{
    const s64 startTick = nn::svc::GetSystemTick();
    const AuxBus bus = static_cast<AuxBus>(arg);
    HardwareManager& manager = GetInstance();
    const OutputMode outputMode = manager.m_OutputMode;
    const int channelCount = outputMode == OUTPUT_MODE_SURROUND ? 4 : 2;
    ut::LinkList<FxBase, 4>& list = manager.m_FxLists[bus];
    for (ut::internal::LinkListImpl::iterator it = list.begin(); it != list.end(); ++it) {
        ut::LinkList<FxBase, 4>::GetPointerFromNode(it.GetNode())
            ->UpdateBuffer(channelCount, pData, sampleLength, SAMPLE_FORMAT_PCM_S32, AUX_SAMPLE_RATE, outputMode);
    }
    manager.m_AuxCallbackTicks[bus] = nn::os::Tick(nn::svc::GetSystemTick() - startTick);
}

// 0x004CD480 | nintendogs:bytes-fuzzy [tier B]
void nw::snd::internal::driver::HardwareManager::SetMasterVolume(f32 volume, int fadeTimes)
{
    if (volume < 0.0f) {
        volume = 0.0f;
    }
    m_MasterVolume.SetTarget(volume, GetFadeFrames(fadeTimes));
    if (fadeTimes == 0) {
        PushVoiceSyncCommand(VOICE_SYNC_MASTER_VOLUME);
    }
}

// 0x004CD4FC (name is ours)
void nw::snd::internal::driver::HardwareManager::SetBiquadFilterCallback(int type, BiquadFilterCallback* pCallback)
{
    if (type > 0) {
        m_BiquadFilterCallbacks[type] = pCallback;
    }
}

// 0x004CD50C | fefates:bytes [tier B]
void nw::snd::internal::driver::HardwareManager::Update()
{
    for (int i = 0; i < AUX_BUS_COUNT; ++i) {
        MoveValue<f32, int>& userVolume = m_AuxUserVolumes[i];
        MoveValue<f32, int>& fadeVolume = m_AuxFadeVolumes[i];
        bool isUpdated = false;
        if (!userVolume.IsFinished()) {
            userVolume.Update();
            isUpdated = true;
        }
        if (!fadeVolume.IsFinished()) {
            fadeVolume.Update();
            if (fadeVolume.IsFinished()) {
                FinalizeEffect(static_cast<AuxBus>(i));
            }
            isUpdated = true;
        }
        if (isUpdated) {
            f32 volume = 1.0f;
            volume *= Clamp01(userVolume.GetValue());
            volume *= Clamp01(fadeVolume.GetValue());
            nn::snd::CTR::SetAuxReturnVolume(static_cast<nn::snd::CTR::AuxBusId>(i), volume);
        }

        // the DSP effects that finished
        for (int j = 0; j < DSP_EFFECT_TYPE_COUNT; ++j) {
            DspEffect& effect = m_DspEffects[i][j];
            if (effect.pEffect == nullptr || effect.state != DspEffect::STATE_FINALIZING) {
                continue;
            }
            bool isInUse;
            switch (j) {
            case DSP_EFFECT_DELAY:
                isInUse = static_cast<nn::snd::CTR::DspFxDelay*>(effect.pEffect)->IsBufferInUse();
                break;
            case DSP_EFFECT_REVERB:
                isInUse = static_cast<nn::snd::CTR::DspFxReverb*>(effect.pEffect)->IsBufferInUse();
                break;
            default:
                continue;
            }
            if (!isInUse) {
                effect.pEffect = nullptr;
                effect.state = DspEffect::STATE_NONE;
            }
        }
    }

    if (!m_MasterVolume.IsFinished()) {
        m_MasterVolume.Update();
        VoiceManager::GetInstance().UpdateAllVoicesSync(VOICE_SYNC_MASTER_VOLUME);
    }
}

// 0x004CD710 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::driver::HardwareManager::Finalize()
{
    if (!m_IsInitialized) {
        return;
    }
    for (int i = 0; i < AUX_BUS_COUNT; ++i) {
        FinalizeEffect(static_cast<AuxBus>(i));
        nn::snd::CTR::RegisterAuxCallback(static_cast<nn::snd::CTR::AuxBusId>(i), m_OldAuxCallbacks[i], m_OldAuxCallbackArgs[i]);
        m_OldAuxCallbacks[i] = nullptr;
        m_OldAuxCallbackArgs[i] = 0;
    }
    m_IsInitialized = false;
}

// 0x004CD768 | fefates:bytes [tier B]
nw::snd::internal::driver::HardwareManager::HardwareManager() : m_IsInitialized(false), m_OutputMode(OUTPUT_MODE_STEREO), m_InterpolationType(2)
{
    m_MasterVolume.InitValue(1.0f);
    for (int i = 0; i < AUX_BUS_COUNT; ++i) {
        m_AuxFadeVolumes[i].InitValue(1.0f);
        m_AuxUserVolumes[i].InitValue(1.0f);
        m_OldAuxCallbacks[i] = nullptr;
        m_OldAuxCallbackArgs[i] = 0;
        m_AuxCallbackTicks[i] = nn::os::Tick(0);
    }
}

// 0x00741458 | fefates:bytes [tier B]
f32 nw::snd::internal::driver::HardwareManager::GetOutputVolume() const
{
    return m_MasterVolume.GetValue();
}

// 0x00741494 (name is ours)
bool nw::snd::internal::driver::HardwareManager::IsEffectFinalized(AuxBus bus) const
{
    return m_FxLists[bus].size() == 0 && m_DspEffects[bus][DSP_EFFECT_DELAY].state == DspEffect::STATE_NONE &&
           m_DspEffects[bus][DSP_EFFECT_REVERB].state == DspEffect::STATE_NONE;
}

} // namespace driver
} // namespace internal
} // namespace snd
namespace ut {
// (the copy for the array of the effect lists)
// 0x007E5DC8 (name is ours)
template nw::ut::LinkList<nw::snd::FxBase, 4l>::LinkList();
} // namespace ut
} // namespace nw
