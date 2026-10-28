#include "nw/snd/internal/snd_WaveSound.h"

#include "nw/snd/internal/snd_DriverCommandManager.h"
#include "nw/snd/internal/snd_DriverCommandPlayer.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0097F6AC
const TypeInfo nw::snd::internal::WaveSound::s_TypeInfo(&BasicSound::s_TypeInfo);

// sets s_TypeInfo
// 0x00797CE0 (static initializer)

// 0x004D4BEC slot 0x0C
void nw::snd::internal::WaveSound::Initialize()
{
    BasicSound::Initialize();
    m_pTempSpecialHandle = nullptr;
    m_InitializeFlag = true;
    m_PreparedFlag = false;
}

// 0x004D4C10 slot 0x2C
void nw::snd::internal::WaveSound::OnUpdateParam()
{
    // empty (in the original too)
}

// 0x004D4C14 | fefates:bytes [tier B]
void nw::snd::internal::WaveSound::RegisterDataLoadTask(const driver::WaveSoundLoader::LoadInfo& loadInfo,
                                                       const driver::WaveSoundPlayer::StartInfo& startInfo)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandWaveSoundLoad* pCommand = manager.AllocCommand<DriverCommandWaveSoundLoad>();
    pCommand->id = DriverCommand::ID_WAVESOUND_LOAD;
    pCommand->player = &m_WaveSoundPlayer;
    pCommand->startInfo = startInfo;
    pCommand->arg.soundDataManager = loadInfo.soundDataManager;
    pCommand->arg.soundArchive = loadInfo.soundArchive;
    pCommand->arg.soundPlayer = loadInfo.soundPlayer;
    pCommand->arg.loadInfoWsd = *loadInfo.loadInfoWsd;
    pCommand->arg.index = startInfo.index;
    manager.PushCommand(pCommand);
}

// 0x004D4C94 | fefates:bytes [tier B]
void nw::snd::internal::WaveSound::InitializeChannelParam(int priority, bool isReleasePriorityFix)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandWaveSoundChannelParam* pCommand = manager.AllocCommand<DriverCommandWaveSoundChannelParam>();
    pCommand->id = DriverCommand::ID_WAVESOUND_CHANNEL_PARAM;
    pCommand->player = &m_WaveSoundPlayer;
    pCommand->priority = priority;
    pCommand->isReleasePriorityFix = isReleasePriorityFix;
    manager.PushCommand(pCommand);
}

// 0x004D4CE4 slot 0x24
void nw::snd::internal::WaveSound::OnUpdatePlayerPriority()
{
    m_pManager->UpdatePriority(this, CalcCurrentPlayerPriority());
}

// 0x004D4D70 slot 0x1C
void nw::snd::internal::WaveSound::DetachTempSpecialHandle()
{
    // empty (in the original too)
}

// 0x004D4D74 slot 0x20
driver::BasicSoundPlayer* nw::snd::internal::WaveSound::GetBasicSoundPlayerHandle()
{
    return &m_WaveSoundPlayer;
}

// 0x004D4D7C slot 0x18
bool nw::snd::internal::WaveSound::IsAttachedTempSpecialHandle()
{
    return m_pTempSpecialHandle != nullptr;
}

// 0x004D4D8C | fefates:bytes [tier B]
void nw::snd::internal::WaveSound::Prepare(const void* pWaveSoundFile, const void* pWaveFile,
                                          const driver::WaveSoundPlayer::StartInfo& startInfo, s8 waveType)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandWaveSoundPrepare* pCommand = manager.AllocCommand<DriverCommandWaveSoundPrepare>();
    pCommand->id = DriverCommand::ID_WAVESOUND_PREPARE;
    pCommand->player = &m_WaveSoundPlayer;
    pCommand->startInfo = startInfo;
    pCommand->arg.waveSoundFile = pWaveSoundFile;
    pCommand->arg.waveFile = pWaveFile;
    pCommand->arg.waveType = waveType;
    manager.PushCommand(pCommand);
    m_PreparedFlag = true;
}

// 0x004D4DF4 slot 0x10
void nw::snd::internal::WaveSound::Finalize()
{
    if (!m_InitializeFlag) {
        return;
    }
    m_InitializeFlag = false;
    m_PreparedFlag = false;
    BasicSound::Finalize();
    m_pManager->Free(this);
}

// 0x004D4E54 | fefates:bytes [tier B]
nw::snd::internal::WaveSound::WaveSound(WaveSoundInstanceManager& manager)
    : m_pManager(&manager), m_InitializeFlag(false), m_PreparedFlag(false)
{
}

// 0x004D4EBC slot 0x04 | fefates:bytes
// 0x004D4E8C (deleting dtor)
nw::snd::internal::WaveSound::~WaveSound()
{
    // (only the members and the base are destroyed)
}

// 0x007431D8 slot 0x14 | fefates:bytes
bool nw::snd::internal::WaveSound::IsPrepared() const
{
    if (m_PreparedFlag) {
        return true;
    }
    return m_IsPlayerAvailable && m_WaveSoundPlayer.m_ResState >= driver::WaveSoundPlayer::RES_STATE_PREPARED;
}

// 0x0074320C slot 0x00 (name after nw::gfx)
const TypeInfo* nw::snd::internal::WaveSound::GetRuntimeTypeInfo() const
{
    return &s_TypeInfo;
}

} // namespace internal
} // namespace snd
} // namespace nw
