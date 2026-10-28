#include "nw/snd/internal/driver/snd_WaveSoundPlayer.h"

#include "nw/snd/internal/driver/snd_DisposeCallbackManager.h"
#include "nw/snd/internal/driver/snd_Voice.h"
#include "nw/snd/internal/snd_PlayerHeap.h"
#include "nw/snd/internal/snd_TaskManager.h"
#include "nw/snd/internal/snd_WaveFileReader.h"
#include "nw/snd/internal/snd_WaveSoundFileReader.h"
#include "nw/snd/snd_SoundPlayer.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
namespace {
// the steps of the parameters of the file (pan 63 to the side, the sends and the biquad value 127)
const f32 PAN_STEP = 1.0f / 63;
const f32 SURROUND_PAN_STEP_HIGH = 1.0f / 64;
const f32 LPF_STEP = 1.0f / 64;
const f32 SEND_STEP = 1.0f / 127;
// the wave type of the loaded data
const s8 LOADED_WAVE_TYPE = 0;
} // namespace

// 0x004CD9EC slot 0x08 | fefates:bytes
void nw::snd::internal::driver::WaveSoundPlayer::Initialize()
{
    BasicSoundPlayer::Initialize();
    m_IsReleasePriorityFix = false;
    m_PanRange = 1.0f;
    m_Priority = CHANNEL_PRIORITY_BASE;
    m_pWaveSoundFile = nullptr;
    m_pWaveFile = nullptr;
    m_StartInfo.index = -1;
    m_WaveSoundInfo.pitch = 1.0f;
    m_WaveSoundInfo.pan = 64;
    m_WaveSoundInfo.surroundPan = 0;
    m_WaveSoundInfo.fxSend[0] = 0;
    m_WaveSoundInfo.fxSend[1] = 0;
    m_WaveSoundInfo.mainSend = 127;
    m_LfoParam.Initialize();
    m_IsChannelStarted = false;
    m_pChannel = nullptr;
    m_WaveType = -1;
    m_ResState = RES_STATE_NONE;
    m_IsRegisteredPlayerCallback = false;
    m_IsLoop = false;
    m_IsInitialized = true;
}

// 0x004CDA6C | fefates:bytes [tier B]
void nw::snd::internal::driver::WaveSoundPlayer::RequestLoad(const StartInfo& startInfo, const WaveSoundLoader::Arg& arg)
{
    m_StartInfo = startInfo;
    m_LoaderArg = arg;
    m_ResState = RES_STATE_LOAD_REQUESTED;
    SoundThread::GetInstance().RegisterPlayerCallback(this);
    m_IsRegisteredPlayerCallback = true;
}

// 0x004CDAB4 | fefates:bytes [tier B]
bool nw::snd::internal::driver::WaveSoundPlayer::StartChannel()
{
    const int priority = m_Priority + CHANNEL_PRIORITY_BASE;
    WaveInfo waveInfo;
    WaveFileReader waveReader(m_pWaveFile, m_WaveType);
    if (!waveReader.ReadWaveInfo(&waveInfo, nullptr)) {
        return false;
    }

    u32 startOffset = 0;
    if (m_StartInfo.startOffsetType == START_OFFSET_TYPE_SAMPLE) {
        startOffset = m_StartInfo.startOffset;
    } else if (m_StartInfo.startOffsetType == START_OFFSET_TYPE_MILLISEC) {
        startOffset = static_cast<s64>(m_StartInfo.startOffset) * static_cast<s32>(waveInfo.sampleRate) / 1000;
    }
    if (waveInfo.loopEndFrame < startOffset) {
        return false;
    }

    const int channelCount = waveInfo.channelCount > 2 ? 2 : waveInfo.channelCount;
    Channel* pChannel = Channel::AllocChannel(channelCount, priority, ChannelCallbackFunc, reinterpret_cast<u32>(this));
    if (pChannel == nullptr) {
        return false;
    }
    WaveSoundFileReader reader(m_pWaveSoundFile);
    if (!reader.ReadWaveSoundInfo(&m_WaveSoundInfo, m_StartInfo.index)) {
        return false;
    }
    pChannel->m_CurveAdshr.SetAttack(m_WaveSoundInfo.adshrCurve.attack);
    pChannel->m_CurveAdshr.SetHold(m_WaveSoundInfo.adshrCurve.hold);
    pChannel->m_CurveAdshr.SetDecay(m_WaveSoundInfo.adshrCurve.decay);
    pChannel->m_CurveAdshr.SetSustain(m_WaveSoundInfo.adshrCurve.sustain);
    pChannel->m_CurveAdshr.SetRelease(m_WaveSoundInfo.adshrCurve.release);
    pChannel->m_IsReleasePriorityFix = m_IsReleasePriorityFix;
    pChannel->m_pVoice->SetFrontBypass(m_IsFrontBypass);
    pChannel->Start(waveInfo, -1, startOffset);
    m_pChannel = pChannel;
    m_IsChannelStarted = true;
    return true;
}

