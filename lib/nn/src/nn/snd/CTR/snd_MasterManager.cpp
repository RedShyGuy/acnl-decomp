#include "nn/snd/CTR/snd_MasterManager.h"
#include "nn/cfg/CTR/CTR_Api.h"
#include "nn/cfg/CTR/detail/detail_Api.h"
#include "nn/snd/CTR/CTR_Api.h"
#include "nn/snd/CTR/snd_Dspsnd.h"
#include "nn/snd/CTR/snd_FxDelay.h"
#include "nn/snd/CTR/snd_FxReverb.h"
#include "nn/snd/CTR/snd_MasterManagerImpl.h"

namespace nn {
namespace snd {
namespace CTR {
namespace {
// the sound output mode of the system settings
const u32 CONFIG_SOUND_OUTPUT_MODE = 0x70001;

// the cycles the DSP needs for the master output by its mode (names are ours)
const u32 OUTPUT_CYCLES_MONO = 0xDF0C;
const u32 OUTPUT_CYCLES_STEREO = 0xD930;
const u32 OUTPUT_CYCLES_SURROUND_HEADPHONE = 0x30BB0;
const u32 OUTPUT_CYCLES_SURROUND_SPEAKER = 0x37140;
const u32 OUTPUT_CYCLES_DEFAULT = 0xCD78;
const u32 CLIPPING_CYCLES_0 = 1500;
const u32 CLIPPING_CYCLES_1 = 10000;
} // namespace

// 0x00AEB834 (name is ours)
MasterManager g_MasterManager;

// 0x00461A48 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::MasterManager::Initialize()
{
    if (m_IsInitialized) {
        return;
    }
    m_IsInitialized = true;
    g_MasterManagerImpl.Initialize();
    m_ClippingMode = 1;
    m_MasterVolume = 1.0f;
    m_Volume8 = 1.0f;
    m_AuxReturnVolume[AUX_BUS_0] = 1.0f;
    m_AuxReturnVolume[AUX_BUS_1] = 1.0f;
    m_AuxCallback[AUX_BUS_0] = NULL;
    m_AuxCallback[AUX_BUS_1] = NULL;
    m_AuxCallbackArg[AUX_BUS_0] = 0;
    m_AuxCallbackArg[AUX_BUS_1] = 0;
    m_AuxFrontBypass[AUX_BUS_0] = false;
    m_AuxFrontBypass[AUX_BUS_1] = false;
    m_Volume2C = 1.0f;
    m_Volume28 = 1.0f;
    g_MasterManagerImpl.InitializeParam();

    // the output mode of the system settings
    nn::cfg::CTR::Initialize();
    u8 mode;
    nn::Result result = nn::cfg::CTR::detail::GetConfig(&mode, sizeof(mode), CONFIG_SOUND_OUTPUT_MODE);
    nn::cfg::CTR::Finalize();
    OutputMode outputMode = OUTPUT_MODE_STEREO;
    if (result.IsSuccess()) {
        if (mode == 0) {
            outputMode = OUTPUT_MODE_MONO;
        } else if (mode == 1) {
        } else if (mode == 2) {
            outputMode = OUTPUT_MODE_SURROUND;
        }
    }
    m_OutputMode = outputMode;
    g_MasterManagerImpl.SetSoundOutputMode(outputMode);
    m_DroppedFrameCount = 0;
    m_Effect[AUX_BUS_0].m_Delay = NULL;
    m_Effect[AUX_BUS_0].m_Reverb = NULL;
    m_Effect[AUX_BUS_1].m_Delay = NULL;
    m_Effect[AUX_BUS_1].m_Reverb = NULL;
    m_Lock.Initialize();
}

// 0x00461B28 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::MasterManager::ClearEffect(AuxBusId bus)
{
    nn::os::CriticalSection::ScopedLock lock(m_Lock);
    if (m_Effect[bus].m_Delay != NULL) {
        m_Effect[bus].m_Delay->Finalize();
    }
    if (m_Effect[bus].m_Reverb != NULL) {
        m_Effect[bus].m_Reverb->Finalize();
    }
    m_Effect[bus].m_Delay = NULL;
    m_Effect[bus].m_Reverb = NULL;
    g_MasterManagerImpl.EnableFx(bus, false);
}

// 0x00461B88 | nintendogs:bytes [confirmed by fefates] [tier A]
u32 nn::snd::CTR::MasterManager::GetDspCycles()
{
    u32 cycles = OUTPUT_CYCLES_DEFAULT;
    switch (m_OutputMode) {
    case OUTPUT_MODE_MONO:
        cycles = OUTPUT_CYCLES_MONO;
        break;
    case OUTPUT_MODE_STEREO:
        cycles = OUTPUT_CYCLES_STEREO;
        break;
    case OUTPUT_MODE_SURROUND:
        cycles = GetHeadphoneStatus() ? OUTPUT_CYCLES_SURROUND_HEADPHONE : OUTPUT_CYCLES_SURROUND_SPEAKER;
        break;
    }
    if (m_ClippingMode == 0) {
        cycles += CLIPPING_CYCLES_0;
    } else if (m_ClippingMode == 1) {
        cycles += CLIPPING_CYCLES_1;
    }
    return cycles;
}

// 0x00461C08 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::MasterManager::ExecuteEffect(AuxBusId bus, uptr buffer)
{
    nn::os::CriticalSection::ScopedLock lock(m_Lock);
    // four channels of one frame
    AuxBusData data;
    data.m_FrontLeft = reinterpret_cast<s32*>(buffer);
    data.m_FrontRight = reinterpret_cast<s32*>(buffer + 640);
    data.m_RearLeft = reinterpret_cast<s32*>(buffer + 1280);
    data.m_RearRight = reinterpret_cast<s32*>(buffer + 1920);
    if (m_Effect[bus].m_Delay != NULL) {
        m_Effect[bus].m_Delay->UpdateBuffer(reinterpret_cast<uptr>(&data));
    } else if (m_Effect[bus].m_Reverb != NULL) {
        m_Effect[bus].m_Reverb->UpdateBuffer(reinterpret_cast<uptr>(&data));
    }
}

// 0x00461C7C | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::MasterManager::GetAuxCallback(AuxBusId bus, AuxCallback* callback, uptr* arg)
{
    *callback = m_AuxCallback[bus];
    *arg = m_AuxCallbackArg[bus];
}

// 0x00461C94 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::MasterManager::AuxUserCallback(AuxBusId bus, uptr buffer)
{
    if (m_IsInitialized) {
        g_MasterManagerImpl.AuxUserCallback(bus, buffer);
    }
}

// 0x00461CB0 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::MasterManager::SetMasterVolume(float volume)
{
    if (m_IsInitialized) {
        m_MasterVolume = volume;
        g_MasterManagerImpl.SetMasterVolume(volume);
    }
}

// 0x00461CD0 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::MasterManager::ClearAuxCallback(AuxBusId bus)
{
    m_AuxCallback[bus] = NULL;
    m_AuxCallbackArg[bus] = 0;
    g_MasterManagerImpl.RegisterAuxCallback(bus, NULL, 0);
}

// 0x00461CF0 (name is ours)
void nn::snd::CTR::MasterManager::SetAuxFrontBypass(AuxBusId bus, bool isBypass)
{
    m_AuxFrontBypass[bus] = isBypass;
    g_MasterManagerImpl.SetAuxFrontBypass(bus, isBypass);
}

// 0x00461D04 (name is ours)
OutputMode nn::snd::CTR::MasterManager::GetSoundOutputMode()
{
    return static_cast<OutputMode>(m_OutputMode);
}

// 0x00461D0C (name is ours)
void nn::snd::CTR::MasterManager::SetAuxReturnVolume(AuxBusId bus, float volume)
{
    if (m_IsInitialized) {
        m_AuxReturnVolume[bus] = volume;
        g_MasterManagerImpl.SetAuxReturnVolume(bus, volume);
    }
}

// 0x00461D30 (name is ours)
bool nn::snd::CTR::MasterManager::SetSoundOutputMode(OutputMode mode)
{
    m_OutputMode = mode;
    return g_MasterManagerImpl.SetSoundOutputMode(mode);
}

// 0x00461D40 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::MasterManager::RegisterAuxCallback(AuxBusId bus, AuxCallback callback, uptr arg)
{
    m_AuxCallback[bus] = callback;
    m_AuxCallbackArg[bus] = arg;
    g_MasterManagerImpl.RegisterAuxCallback(bus, callback, arg);
}

// 0x00461D58 (name is ours)
void nn::snd::CTR::MasterManager::SetOutputBufferCount(int count)
{
    g_MasterManagerImpl.SetOutputBufferCount(count);
}

// 0x00461D64 | nintendogs:callgraph [confirmed by fefates, mk7dlp] [tier A]
bool nn::snd::CTR::MasterManager::SetIsHeadsetConnected(bool isConnected)
{
    m_IsHeadsetConnected = isConnected;
    return g_MasterManagerImpl.SetIsHeadsetConnected(isConnected);
}

// 0x00461D74 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::MasterManager::UpdateDroppedSoundFrameCount()
{
    const s32 count = g_Dspsnd.GetDroppedFrameCount();
    if (count >= 0) {
        m_DroppedFrameCount += count;
    }
}

// 0x00461DA0 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::MasterManager::Finalize()
{
    if (m_IsInitialized) {
        g_MasterManagerImpl.Finalize();
        m_Lock.Finalize();
        m_IsInitialized = false;
    }
}

// 0x00461DD4 | nintendogs:bytes [confirmed by fefates] [tier A]
bool nn::snd::CTR::MasterManager::SetEffect(AuxBusId bus, FxDelay* delay)
{
    if (delay == NULL) {
        return false;
    }
    ClearEffect(bus);
    nn::os::CriticalSection::ScopedLock lock(m_Lock);
    m_Effect[bus].m_Delay = delay;
    delay->Initialize();
    g_MasterManagerImpl.EnableFx(bus, true);
    return true;
}

// 0x00461E78 | nintendogs:bytes [confirmed by fefates] [tier A]
bool nn::snd::CTR::MasterManager::SetEffect(AuxBusId bus, FxReverb* reverb)
{
    if (reverb == NULL) {
        return false;
    }
    ClearEffect(bus);
    nn::os::CriticalSection::ScopedLock lock(m_Lock);
    m_Effect[bus].m_Reverb = reverb;
    reverb->Initialize();
    g_MasterManagerImpl.EnableFx(bus, true);
    return true;
}

// 0x00461F1C (name is ours)
nn::snd::CTR::MasterManager::~MasterManager()
{
}

} // namespace CTR
} // namespace snd
} // namespace nn
