#include "nn/snd/CTR/snd_MasterManagerImpl.h"
#include "nn/dsp/CTR/CTR_Api.h"
#include "nn/snd/CTR/snd_Dspsnd.h"

namespace nn {
namespace snd {
namespace CTR {
// 0x00AF62DC (name is ours)
MasterManagerImpl g_MasterManagerImpl;

// 0x00462E3C | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::MasterManagerImpl::Initialize()
{
    if (!m_IsInitialized) {
        m_Lock.Initialize();
        m_IsInitialized = true;
    }
}

// 0x00462E64 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::MasterManagerImpl::AuxUserCallback(AuxBusId bus, uptr buffer)
{
    if (!m_IsInitialized) {
        return;
    }
    nn::os::CriticalSection::ScopedLock lock(m_Lock);
    if (m_AuxCallback[bus] != NULL) {
        // four channels of one frame
        AuxBusData data;
        data.m_FrontLeft = reinterpret_cast<s32*>(buffer);
        data.m_FrontRight = reinterpret_cast<s32*>(buffer + 640);
        data.m_RearLeft = reinterpret_cast<s32*>(buffer + 1280);
        data.m_RearRight = reinterpret_cast<s32*>(buffer + 1920);
        m_AuxCallback[bus](&data, Dspsnd::FRAME_SAMPLES, m_AuxCallbackArg[bus]);
    }
}

// 0x00462EE4 | nintendogs:callgraph [confirmed by fefates] [tier A]
void nn::snd::CTR::MasterManagerImpl::InitializeParam()
{
    SetMasterVolume(1.0f);
    if (m_IsInitialized) {
        m_Volume8 = 1.0f;
        g_Dspsnd.SetMasterVolume(m_MasterVolume * 1.0f);
    }
    SetAuxReturnVolume(AUX_BUS_0, 1.0f);
    SetAuxReturnVolume(AUX_BUS_1, 1.0f);
    RegisterAuxCallback(AUX_BUS_0, NULL, 0);
    RegisterAuxCallback(AUX_BUS_1, NULL, 0);
    m_AuxFrontBypass[AUX_BUS_0] = false;
    g_Dspsnd.SetAuxFrontBypass(AUX_BUS_0, false);
    m_AuxFrontBypass[AUX_BUS_1] = false;
    g_Dspsnd.SetAuxFrontBypass(AUX_BUS_1, false);
    m_RearRatio = 0x8000;
    g_Dspsnd.SetRearRatio(0x8000);
    m_SurroundDepth = 0x7FFF;
    g_Dspsnd.SetSurroundDepth(0x7FFF);
    nn::dsp::CTR::LockComponent();
    if (nn::dsp::CTR::IsComponentLoaded()) {
        m_ClippingMode = 1;
        g_Dspsnd.SetClippingMode(static_cast<ClippingMode>(1));
    }
    nn::dsp::CTR::UnlockComponent();
    m_IsFxEnabled[AUX_BUS_0] = false;
    m_IsFxEnabled[AUX_BUS_1] = false;
    m_OutputBufferCount = 2;
    g_Dspsnd.SetOutputBufferCount(2);
    m_SyncMode = 0;
    g_Dspsnd.SetSyncMode(static_cast<SyncMode>(0));
}

// 0x004630CC | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void nn::snd::CTR::MasterManagerImpl::SetMasterVolume(float volume)
{
    if (m_IsInitialized) {
        m_MasterVolume = volume;
        g_Dspsnd.SetMasterVolume(volume * m_Volume8);
    }
}

// 0x004630F4 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::MasterManagerImpl::ForceUpdateParams()
{
    g_Dspsnd.SetMasterVolume(m_MasterVolume * m_Volume8);
    for (s32 i = 0; i < AUX_BUS_COUNT; i++) {
        const AuxBusId bus = static_cast<AuxBusId>(i);
        g_Dspsnd.SetAuxReturnVolume(bus, m_AuxReturnVolume[bus]);
        g_Dspsnd.EnableAuxBus(bus, m_AuxCallback[bus] != NULL || m_IsFxEnabled[bus]);
        g_Dspsnd.SetAuxFrontBypass(bus, m_AuxFrontBypass[bus]);
    }
    g_Dspsnd.SetSoundOutputMode(static_cast<OutputMode>(m_OutputMode));
    g_Dspsnd.SetClippingMode(static_cast<ClippingMode>(m_ClippingMode));
    g_Dspsnd.SetSurroundDepth(m_SurroundDepth);
    g_Dspsnd.SetSurroundSpeakerPosition(static_cast<SurroundSpeakerPosition>(m_SurroundSpeakerPosition));
    g_Dspsnd.SetRearRatio(m_RearRatio);
    g_Dspsnd.SetOutputBufferCount(m_OutputBufferCount);
    m_Unknown2C = 0;
    g_Dspsnd.SetSyncMode(static_cast<SyncMode>(m_SyncMode));
}

// 0x004631E0 (name is ours)
void nn::snd::CTR::MasterManagerImpl::SetAuxFrontBypass(AuxBusId bus, bool isBypass)
{
    m_AuxFrontBypass[bus] = isBypass;
    g_Dspsnd.SetAuxFrontBypass(bus, isBypass);
}

// 0x004631F4 (name is ours)
void nn::snd::CTR::MasterManagerImpl::SetAuxReturnVolume(AuxBusId bus, float volume)
{
    if (m_IsInitialized) {
        m_AuxReturnVolume[bus] = volume;
        g_Dspsnd.SetAuxReturnVolume(bus, volume);
    }
}

// 0x00463218 (name is ours)
bool nn::snd::CTR::MasterManagerImpl::SetSoundOutputMode(OutputMode mode)
{
    nn::dsp::CTR::LockComponent();
    if (nn::dsp::CTR::IsComponentLoaded()) {
        m_OutputMode = mode;
        const bool result = g_Dspsnd.SetSoundOutputMode(mode);
        nn::dsp::CTR::UnlockComponent();
        return result;
    }
    nn::dsp::CTR::UnlockComponent();
    return false;
}

// 0x00463268 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::MasterManagerImpl::RegisterAuxCallback(AuxBusId bus, AuxCallback callback, uptr arg)
{
    nn::os::CriticalSection::ScopedLock lock(m_Lock);
    m_AuxCallback[bus] = callback;
    m_AuxCallbackArg[bus] = arg;
    if (callback != NULL) {
        g_Dspsnd.EnableAuxBus(bus, true);
    } else {
        g_Dspsnd.EnableAuxBus(bus, m_IsFxEnabled[bus]);
    }
}

// 0x004632D4 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::MasterManagerImpl::SetOutputBufferCount(int count)
{
    if (count < 2) {
        count = 2;
    } else if (count > 3) {
        count = 3;
    }
    m_OutputBufferCount = count;
    g_Dspsnd.SetOutputBufferCount(count);
}

// 0x004632F8 (name is ours)
bool nn::snd::CTR::MasterManagerImpl::SetIsHeadsetConnected(bool isConnected)
{
    return g_Dspsnd.SetIsHeadsetConnected(isConnected);
}

// 0x00463304 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::MasterManagerImpl::EnableFx(AuxBusId bus, bool isEnabled)
{
    nn::os::CriticalSection::ScopedLock lock(m_Lock);
    m_IsFxEnabled[bus] = isEnabled;
    // the bus stays on for its callback
    if (isEnabled) {
        g_Dspsnd.EnableAuxBus(bus, true);
    } else {
        g_Dspsnd.EnableAuxBus(bus, m_AuxCallback[bus] != NULL);
    }
}

// 0x00463368 | nintendogs:bytes [confirmed by fefates] [tier A]
void nn::snd::CTR::MasterManagerImpl::Finalize()
{
    if (m_IsInitialized) {
        m_Lock.Finalize();
        m_IsInitialized = false;
    }
}

// 0x00463388 (name is ours)
nn::snd::CTR::MasterManagerImpl::~MasterManagerImpl()
{
}

} // namespace CTR
} // namespace snd
} // namespace nn