// 0x004CDC10 | fefates:bytes [tier B]
void nw::snd::internal::driver::WaveSoundPlayer::UpdateChannel()
{
    if (m_pChannel == nullptr) {
        return;
    }
    const f32 volume = m_Volume;
    const f32 pitch = m_WaveSoundInfo.pitch * m_Pitch;

    f32 pan = 0.0f;
    const u8 filePan = m_WaveSoundInfo.pan;
    if (filePan <= 1) {
        pan += static_cast<int>(filePan - 63) * PAN_STEP;
    } else {
        pan += static_cast<int>(filePan - 64) * PAN_STEP;
    }
    pan = m_Pan + m_PanRange * pan;

    f32 surroundPan = 0.0f;
    const u8 fileSurroundPan = static_cast<u8>(m_WaveSoundInfo.surroundPan);
    if (fileSurroundPan <= 63) {
        surroundPan += fileSurroundPan * PAN_STEP;
    } else {
        surroundPan += static_cast<u32>(fileSurroundPan + 1) * SURROUND_PAN_STEP_HIGH;
    }
    surroundPan = m_SurroundPan + surroundPan;

    const f32 lpfFreq = m_LpfFreq + static_cast<int>(m_WaveSoundInfo.lpfFreq - 64) * LPF_STEP;

    int biquadType = m_WaveSoundInfo.biquadType;
    f32 biquadValue = m_WaveSoundInfo.biquadValue * SEND_STEP;
    if (static_cast<s8>(m_BiquadType) != -1) {
        biquadType = static_cast<s8>(m_BiquadType);
        biquadValue = m_BiquadValue;
    }

    const f32 mainSend = m_MainSend + (m_WaveSoundInfo.mainSend * SEND_STEP - 1.0f);
    f32 fxSends[2];
    for (int i = 0; i < 2; ++i) {
        fxSends[i] = 0.0f;
        fxSends[i] += m_WaveSoundInfo.fxSend[i] * SEND_STEP;
        fxSends[i] += GetFxSend(static_cast<AuxBus>(i));
    }

    m_pChannel->m_pVoice->SetPanMode(m_PanMode);
    m_pChannel->m_pVoice->SetPanCurve(m_PanCurve);
    m_pChannel->m_UserVolume = volume;
    m_pChannel->m_UserPitchRatio = pitch;
    m_pChannel->m_UserPan = pan;
    m_pChannel->m_UserLpfFreq = lpfFreq;
    m_pChannel->SetBiquadFilter(biquadType, biquadValue);
    m_pChannel->m_MainSend = mainSend;
    m_pChannel->m_FxSends[0] = fxSends[0];
    m_pChannel->m_FxSends[1] = fxSends[1];
    m_pChannel->m_UserSurroundPan = surroundPan;
    m_pChannel->m_Lfos[0].m_Param = m_LfoParam;
    m_pChannel->SetLoopFlag(m_IsLoop);
}

// 0x004CDE44 slot 0x1C
// 0x007788A4 (thunk)
void nw::snd::internal::driver::WaveSoundPlayer::InvalidateData(const void* pStart, const void* pEnd)
{
    if (!m_IsActive) {
        return;
    }
    if (pStart <= m_pWaveSoundFile && m_pWaveSoundFile <= pEnd) {
        WaveSoundPlayer::Stop();
    }
}

// 0x004CDEA4 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::WaveSoundPlayer::ChannelCallbackFunc(Channel* pChannel, Channel::ChannelCallbackStatus status, u32 userData)
{
    WaveSoundPlayer* pPlayer = reinterpret_cast<WaveSoundPlayer*>(userData);
    if (status == Channel::CALLBACK_STATUS_FINISH) {
        Channel::FreeChannel(pChannel);
    }
    pPlayer->m_pChannel = nullptr;
}

// 0x004CDEC0 slot 0x24
// 0x00778B20 (thunk)
void nw::snd::internal::driver::WaveSoundPlayer::OnShutdown()
{
    Stop();
}

// 0x004CDECC slot 0x14
void nw::snd::internal::driver::WaveSoundPlayer::Stop()
{
    m_IsFinished = true;
    if (m_IsRegisteredPlayerCallback) {
        SoundThread::GetInstance().UnregisterPlayerCallback(this);
        m_IsRegisteredPlayerCallback = false;
    }
    if (m_IsStarted) {
        m_IsStarted = false;
    }
}

// 0x004CDF0C slot 0x18 | fefates:bytes
void nw::snd::internal::driver::WaveSoundPlayer::Pause(bool isPause)
{
    m_IsPause = isPause;
    if (m_pChannel != nullptr) {
        m_pChannel->Pause(isPause);
    }
}

// 0x004CDF48 slot 0x10
void nw::snd::internal::driver::WaveSoundPlayer::Start()
{
    m_IsStarted = true;
}

// 0x004CDF54 slot 0x20
// 0x00778B2C (thunk)
void nw::snd::internal::driver::WaveSoundPlayer::OnUpdateFrame()
{
    Update();
}

