#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_Types.h"
#include "nw/snd/internal/driver/snd_BasicSoundPlayer.h"
#include "nw/snd/internal/driver/snd_MultiVoice.h"
#include "nw/snd/internal/driver/snd_SoundThread.h"
#include "nw/snd/internal/driver/snd_SoundThread_PlayerCallback.h"
#include "nw/snd/internal/driver/snd_StreamBufferPool.h"
#include "nw/snd/internal/driver/snd_StreamChannel.h"
#include "nw/snd/internal/driver/snd_StreamSoundLoader.h"
#include "nw/snd/internal/driver/snd_VoiceManager.h"
#include "nw/snd/internal/snd_StreamDataInfoDetail.h"
#include "nw/snd/internal/snd_StreamSoundPrefetchFileReader.h"
#include "nw/snd/snd_Global.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// A track of a stream: up to two channels and their parameters (the struct name is from the reference
// symbols, the members are ours). The parameters of the file are copies of StreamDataInfoDetail::TrackInfo.
struct StreamTrack
{
    static const int CHANNEL_MAX = 2;

    bool m_IsActive;                        // 0x00 allocated by the sound archive
    StreamChannel* m_pChannels[CHANNEL_MAX]; // 0x04
    u8 m_ChannelCount;                      // 0x0C
    u8 m_Volume;                            // 0x0D
    u8 m_Pan;                               // 0x0E
    u8 m_SurroundPan;                       // 0x0F
    u8 m_MainSend;                          // 0x10
    u8 m_FxSend[2];                         // 0x11
    u8 m_LpfFreq;                           // 0x13
    s8 m_BiquadType;                        // 0x14
    u8 m_BiquadValue;                       // 0x15
    u8 m_Flags;                             // 0x16 front bypass 1, 3D surround preprocessed 2
    u8 padding0x17;                         // 0x17
    f32 m_UserVolume;                       // 0x18 SetTrackVolume
    f32 m_UserPan;                          // 0x1C SetTrackPan
    f32 m_UserSurroundPan;                  // 0x20 SetTrackSurroundPan
};
ASSERT_SIZE(StreamTrack, 0x24);

