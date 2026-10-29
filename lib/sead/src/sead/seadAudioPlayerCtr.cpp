#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadAudioPlayer.h"
#include "sead/hostio/seadNode.h"
#include "nw/snd/snd_SoundArchivePlayer.h"
#include "sead/seadAudioPlayerCtr.h"
#include "nw/snd/snd_SoundSystem.h"
#include "sead/seadAudioSystemCtr.h"
#include "sead/seadAudioResetter.h"
#include "sead/seadAudioMgr.h"
#include "sead/seadNew.h"
#include "sead/seadAudioSoundDataMgrCtr.h"
#include "sead/seadAudioSoundHeapCtr.h"

namespace sead {
// 0x0012C814 (name is ours)
sead::AudioPlayerCtr::AudioPlayerCtr()
    : AudioPlayer(), SoundArchivePlayer(), hostio::Node(), m_pSetupBuffer(nullptr), m_SetupBufferSize(0),
      m_pStreamBuffer(nullptr), m_StreamBufferSize(0), m_RequiredStreamBufferSize(0), m_Unknown134(0),
      m_pStreamCacheBuffer(nullptr), m_StreamCacheSize(0), m_UserParamSizePerSound(0), m_pSoundDataMgr(nullptr),
      m_pSoundHeap(nullptr), m_IsPause(false), m_CriticalSection(), m_IsLockUpdate(false)
{
    // (with the global nothrow operator new)
    m_pSoundDataMgr = new (std::nothrow) AudioSoundDataMgrCtr();
}

// 0x00543978 slot 0x08
// 0x00543960 (deleting dtor)
// 0x00543970 (thunk)
// 0x00778358 (deleting thunk)
sead::AudioPlayerCtr::~AudioPlayerCtr()
{
    if (m_pSetupBuffer != nullptr) {
        delete[] m_pSetupBuffer;
        m_pSetupBuffer = nullptr;
    }
    if (m_pStreamBuffer != nullptr) {
        delete[] m_pStreamBuffer;
        m_pStreamBuffer = nullptr;
    }
    if (m_pStreamCacheBuffer != nullptr) {
        delete[] m_pStreamCacheBuffer;
        m_pStreamCacheBuffer = nullptr;
    }
    if (m_pSoundHeap != nullptr) {
        delete m_pSoundHeap;
        m_pSoundHeap = nullptr;
    }
    if (m_pSoundDataMgr != nullptr) {
        delete m_pSoundDataMgr;
        m_pSoundDataMgr = nullptr;
    }
}

// 0x00543294 slot 0x10 (name is ours)
void sead::AudioPlayerCtr::initialize()
{
    // empty (in the original too)
}

// 0x00543298 slot 0x20 (name is ours)
bool sead::AudioPlayerCtr::startSound(nw::snd::SoundHandle* handle, const char* label)
{
    return SoundArchivePlayer::StartSound(handle, label, nullptr) == START_SUCCESS;
}

// 0x005432B8 slot 0x1C (name is ours)
bool sead::AudioPlayerCtr::startSound(nw::snd::SoundHandle* handle, u32 soundId)
{
    return SoundArchivePlayer::StartSound(handle, soundId, nullptr) == START_SUCCESS;
}

// 0x005432D8 (name is ours)
void sead::AudioPlayerCtr::unpauseAll(int fadeFrames)
{
    u32 count = m_SoundPlayerCount;
    if (count == 0) {
        return;
    }
    for (u32 i = 0; i < count; i++) {
        GetSoundPlayer(0x04000000 + i).PauseAllSound(false, fadeFrames);
    }
    m_IsPause = false;
}

// 0x0054332C (name is ours)
void sead::AudioPlayerCtr::initializeAfterSoundSystem()
{
    DynamicCast<AudioSystemCtr>(AudioMgr::s_pInstance->mSystem)->initializeSoundSystem_();
    nw::snd::SoundArchive* archive = m_pSoundDataMgr->getSoundArchive();
    if (!archive->IsAvailable()) {
        return;
    }
    InitializeParam param;
    param.soundArchive = archive;
    param.soundDataManager = m_pSoundDataMgr;
    param.buffer = m_pSetupBuffer;
    param.bufferSize = m_SetupBufferSize;
    param.streamBuffer = m_pStreamBuffer;
    param.streamBufferSize = m_StreamBufferSize;
    param.streamCacheBuffer = m_pStreamCacheBuffer;
    param.streamCacheSize = m_StreamCacheSize;
    param.userParamSizePerSound = m_UserParamSizePerSound;
    SoundArchivePlayer::Initialize(param);
}

// 0x00543458 slot 0x38 (name of the reference symbols)
nw::snd::SoundStartable::StartResult sead::AudioPlayerCtr::detail_SetupSound(nw::snd::SoundHandle* pHandle,
                                                                             u32 soundId, bool isHold,
                                                                             const StartInfo* pStartInfo)
{
    if (AudioMgr::s_pInstance->mResetter->isResetting()) {
        return static_cast<StartResult>(128);
    }
    return SoundArchivePlayer::detail_SetupSound(pHandle, soundId, isHold, pStartInfo);
}

// 0x005434C8 (name is ours)
void sead::AudioPlayerCtr::setupBuffers(const SetupArg& arg)
{
    nw::snd::SoundArchive* archive = m_pSoundDataMgr->getSoundArchive();
    u32 streamBufferSize = GetRequiredStreamBufferSize(archive);
    m_RequiredStreamBufferSize = streamBufferSize;
    if (streamBufferSize != 0) {
        streamBufferSize = static_cast<u32>(static_cast<f32>(streamBufferSize) * arg.streamBufferScale);
    }
    setupBuffers_(archive, streamBufferSize, arg.streamCacheSize, arg.userParamSizePerSound, arg.heap);
}

// 0x00543534 (name is ours)
void sead::AudioPlayerCtr::setupBuffers(u32 extraStreamBufferSize, u32 streamCacheSize, u32 userParamSizePerSound,
                                        Heap* heap)
{
    nw::snd::SoundArchive* archive = m_pSoundDataMgr->getSoundArchive();
    u32 streamBufferSize = GetRequiredStreamBufferSize(archive);
    m_RequiredStreamBufferSize = streamBufferSize;
    if (streamBufferSize != 0) {
        streamBufferSize += extraStreamBufferSize;
    }
    setupBuffers_(archive, streamBufferSize, streamCacheSize, userParamSizePerSound, heap);
}

// 0x0054359C (name is ours)
void sead::AudioPlayerCtr::setupBuffers_(nw::snd::SoundArchive* archive, u32 streamBufferSize, u32 streamCacheSize,
                                         u32 userParamSizePerSound, Heap* heap)
{
    if (!archive->IsAvailable()) {
        return;
    }
    m_UserParamSizePerSound = userParamSizePerSound;
    m_SetupBufferSize = GetRequiredMemSize(archive, userParamSizePerSound);
    m_pSetupBuffer = new (heap, 32) u8[m_SetupBufferSize];
    m_StreamBufferSize = streamBufferSize;
    if (streamBufferSize == 0) {
        m_pStreamBuffer = nullptr;
    } else {
        m_pStreamBuffer = new (heap, 32) u8[streamBufferSize];
        if (streamCacheSize != 0) {
            m_StreamCacheSize = GetRequiredStreamCacheSize(archive, (streamCacheSize + 31) & ~31u);
            m_pStreamCacheBuffer = new (heap, 32) u8[m_StreamCacheSize];
        }
    }
    // (with the split initialization the archive player waits for the sound system)
    if (DynamicCast<AudioSystemCtr>(AudioMgr::s_pInstance->mSystem)->mIsSplitInitialize &&
        !DynamicCast<AudioSystemCtr>(AudioMgr::s_pInstance->mSystem)->mIsSoundSystemInitialized) {
        return;
    }
    InitializeParam param;
    param.soundArchive = archive;
    param.soundDataManager = m_pSoundDataMgr;
    param.buffer = m_pSetupBuffer;
    param.bufferSize = m_SetupBufferSize;
    param.streamBuffer = m_pStreamBuffer;
    param.streamBufferSize = m_StreamBufferSize;
    param.streamCacheBuffer = m_pStreamCacheBuffer;
    param.streamCacheSize = m_StreamCacheSize;
    param.userParamSizePerSound = m_UserParamSizePerSound;
    SoundArchivePlayer::Initialize(param);
}

// 0x005437B8 slot 0x18 (name is ours)
void sead::AudioPlayerCtr::calc()
{
    if (!nw::snd::SoundSystem::IsInitialized()) {
        return;
    }
    if (m_IsLockUpdate) {
        m_CriticalSection.lock();
        SoundArchivePlayer::Update();
        m_CriticalSection.unlock();
    } else {
        SoundArchivePlayer::Update();
    }
}

// 0x00543808 (name is ours)
void sead::AudioPlayerCtr::stopAllSound(int fadeFrames)
{
    u32 count = m_SoundPlayerCount;
    if (count == 0) {
        return;
    }
    for (u32 i = 0; i < count; i++) {
        GetSoundPlayer(0x04000000 + i).StopAllSound(fadeFrames);
    }
}

// 0x00543854 slot 0x14 (name is ours)
void sead::AudioPlayerCtr::finalize()
{
    if (!IsAvailable()) {
        return;
    }
    stopAllSound(0);
    m_pSoundDataMgr->connectSoundHeap(nullptr);
    if (m_pSoundHeap != nullptr) {
        delete m_pSoundHeap;
        m_pSoundHeap = nullptr;
    }
    SoundArchivePlayer::Finalize();
    SoundArchivePlayer::Finalize();
    if (m_pStreamBuffer != nullptr) {
        delete[] m_pStreamBuffer;
        m_pStreamBuffer = nullptr;
    }
    if (m_pSetupBuffer != nullptr) {
        delete[] m_pSetupBuffer;
        m_pSetupBuffer = nullptr;
    }
    m_pSoundDataMgr->finalize();
}

// 0x00543920 slot 0x28 (name is ours)
bool sead::AudioPlayerCtr::holdSound(nw::snd::SoundHandle* handle, const char* label)
{
    return SoundArchivePlayer::HoldSound(handle, label, nullptr) == START_SUCCESS;
}

// 0x00543940 slot 0x24 (name is ours)
bool sead::AudioPlayerCtr::holdSound(nw::snd::SoundHandle* handle, u32 soundId)
{
    return SoundArchivePlayer::HoldSound(handle, soundId, nullptr) == START_SUCCESS;
}

// 0x0054A20C (name is ours)
void sead::AudioPlayerCtr::createSoundHeap(u32 size, Heap* heap)
{
    m_pSoundHeap = new (heap, 32) AudioSoundHeapCtr(size, heap);
    m_pSoundDataMgr->connectSoundHeap(m_pSoundHeap);
}

// 0x0074BA44 slot 0x34 (name is ours)
u32 sead::AudioPlayerCtr::getSoundId(const char* label) const
{
    nw::snd::SoundArchive* archive = m_pSoundDataMgr->getSoundArchive();
    if (archive == nullptr || !archive->IsAvailable()) {
        return 0xFFFFFFFF;
    }
    return archive->GetItemId(label);
}

// 0x0074BA80 slot 0x30 (name is ours)
const char* sead::AudioPlayerCtr::getSoundLabel(u32 soundId) const
{
    nw::snd::SoundArchive* archive = m_pSoundDataMgr->getSoundArchive();
    if (archive == nullptr || !archive->IsAvailable()) {
        return nullptr;
    }
    return archive->GetItemLabel(soundId);
}

// 0x0074BAB8 slot 0x2C (name is ours)
u32 sead::AudioPlayerCtr::getSoundCount() const
{
    nw::snd::SoundArchive* archive = m_pSoundDataMgr->getSoundArchive();
    if (archive == nullptr || !archive->IsAvailable()) {
        return 0;
    }
    return archive->GetSoundCount();
}
} // namespace sead

// the type information of the classes below AudioPlayer
// 0x008289FC (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::AudioPlayer>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