// 0x004CDF58 | fefates:bytes [tier B]
void nw::snd::internal::driver::WaveSoundPlayer::Update()
{
    switch (m_ResState) {
    case RES_STATE_LOAD_REQUESTED: {
        if (m_pLoaderManager == nullptr) {
            return;
        }
        WaveSoundLoader* pLoader = m_pLoaderManager->Alloc();
        if (pLoader == nullptr) {
            return;
        }
        m_pLoader = pLoader;
        m_ResState = RES_STATE_LOADING;
        pLoader->Initialize(m_LoaderArg);
    }
        // fall through
    case RES_STATE_LOADING: {
        WaveSoundLoader::DataLoadTask& task = m_pLoader->m_Task;
        if (task.m_pPlayerHeap == nullptr) {
            task.m_pPlayerHeap = task.m_Arg.soundPlayer->detail_AllocPlayerHeap();
            if (task.m_pPlayerHeap == nullptr) {
                return;
            }
            task.m_pPlayerHeap->Clear();
        }
        if (task.m_Status == Task::STATUS_FREE) {
            TaskManager::GetInstance().AppendTask(&task, TaskManager::PRIORITY_MIDDLE);
            return;
        }
        if (task.m_Status != Task::STATUS_DONE && task.m_Status != Task::STATUS_CANCEL) {
            return;
        }
        if (!m_pLoader->m_Task.m_IsSucceeded) {
            WaveSoundPlayer::Stop();
            return;
        }

        // prepared from the player heap
        PrepareArg arg;
        arg.waveSoundFile = m_pLoader->m_Task.m_pWaveSoundFile;
        arg.waveFile = m_pLoader->m_Task.m_pWaveFile;
        arg.waveType = LOADED_WAVE_TYPE;
        if (m_IsActive) {
            WaveSoundPlayer::Stop();
        }
        m_ResState = RES_STATE_PREPARED;
        m_WaveType = arg.waveType;
        m_IsActive = true;
        m_pWaveSoundFile = arg.waveSoundFile;
        m_pWaveFile = arg.waveFile;
        DisposeCallbackManager::GetInstance().RegisterDisposeCallback(this);
        break;
    }
    default:
        break;
    }

    if (!m_IsActive || !m_IsStarted) {
        return;
    }
    if (!m_IsPause) {
        if (m_IsChannelStarted) {
            if (m_pChannel == nullptr) {
                WaveSoundPlayer::Stop();
                return;
            }
        } else if (!StartChannel()) {
            WaveSoundPlayer::Stop();
            return;
        }
    }
    UpdateChannel();
}

// 0x004CE1D4 | fefates:bytes [tier B]
void nw::snd::internal::driver::WaveSoundPlayer::Prepare(const StartInfo& startInfo, const PrepareArg& arg)
{
    if (m_IsActive) {
        WaveSoundPlayer::Stop();
    }
    m_StartInfo = startInfo;
    m_pWaveSoundFile = arg.waveSoundFile;
    m_pWaveFile = arg.waveFile;
    m_WaveType = arg.waveType;
    m_ResState = RES_STATE_PREPARED;
    m_IsActive = true;
    DisposeCallbackManager::GetInstance().RegisterDisposeCallback(this);
    SoundThread::GetInstance().RegisterPlayerCallback(this);
    m_IsRegisteredPlayerCallback = true;
}

// 0x004CE290 slot 0x0C | fefates:bytes
void nw::snd::internal::driver::WaveSoundPlayer::Finalize()
{
    WaveSoundPlayer::Stop();
    if (m_IsActive) {
        DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(this);
        if (m_pChannel != nullptr) {
            if (m_pChannel->m_ActiveFlag) {
                UpdateChannel();
                m_pChannel->Release();
            }
            if (m_pChannel != nullptr) {
                Channel::FreeChannel(m_pChannel);
                m_pChannel = nullptr;
            }
        }
        m_IsActive = false;
    }
    if (m_IsInitialized) {
        BasicSoundPlayer::Finalize();
        m_IsInitialized = false;
    }
    if (m_pLoader != nullptr) {
        m_pLoaderManager->Free(m_pLoader);
        m_pLoader = nullptr;
    }
}

// 0x004CE3B0 | fefates:bytes [tier B]
nw::snd::internal::driver::WaveSoundPlayer::WaveSoundPlayer()
    : m_IsInitialized(false), m_IsRegisteredPlayerCallback(false), m_IsLoop(false), m_pLoaderManager(nullptr), m_pLoader(nullptr)
{
}

// 0x004CE488 slot 0x00 | fefates:bytes
// 0x004CE444 (deleting dtor)
// 0x00778948 (thunk 68)
// 0x00778904 (deleting thunk 68)
// 0x00778B78 (thunk 80)
// 0x00778B34 (deleting thunk 80)
nw::snd::internal::driver::WaveSoundPlayer::~WaveSoundPlayer()
{
    WaveSoundPlayer::Finalize();
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
