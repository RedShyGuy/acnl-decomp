#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_Types.h"
#include "nw/io/io_FileStream.h"
#include "nw/snd/internal/snd_PoolImpl.h"
#include "nw/snd/internal/snd_StreamDataInfoDetail.h"
#include "nw/snd/internal/snd_StreamSoundFileLoader.h"
#include "nw/snd/internal/snd_Task.h"
#include "nw/snd/internal/snd_TaskManager.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace snd {
namespace internal {
class StreamSoundFileReader;
struct DriverCommandStreamSoundLoadHeader;
struct DriverCommandStreamSoundLoadData;

// A block of a stream the task thread loaded, for StreamSoundPlayer::LoadStreamData (the struct name is
// from the reference symbols, the members are ours).
struct LoadDataParam
{
    static const int CHANNEL_MAX = 8;

    // (inline)
    LoadDataParam()
        : blockIndex(0), sampleCount(0), samplePosition(0), sampleOffset(0), isAdpcmContextValid(false),
          loopCount(0), isLastBlock(false)
    {
    }

    u32 blockIndex;              // 0x00 the buffer block of the player
    u32 sampleCount;             // 0x04 (0: nothing loaded)
    u32 samplePosition;          // 0x08 the position of the first sample in the stream
    u32 sampleOffset;            // 0x0C the samples to skip at the start of the block
    bool isAdpcmContextValid;    // 0x10
    u8 padding0x11;              // 0x11
    nn::snd::CTR::AdpcmContext adpcmContext[CHANNEL_MAX]; // 0x12 the state at the start of the block
    u8 padding0x42[2];           // 0x42
    u32 loopCount;               // 0x44 the loops while loading the block
    bool isLastBlock;            // 0x48
};
ASSERT_SIZE(LoadDataParam, 0x4C);

namespace driver {
class StreamSoundPlayer;

// asks the game for the region of a stream to play next (false: the stream ends; names are ours)
typedef bool (*StreamRegionCallback)(u32* pRegionIndex, void* pArg);

// Reads a stream that is not a BCSTM file (file type 1; the class and all names are ours): the game
// sets it, the samples are PCM16 blocks of 2048 bytes.
class ExternalStreamFileReader
{
public:
    struct HeaderInfo
    {
        u32 channelCount;     // 0x0
        u32 sampleRate;       // 0x4
        u32 blockSampleCount; // 0x8
    };

    virtual ~ExternalStreamFileReader() {} // slot 0x00

    virtual bool ReadHeader(io::FileStream* pStream, HeaderInfo* pInfo) = 0; // slot 0x08
    virtual void SkipBlock(io::FileStream* pStream) = 0; // slot 0x0C
    // a block of each channel
    virtual bool ReadBlock(io::FileStream* pStream, u32 channelCount, void** buffers) = 0; // slot 0x10
};

extern ExternalStreamFileReader* g_pExternalStreamFileReader; // (name is ours)

// Loads a stream in the task thread: the header, then a block of all channels per task into the buffer
// blocks of the player, which gets the results as commands 42 and 43 (the class and function names are
// from the reference symbols, the members are ours). A member of the StreamSound.
class StreamSoundLoader
{
public:
    // the file types (names are ours)
    enum FileType : u8
    {
        FILE_TYPE_BCSTM = 0,
        FILE_TYPE_EXTERNAL = 1, // ExternalStreamFileReader
    };
    static const int CHANNEL_MAX = 8;
    static const int DATA_LOAD_TASK_COUNT = 32;
    // the read buffer of the tasks: two blocks
    static const u32 LOAD_BUFFER_SIZE = 0x4000; // (name is ours)
    // the block size of the external streams
    static const u32 EXTERNAL_BLOCK_SIZE = 2048; // (name is ours)
    static const u32 EXTERNAL_LAST_BLOCK_SAMPLE_COUNT = 1024; // (name is ours)

    // the ADPCM state of a channel (names are ours)
    struct AdpcmInfo
    {
        nn::snd::CTR::AdpcmParam param;          // 0x00
        nn::snd::CTR::AdpcmContext context;      // 0x20 at the start
        nn::snd::CTR::AdpcmContext loopContext;  // 0x26 at the loop start
    };
    ASSERT_SIZE(AdpcmInfo, 0x2C);

    // (the members are ours)
    // RTTI N2nw3snd8internal6driver17StreamSoundLoader20StreamHeaderLoadTaskE @ 0x008D0AD8
    // vtable 0x009033F4 (vptr 0x009033FC), offset_to_top 0, 3 entries
    class StreamHeaderLoadTask : public ::nw::snd::internal::Task
    {
    public:
        // (inline)
        StreamHeaderLoadTask() {}
        virtual ~StreamHeaderLoadTask(); // 0x004CF724 slot 0x00

