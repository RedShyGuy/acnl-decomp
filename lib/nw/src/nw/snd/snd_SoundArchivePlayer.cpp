#include "nw/snd/snd_SoundArchivePlayer.h"

#include <new>

#include "nn/os/os_Api.h"
#include "nw/snd/internal/Util/Util_Api.h"
#include "nw/snd/internal/driver/snd_Bank.h"
#include "nw/snd/internal/driver/snd_MmlSequenceTrack.h"
#include "nw/snd/internal/driver/snd_SoundThread.h"
#include "nw/snd/internal/snd_DriverCommandManager.h"
#include "nw/snd/internal/snd_ExternalSoundPlayer.h"
#include "nw/snd/internal/snd_MemoryFileStream.h"
#include "nw/snd/internal/snd_PlayerHeap.h"
#include "nw/snd/internal/snd_SequenceSoundFileReader.h"
#include "nw/snd/internal/snd_WaveArchiveFileReader.h"
#include "nw/snd/snd_SoundActor.h"
#include "nw/snd/snd_SoundDataManager.h"
#include "nw/snd/snd_SoundHandle.h"
#include "nw/snd/snd_SoundSystem.h"
#include "nw/ut/ut_Api.h"

namespace nw {
namespace snd {
namespace {
// the alignment of the parts of the setup buffer
const u32 BUFFER_ALIGNMENT = 32;
// the priorities of the sounds
const int SOUND_PRIORITY_MAX = 127;
// the volumes of the sound archive (127: 1.0)
const f32 VOLUME_STEP = 1.0f / 127;
// the file types the files hook is asked for (strings 0x8B6244..)
const char* const ITEM_TYPE_SEQ = "seq";
const char* const ITEM_TYPE_WSD = "wsd";
const char* const ITEM_TYPE_STM = "stm";
const char* const FILE_TYPE_SEQ = "bxseq";
const char* const FILE_TYPE_WSD = "bxwsd";
const char* const FILE_TYPE_BANK = "bxbnk";
const char* const FILE_TYPE_WARC = "bxwar";
const char* const FILE_TYPE_STM = "bxstm";
const char* const FILE_TYPE_STP = "bxstp";

// (name is ours) whether p is beyond the end of the buffer
inline bool IsOverBuffer(const void* p, const void* pEnd)
{
    return static_cast<s32>(reinterpret_cast<uptr>(p) - reinterpret_cast<uptr>(pEnd)) > 0;
}

// (name is ours) a sound of the kind; its id and ambient information are set
template <typename TSound>
TSound* AllocSound(internal::SoundInstanceManager<TSound>* pManager, u32 soundId, int priority, int ambientPriority,
                   internal::BasicSound::AmbientInfo* pAmbientInfo)
{
    TSound* pSound = pManager->Alloc(priority, ambientPriority);
    if (pSound == nullptr) {
        return nullptr;
    }
    pSound->m_Id = soundId;
    if (pAmbientInfo != nullptr) {
        pSound->SetAmbientInfo(*pAmbientInfo);
    }
    return pSound;
}
} // namespace

// 0x004C0B04 | fefates:bytes [tier B]
bool nw::snd::SoundArchivePlayer::Initialize(const InitializeParam& param)
{
    const SoundArchive* pArchive = param.soundArchive;
    void* pStreamBuffer = param.streamBuffer;
    const u32 streamBufferSize = param.streamBufferSize;
    bool result = false;
    if (SoundSystem::IsInitialized()) {
        bool isValidStreamBuffer = true;
        if (streamBufferSize != 0) {
            // the stream buffer must be in device memory
            const uptr deviceMemoryAddress = nn::os::GetDeviceMemoryAddress();
            const uptr deviceMemoryEnd = deviceMemoryAddress + nn::os::GetDeviceMemorySize();
            if (deviceMemoryAddress > reinterpret_cast<uptr>(pStreamBuffer) ||
                reinterpret_cast<uptr>(pStreamBuffer) + streamBufferSize > deviceMemoryEnd) {
                isValidStreamBuffer = false;
            }
        }
        if (isValidStreamBuffer && SetupMram(pArchive, param.buffer, param.bufferSize, param.userParamSizePerSound)) {
            SoundArchive::SoundArchivePlayerInfo info;
            u32 streamChannelCount = 0;
            if (pArchive->ReadSoundArchivePlayerInfo(&info)) {
                streamChannelCount = info.streamChannelMax;
            }
            if (streamChannelCount * STREAM_BUFFER_SIZE_PER_CHANNEL <= streamBufferSize) {
                streamChannelCount = 0;
                if (pArchive->ReadSoundArchivePlayerInfo(&info)) {
                    streamChannelCount = info.streamChannelMax;
                }
                m_StreamBufferPool.Initialize(pStreamBuffer, streamBufferSize, streamChannelCount);
                m_pSequenceTrackAllocator = &m_MmlSequenceTrackAllocator;
                result = true;
                m_pSoundDataManager = param.soundDataManager;
            }
        }
    }

    // the stream caches of the stream sounds
    if (param.streamCacheBuffer != nullptr) {
        SoundArchive::SoundArchivePlayerInfo info;
        if (!param.soundArchive->ReadSoundArchivePlayerInfo(&info)) {
            return false;
        }
        const u32 cacheSize = param.streamCacheSize / info.streamSoundMax;
        u8* pCache = static_cast<u8*>(param.streamCacheBuffer);
        typedef internal::SoundInstanceManager<internal::StreamSound>::SoundList StreamSoundList;
        for (StreamSoundList::iterator it = m_StreamSoundInstanceManager.m_FreeList.begin();
             it != m_StreamSoundInstanceManager.m_FreeList.end(); ++it) {
            internal::StreamSound* pSound = StreamSoundList::GetPointerFromNode(it.GetNode());
            pSound->m_pCacheBuffer = pCache;
            pSound->m_CacheSize = cacheSize;
            pCache += cacheSize;
        }
    }
    return result;
}

// 0x004C0C80 | fefates:bytes [tier B]
nw::snd::SoundPlayer& nw::snd::SoundArchivePlayer::GetSoundPlayer(u32 playerId)
{
    return m_pSoundPlayers[SoundArchive::GetItemIndex(playerId)];
}

// 0x004C0C98 | fefates:bytes [tier B]
bool nw::snd::SoundArchivePlayer::SetupWaveSound(int count, void** ppBuffer, const void* pEnd)
{
    if (reinterpret_cast<uptr>(*ppBuffer) % BUFFER_ALIGNMENT != 0) {
        return false;
    }
    void* pSoundBuffer = *ppBuffer;
    const u32 soundBufferSize = ut::RoundUp(count * sizeof(internal::WaveSound), BUFFER_ALIGNMENT);
    void* pLoaderBuffer = static_cast<u8*>(pSoundBuffer) + soundBufferSize;
    if (IsOverBuffer(pLoaderBuffer, pEnd)) {
        return false;
    }
    m_WaveSoundInstanceManager.Create(pSoundBuffer, soundBufferSize);
    *ppBuffer = pLoaderBuffer;

    const u32 loaderBufferSize = ut::RoundUp(count * sizeof(internal::driver::WaveSoundLoader), BUFFER_ALIGNMENT);
    void* pNext = static_cast<u8*>(pLoaderBuffer) + loaderBufferSize;
    if (IsOverBuffer(pNext, pEnd)) {
        return false;
    }
    m_WaveSoundLoaderManager.Create(pLoaderBuffer, loaderBufferSize);
    *ppBuffer = pNext;

    typedef internal::SoundInstanceManager<internal::WaveSound>::SoundList WaveSoundList;
    for (WaveSoundList::iterator it = m_WaveSoundInstanceManager.m_FreeList.begin();
         it != m_WaveSoundInstanceManager.m_FreeList.end(); ++it) {
        WaveSoundList::GetPointerFromNode(it.GetNode())->m_WaveSoundPlayer.m_pLoaderManager = &m_WaveSoundLoaderManager;
    }
    internal::driver::SoundThread::GetInstance().RegisterSoundFrameCallback(&m_WaveSoundLoaderManager);
    return true;
}

// 0x004C0E8C | fefates:bytes [tier B]
bool nw::snd::SoundArchivePlayer::SetupSoundPlayer(const SoundArchive* pArchive, void** ppBuffer, const void* pEnd)
{
    if (reinterpret_cast<uptr>(*ppBuffer) % BUFFER_ALIGNMENT != 0) {
        return false;
    }
    const u32 playerCount = pArchive->GetPlayerCount();
    u8* pPlayerBuffer = static_cast<u8*>(*ppBuffer);
    void* pNext = pPlayerBuffer + ut::RoundUp(playerCount * sizeof(SoundPlayer), BUFFER_ALIGNMENT);
    if (IsOverBuffer(pNext, pEnd)) {
        return false;
    }
    *ppBuffer = pNext;
    m_SoundPlayerCount = playerCount;
    m_pSoundPlayers = reinterpret_cast<SoundPlayer*>(pPlayerBuffer);

    for (u32 i = 0; i < playerCount; i++) {
        SoundPlayer* pPlayer = pPlayerBuffer != nullptr ? new (pPlayerBuffer) SoundPlayer() : nullptr;
        SoundArchive::PlayerInfo info;
        info.playableSoundMax = 0;
        info.playerHeapSize = 0;
        if (pArchive->ReadPlayerInfo(SoundArchive::MakeItemId(SoundArchive::ITEM_TYPE_PLAYER, i), &info)) {
            pPlayer->SetPlayableSoundCount(info.playableSoundMax);
            if (info.playerHeapSize != 0) {
                // a player heap per playable sound
                for (int j = 0; j < static_cast<int>(info.playableSoundMax); j++) {
                    if (reinterpret_cast<uptr>(*ppBuffer) % BUFFER_ALIGNMENT != 0) {
                        return false;
                    }
                    void* pHeapMemory = *ppBuffer;
                    void* pHeapBuffer = ut::RoundUp(static_cast<u8*>(pHeapMemory) + sizeof(internal::PlayerHeap), BUFFER_ALIGNMENT);
                    if (IsOverBuffer(pHeapBuffer, pEnd)) {
                        return false;
                    }
                    internal::PlayerHeap* pHeap = pHeapMemory != nullptr ? new (pHeapMemory) internal::PlayerHeap() : nullptr;
                    *ppBuffer = pHeapBuffer;
                    pHeapBuffer = ut::RoundUp(*ppBuffer, BUFFER_ALIGNMENT);
                    pNext = ut::RoundUp(static_cast<u8*>(pHeapBuffer) + info.playerHeapSize, BUFFER_ALIGNMENT);
                    *ppBuffer = pHeapBuffer;
                    if (IsOverBuffer(pNext, pEnd)) {
                        return false;
                    }
                    *ppBuffer = pNext;
                    if (!pHeap->Create(pHeapBuffer, info.playerHeapSize)) {
                        return false;
                    }
                    if (pHeap == nullptr) {
                        return false;
                    }
                    pPlayer->detail_AppendPlayerHeap(pHeap);
                }
                pPlayer->m_PlayerHeapCount = info.playableSoundMax;
            }
        }
        pPlayerBuffer += sizeof(SoundPlayer);
    }
    return true;
}

// 0x004C1038 slot 0x08 | nintendogs:bytes
nw::snd::SoundStartable::StartResult nw::snd::SoundArchivePlayer::detail_SetupSound(SoundHandle* pHandle, u32 soundId,
                                                                                    bool isHold,
                                                                                    const StartInfo* pStartInfo)
{
    return detail_SetupSoundImpl(pHandle, soundId, nullptr, nullptr, isHold, pStartInfo);
}

// 0x004C1068 | fefates:bytes [tier B]
bool nw::snd::SoundArchivePlayer::SetupSequenceSound(int count, void** ppBuffer, const void* pEnd)
{
    if (reinterpret_cast<uptr>(*ppBuffer) % BUFFER_ALIGNMENT != 0) {
        return false;
    }
    void* pSoundBuffer = *ppBuffer;
    const u32 soundBufferSize = ut::RoundUp(count * sizeof(internal::SequenceSound), BUFFER_ALIGNMENT);
    void* pLoaderBuffer = static_cast<u8*>(pSoundBuffer) + soundBufferSize;
    if (IsOverBuffer(pLoaderBuffer, pEnd)) {
        return false;
    }
    m_SequenceSoundInstanceManager.Create(pSoundBuffer, soundBufferSize);
    *ppBuffer = pLoaderBuffer;

    const u32 loaderBufferSize = ut::RoundUp(count * sizeof(internal::driver::SequenceSoundLoader), BUFFER_ALIGNMENT);
    void* pNext = static_cast<u8*>(pLoaderBuffer) + loaderBufferSize;
    if (IsOverBuffer(pNext, pEnd)) {
        return false;
    }
    m_SequenceSoundLoaderManager.Create(pLoaderBuffer, loaderBufferSize);
    *ppBuffer = pNext;

    typedef internal::SoundInstanceManager<internal::SequenceSound>::SoundList SequenceSoundList;
    for (SequenceSoundList::iterator it = m_SequenceSoundInstanceManager.m_FreeList.begin();
         it != m_SequenceSoundInstanceManager.m_FreeList.end(); ++it) {
        SequenceSoundList::GetPointerFromNode(it.GetNode())->m_SequenceSoundPlayer.m_pLoaderManager = &m_SequenceSoundLoaderManager;
    }
    internal::driver::SoundThread::GetInstance().RegisterSoundFrameCallback(&m_SequenceSoundLoaderManager);
    return true;
}

// 0x004C1294 | fefates:bytes [tier B]
nw::snd::SoundStartable::StartResult nw::snd::SoundArchivePlayer::PrepareWaveSoundImpl(
    internal::WaveSound* pSound, const SoundArchive::SoundInfo* pSoundInfo, const SoundArchive::WaveSoundInfo* pInfo,
    StartInfo::StartOffsetType startOffsetType, int startOffset, const StartInfo::WaveSoundInfo* pStartInfo)
{
    const bool canLoad = pSound->m_pSoundPlayer->m_PlayerHeapCount != 0;
    internal::LoadItemInfo loadInfoWsd;

    // the files of the files hook
    const void* pWaveSoundFile = nullptr;
    const void* pWaveFile = nullptr;
    bool isHookFile = false;
    if (m_pFilesHook != nullptr) {
        const char* pLabel = m_pSoundArchive->GetItemLabel(pSound->m_Id);
        if (pLabel != nullptr) {
            pWaveSoundFile = m_pFilesHook->IsEnabled() ? m_pFilesHook->GetFileAddress(pLabel, ITEM_TYPE_WSD, FILE_TYPE_WSD, 0) : nullptr;
            const void* pWaveArchiveFile =
                m_pFilesHook->IsEnabled() ? m_pFilesHook->GetFileAddress(pLabel, ITEM_TYPE_WSD, FILE_TYPE_WARC, 0) : nullptr;
            if (pWaveSoundFile != nullptr && pWaveArchiveFile != nullptr) {
                internal::WaveArchiveFileReader reader(pWaveArchiveFile, false);
                pWaveFile = reader.GetWaveFile(pInfo->index);
                isHookFile = pWaveFile != nullptr;
            }
        }
    }
    if (!isHookFile) {
        pWaveSoundFile = m_pSoundDataManager->detail_GetFileAddress(pSoundInfo->fileId);
    }
    loadInfoWsd.itemId = pSound->m_Id;
    loadInfoWsd.address = pWaveSoundFile;

    // the data is loaded by the task thread if it is not loaded
    bool isRegisterDataLoadTask = false;
    if (pWaveSoundFile == nullptr) {
        if (!canLoad) {
            return START_ERR_WSD_NOT_LOADED;
        }
        isRegisterDataLoadTask = true;
    } else if (pWaveFile == nullptr) {
        if (pStartInfo != nullptr) {
            pWaveFile = pStartInfo->pWaveFile;
        } else {
            pWaveFile = internal::Util::GetWaveFileOfWaveSound(pWaveSoundFile, pInfo->index, *m_pSoundArchive,
                                                               *static_cast<const internal::SoundArchiveLoader*>(m_pSoundDataManager));
        }
        if (pWaveFile == nullptr) {
            if (!canLoad) {
                return START_ERR_WARC_NOT_LOADED;
            }
            isRegisterDataLoadTask = true;
        }
    }

    internal::driver::WaveSoundPlayer::StartInfo startInfo;
    internal::driver::WaveSoundPlayer::StartOffsetType playerStartOffsetType;
    switch (startOffsetType) {
    case StartInfo::START_OFFSET_TYPE_MILLISEC:
        playerStartOffsetType = internal::driver::WaveSoundPlayer::START_OFFSET_TYPE_MILLISEC;
        break;
    case StartInfo::START_OFFSET_TYPE_SAMPLE:
        playerStartOffsetType = internal::driver::WaveSoundPlayer::START_OFFSET_TYPE_SAMPLE;
        break;
    default:
        playerStartOffsetType = internal::driver::WaveSoundPlayer::START_OFFSET_TYPE_SAMPLE;
        startOffset = 0;
        break;
    }
    startInfo.startOffsetType = playerStartOffsetType;
    startInfo.index = pInfo->index;
    startInfo.startOffset = startOffset;

    if (isRegisterDataLoadTask) {
        internal::driver::WaveSoundLoader::LoadInfo loadInfo;
        loadInfo.soundArchive = m_pSoundArchive;
        loadInfo.soundDataManager = m_pSoundDataManager;
        loadInfo.loadInfoWsd = &loadInfoWsd;
        loadInfo.soundPlayer = pSound->m_pSoundPlayer;
        pSound->RegisterDataLoadTask(loadInfo, startInfo);
    } else {
        pSound->Prepare(pWaveSoundFile, pWaveFile, startInfo, pStartInfo != nullptr ? pStartInfo->waveType : 0);
    }
    pSound->InitializeChannelParam(pInfo->channelPriority, pInfo->isReleasePriorityFix);
    return START_SUCCESS;
}

namespace {
// (name is ours) the files hook is locked while a sound starts; the hooks the sound does not use are disabled
struct FilesHookScope
{
    FilesHookScope(SoundArchivePlayer* pPlayer, bool isDisabled) : m_pPlayer(pPlayer), m_IsDisabled(isDisabled) {}
    ~FilesHookScope()
    {
        if (m_IsDisabled) {
            m_pPlayer->m_pSoundArchive->m_pInfoHook->m_IsEnabled = true;
            m_pPlayer->m_pFilesHook->m_IsEnabled = true;
        }
        if (m_pPlayer->m_pFilesHook != nullptr && m_pPlayer->m_pFilesHook->IsEnabled()) {
            m_pPlayer->m_pFilesHook->Unlock();
        }
    }

