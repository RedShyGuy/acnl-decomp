#include "nw/snd/internal/snd_SequenceSound.h"

#include "nw/snd/internal/snd_DriverCommandManager.h"
#include "nw/snd/internal/snd_DriverCommandPlayer.h"
#include "nw/snd/snd_SequenceSoundHandle.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0097618C
const TypeInfo nw::snd::internal::SequenceSound::s_TypeInfo(&BasicSound::s_TypeInfo);

// sets s_TypeInfo
// 0x0079C368 (static initializer)

namespace {
// (inline) a command for the player of the sound
template <typename TCommand>
TCommand* AllocSequenceCommand(DriverCommandManager& manager, DriverCommand::Id id, driver::SequenceSoundPlayer* pPlayer)
{
    TCommand* pCommand = manager.AllocCommand<TCommand>();
    pCommand->id = id;
    pCommand->player = pPlayer;
    return pCommand;
}
} // namespace

// 0x004C7A68 slot 0x0C
void nw::snd::internal::SequenceSound::Initialize()
{
    BasicSound::Initialize();
    m_pTempSpecialHandle = nullptr;
    m_InitializeFlag = true;
    m_PreparedFlag = false;
}

// 0x004C7A8C (name after SequenceSoundPlayer)
void nw::snd::internal::SequenceSound::SetTrackMute(u32 trackBitFlag, SeqMute mute)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandSequenceTrackMute* pCommand =
        AllocSequenceCommand<DriverCommandSequenceTrackMute>(manager, DriverCommand::ID_SEQTRACK_MUTE, &m_SequenceSoundPlayer);
    pCommand->trackBitFlag = trackBitFlag;
    pCommand->mute = mute;
    manager.PushCommand(pCommand);
}

// 0x004C7AD8 (name after SequenceSoundPlayer)
void nw::snd::internal::SequenceSound::SetTrackMute(u32 trackBitFlag, bool isMute)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandSequenceTrackMute* pCommand =
        AllocSequenceCommand<DriverCommandSequenceTrackMute>(manager, DriverCommand::ID_SEQTRACK_MUTE, &m_SequenceSoundPlayer);
    pCommand->trackBitFlag = trackBitFlag;
    pCommand->mute = isMute ? SEQ_MUTE_STOP : SEQ_MUTE_OFF;
    manager.PushCommand(pCommand);
}

// 0x004C7B34 slot 0x2C
void nw::snd::internal::SequenceSound::OnUpdateParam()
{
    // empty (in the original too)
}

// 0x004C7B38 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::SequenceSound::SetTempoRatio(f32 tempoRatio)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandSequenceSoundTempoRatio* pCommand =
        AllocSequenceCommand<DriverCommandSequenceSoundTempoRatio>(manager, DriverCommand::ID_SEQSOUND_TEMPORATIO, &m_SequenceSoundPlayer);
    pCommand->tempoRatio = tempoRatio;
    manager.PushCommand(pCommand);
}

// 0x004C7B88 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::SequenceSound::WriteVariable(int varNo, s16 value)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandSequenceSoundSetVar* pCommand =
        AllocSequenceCommand<DriverCommandSequenceSoundSetVar>(manager, DriverCommand::ID_SEQSOUND_SETVAR, &m_SequenceSoundPlayer);
    pCommand->varNo = varNo;
    pCommand->value = value;
    manager.PushCommand(pCommand);
}

// 0x004C7BD4 (name after SequenceSoundPlayer)
void nw::snd::internal::SequenceSound::SetTrackVolume(u32 trackBitFlag, f32 volume)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandSequenceTrackParam* pCommand =
        AllocSequenceCommand<DriverCommandSequenceTrackParam>(manager, DriverCommand::ID_SEQTRACK_VOLUME, &m_SequenceSoundPlayer);
    pCommand->trackBitFlag = trackBitFlag;
    pCommand->value = volume;
    manager.PushCommand(pCommand);
}

// 0x004C7C2C (name after SequenceSoundPlayer)
void nw::snd::internal::SequenceSound::SetTrackSilence(u32 trackBitFlag, bool isSilence, int fadeTimes)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandSequenceTrackSilence* pCommand =
        AllocSequenceCommand<DriverCommandSequenceTrackSilence>(manager, DriverCommand::ID_SEQTRACK_SILENCE, &m_SequenceSoundPlayer);
    pCommand->trackBitFlag = trackBitFlag;
    pCommand->isSilence = isSilence;
    pCommand->fadeTimes = fadeTimes;
    manager.PushCommand(pCommand);
}

