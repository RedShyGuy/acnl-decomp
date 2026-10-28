#include "nw/snd/internal/snd_StreamSound.h"

#include "nw/snd/internal/snd_DriverCommandManager.h"
#include "nw/snd/internal/snd_DriverCommandPlayer.h"
#include "nw/snd/snd_StreamSoundHandle.h"

namespace nw {
namespace snd {
namespace internal {
// 0x009761A8
const TypeInfo nw::snd::internal::StreamSound::s_TypeInfo(&BasicSound::s_TypeInfo);

// sets s_TypeInfo
// 0x0079A5B4 (static initializer)

// 0x004C6168 slot 0x0C | nintendogs:callseq
void nw::snd::internal::StreamSound::Initialize()
{
    driver::BasicSoundPlayer* pPlayer = GetBasicSoundPlayerHandle();
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    if (!pPlayer->m_Event.TryWait()) {
        manager.FlushCommand(true);
        pPlayer->m_Event.Wait();
    }
    BasicSound::Initialize();
    m_StreamSoundLoader.Initialize();
    m_pTempSpecialHandle = nullptr;
    for (int i = 0; i < TRACK_MAX; i++) {
        m_TrackVolumes[i].InitValue(0.0f);
        m_TrackVolumes[i].SetTarget(1.0f, 1);
    }
    m_InitializeFlag = true;
}

// 0x004C6220 (name after the player)
void nw::snd::internal::StreamSound::SetTrackPan(u32 trackBitFlag, f32 pan)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandStreamSoundTrackParam* pCommand = manager.AllocCommand<DriverCommandStreamSoundTrackParam>();
    pCommand->id = DriverCommand::ID_STRMTRACK_PAN;
    pCommand->player = &m_StreamSoundPlayer;
    pCommand->trackBitFlag = trackBitFlag;
    pCommand->value = pan;
    manager.PushCommand(pCommand);
}

// 0x004C6278 slot 0x30 | mk7dlp:bytes-fuzzy
void nw::snd::internal::StreamSound::UpdateParam()
{
    BasicSound::UpdateParam();
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    u32 trackFlags = m_AllocTrackFlag;
    for (u32 i = 0; i < TRACK_MAX; i++, trackFlags >>= 1) {
        if (trackFlags & 1) {
            DriverCommandStreamSoundTrackParam* pCommand = manager.AllocCommand<DriverCommandStreamSoundTrackParam>();
            pCommand->id = DriverCommand::ID_STRMTRACK_VOLUME;
            pCommand->player = &m_StreamSoundPlayer;
            pCommand->trackBitFlag = 1 << i;
            pCommand->value = m_TrackVolumes[i].GetValue();
            manager.PushCommand(pCommand);
        }
    }
}

// 0x004C6340 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::StreamSound::SetTrackVolume(u32 trackBitFlag, f32 volume, int frames)
{
    if (volume < 0.0f) {
        volume = 0.0f;
    }
    u32 trackFlags = m_AllocTrackFlag & trackBitFlag;
    for (int i = 0; i < TRACK_MAX && trackBitFlag != 0; i++, trackFlags >>= 1) {
        if (trackFlags & 1) {
            m_TrackVolumes[i].SetTarget(volume, frames);
        }
    }
}

// 0x004C63F4 | fefates:bytes [tier B]
void nw::snd::internal::StreamSound::PreparePrefetch(const void* pPrefetchData,
                                                    const driver::StreamSoundPlayer::PrepareBaseArg& arg)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandStreamSoundPreparePrefetch* pCommand = manager.AllocCommand<DriverCommandStreamSoundPreparePrefetch>();
    pCommand->id = DriverCommand::ID_STRMSOUND_PREPARE_PREFETCH;
    pCommand->player = &m_StreamSoundPlayer;
    pCommand->arg.pPrefetchData = pPrefetchData;
    pCommand->arg.baseArg = arg;
    manager.PushCommand(pCommand);
}

// 0x004C644C slot 0x28 | fefates:bytes
void nw::snd::internal::StreamSound::UpdateMoveValue()
{
    BasicSound::UpdateMoveValue();
    u32 trackFlags = m_AllocTrackFlag;
    for (int i = 0; i < TRACK_MAX; i++, trackFlags >>= 1) {
        if (trackFlags & 1) {
            m_TrackVolumes[i].Update();
        }
    }
}

// 0x004C6504 (name after the player)
void nw::snd::internal::StreamSound::SetTrackSurroundPan(u32 trackBitFlag, f32 surroundPan)
{
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandStreamSoundTrackParam* pCommand = manager.AllocCommand<DriverCommandStreamSoundTrackParam>();
    pCommand->id = DriverCommand::ID_STRMTRACK_SURROUND_PAN;
    pCommand->player = &m_StreamSoundPlayer;
    pCommand->trackBitFlag = trackBitFlag;
    pCommand->value = surroundPan;
    manager.PushCommand(pCommand);
}

// 0x004C655C slot 0x24 | fefates:bytes
void nw::snd::internal::StreamSound::OnUpdatePlayerPriority()
{
    m_pManager->UpdatePriority(this, CalcCurrentPlayerPriority());
}

// 0x004C6604 slot 0x20
driver::BasicSoundPlayer* nw::snd::internal::StreamSound::GetBasicSoundPlayerHandle()
{
    return &m_StreamSoundPlayer;
}

// 0x004C660C slot 0x18 | fefates:bytes
bool nw::snd::internal::StreamSound::IsAttachedTempSpecialHandle()
{
    return m_pTempSpecialHandle != nullptr;
}

// 0x004C6620 | fefates:bytes [tier B]
void nw::snd::internal::StreamSound::Setup(const driver::StreamSoundPlayer::SetupArg& arg)
{
    m_AllocTrackFlag = arg.allocateTrackFlags;
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandStreamSoundSetup* pCommand = manager.AllocCommand<DriverCommandStreamSoundSetup>();
    pCommand->id = DriverCommand::ID_STRMSOUND_SETUP;
    pCommand->player = &m_StreamSoundPlayer;
    pCommand->arg = arg;
    manager.PushCommand(pCommand);
}

// 0x004C6688 | fefates:bytes [tier B]
void nw::snd::internal::StreamSound::Prepare(io::FileStream* pFileStream,
                                            const driver::StreamSoundPlayer::PrepareBaseArg& arg)
{
    m_StreamSoundLoader.m_pFileStream = pFileStream;
    DriverCommandManager& manager = DriverCommandManager::GetInstance();
    DriverCommandStreamSoundPrepare* pCommand = manager.AllocCommand<DriverCommandStreamSoundPrepare>();
    pCommand->id = DriverCommand::ID_STRMSOUND_PREPARE;
    pCommand->player = &m_StreamSoundPlayer;
    pCommand->arg.pLoader = &m_StreamSoundLoader;
    pCommand->arg.baseArg = arg;
    manager.PushCommand(pCommand);
}

// 0x004C66EC slot 0x10 | fefates:bytes
void nw::snd::internal::StreamSound::Finalize()
{
    if (!m_InitializeFlag) {
        return;
    }
    m_InitializeFlag = false;
    m_StreamSoundLoader.Finalize();
    BasicSound::Finalize();
    m_pManager->Free(this);
}

// 0x004C675C | fefates:bytes [tier B]
nw::snd::internal::StreamSound::StreamSound(StreamSoundInstanceManager& manager)
    : m_pManager(&manager), m_InitializeFlag(false), m_pCacheBuffer(nullptr), m_CacheSize(0)
{
}

// 0x004C6818 slot 0x04 | fefates:bytes
// 0x004C67D4 (deleting dtor)
nw::snd::internal::StreamSound::~StreamSound()
{
    // (only the members and the base are destroyed)
}

// 0x004C08B0 slot 0x1C
void nw::snd::internal::StreamSound::DetachTempSpecialHandle()
{
    m_pTempSpecialHandle->DetachSound();
}

// 0x0073F344 slot 0x14 | fefates:bytes
bool nw::snd::internal::StreamSound::IsPrepared() const
{
    if (!m_IsPlayerAvailable) {
        return false;
    }
    return m_StreamSoundPlayer.m_IsPrepared;
}

// 0x0073F35C (name is ours)
long nw::snd::internal::StreamSound::GetPlayLoopCount() const
{
    if (!m_IsPlayerAvailable) {
        return 0;
    }
    if (!m_StreamSoundPlayer.m_IsActive) {
        return -1;
    }
    return m_StreamSoundPlayer.m_LoopCounter;
}

// 0x0073F380 slot 0x00 (name after nw::gfx)
const TypeInfo* nw::snd::internal::StreamSound::GetRuntimeTypeInfo() const
{
    return &s_TypeInfo;
}

// 0x0073F38C (name after the player)
bool nw::snd::internal::StreamSound::ReadStreamDataInfo(StreamDataInfo* pInfo) const
{
    if (!m_IsPlayerAvailable) {
        return false;
    }
    return m_StreamSoundPlayer.ReadStreamDataInfo(pInfo);
}

// 0x0073F3A8 (name after the player)
long nw::snd::internal::StreamSound::GetPlaySamplePosition(bool isOriginal) const
{
    if (!m_IsPlayerAvailable) {
        return 0;
    }
    return m_StreamSoundPlayer.GetPlaySamplePosition(isOriginal);
}

} // namespace internal
} // namespace snd
} // namespace nw