    SoundArchivePlayer* m_pPlayer;
    bool m_IsDisabled;
};
} // namespace

// 0x004C14F8 | nintendogs:callgraph [tier A]
nw::snd::SoundStartable::StartResult nw::snd::SoundArchivePlayer::detail_SetupSoundImpl(
    SoundHandle* pHandle, u32 soundId, internal::BasicSound::AmbientInfo* pAmbientInfo, SoundActor* pActor, bool isHold,
    const StartInfo* pStartInfo)
{
    if (!IsAvailable()) {
        return START_ERR_NOT_AVAILABLE;
    }
    if (pHandle->m_pSound != nullptr) {
        pHandle->DetachSound();
    }

    // the kind of the sound: the hooks know the sounds of their labels
    const char* pLabel = m_pSoundArchive->GetItemLabel(soundId);
    bool isHookDisabled = false;
    SoundArchive::SoundType soundType;
    if (m_pFilesHook != nullptr && m_pFilesHook->IsEnabled() && pLabel != nullptr) {
        m_pFilesHook->Lock();
        if (m_pFilesHook->IsEnabled() && m_pFilesHook->IsTargetItem(pLabel)) {
            SoundArchive::InfoHook* pInfoHook = m_pSoundArchive->m_pInfoHook;
            soundType = pInfoHook->IsEnabled() ? pInfoHook->GetSoundType(pLabel) : SoundArchive::SOUND_TYPE_INVALID;
            switch (soundType) {
            case SoundArchive::SOUND_TYPE_SEQ: {
                const bool isFound = m_pFilesHook->IsEnabled() &&
                                     m_pFilesHook->GetFileAddress(pLabel, ITEM_TYPE_SEQ, FILE_TYPE_SEQ, 0) != nullptr;
                isHookDisabled = !isFound;
                break;
            }
            case SoundArchive::SOUND_TYPE_STRM:
                isHookDisabled = false;
                break;
            case SoundArchive::SOUND_TYPE_WAVE: {
                const bool isFound = m_pFilesHook->IsEnabled() &&
                                     m_pFilesHook->GetFileAddress(pLabel, ITEM_TYPE_WSD, FILE_TYPE_WSD, 0) != nullptr;
                isHookDisabled = !isFound;
                break;
            }
            default:
                break;
            }
            if (isHookDisabled) {
                m_pSoundArchive->m_pInfoHook->m_IsEnabled = false;
                m_pFilesHook->m_IsEnabled = false;
            }
        } else {
            isHookDisabled = true;
            m_pSoundArchive->m_pInfoHook->m_IsEnabled = false;
            m_pFilesHook->m_IsEnabled = false;
            soundType = m_pSoundArchive->GetSoundType(soundId);
        }
    } else {
        if (soundId == 0xFFFFFFFF) {
            return START_ERR_INVALID_LABEL;
        }
        soundType = m_pSoundArchive->GetSoundType(soundId);
    }

    internal::BasicSound* pSound;
    int playerPriority;
    internal::ExternalSoundPlayer* pExtPlayer = nullptr;
    SoundArchive::SoundInfo soundInfo;
    {
        FilesHookScope hookScope(this, isHookDisabled);
        if (!m_pSoundArchive->ReadSoundInfo(soundId, &soundInfo)) {
            return START_ERR_INVALID_SOUND;
        }

        // the parameters of the start info
        StartInfo::StartOffsetType startOffsetType = StartInfo::START_OFFSET_TYPE_MILLISEC;
        int startOffset = 0;
        playerPriority = soundInfo.playerPriority;
        u32 playerId = soundInfo.playerId;
        u32 actorPlayerId = soundInfo.actorPlayerId;
        const StartInfo::SeqSoundInfo* pSeqSoundInfo = nullptr;
        const StartInfo::StreamSoundInfo* pStreamSoundInfo = nullptr;
        const SoundArchive::StreamSoundInfo* pStreamSoundMetaInfo = nullptr;
        const SoundArchive::StreamSoundInfo2* pStreamSoundMetaInfo2 = nullptr;
        const StartInfo::WaveSoundInfo* pWaveSoundInfo = nullptr;
        if (pStartInfo != nullptr) {
            const u32 flags = pStartInfo->enableFlag;
            if (flags & StartInfo::ENABLE_START_OFFSET) {
                startOffsetType = pStartInfo->startOffsetType;
                startOffset = pStartInfo->startOffset;
            }
            if (flags & StartInfo::ENABLE_PLAYER_PRIORITY) {
                playerPriority = pStartInfo->playerPriority;
            }
            if (flags & StartInfo::ENABLE_PLAYER_ID) {
                playerId = pStartInfo->playerId;
            }
            if (flags & StartInfo::ENABLE_ACTOR_PLAYER_ID) {
                actorPlayerId = pStartInfo->actorPlayerId;
            }
            if (flags & StartInfo::ENABLE_SEQ_SOUND_INFO) {
                pSeqSoundInfo = &pStartInfo->seqSoundInfo;
            }
            if (flags & StartInfo::ENABLE_STRM_SOUND_INFO) {
                pStreamSoundInfo = &pStartInfo->streamSoundInfo;
            }
            if (flags & StartInfo::ENABLE_STRM_META_INFO) {
                pStreamSoundMetaInfo = &pStartInfo->streamSoundMetaInfo;
            }
            if (flags & StartInfo::ENABLE_STRM_META_INFO2) {
                pStreamSoundMetaInfo2 = &pStartInfo->streamSoundMetaInfo2;
            }
            if (flags & StartInfo::ENABLE_WAVE_SOUND_INFO) {
                pWaveSoundInfo = &pStartInfo->waveSoundInfo;
            }
        }

        // a held sound has a lower priority
        int priority = playerPriority;
        if (isHold) {
            priority--;
        }
        int ambientPriority = 0;
        if (pAmbientInfo != nullptr) {
            ambientPriority = internal::BasicSound::GetAmbientPriority(*pAmbientInfo, soundId);
        }
        int allocPriority = priority + ambientPriority;
        if (allocPriority > SOUND_PRIORITY_MAX) {
            allocPriority = SOUND_PRIORITY_MAX;
        } else if (allocPriority < 0) {
            allocPriority = 0;
        }

        if (pActor != nullptr) {
            if (actorPlayerId >= SoundActor::ACTOR_PLAYER_COUNT) {
                return START_ERR_INVALID_PLAYER;
            }
            pExtPlayer = &pActor->m_ActorPlayers[actorPlayerId];
            if (pExtPlayer == nullptr) {
                return START_ERR_INVALID_PLAYER;
            }
        }
        if (playerId == 0xFFFFFFFF) {
            return START_ERR_INVALID_PLAYER;
        }
        SoundPlayer& player = GetSoundPlayer(playerId);
        if (!player.detail_CanPlaySound(allocPriority)) {
            return START_ERR_LOW_PRIORITY;
        }
        if (pExtPlayer != nullptr && !pExtPlayer->detail_CanPlaySound(allocPriority)) {
            return START_ERR_LOW_PRIORITY;
        }

        // the sound
        internal::SequenceSound* pSequenceSound = nullptr;
        internal::StreamSound* pStreamSound = nullptr;
        internal::WaveSound* pWaveSound = nullptr;
        switch (soundType) {
        case SoundArchive::SOUND_TYPE_SEQ:
            pSequenceSound = AllocSound(&m_SequenceSoundInstanceManager, soundId, priority, ambientPriority, pAmbientInfo);
            pSound = pSequenceSound;
            break;
        case SoundArchive::SOUND_TYPE_STRM:
            pStreamSound = AllocSound(&m_StreamSoundInstanceManager, soundId, priority, ambientPriority, pAmbientInfo);
            pSound = pStreamSound;
            break;
        case SoundArchive::SOUND_TYPE_WAVE:
            pWaveSound = AllocSound(&m_WaveSoundInstanceManager, soundId, priority, ambientPriority, pAmbientInfo);
            pSound = pWaveSound;
            break;
        default:
            return START_ERR_INVALID_SOUND;
        }
        if (pSound == nullptr) {
            return START_ERR_NO_SOUND_INSTANCE;
        }

        if (!player.detail_AppendSound(pSound)) {
            pSound->Finalize();
            return START_ERR_UNKNOWN;
        }
        pSound->SetFrontBypass(soundInfo.isFrontBypass);
        if (pStartInfo != nullptr && (pStartInfo->enableFlag & StartInfo::ENABLE_STOP_CALLBACK)) {
            pSound->m_StopCallback = pStartInfo->stopCallback;
        }

        // the data of the sound
        StartResult result;
        switch (soundType) {
        case SoundArchive::SOUND_TYPE_SEQ: {
            SoundArchive::SequenceSoundInfo info;
            if (!m_pSoundArchive->ReadSequenceSoundInfo(soundId, &info)) {
                pSound->Finalize();
                return START_ERR_INVALID_SOUND;
            }
            if (pSeqSoundInfo != nullptr) {
                for (int i = 0; i < 4; i++) {
                    if (pSeqSoundInfo->bankIds[i] != 0xFFFFFFFF) {
                        info.bankIds[i] = pSeqSoundInfo->bankIds[i];
                    }
                }
            }
            result = PrepareSequenceSoundImpl(pSequenceSound, &soundInfo, &info, startOffsetType, startOffset, pSeqSoundInfo);
            if (result != START_SUCCESS) {
                pSound->Finalize();
                return result;
            }
            break;
        }
        case SoundArchive::SOUND_TYPE_STRM: {
            SoundArchive::StreamSoundInfo info;
            if (pStreamSoundMetaInfo != nullptr) {
                info = *pStreamSoundMetaInfo;
            } else if (!m_pSoundArchive->ReadStreamSoundInfo(soundId, &info)) {
                pSound->Finalize();
                return START_ERR_INVALID_SOUND;
            }
            SoundArchive::StreamSoundInfo2 info2;
            const SoundArchive::StreamSoundInfo2* pInfo2 = nullptr;
            if (info.streamFileType == 2) {
                pInfo2 = pStreamSoundMetaInfo2;
                if (pInfo2 == nullptr) {
                    if (!m_pSoundArchive->detail_ReadStreamSoundInfo2(soundId, &info2)) {
                        pSound->Finalize();
                        return START_ERR_INVALID_SOUND;
                    }
                    pInfo2 = &info2;
                }
            }
            result = PrepareStreamSoundImpl(pStreamSound, &soundInfo, &info, pInfo2, startOffsetType, startOffset,
                                            pStreamSoundInfo);
            if (result != START_SUCCESS) {
                pSound->Finalize();
                return result;
            }
            break;
        }
        case SoundArchive::SOUND_TYPE_WAVE: {
            SoundArchive::WaveSoundInfo info;
            if (!m_pSoundArchive->detail_ReadWaveSoundInfo(soundId, &info)) {
                pSound->Finalize();
                return START_ERR_INVALID_SOUND;
            }
            result = PrepareWaveSoundImpl(pWaveSound, &soundInfo, &info, startOffsetType, startOffset, pWaveSoundInfo);
            if (result != START_SUCCESS) {
                pSound->Finalize();
                return result;
            }
            break;
        }
        default:
            pSound->Finalize();
            return START_ERR_INVALID_SOUND;
        }
    }

    pSound->SetInitialVolume(static_cast<int>(soundInfo.volume) * VOLUME_STEP);
    pSound->SetPanMode(soundInfo.panMode);
    pSound->SetPanCurve(soundInfo.panCurve);
    if (pExtPlayer != nullptr && !pExtPlayer->AppendSound(pSound)) {
        pSound->Finalize();
        return START_ERR_UNKNOWN;
    }
    if (pActor != nullptr) {
        pSound->m_pSoundActor = pActor;
    }
    if (isHold) {
        pSound->SetPlayerPriority(playerPriority);
    }
    pHandle->detail_AttachSound(pSound);
    return START_SUCCESS;
}

// 0x004C24BC | fefates:bytes [tier B]
nw::snd::SoundStartable::StartResult nw::snd::SoundArchivePlayer::PrepareStreamSoundImpl(
    internal::StreamSound* pSound, const SoundArchive::SoundInfo* pSoundInfo, const SoundArchive::StreamSoundInfo* pInfo,
    const SoundArchive::StreamSoundInfo2* pInfo2, StartInfo::StartOffsetType startOffsetType, int startOffset,
    const StartInfo::StreamSoundInfo* pStartInfo)
{
    // the tracks and the buffers
    internal::driver::StreamSoundPlayer::SetupArg setupArg;
    switch (pInfo->streamFileType) {
    case 1:
        setupArg.fileType = internal::driver::StreamSoundLoader::FILE_TYPE_BCSTM;
        break;
    case 2:
        setupArg.fileType = internal::driver::StreamSoundLoader::FILE_TYPE_EXTERNAL;
        break;
    default:
        return START_ERR_UNKNOWN;
    }
    for (int i = 0; i < SoundArchive::STREAM_TRACK_MAX; i++) {
        internal::StreamDataInfoDetail::TrackInfo& trackInfo = setupArg.trackInfo[i];
        const SoundArchive::StreamTrackInfo& srcInfo = pInfo->trackInfo[i];
        trackInfo.volume = srcInfo.volume;
        trackInfo.pan = srcInfo.pan;
        trackInfo.span = srcInfo.span;
        trackInfo.flags = srcInfo.flags;
        trackInfo.mainSend = srcInfo.mainSend;
        trackInfo.fxSend[0] = srcInfo.fxSend[0];
        trackInfo.fxSend[1] = srcInfo.fxSend[1];
        trackInfo.lpfFreq = srcInfo.lpfFreq;
        trackInfo.biquadType = srcInfo.biquadType;
        trackInfo.biquadValue = srcInfo.biquadValue;
        trackInfo.channelCount = srcInfo.channelCount;
        trackInfo.globalChannelIndex[0] = srcInfo.globalChannelIndex[0];
        trackInfo.globalChannelIndex[1] = srcInfo.globalChannelIndex[1];
    }
    setupArg.pBufferPool = &m_StreamBufferPool;
    setupArg.unknown4 = pInfo->allocateChannelCount;
    setupArg.allocateTrackFlags = pInfo->allocateTrackFlags;
    setupArg.isLoop = false;
    setupArg.loopStart = 0;
    setupArg.loopEnd = 0xFFFFFFFF;
    if (pInfo2 != nullptr) {
        setupArg.isLoop = pInfo2->isLoop;
        if (setupArg.isLoop) {
            setupArg.loopStart = pInfo2->loopStartFrame;
            setupArg.loopEnd = pInfo2->loopEndFrame;
        }
    }
    setupArg.pitch = pInfo->pitch;
    setupArg.mainSend = pInfo->mainSend;
    setupArg.fxSend[0] = pInfo->fxSend[0];
    setupArg.fxSend[1] = pInfo->fxSend[1];
    pSound->Setup(setupArg);

    internal::driver::StreamSoundPlayer::PrepareBaseArg prepareArg;
    switch (startOffsetType) {
    case StartInfo::START_OFFSET_TYPE_MILLISEC:
        prepareArg.startOffsetType = internal::driver::StreamSoundPlayer::START_OFFSET_TYPE_MILLISEC;
        break;
    case StartInfo::START_OFFSET_TYPE_SAMPLE:
        prepareArg.startOffsetType = internal::driver::StreamSoundPlayer::START_OFFSET_TYPE_SAMPLE;
        break;
    default:
        prepareArg.startOffsetType = internal::driver::StreamSoundPlayer::START_OFFSET_TYPE_SAMPLE;
        startOffset = 0;
        break;
    }
    prepareArg.startOffset = startOffset;
    prepareArg.regionCallback = nullptr;
    prepareArg.regionCallbackArg = nullptr;
    if (pStartInfo != nullptr) {
        prepareArg.regionCallback = pStartInfo->regionCallback;
        prepareArg.regionCallbackArg = pStartInfo->regionCallbackArg;
    }

    // the file stream: of the files hook, the start info or the archive
    void* pStreamBuffer = pSound->m_FileStreamBuffer;
    io::FileStream* pStream = nullptr;
    if (m_pFilesHook != nullptr) {
        void* pCacheBuffer = pSound->m_pCacheBuffer;
        const u32 cacheSize = pSound->m_CacheSize;
        const char* pLabel = m_pSoundArchive->GetItemLabel(pSound->m_Id);
        if (m_pFilesHook->IsEnabled()) {
            pStream = m_pFilesHook->OpenFileStream(pStreamBuffer, STREAM_FILE_STREAM_BUFFER_SIZE, pCacheBuffer, cacheSize,
                                                   pLabel, FILE_TYPE_STM);
        }
    }
    if (pStream == nullptr) {
        const char* pExtFilePath = nullptr;
        bool isMemory = false;
        if (pStartInfo != nullptr) {
            if (pStartInfo->pExtFilePath != nullptr) {
                pExtFilePath = pStartInfo->pExtFilePath;
            } else if (pStartInfo->pData != nullptr) {
                isMemory = true;
            }
        }
        if (isMemory) {
            if (pStreamBuffer == nullptr) {
                return START_ERR_CANNOT_OPEN_STREAM;
            }
            pStream = new (pStreamBuffer) internal::MemoryFileStream(pStartInfo->pData, pStartInfo->dataSize);
        } else if (pSound->m_pCacheBuffer != nullptr && pSound->m_CacheSize != 0) {
            pStream = const_cast<SoundArchive*>(m_pSoundArchive)->detail_OpenFileStream(pSoundInfo->fileId, pStreamBuffer, STREAM_FILE_STREAM_BUFFER_SIZE,
                                                             pSound->m_pCacheBuffer, pSound->m_CacheSize, pExtFilePath,
                                                             SoundArchive::FILE_STREAM_PRIORITY_HIGH);
        } else {
            pStream = const_cast<SoundArchive*>(m_pSoundArchive)->detail_OpenFileStream(pSoundInfo->fileId, pStreamBuffer, STREAM_FILE_STREAM_BUFFER_SIZE,
                                                             nullptr, 0, pExtFilePath, SoundArchive::FILE_STREAM_PRIORITY_HIGH);
        }
        if (pStream == nullptr) {
            return START_ERR_CANNOT_OPEN_STREAM;
        }
    }

    // the prefetch data
    const void* pPrefetchData = nullptr;
    if (pStartInfo != nullptr && pStartInfo->pPrefetchData != nullptr) {
        pPrefetchData = pStartInfo->pPrefetchData;
    } else if (pInfo->prefetchFileId != 0xFFFFFFFF) {
        if (m_pFilesHook != nullptr) {
            const char* pLabel = m_pSoundArchive->GetItemLabel(pSound->m_Id);
            if (pLabel != nullptr && m_pFilesHook->IsEnabled()) {
                pPrefetchData = m_pFilesHook->GetFileAddress(pLabel, ITEM_TYPE_STM, FILE_TYPE_STP, 0);
            }
        }
        if (pPrefetchData == nullptr) {
            pPrefetchData = m_pSoundDataManager->detail_GetFileAddress(pInfo->prefetchFileId);
        }
    }
    if (pPrefetchData != nullptr && startOffset == 0) {
        pSound->PreparePrefetch(pPrefetchData, prepareArg);
    }
    pSound->Prepare(pStream, prepareArg);

    pSound->SetInitialVolume(static_cast<int>(pSoundInfo->volume) * VOLUME_STEP);
    pSound->SetPanMode(pSoundInfo->panMode);
    pSound->SetPanCurve(pSoundInfo->panCurve);
    return START_SUCCESS;
}

// 0x004C2B7C slot 0x00
// 0x004C2B78 (deleting dtor)
nw::snd::SoundArchivePlayer::SequenceNoteOnCallback::~SequenceNoteOnCallback()
{
    // empty (in the original too)
}

// 0x004C2AF4 slot 0x08 | nintendogs:callseq
nw::snd::internal::driver::Channel* nw::snd::SoundArchivePlayer::SequenceNoteOnCallback::NoteOn(
    internal::driver::SequenceSoundPlayer* pPlayer, u8 bankIndex, const internal::driver::NoteOnInfo& noteOnInfo)
{
    if (!m_SoundArchivePlayer.IsAvailable()) {
        return nullptr;
    }
    internal::driver::Bank bank;
    internal::driver::Channel* pChannel =
        bank.NoteOn(pPlayer->m_BankReaders[bankIndex], pPlayer->m_WarcReaders[bankIndex], noteOnInfo);
    internal::driver::SoundThread::GetInstance().m_NoteOnCount++;
    return pChannel;
}

// 0x004C2B80 | nintendogs:callseq [tier A]
nw::snd::SoundStartable::StartResult nw::snd::SoundArchivePlayer::PrepareSequenceSoundImpl(
    internal::SequenceSound* pSound, const SoundArchive::SoundInfo* pSoundInfo, const SoundArchive::SequenceSoundInfo* pInfo,
    StartInfo::StartOffsetType startOffsetType, int startOffset, const StartInfo::SeqSoundInfo* pStartInfo)
{
    // the sequence data: of the files hook, the start info or the archive
    const void* pSeqFile = nullptr;
    if (m_pFilesHook != nullptr) {
        const char* pLabel = m_pSoundArchive->GetItemLabel(pSound->m_Id);
        if (pLabel != nullptr) {
            pSeqFile = m_pFilesHook->IsEnabled() ? m_pFilesHook->GetFileAddress(pLabel, ITEM_TYPE_SEQ, FILE_TYPE_SEQ, 0) : nullptr;
        }
    }
    u32 seqOffset = pInfo->startOffset;
    u32 allocTracks = pInfo->allocateTrackFlags;
    if (pSeqFile == nullptr) {
        pSeqFile = m_pSoundDataManager->detail_GetFileAddress(pSoundInfo->fileId);
        if (pStartInfo != nullptr) {
            if (pStartInfo->pSeqFile != nullptr) {
                pSeqFile = pStartInfo->pSeqFile;
            }
            internal::SequenceSoundFileReader reader(pSeqFile);
            if (pStartInfo->pStartLabel != nullptr) {
                if (!reader.GetOffsetByLabel(pStartInfo->pStartLabel, &seqOffset)) {
                    return START_ERR_INVALID_START_LABEL;
                }
                seqOffset = internal::driver::MmlParser::ParseAllocTrack(reader.m_pDataBlockBody, seqOffset, &allocTracks);
            }
        }
    }

    const bool canLoad = pSound->m_pSoundPlayer->m_PlayerHeapCount != 0;
    bool isRegisterDataLoadTask = false;
    internal::LoadItemInfo loadInfoBanks[internal::driver::SequenceSoundPlayer::BANK_COUNT];
    internal::LoadItemInfo loadInfoWarcs[internal::driver::SequenceSoundPlayer::BANK_COUNT];
    bool isWarcIndividual[internal::driver::SequenceSoundPlayer::BANK_COUNT];
    internal::LoadItemInfo loadInfoSeq;
    loadInfoSeq.itemId = pSound->m_Id;
    loadInfoSeq.address = pSeqFile;
    if (pSeqFile == nullptr) {
        if (!canLoad) {
            return START_ERR_SEQ_NOT_LOADED;
        }
        isRegisterDataLoadTask = true;
    }

    // the banks and wave archives of the files hook (enabled for them)
    if (m_pFilesHook != nullptr) {
        const char* pLabel = m_pSoundArchive->GetItemLabel(pSound->m_Id);
        if (pLabel != nullptr) {
            for (u32 i = 0; i < internal::driver::SequenceSoundPlayer::BANK_COUNT; i++) {
                const bool isEnabled = m_pFilesHook->m_IsEnabled;
                if (!isEnabled) {
                    m_pSoundArchive->m_pInfoHook->m_IsEnabled = true;
                    m_pFilesHook->m_IsEnabled = true;
                }
                const void* pBankFile =
                    m_pFilesHook->IsEnabled() ? m_pFilesHook->GetFileAddress(pLabel, ITEM_TYPE_SEQ, FILE_TYPE_BANK, i) : nullptr;
                const void* pWarcFile =
                    m_pFilesHook->IsEnabled() ? m_pFilesHook->GetFileAddress(pLabel, ITEM_TYPE_SEQ, FILE_TYPE_WARC, i) : nullptr;
                if (!isEnabled) {
                    m_pSoundArchive->m_pInfoHook->m_IsEnabled = false;
                    m_pFilesHook->m_IsEnabled = false;
                }
                if (pBankFile != nullptr && pWarcFile != nullptr) {
                    loadInfoBanks[i].address = pBankFile;
                    loadInfoWarcs[i].address = pWarcFile;
                    isWarcIndividual[i] = false;
                } else {
                    loadInfoBanks[i].address = nullptr;
                    loadInfoWarcs[i].address = nullptr;
                }
            }
        }
    }

    // the banks of the archive
    if (pStartInfo != nullptr) {
        for (int i = 0; i < 4; i++) {
            loadInfoBanks[i].itemId = pStartInfo->bankIds[i];
        }
    } else {
        for (int i = 0; i < 4; i++) {
            loadInfoBanks[i].itemId = pInfo->bankIds[i];
        }
    }
    for (u32 i = 0; i < internal::driver::SequenceSoundPlayer::BANK_COUNT; i++) {
        if (m_pFilesHook != nullptr && loadInfoBanks[i].address != nullptr && loadInfoWarcs[i].address != nullptr) {
            continue;
        }
        SoundArchive::BankInfo bankInfo;
        if (!m_pSoundArchive->ReadBankInfo(loadInfoBanks[i].itemId, &bankInfo)) {
            continue;
        }
        loadInfoBanks[i].address = m_pSoundDataManager->detail_GetFileAddress(bankInfo.fileId);
        if (loadInfoBanks[i].address == nullptr) {
            if (canLoad) {
                isRegisterDataLoadTask = true;
                break;
            }
            continue;
        }
        switch (internal::Util::GetWaveArchiveOfBank(loadInfoWarcs[i], isWarcIndividual[i], loadInfoBanks[i].address,
                                                      *m_pSoundArchive, *static_cast<const internal::SoundArchiveLoader*>(m_pSoundDataManager))) {
        case -2:
            return START_ERR_INVALID_WARC;
        case -1:
            if (!canLoad) {
                return START_ERR_WARC_NOT_LOADED;
            }
            isRegisterDataLoadTask = true;
            break;
        case 0:
        case 1:
            break;
        case 2:
            if (canLoad) {
                isRegisterDataLoadTask = true;
            }
            break;
        default:
            return START_ERR_UNKNOWN;
        }
    }

    pSound->Setup(m_pSequenceTrackAllocator, allocTracks, &m_SequenceCallback, pInfo->channelPriority,
                  pInfo->isReleasePriorityFix, m_SequenceUserprocCallback, m_pSequenceUserprocCallbackArg);

    internal::driver::SequenceSoundPlayer::StartInfo startInfo;
    switch (startOffsetType) {
    case StartInfo::START_OFFSET_TYPE_MILLISEC:
        startInfo.startOffsetType = internal::driver::SequenceSoundPlayer::OFFSET_TYPE_MILLISEC;
        break;
    case StartInfo::START_OFFSET_TYPE_TICK:
        startInfo.startOffsetType = internal::driver::SequenceSoundPlayer::OFFSET_TYPE_TICK;
        break;
    default:
        startInfo.startOffsetType = internal::driver::SequenceSoundPlayer::OFFSET_TYPE_TICK;
        startOffset = 0;
        break;
    }
    startInfo.startOffset = startOffset;
    startInfo.seqOffset = seqOffset;

    if (isRegisterDataLoadTask) {
        internal::driver::SequenceSoundLoader::LoadInfo loadInfo(m_pSoundArchive, m_pSoundDataManager, &loadInfoSeq,
                                                                 loadInfoBanks, pSound->m_pSoundPlayer);
        pSound->RegisterDataLoadTask(loadInfo, startInfo);
    } else {
        internal::driver::SequenceSoundPlayer::PrepareArg prepareArg;
        prepareArg.seqFile = pSeqFile;
        for (int i = 0; i < internal::driver::SequenceSoundPlayer::BANK_COUNT; i++) {
            prepareArg.bankFiles[i] = loadInfoBanks[i].address;
            prepareArg.warcFiles[i] = loadInfoWarcs[i].address;
            prepareArg.warcIsIndividual[i] = isWarcIndividual[i];
        }
        pSound->Prepare(prepareArg, startInfo);
    }
    return START_SUCCESS;
}

// 0x004C30F0 | fefates:bytes [tier B]
bool nw::snd::SoundArchivePlayer::SetupUserParamForBasicSound(void** ppBuffer, const void* pEnd, u32 size)
{
    u8* p = static_cast<u8*>(*ppBuffer);
    if (reinterpret_cast<uptr>(p) % BUFFER_ALIGNMENT != 0) {
        return false;
    }
    const u32 alignedSize = ut::RoundUp(size, 4);
    typedef internal::SoundInstanceManager<internal::WaveSound>::SoundList WaveSoundList;
    for (WaveSoundList::iterator it = m_WaveSoundInstanceManager.m_FreeList.begin();
         it != m_WaveSoundInstanceManager.m_FreeList.end(); ++it) {
        internal::WaveSound* pSound = WaveSoundList::GetPointerFromNode(it.GetNode());
        pSound->m_pUserParam = p;
        pSound->m_UserParamSize = alignedSize;
        p += alignedSize;
    }
    typedef internal::SoundInstanceManager<internal::SequenceSound>::SoundList SequenceSoundList;
    for (SequenceSoundList::iterator it = m_SequenceSoundInstanceManager.m_FreeList.begin();
         it != m_SequenceSoundInstanceManager.m_FreeList.end(); ++it) {
        internal::SequenceSound* pSound = SequenceSoundList::GetPointerFromNode(it.GetNode());
        pSound->m_pUserParam = p;
        pSound->m_UserParamSize = alignedSize;
        p += alignedSize;
    }
    typedef internal::SoundInstanceManager<internal::StreamSound>::SoundList StreamSoundList;
    for (StreamSoundList::iterator it = m_StreamSoundInstanceManager.m_FreeList.begin();
         it != m_StreamSoundInstanceManager.m_FreeList.end(); ++it) {
        internal::StreamSound* pSound = StreamSoundList::GetPointerFromNode(it.GetNode());
        pSound->m_pUserParam = p;
        pSound->m_UserParamSize = alignedSize;
        p += alignedSize;
    }
    m_SoundUserParamSize = alignedSize;
    return !IsOverBuffer(p, pEnd);
}

// 0x004C31C4 | nintendogs:callseq [tier A]
void nw::snd::SoundArchivePlayer::Update()
{
    for (u32 i = 0; i < m_SoundPlayerCount; i++) {
        GetSoundPlayer(i).Update();
    }
    m_SequenceSoundInstanceManager.SortPriorityList();
    m_WaveSoundInstanceManager.SortPriorityList();
    m_StreamSoundInstanceManager.SortPriorityList();

    internal::DriverCommandManager& manager = internal::DriverCommandManager::GetInstance();
    manager.RecvCommandReply();
    manager.FlushCommand(false);
}

// 0x004C3488 | fefates:bytes [tier B]
void nw::snd::SoundArchivePlayer::Finalize()
{
    m_pSoundArchive = nullptr;
    m_pSequenceTrackAllocator = nullptr;
    for (u32 i = 0; i < m_SoundPlayerCount; i++) {
        m_pSoundPlayers[i].StopAllSound(0);
    }
    internal::DriverCommandManager& manager = internal::DriverCommandManager::GetInstance();
    manager.WaitCommandReply(manager.FlushCommand(true));

    m_SoundPlayerCount = 0;
    m_pSoundPlayers = nullptr;
    if (m_pSetupBuffer != nullptr) {
        m_SequenceSoundInstanceManager.Destroy();
        m_SequenceSoundLoaderManager.Destroy();
        internal::driver::SoundThread::GetInstance().UnregisterSoundFrameCallback(&m_SequenceSoundLoaderManager);
        m_MmlSequenceTrackAllocator.Destroy(m_pSetupBuffer, m_SetupBufferSize);
        m_WaveSoundInstanceManager.Destroy();
        m_WaveSoundLoaderManager.Destroy();
        internal::driver::SoundThread::GetInstance().UnregisterSoundFrameCallback(&m_WaveSoundLoaderManager);
        m_StreamBufferPool.Finalize();
        m_StreamSoundInstanceManager.Destroy();
        m_pSetupBuffer = nullptr;
        m_SetupBufferSize = 0;
    }
}

// 0x004C3744 | fefates:bytes [tier B]
bool nw::snd::SoundArchivePlayer::SetupMram(const SoundArchive* pArchive, void* pBuffer, u32 bufferSize,
                                           u32 userParamSizePerSound)
{
    if (reinterpret_cast<uptr>(pBuffer) % BUFFER_ALIGNMENT != 0) {
        return false;
    }
    void* p = pBuffer;
    const void* pEnd = static_cast<u8*>(pBuffer) + bufferSize;
    if (!SetupSoundPlayer(pArchive, &p, pEnd)) {
        return false;
    }

    SoundArchive::SoundArchivePlayerInfo info;
    if (pArchive->ReadSoundArchivePlayerInfo(&info)) {
        if (!SetupSequenceSound(info.sequenceSoundMax, &p, pEnd)) {
            return false;
        }
        if (!SetupWaveSound(info.waveSoundMax, &p, pEnd)) {
            return false;
        }

        // the stream sounds
        if (reinterpret_cast<uptr>(p) % BUFFER_ALIGNMENT != 0) {
            return false;
        }
        void* pStreamSoundBuffer = p;
        const u32 streamSoundBufferSize = ut::RoundUp(info.streamSoundMax * sizeof(internal::StreamSound), BUFFER_ALIGNMENT);
        void* pNext = static_cast<u8*>(pStreamSoundBuffer) + streamSoundBufferSize;
        if (IsOverBuffer(pNext, pEnd)) {
            return false;
        }
        m_StreamSoundInstanceManager.Create(pStreamSoundBuffer, streamSoundBufferSize);
        p = pNext;

        // the sequence tracks
        if (reinterpret_cast<uptr>(p) % BUFFER_ALIGNMENT != 0) {
            return false;
        }
        const u32 trackBufferSize = ut::RoundUp(info.sequenceTrackMax * sizeof(internal::driver::MmlSequenceTrack), BUFFER_ALIGNMENT);
        pNext = static_cast<u8*>(p) + trackBufferSize;
        if (IsOverBuffer(pNext, pEnd)) {
            return false;
        }
        m_MmlSequenceTrackAllocator.Create(p, trackBufferSize);
        p = pNext;
    }

    if (userParamSizePerSound != 0) {
        if (!SetupUserParamForBasicSound(&p, pEnd, userParamSizePerSound)) {
            return false;
        }
    }
    m_pSoundArchive = pArchive;
    m_pSetupBuffer = pBuffer;
    m_SetupBufferSize = bufferSize;
    return true;
}

// 0x00137CB4 (name is ours)
void nw::snd::SoundArchivePlayer::SetSequenceUserprocCallback(SequenceUserprocCallback callback, void* pArg)
{
    m_SequenceUserprocCallback = callback;
    m_pSequenceUserprocCallbackArg = pArg;
}

// 0x004C393C slot 0x00 | fefates:bytes
// 0x004C3914 (deleting dtor)
nw::snd::SoundArchivePlayer::~SoundArchivePlayer()
{
    // (only the members are destroyed)
}

// 0x0073EDA4 slot 0x0C
u32 nw::snd::SoundArchivePlayer::detail_GetItemId(const char* pLabel)
{
    return m_pSoundArchive->GetItemId(pLabel);
}

// 0x0073F124 | fefates:bytes [tier B]
u32 nw::snd::SoundArchivePlayer::GetRequiredMemSize(const SoundArchive* pArchive, u32 userParamSizePerSound) const
{
    // the sound players and their heaps
    const u32 playerCount = pArchive->GetPlayerCount();
    u32 size = ut::RoundUp(playerCount * sizeof(SoundPlayer), BUFFER_ALIGNMENT);
    for (u32 i = 0; i < playerCount; i++) {
        SoundArchive::PlayerInfo info;
        info.playableSoundMax = 0;
        info.playerHeapSize = 0;
        if (pArchive->ReadPlayerInfo(SoundArchive::MakeItemId(SoundArchive::ITEM_TYPE_PLAYER, i), &info) &&
            info.playerHeapSize != 0) {
            size += info.playableSoundMax *
                    (ut::RoundUp(info.playerHeapSize, BUFFER_ALIGNMENT) + ut::RoundUp(sizeof(internal::PlayerHeap), BUFFER_ALIGNMENT));
        }
    }

    // the sounds, their loaders and the tracks
    SoundArchive::SoundArchivePlayerInfo info;
    if (pArchive->ReadSoundArchivePlayerInfo(&info)) {
        size += ut::RoundUp(info.sequenceSoundMax * sizeof(internal::SequenceSound), BUFFER_ALIGNMENT);
        size += ut::RoundUp(info.sequenceSoundMax * sizeof(internal::driver::SequenceSoundLoader), BUFFER_ALIGNMENT);
        size += ut::RoundUp(info.waveSoundMax * sizeof(internal::WaveSound), BUFFER_ALIGNMENT);
        size += ut::RoundUp(info.waveSoundMax * sizeof(internal::driver::WaveSoundLoader), BUFFER_ALIGNMENT);
        size += ut::RoundUp(info.streamSoundMax * sizeof(internal::StreamSound), BUFFER_ALIGNMENT);
        size += ut::RoundUp(info.sequenceTrackMax * sizeof(internal::driver::MmlSequenceTrack), BUFFER_ALIGNMENT);
    }
    if (userParamSizePerSound != 0) {
        size += ut::RoundUp(userParamSizePerSound, 4) * (info.sequenceSoundMax + info.waveSoundMax + info.streamSoundMax);
    }
    return size;
}

// 0x0073F288 | fefates:bytes [tier B]
u32 nw::snd::SoundArchivePlayer::GetRequiredStreamCacheSize(const SoundArchive* pArchive, u32 cacheSizePerSound) const
{
    SoundArchive::SoundArchivePlayerInfo info;
    u32 streamSoundCount = 0;
    if (pArchive->ReadSoundArchivePlayerInfo(&info)) {
        streamSoundCount = info.streamSoundMax;
    }
    return streamSoundCount * cacheSizePerSound;
}

// 0x0073F2B8 | nintendogs:bytes [tier A]
u32 nw::snd::SoundArchivePlayer::GetRequiredStreamBufferSize(const SoundArchive* pArchive)
{
    SoundArchive::SoundArchivePlayerInfo info;
    u32 streamChannelCount = 0;
    if (pArchive->ReadSoundArchivePlayerInfo(&info)) {
        streamChannelCount = info.streamChannelMax;
    }
    return streamChannelCount * STREAM_BUFFER_SIZE_PER_CHANNEL;
}

// 0x001323C4 | fefates:bytes [tier B]
nw::snd::SoundArchivePlayer::SoundArchivePlayer()
    : m_pSoundArchive(nullptr), m_SequenceCallback(*this), m_SequenceUserprocCallback(nullptr),
      m_pSequenceUserprocCallbackArg(nullptr), m_SoundPlayerCount(0), m_pSoundPlayers(nullptr),
      m_pSequenceTrackAllocator(nullptr), m_MmlSequenceTrackAllocator(&m_MmlParser), m_pSetupBuffer(nullptr),
      m_SetupBufferSize(0), m_SoundUserParamSize(0), m_pSoundDataManager(nullptr), m_pFilesHook(nullptr)
{
}

} // namespace snd
namespace ut {
// (the copies for the arrays of SoundInstanceManager::SortPriorityList)
// 0x007E5DF8 (name is ours)
template nw::ut::LinkList<nw::snd::internal::StreamSound, 248l>::LinkList();
// 0x007E5E10 (name is ours)
template nw::ut::LinkList<nw::snd::internal::SequenceSound, 248l>::LinkList();
// 0x007E5E40 (name is ours)
template nw::ut::LinkList<nw::snd::internal::WaveSound, 248l>::LinkList();
} // namespace ut
} // namespace nw