// 0x004C7C84 (name is ours)
bool nw::snd::internal::SequenceSound::ReadGlobalVariable(int varNo, s16* pValue)
{
    *pValue = driver::SequenceSoundPlayer::GetGlobalVariable(varNo);
    return true;
}

// 0x004C7C9C (name is ours)
void nw::snd::internal::SequenceSound::SetChannelPriority(int priority)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandSequenceSoundChannelPrio* pCommand =
        AllocSequenceCommand<DriverCommandSequenceSoundChannelPrio>(manager, DriverCommand::ID_SEQSOUND_CHANNELPRIO, &m_SequenceSoundPlayer);
    pCommand->priority = priority;
    manager.PushCommand(pCommand);
}

// 0x004C7CE4 (name is ours)
void nw::snd::internal::SequenceSound::WriteTrackVariable(int trackNo, int varNo, s16 value)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandSequenceSoundSetVar* pCommand =
        AllocSequenceCommand<DriverCommandSequenceSoundSetVar>(manager, DriverCommand::ID_SEQSOUND_SETTVAR, &m_SequenceSoundPlayer);
    pCommand->trackNo = trackNo;
    pCommand->varNo = varNo;
    pCommand->value = value;
    manager.PushCommand(pCommand);
}

// 0x004C7D34 (name is ours)
void nw::snd::internal::SequenceSound::WriteGlobalVariable(int varNo, s16 value)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandSequenceSoundSetVar* pCommand = manager.AllocCommand<DriverCommandSequenceSoundSetVar>();
    pCommand->id = DriverCommand::ID_SEQSOUND_SETGVAR;
    pCommand->varNo = varNo;
    pCommand->value = value;
    manager.PushCommand(pCommand);
}

// 0x004C7D78 | fefates:bytes [tier B]
void nw::snd::internal::SequenceSound::RegisterDataLoadTask(const driver::SequenceSoundLoader::LoadInfo& loadInfo,
                                                           const driver::SequenceSoundPlayer::StartInfo& startInfo)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandSequenceSoundLoad* pCommand =
        AllocSequenceCommand<DriverCommandSequenceSoundLoad>(manager, DriverCommand::ID_SEQSOUND_LOAD, &m_SequenceSoundPlayer);
    pCommand->startInfo = startInfo;
    pCommand->arg.soundArchive = loadInfo.soundArchive;
    pCommand->arg.soundDataManager = loadInfo.soundDataManager;
    pCommand->arg.soundPlayer = loadInfo.soundPlayer;
    pCommand->arg.loadInfoSeq = *loadInfo.loadInfoSeq;
    for (int i = 0; i < driver::SequenceSoundLoader::BANK_COUNT; ++i) {
        pCommand->arg.loadInfoBanks[i] = *loadInfo.loadInfoBanks[i];
    }
    manager.PushCommand(pCommand);
}

// 0x004C7E20 slot 0x24
void nw::snd::internal::SequenceSound::OnUpdatePlayerPriority()
{
    m_pManager->UpdatePriority(this, CalcCurrentPlayerPriority());
}

// 0x004C7EAC slot 0x20
nw::snd::internal::driver::BasicSoundPlayer* nw::snd::internal::SequenceSound::GetBasicSoundPlayerHandle()
{
    return &m_SequenceSoundPlayer;
}

// 0x004C7EB4 slot 0x18
bool nw::snd::internal::SequenceSound::IsAttachedTempSpecialHandle()
{
    return m_pTempSpecialHandle != nullptr;
}