        virtual void Execute(); // 0x004CF680 slot 0x08 | fefates:bytes

        StreamSoundLoader* m_pLoader; // 0x1C
    };
    ASSERT_SIZE(StreamHeaderLoadTask, 0x20);

    // (the members are ours)
    // RTTI N2nw3snd8internal6driver17StreamSoundLoader18StreamDataLoadTaskE @ 0x008D0ACC
    // vtable 0x009033E0 (vptr 0x009033E8), offset_to_top 0, 3 entries
    class StreamDataLoadTask : public ::nw::snd::internal::Task
    {
    public:
        // implicit (new T() clears the object first), out of line for the array of the loader
        DECOMP_NOINLINE StreamDataLoadTask() = default; // 0x004CF640
        virtual ~StreamDataLoadTask(); // 0x004CF67C slot 0x00

        virtual void Execute(); // 0x004CF548 slot 0x08 | fefates:bytes

        void* m_Buffers[CHANNEL_MAX];  // 0x1C
        u32 m_BlockIndex;              // 0x3C
        u32 m_StartOffset;             // 0x40
        u32 m_PrefetchOffset;          // 0x44
        s8 m_IsLoopFromStart;          // 0x48
        StreamSoundLoader* m_pLoader;  // 0x4C
        ut::LinkListNode m_Link;       // 0x50
    };
    ASSERT_SIZE(StreamDataLoadTask, 0x58);

    typedef ut::LinkList<StreamDataLoadTask, 0x50> StreamDataLoadTaskList;

    StreamSoundLoader(); // 0x004D0510 (symbols.json: StreamSoundPlayer::StreamSoundPlayer)
    ~StreamSoundLoader(); // 0x004D058C | fefates:bytes [tier B]

    void Initialize(); // 0x004CEC48 | fefates:bytes [tier B]
    // the tasks are taken out of the task thread
    void Finalize(); // 0x004CF8F8 | fefates:bytes [tier B]
    void CancelRequest(); // 0x004CF22C | fefates:bytes [tier B]
    // waits for the tasks and closes the stream
    void WaitFinalize(); // 0x004CF194 | fefates:bytes [tier B]
    // frees the finished tasks (sound thread)
    void Update(); // 0x004CF88C | fefates:bytes [tier B]
    bool IsBusy() const; // 0x00741534 | fefates:bytes [tier B]

    void RequestLoadHeader(); // 0x004C6858 | fefates:bytes [tier B]
    // a task that loads the next block into the buffers (symbols.json: StreamSoundPlayer::UpdateLoadingBlockIndex)
    void RequestLoadData(void** buffers, u32 blockIndex, u32 startOffset, u32 prefetchOffset,
                         TaskManager::TaskPriority priority, s8 isLoopFromStart); // 0x004CF244 | fefates:bytes

    bool LoadHeader1(DriverCommandStreamSoundLoadHeader* pCommand); // 0x004CEC90 | fefates:bytes [tier B]
    bool LoadHeader2(DriverCommandStreamSoundLoadHeader* pCommand); // 0x004CEFB0 | fefates:bytes [tier B]
    bool LoadData1(DriverCommandStreamSoundLoadData* pCommand, void** buffers, u32 blockIndex, u32 startOffset,
                   u32 prefetchOffset); // 0x004CF910 | fefates:bytes [tier B]
    bool LoadData2(DriverCommandStreamSoundLoadData* pCommand, void** buffers, u32 blockIndex, u32 startOffset,
                   u32 prefetchOffset); // 0x004D00EC | fefates:bytes [tier B]

    // moves the position by offset samples over the ends of the regions
    bool ApplyStartOffset(u32 offset, u32* pLoopCount); // 0x004CF358 | fefates:bytes [tier B]
    void ChangeRegionInfo(u32 regionIndex); // 0x004CF490 | fefates:bytes [tier B]
    bool ReadTrackInfoFromStreamSoundFile(StreamSoundFileReader& reader); // 0x004CF728 | fefates:bytes [tier B]