// Plays a stream in the sound thread: the channels play the buffer blocks the loader fills in the task
// thread (the class and function names are from the reference symbols, the members and the names of the
// inline helpers are ours).
// RTTI N2nw3snd8internal6driver17StreamSoundPlayerE @ 0x008D0AE4
// vtable 0x00903408 (vptr 0x00903410), offset_to_top 0, 10 entries
// vtable 0x00903438 (vptr 0x00903440), offset_to_top -68, 5 entries
class StreamSoundPlayer : public ::nw::snd::internal::driver::BasicSoundPlayer,
                          public ::nw::snd::internal::driver::SoundThread::PlayerCallback
{
public:
    static const int CHANNEL_MAX = 8;
    static const int TRACK_MAX = 4;
    static const int BUFFER_BLOCK_COUNT_MIN = 4;
    static const int BUFFER_BLOCK_COUNT_MAX = 32;
    // the largest block of a stream (name is ours)
    static const u32 DATA_BLOCK_SIZE_MAX = 0x2000;
    // the priority of the voices: the highest (name is ours)
    static const int VOICE_PRIORITY = 0x7FFF;

    // (names are ours)
    enum StartOffsetType : u8
    {
        START_OFFSET_TYPE_SAMPLE = 0,
        START_OFFSET_TYPE_MILLISEC = 1,
    };
    // the results of Setup (names are ours)
    enum SetupResult
    {
        SETUP_SUCCESS = 0,
        SETUP_ERR_NO_TRACK = 2,
    };

    // what the sound archive player knows of the stream (names are ours)
    struct SetupArg
    {
        StreamBufferPool* pBufferPool;              // 0x00
        u32 unknown4;                               // 0x04
        u16 allocateTrackFlags;                     // 0x08
        u8 fileType;                                // 0x0A StreamSoundLoader::FileType
        bool isLoop;                                // 0x0B external
        StreamDataInfoDetail::TrackInfo trackInfo[TRACK_MAX]; // 0x0C
        u32 loopStart;                              // 0x40 external
        u32 loopEnd;                                // 0x44 external
        f32 pitch;                                  // 0x48
        u8 mainSend;                                // 0x4C
        u8 fxSend[2];                               // 0x4D
        u8 padding;                                 // 0x4F
    };
    ASSERT_SIZE(SetupArg, 0x50);
    // (names are ours)
    struct PrepareBaseArg
    {
        StartOffsetType startOffsetType;        // 0x0
        int startOffset;                        // 0x4
        StreamRegionCallback regionCallback;    // 0x8
        void* regionCallbackArg;                // 0xC
    };
    ASSERT_SIZE(PrepareBaseArg, 0x10);
    struct PrepareArg
    {
        PrepareBaseArg baseArg;      // 0x00
        StreamSoundLoader* pLoader;  // 0x10
    };
    ASSERT_SIZE(PrepareArg, 0x14);
    struct PreparePrefetchArg
    {
        PrepareBaseArg baseArg;      // 0x00
        const void* pPrefetchData;   // 0x10 a prefetch file (CSTP)
    };
    ASSERT_SIZE(PreparePrefetchArg, 0x14);
    // a loaded buffer block (names are ours)
    struct BlockInfo
    {
        u32 samplePosition; // 0x0
        u32 loopCount;      // 0x4
    };

    StreamSoundPlayer(); // 0x004D22B4 | fefates:bytes [tier B]
    virtual ~StreamSoundPlayer(); // 0x004D2324 slot 0x00 | fefates:bytes

    virtual void Initialize(); // 0x004D05D0 slot 0x08 | fefates:bytes
    virtual void Finalize(); // 0x004D21F0 slot 0x0C | fefates:bytes
    virtual void Start(); // 0x004D1E90 slot 0x10 | fefates:bytes
    virtual void Stop(); // 0x004D1C74 slot 0x14 | nintendogs:callseq
    virtual void Pause(bool isPause); // 0x004D1CE4 slot 0x18 | fefates:bytes
    virtual void OnUpdateFrame(); // 0x004D1FDC slot 0x1C
    virtual void OnUpdateBuffer(); // 0x004D0BB4 slot 0x20
    virtual void OnShutdown(); // 0x004D1C24 slot 0x24

    SetupResult Setup(const SetupArg& arg); // 0x004D1D64 | fefates:bytes [tier B]
    bool Prepare(const PrepareArg& arg); // 0x004D211C | fefates:bytes [tier B]
    void PreparePrefetch(const PreparePrefetchArg& arg); // 0x004D10E4 | fefates:bytes [tier B]
    // the commands of the task thread
    bool LoadHeader(bool result, nn::snd::CTR::AdpcmParam** ppAdpcmParams, u16 requestId); // 0x004D0694 | fefates:bytes [tier B]
    bool LoadStreamData(bool result, const LoadDataParam& param); // 0x004D0EB8 | fefates:bytes [tier B]
    // isPrefetch: the samples are in the prefetch data, at block prefetchBlockIndex of size prefetchBlockSize
    DECOMP_NOINLINE bool LoadStreamData(bool result, const LoadDataParam& param, bool isPrefetch, u32 prefetchBlockIndex,
                        u32 prefetchBlockSize); // 0x004D0ED8 | fefates:bytes [tier B]

    void Update(); // 0x004D1FE0 | fefates:bytes [tier B]
    DECOMP_NOINLINE void UpdateBuffer(); // 0x004D0BB8 | fefates:bytes [tier B]
    DECOMP_NOINLINE void UpdateVoiceParams(StreamTrack* pTrack); // 0x004D1318 | fefates:bytes [tier B]
    bool ApplyStreamDataInfo(const StreamDataInfoDetail& info); // 0x004D1B14 | fefates:bytes [tier B]
    void ApplyTrackDataInfo(const StreamDataInfoDetail& info); // 0x004D1650 | fefates:bytes [tier B]
    bool LoadPrefetchBlocks(StreamSoundPrefetchFileReader& reader); // 0x004D1774 | fefates:bytes [tier B]
    static void VoiceCallbackFunc(MultiVoice* pVoice, MultiVoice::VoiceCallbackStatus status, void* pArg); // 0x004D1618 | fefates:bytes [tier B]

    // the tracks of a bit field
    void SetTrackVolume(u32 trackBitFlag, f32 volume); // 0x004D10A4 | fefates:bytes [tier B]
    void SetTrackInitialVolume(u32 trackBitFlag, u32 volume); // 0x004D1C30 | fefates:bytes [tier B]
    void SetTrackPan(u32 trackBitFlag, f32 pan); // 0x004D0B74 | fefates:bytes [tier B]
    void SetTrackSurroundPan(u32 trackBitFlag, f32 surroundPan); // 0x004D1BE4 | fefates:bytes [tier B]

    // (game threads, under the lock of the sound thread)
    bool ReadStreamDataInfo(StreamDataInfo* pInfo) const; // 0x00741554 | fefates:bytes [tier B]
    // isOriginal: in the samples of the original loop (-1: not playing)
    long GetPlaySamplePosition(bool isOriginal) const; // 0x007415C8 | fefates:bytes [tier B]

    // (inline; name is ours) the start offset in samples
    u32 GetStartOffsetSamples(u32 sampleRate) const
    {
        u32 offset = 0;
        if (m_StartOffsetType == START_OFFSET_TYPE_SAMPLE) {
            offset = m_StartOffset;
        } else if (m_StartOffsetType == START_OFFSET_TYPE_MILLISEC) {
            const u64 samples = static_cast<u64>(static_cast<s64>(m_StartOffset) * sampleRate) / 1000;
            offset = samples > 0xFFFFFFFF ? 0xFFFFFFFF : static_cast<u32>(samples);
        }
        return offset;
    }
    // (inline; name is ours) the voices are paused by the user or while the loader is late
    void UpdatePauseStatus()
    {
        const bool isPause = m_IsPause || m_IsLoadingDelayState;
        if (isPause != m_IsPausing) {
            for (u32 i = 0; i < m_ChannelCount; i++) {
                MultiVoice* pVoice = m_Channels[i].m_pVoice;
                if (pVoice != nullptr) {
                    pVoice->Pause(isPause);
                }
            }
            m_IsPausing = isPause;
        }
    }
    // (inline; name is ours) the next buffer block is loaded
    void RequestLoadData()
    {
        void* buffers[CHANNEL_MAX];
        for (u32 i = 0; i < m_ChannelCount; i++) {
            buffers[i] = static_cast<u8*>(m_Channels[i].m_pBuffer) + m_DataInfo.blockSize * m_LoadingBufferBlockIndex;
        }
        const u32 startOffset = GetStartOffsetSamples(m_DataInfo.sampleRate);
        const u32 prefetchOffset = m_PrefetchOffset;
        m_StartOffset = 0;
        m_PrefetchOffset = 0;
        m_pLoader->RequestLoadData(buffers, m_LoadingBufferBlockIndex, startOffset, prefetchOffset,
                                   m_IsStarted ? TaskManager::PRIORITY_HIGH : TaskManager::PRIORITY_MIDDLE,
                                   m_IsLoopFromStart);
        if (++m_LoadingBufferBlockIndex >= m_BufferBlockCount) {
            m_LoadingBufferBlockIndex = 0;
        }
    }
    // (inline; name is ours) the number of the buffer blocks
    bool SetupBufferBlocks()
    {
        if (m_DataInfo.blockSize > DATA_BLOCK_SIZE_MAX) {
            return false;
        }
        m_BufferBlockCount = m_pBufferPool->m_BlockSize / m_DataInfo.blockSize;
        if (m_BufferBlockCount < BUFFER_BLOCK_COUNT_MIN) {
            return false;
        }
        if (m_BufferBlockCount > BUFFER_BLOCK_COUNT_MAX) {
            m_BufferBlockCount = BUFFER_BLOCK_COUNT_MAX;
        }
        m_LoadingBufferBlockIndex = 0;
        m_PlayingBufferBlockIndex = 0;
        m_PlaySamplePosition = 0;
        m_OriginalPlaySamplePosition = 0;
        return true;
    }
    // (inline; name is ours) a voice per channel
    bool AllocVoices()
    {
        if (m_ChannelCount > static_cast<u32>(VoiceManager::GetInstance().m_VoiceCountLimit)) {
            return false;
        }
        for (u32 i = 0; i < m_ChannelCount; i++) {
            MultiVoice* pVoice = VoiceManager::GetInstance().AllocVoice(1, VOICE_PRIORITY, VoiceCallbackFunc, &m_Channels[i]);
            if (pVoice == nullptr) {
                for (u32 j = 0; j < i; j++) {
                    if (m_Channels[j].m_pVoice != nullptr) {
                        m_Channels[j].m_pVoice->Free();
                        m_Channels[j].m_pVoice = nullptr;
                    }
                }
                return false;
            }
            m_Channels[i].m_pVoice = pVoice;
        }
        return true;
    }
    // (inline; names are ours)
    void FreeBuffers()
    {
        for (u32 i = 0; i < m_ChannelCount; i++) {
            if (m_Channels[i].m_pBuffer != nullptr) {
                m_pBufferPool->Free(m_Channels[i].m_pBuffer);
                m_Channels[i].m_pBuffer = nullptr;
            }
        }
    }
    void FreeVoices()
    {
        for (u32 i = 0; i < m_ChannelCount; i++) {
            if (m_Channels[i].m_pVoice != nullptr) {
                m_Channels[i].m_pVoice->Free();
                m_Channels[i].m_pVoice = nullptr;
            }
        }
    }

    bool m_IsInitialized;                     // 0x050 Setup
    bool m_IsPrepared;                        // 0x051 the first blocks are loaded
    bool m_IsFinalizing;                      // 0x052 the loader stops loading
    bool m_IsPrefetch;                        // 0x053
    s8 m_IsPausing;                           // 0x054 the voices are paused
    bool m_IsLoadingDelayState;               // 0x055 waiting for the loader
    bool m_IsLoadFinished;                    // 0x056 the last block is loaded
    bool m_IsLoadingDelay;                    // 0x057 the loader was late this frame
    u32 m_LoopCounter;                        // 0x058
    u32 m_PrepareCounter;                     // 0x05C the blocks to load until prepared
    StreamSoundLoader* m_pLoader;             // 0x060 of the sound
    StreamBufferPool* m_pBufferPool;          // 0x064
    u32 m_BufferBlockCount;                   // 0x068
    u32 m_LoadingBufferBlockIndex;            // 0x06C
    u32 m_PlayingBufferBlockIndex;            // 0x070
    u32 m_PlaySamplePosition;                 // 0x074
    u32 m_OriginalPlaySamplePosition;         // 0x078 in the original loop
    StartOffsetType m_StartOffsetType;        // 0x07C
    int m_StartOffset;                        // 0x080
    s8 m_IsLoopFromStart;                     // 0x084 (command 44)
    StreamDataInfoDetail m_DataInfo;          // 0x088
    u8 m_FileType;                            // 0x0F0 StreamSoundLoader::FileType
    bool m_IsLoop;                            // 0x0F1 external
    u32 m_LoopStart;                          // 0x0F4 external
    u32 m_LoopEnd;                            // 0x0F8 external
    f32 m_StreamPitch;                        // 0x0FC
    f32 m_StreamMainSend;                     // 0x100
    f32 m_StreamFxSend[2];                    // 0x104
    const void* m_pPrefetchData;              // 0x10C
    nn::snd::CTR::AdpcmParam m_AdpcmParams[CHANNEL_MAX]; // 0x110 of the prefetch data
    StreamSoundPrefetchFileReader::PrefetchDataInfo m_PrefetchDataInfo; // 0x210
    u32 m_PrefetchOffset;                     // 0x21C the samples in the prefetch data
    u32 m_ChannelCount;                       // 0x220
    u32 m_TrackCount;                         // 0x224
    StreamChannel m_Channels[CHANNEL_MAX];    // 0x228
    StreamTrack m_Tracks[TRACK_MAX];          // 0x2068
    BlockInfo m_BlockInfos[BUFFER_BLOCK_COUNT_MAX]; // 0x20F8
    u16 m_RequestId;                          // 0x21F8
    u8 padding0x21FA[2];                      // 0x21FA
};
ASSERT_SIZE(StreamSoundPlayer, 0x21FC);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