// 0x004C7EC4 | fefates:bytes [tier B]
void nw::snd::internal::SequenceSound::Setup(driver::SequenceTrackAllocator* pAllocator, u32 allocTracks, driver::NoteOnCallback* pCallback,
                                            int channelPriority, bool isReleasePriorityFix, SequenceUserprocCallback userproc,
                                            void* pUserprocArg)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandSequenceSoundSetup* pCommand =
        AllocSequenceCommand<DriverCommandSequenceSoundSetup>(manager, DriverCommand::ID_SEQSOUND_SETUP, &m_SequenceSoundPlayer);
    pCommand->arg.trackAllocator = pAllocator;
    pCommand->arg.allocTracks = allocTracks;
    pCommand->arg.callback = pCallback;
    pCommand->channelPriority = channelPriority;
    pCommand->isReleasePriorityFix = isReleasePriorityFix;
    pCommand->userproc = userproc;
    pCommand->userprocArg = pUserprocArg;
    manager.PushCommand(pCommand);
}

// 0x004C7F3C (name after nintendogs)
void nw::snd::internal::SequenceSound::Prepare(const driver::SequenceSoundPlayer::PrepareArg& arg, const StartInfo& startInfo)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandSequenceSoundPrepare* pCommand =
        AllocSequenceCommand<DriverCommandSequenceSoundPrepare>(manager, DriverCommand::ID_SEQSOUND_PREPARE, &m_SequenceSoundPlayer);
    pCommand->arg = arg;
    pCommand->arg.seqOffset = startInfo.seqOffset;
    manager.PushCommand(pCommand);

    // the start offset
    if (startInfo.startOffset > 0) {
        DriverCommandManager& manager2 = DriverCommandManager::GetInstance();
        DriverCommandSequenceSoundSkip* pSkip =
            AllocSequenceCommand<DriverCommandSequenceSoundSkip>(manager2, DriverCommand::ID_SEQSOUND_SKIP, &m_SequenceSoundPlayer);
        pSkip->offsetType = startInfo.startOffsetType;
        pSkip->offset = startInfo.startOffset;
        manager2.PushCommand(pSkip);
    }
    m_PreparedFlag = true;
}

// 0x004C8070 slot 0x10
void nw::snd::internal::SequenceSound::Finalize()
{
    if (!m_InitializeFlag) {
        return;
    }
    m_InitializeFlag = false;
    m_PreparedFlag = false;
    BasicSound::Finalize();
    m_pManager->Free(this);
}

// 0x004C80D0 | fefates:bytes [tier B]
nw::snd::internal::SequenceSound::SequenceSound(SequenceSoundInstanceManager& manager)
    : m_pManager(&manager), m_InitializeFlag(false), m_PreparedFlag(false)
{
}

// 0x004C8138 slot 0x04 | fefates:bytes
// 0x004C8108 (deleting dtor)
nw::snd::internal::SequenceSound::~SequenceSound()
{
    // (only the members and the base are destroyed)
}

// 0x00132528 slot 0x1C
void nw::snd::internal::SequenceSound::DetachTempSpecialHandle()
{
    m_pTempSpecialHandle->DetachSound();
}

// 0x0073F404 slot 0x14 | fefates:bytes
bool nw::snd::internal::SequenceSound::IsPrepared() const
{
    if (m_PreparedFlag) {
        return true;
    }
    return m_IsPlayerAvailable && m_SequenceSoundPlayer.m_ResState >= driver::SequenceSoundPlayer::RES_STATE_PREPARED;
}

// 0x0073F438 (name is ours)
bool nw::snd::internal::SequenceSound::ReadVariable(int varNo, s16* pValue) const
{
    *pValue = m_IsPlayerAvailable ? m_SequenceSoundPlayer.GetLocalVariable(varNo) : -1;
    return true;
}

// 0x0073F460 slot 0x00 (name after nw::gfx)
const TypeInfo* nw::snd::internal::SequenceSound::GetRuntimeTypeInfo() const
{
    return &s_TypeInfo;
}

// 0x0073F46C (name is ours)
u32 nw::snd::internal::SequenceSound::GetTick() const
{
    if (!m_IsPlayerAvailable) {
        return 0;
    }
    return m_SequenceSoundPlayer.m_TickCounter;
}

} // namespace internal

// (armlink folded the code with WaveSoundHandle::DetachSound 0x00132530)
void nw::snd::SequenceSoundHandle::DetachSound()
{
    if (m_pSound == nullptr) {
        return;
    }
    if (m_pSound->m_pTempSpecialHandle == this) {
        m_pSound->m_pTempSpecialHandle = nullptr;
    }
    if (m_pSound != nullptr) {
        m_pSound = nullptr;
    }
}

} // namespace snd
} // namespace nw