    // (inline; name is ours) at the end of the region: the next one (false: the stream ends)
    bool MoveToNextRegion()
    {
        if (m_IsLoopFromStart) {
            m_RegionStart = 0;
            m_RegionEnd = m_pDataInfo->frameCount;
            m_Position = 0;
        } else if (m_pDataInfo->isLoop) {
            m_RegionStart = m_pDataInfo->loopStart;
            m_RegionEnd = m_pDataInfo->frameCount;
            m_Position = m_RegionStart;
        } else {
            u32 regionIndex = m_RegionIndex;
            if (m_RegionCallback == nullptr || !m_RegionCallback(&regionIndex, m_RegionCallbackArg)) {
                m_Position = m_RegionEnd;
                m_IsLoadEnd = true;
                return false;
            }
            m_RegionIndex = regionIndex;
            ChangeRegionInfo(regionIndex);
            m_Position = m_RegionStart;
        }
        return true;
    }
    // (inline; name is ours) BCSTM: the block of the position, the stream at its data
    void SeekToPositionBlock()
    {
        m_LoadingBlockIndex = m_Position / m_pDataInfo->blockSampleCount;
        m_pFileStream->Seek(m_DataOffset + m_pDataInfo->blockSize * m_ChannelCount * m_LoadingBlockIndex,
                            io::FileStream::SEEK_ORIGIN_BEGIN);
    }
    // (inline; name is ours) external: the block of the position, the stream skips the blocks before it
    void SkipToPositionBlock()
    {
        m_LoadingBlockIndex = m_Position / m_pDataInfo->blockSampleCount;
        m_pFileStream->Seek(0, io::FileStream::SEEK_ORIGIN_BEGIN);
        for (u32 i = 0; i < m_LoadingBlockIndex; i++) {
            if (m_LoopStartBlockIndex != 0 && m_LoopStartBlockIndex - 1 == i) {
                m_LoopStartFilePosition = m_pFileStream->Tell();
            }
            g_pExternalStreamFileReader->SkipBlock(m_pFileStream);
        }
    }
    // (inline; name is ours) external: remembers where the block before the loop start ends
    void UpdateLoopStartFilePosition()
    {
        if (m_LoopStartBlockIndex != 0 && m_LoopStartBlockIndex - 1 == m_LoadingBlockIndex) {
            m_LoopStartFilePosition = m_pFileStream->Tell();
        }
    }

    StreamSoundFileLoader m_FileLoader;      // 0x000
    StreamSoundPlayer* m_pPlayer;            // 0x010
    io::FileStream* m_pFileStream;           // 0x014
    StreamDataInfoDetail* m_pDataInfo;       // 0x018 of the player
    FileType m_FileType;                     // 0x01C
    u8 padding0x1D[3];                       // 0x01D
    u32 m_ChannelCount;                      // 0x020
    u16 m_RequestId;                         // 0x024 the header command carries it
    bool m_IsLoop;                           // 0x026 external
    s8 m_IsLoopFromStart;                    // 0x027 at the end of a region the stream starts again at 0
    u32 m_LoopStart;                         // 0x028 external
    u32 m_LoopEnd;                           // 0x02C external
    StreamRegionCallback m_RegionCallback;   // 0x030
    void* m_RegionCallbackArg;               // 0x034
    u32 m_LoadingBlockIndex;                 // 0x038
    u32 m_LastBlockIndex;                    // 0x03C
    u32 m_LoopStartBlockIndex;               // 0x040
    u32 m_DataOffset;                        // 0x044 BCSTM: the samples in the file
    u32 m_LoopStartFilePosition;             // 0x048 the block of the loop start in the file
    u32 m_LoopStartInBlockOffset;            // 0x04C external
    bool m_IsLoopJumped;                     // 0x050 external: the stream went back to the loop start
    bool m_IsLoadEnd;                        // 0x051
    u8 padding0x52[2];                       // 0x052
    u32 m_RegionIndex;                       // 0x054
    u32 m_Position;                          // 0x058 the next sample to load
    u32 m_RegionStart;                       // 0x05C
    u32 m_RegionEnd;                         // 0x060
    StreamHeaderLoadTask m_HeaderLoadTask;   // 0x064
    StreamDataLoadTaskList m_DataLoadTaskList; // 0x084
    PoolImpl m_DataLoadTaskPool;             // 0x090
    StreamDataLoadTask m_DataLoadTasks[DATA_LOAD_TASK_COUNT]; // 0x094
    SampleFormat m_SampleFormat;             // 0xB94
    u8 padding0xB95;                         // 0xB95
    AdpcmInfo m_AdpcmInfos[CHANNEL_MAX];     // 0xB96
    u8 padding0xCF6[2];                      // 0xCF6
    u32 m_AdpcmContextPosition;              // 0xCF8 the position m_AdpcmContexts belong to
    nn::snd::CTR::AdpcmContext m_AdpcmContexts[CHANNEL_MAX]; // 0xCFC
};
ASSERT_SIZE(StreamSoundLoader, 0xD2C);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
