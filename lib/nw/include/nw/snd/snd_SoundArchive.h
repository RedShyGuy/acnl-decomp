#pragma once

#include "decomp.h"
#include "nn/os/os_CriticalSection.h"
#include "nw/io/io_FileStream.h"
#include "nw/snd/snd_Global.h"

namespace nw {
namespace snd {
namespace internal {
class SoundArchiveFileReader;
template <typename T>
struct BinaryTable;
} // namespace internal

// RTTI N2nw3snd12SoundArchiveE @ 0x008D08E0
// vtable 0x00902EDC (vptr 0x00902EE4), offset_to_top 0, 6 entries
// A sound archive (BCSAR): the information of its items read through the file reader, an optional
// hook that can change it, and the streams of its files (the class, the info struct and the
// function names are from the reference symbols; the members are ours).
class SoundArchive
{
public:
    // the kind of an item (the top byte of an item id; names are ours)
    enum ItemType
    {
        ITEM_TYPE_SOUND = 1,
        ITEM_TYPE_SOUND_GROUP = 2,
        ITEM_TYPE_BANK = 3,
        ITEM_TYPE_PLAYER = 4,
        ITEM_TYPE_WAVE_ARCHIVE = 5,
        ITEM_TYPE_GROUP = 6,
    };
    static const u32 INVALID_ID = 0xFFFFFFFF;

    // (inline)
    static u32 GetItemType(u32 id) { return id >> 24; }
    static u32 GetItemIndex(u32 id) { return id & 0x00FFFFFF; }
    // the id of the item of a type with an index (name is ours)
    static u32 MakeItemId(ItemType type, u32 index) { return (static_cast<u32>(type) << 24) | index; }

    enum SoundType
    {
        SOUND_TYPE_INVALID = 0,
        SOUND_TYPE_SEQ = 1,
        SOUND_TYPE_STRM = 2,
        SOUND_TYPE_WAVE = 3,
    };

    // the priority of the file system requests of a stream (names are ours)
    enum FileStreamPriority
    {
        FILE_STREAM_PRIORITY_HIGH = 0,
        FILE_STREAM_PRIORITY_NORMAL = 1,
        FILE_STREAM_PRIORITY_LOW = 2,
    };

    class StreamTrackInfo;

    struct SoundInfo
    {
        u32 fileId;            // 0x00
        u32 playerId;          // 0x04
        u32 actorPlayerId;     // 0x08
        u32 playerPriority;    // 0x0C
        u32 volume;            // 0x10
        PanMode panMode;       // 0x14
        PanCurve panCurve;     // 0x15
        bool isFrontBypass;    // 0x16
    };
    struct SequenceSoundInfo
    {
        // (inline)
        SequenceSoundInfo() : startOffset(0), allocateTrackFlags(0), channelPriority(0), isReleasePriorityFix(false)
        {
            for (int i = 0; i < 4; ++i) {
                bankIds[i] = 0xFFFFFFFF;
            }
        }

        u32 startOffset;          // 0x00
        u32 bankIds[4];           // 0x04
        u32 allocateTrackFlags;   // 0x14
        u8 channelPriority;       // 0x18
        bool isReleasePriorityFix; // 0x19
    };
    struct WaveSoundInfo
    {
        // (inline)
        WaveSoundInfo() : allocateTrackCount(0), channelPriority(0), isReleasePriorityFix(false) {}

        u32 index;                // 0x0
        u32 allocateTrackCount;   // 0x4
        u8 channelPriority;       // 0x8
        bool isReleasePriorityFix; // 0x9
    };
    struct BankInfo
    {
        // (inline)
        BankInfo() : fileId(0xFFFFFFFF) {}

        u32 fileId; // 0x0
    };
    struct PlayerInfo
    {
        u32 playableSoundMax; // 0x0
        u32 playerHeapSize;   // 0x4
    };
    struct SoundGroupInfo
    {
        // (inline)
        SoundGroupInfo() : startId(0xFFFFFFFF), endId(0xFFFFFFFF), pFileIdTable(nullptr) {}

        u32 startId;                                // 0x0
        u32 endId;                                  // 0x4
        const internal::BinaryTable<u32>* pFileIdTable; // 0x8
    };
    struct GroupInfo
    {
        // (inline)
        GroupInfo() : fileId(0xFFFFFFFF), fileSize(0) {}

        u32 fileId;   // 0x0
        u32 fileSize; // 0x4 (-1: an external file)
    };
    struct WaveArchiveInfo
    {
        // (inline)
        WaveArchiveInfo() : fileId(0xFFFFFFFF), isLoadIndividual(false) {}

        u32 fileId;             // 0x0
        u32 waveCount;          // 0x4
        bool isLoadIndividual;  // 0x8
    };
    struct FileInfo
    {
        // (inline)
        FileInfo() : fileSize(0xFFFFFFFF), offsetFromFileBlockHead(0xFFFFFFFF), externalFilePath(nullptr) {}

        u32 fileSize;                // 0x0
        u32 offsetFromFileBlockHead; // 0x4 (-1: none)
        const char* externalFilePath; // 0x8
    };
    struct Sound3DInfo
    {
        u32 flags;          // 0x0
        f32 decayRatio;     // 0x4
        u8 decayCurve;      // 0x8
        u8 dopplerFactor;   // 0x9
    };
    struct SoundArchivePlayerInfo
    {
        u32 sequenceSoundMax;      // 0x00
        u32 sequenceTrackMax;      // 0x04
        u32 streamSoundMax;        // 0x08
        u32 streamTrackMax;        // 0x0C
        u32 streamChannelMax;      // 0x10
        u32 waveSoundMax;          // 0x14
        u32 waveTrackMax;          // 0x18
        u32 streamBufferTimes;     // 0x1C (1 before version 2.3.2.0)
    };
    // a track of a stream sound
    class StreamTrackInfo
    {
    public:
        StreamTrackInfo(); // 0x004C01D8 | fefates:bytes [tier B]

        u8 volume;             // 0x00
        u8 pan;                // 0x01
        u8 span;               // 0x02
        u8 flags;              // 0x03
        u8 mainSend;           // 0x04
        u8 fxSend[2];          // 0x05
        u8 lpfFreq;            // 0x07
        u8 biquadType;         // 0x08
        u8 biquadValue;        // 0x09
        u8 channelCount;       // 0x0A
        u8 globalChannelIndex[2]; // 0x0B
    };
    static const int STREAM_TRACK_MAX = 4;
    struct StreamSoundInfo
    {
        // (inline)
        StreamSoundInfo() : allocateTrackFlags(0), allocateChannelCount(0), pitch(1.0f), mainSend(127), streamFileType(0),
                            prefetchFileId(0xFFFFFFFF)
        {
            fxSend[0] = 0;
            fxSend[1] = 0;
        }

        u16 allocateTrackFlags;   // 0x00
        u16 allocateChannelCount; // 0x02
        StreamTrackInfo trackInfo[STREAM_TRACK_MAX]; // 0x04
        f32 pitch;                // 0x38
        u8 mainSend;              // 0x3C
        u8 fxSend[2];             // 0x3D
        u8 streamFileType;        // 0x3F
        u32 prefetchFileId;       // 0x40
    };
    struct StreamSoundInfo2
    {
        // (inline)
        StreamSoundInfo2() : isLoop(false), loopStartFrame(0), loopEndFrame(0) {}

        bool isLoop;       // 0x0
        u32 loopStartFrame; // 0x4
        u32 loopEndFrame;   // 0x8
    };

    // Changes the information of the items (the class and its members are ours; only the slots are
    // known): each Read function adds its result to the one of the file.
    class InfoHook
    {
    public:
        virtual ~InfoHook() {}
        virtual bool IsTargetItem(u32 id) = 0;
        virtual const char* GetItemLabel(u32 id) const = 0;
        virtual u32 GetItemId(const char* pLabel) const = 0;
        // the kind of the sound of a label (name is ours)
        virtual SoundType GetSoundType(const char* pLabel) const = 0;
        virtual bool ReadSoundInfo(u32 id, SoundInfo* pInfo) const = 0;
        virtual bool ReadBankInfo(u32 id, BankInfo* pInfo) const = 0;
        virtual bool ReadPlayerInfo(u32 id, PlayerInfo* pInfo) const = 0;
        virtual bool ReadSoundGroupInfo(u32 id, SoundGroupInfo* pInfo) const = 0;
        virtual bool ReadGroupInfo(u32 id, GroupInfo* pInfo) const = 0;
        virtual bool ReadWaveArchiveInfo(u32 id, WaveArchiveInfo* pInfo) const = 0;
        virtual bool ReadFileInfo(u32 id, FileInfo* pInfo, int index) const = 0;
        virtual bool ReadSoundArchivePlayerInfo(SoundArchivePlayerInfo* pInfo) const = 0;
        virtual bool ReadSound3DInfo(u32 id, Sound3DInfo* pInfo) const = 0;
        virtual bool ReadSequenceSoundInfo(u32 id, SequenceSoundInfo* pInfo) const = 0;
        virtual bool ReadStreamSoundInfo(u32 id, StreamSoundInfo* pInfo) const = 0;
        virtual bool ReadStreamSoundInfo2(u32 id, StreamSoundInfo2* pInfo) const = 0;
        virtual bool ReadWaveSoundInfo(u32 id, WaveSoundInfo* pInfo) const = 0;
        virtual bool ReadSoundUserParam(u32 id, int index, u32& value) const = 0;

        // (inline)
        bool IsEnabled() const { return m_IsEnabled; }

        bool m_IsEnabled; // 0x4
    };

    SoundArchive(); // 0x004C0304 | fefates:bytes [tier B]
    virtual ~SoundArchive(); // 0x004C0348 slot 0x00 | fefates:callgraph

    // the memory of a file of the archive (NULL: not in memory)
    virtual const void* detail_GetFileAddress(u32 fileId) const = 0; // slot 0x08
    // the size of the buffer a stream of the archive needs (name is ours)
    virtual size_t detail_GetRequiredStreamBufferSize() const = 0; // slot 0x0C
    // a stream of a part of the archive in the buffer
    virtual io::FileStream* OpenStream(void* pBuffer, int bufferSize, u32 begin, u32 length) = 0; // slot 0x10
    // a stream of an external file in the buffer
    virtual io::FileStream* OpenExtStream(void* pBuffer, int bufferSize, const char* pExtFilePath, void* pCacheBuffer,
                                          u32 cacheSize, FileStreamPriority priority) const = 0; // slot 0x14

    bool IsAvailable() const; // 0x00139ACC | nintendogs:callseq-callee [tier A]
    SoundType GetSoundType(u32 soundId) const; // 0x00139D4C | nintendogs:callgraph [tier A]
    bool ReadBankInfo(u32 bankId, BankInfo* pInfo) const; // 0x0013CF84 | nintendogs:callseq-callee [tier A]
    bool ReadSoundInfo(u32 soundId, SoundInfo* pInfo) const; // 0x0013CFF0 | nintendogs:callseq-callee [tier A]
    bool ReadSequenceSoundInfo(u32 soundId, SequenceSoundInfo* pInfo) const; // 0x0013D048 | nintendogs:callgraph [tier A]
    bool detail_ReadWaveSoundInfo(u32 soundId, WaveSoundInfo* pInfo) const; // 0x0013D0A0 | nintendogs:callgraph [tier A]
    bool detail_ReadSoundGroupInfo(u32 soundGroupId, SoundGroupInfo* pInfo) const; // 0x0013D0F8 | nintendogs:callgraph [tier A]
    u32 GetItemFileId(u32 id) const; // 0x0013D520 | nintendogs:callgraph [tier A]
    // the file of the prefetch data of a stream sound
    u32 GetItemPrefetchFileId(u32 id) const; // 0x0013D5D8 (name after the InfoBlockBody)
    bool ReadWaveArchiveInfo(u32 waveArchiveId, WaveArchiveInfo* pInfo) const; // 0x0013FA10 | nintendogs:callgraph [tier A]
    bool detail_ReadFileInfo(u32 fileId, FileInfo* pInfo) const; // 0x0013FA68 | nintendogs:callgraph [tier A]
    io::FileStream* detail_OpenFileStream(u32 fileId, void* pBuffer, int bufferSize, void* pCacheBuffer, u32 cacheSize,
                                          const char* pExtFilePath, FileStreamPriority priority); // 0x001410F4 | fefates:bytes [tier B]
    const internal::BinaryTable<u32>* detail_GetWaveArchiveIdTable(u32 id) const; // 0x0014200C | nintendogs:callgraph [tier A]
    io::FileStream* OpenExtStreamImpl(void* pBuffer, int bufferSize, const char* pExtFilePath, void* pCacheBuffer, u32 cacheSize,
                                      FileStreamPriority priority) const; // 0x00143980 | nintendogs:bytes-fuzzy [tier A]
    void Initialize(internal::SoundArchiveFileReader* pFileReader); // 0x004C01B4 | fefates:bytes [tier B]
    void SetExternalFileRoot(const char* pExtFileRoot); // 0x004C022C | nintendogs:bytes-fuzzy [tier A]
    void Finalize(); // 0x004C02EC | fefates:bytes [tier B]
    const char* GetItemLabel(u32 id) const; // 0x0073EAF4 | fefates:bytes [tier B]
    bool detail_ReadGroupInfo(u32 groupId, GroupInfo* pInfo) const; // 0x0073EB3C (name after the other Read functions)
    bool ReadPlayerInfo(u32 playerId, PlayerInfo* pInfo) const; // 0x0073EB94 | fefates:bytes [tier B]
    bool ReadSound3DInfo(u32 soundId, Sound3DInfo* pInfo) const; // 0x0073EBEC | fefates:bytes [tier B]
    bool ReadSoundUserParam(u32 soundId, int index, u32& value) const; // 0x0073EC44 | fefates:bytes [tier B]
    bool ReadStreamSoundInfo(u32 soundId, StreamSoundInfo* pInfo) const; // 0x0073ECA4 | fefates:bytes [tier B]
    bool ReadSoundArchivePlayerInfo(SoundArchivePlayerInfo* pInfo) const; // 0x0073ECFC | nintendogs:callgraph [tier A]
    bool detail_ReadStreamSoundInfo2(u32 soundId, StreamSoundInfo2* pInfo) const; // 0x0073ED4C | fefates:bytes [tier B]
    u32 GetItemId(const char* pLabel) const; // 0x0073EDAC | fefates:bytes [tier B]
    u32 detail_GetFileCount() const; // 0x00740C94 | nintendogs:callgraph [tier A]
    u32 GetGroupCount() const; // 0x00740CEC | nintendogs:callgraph [tier C]
    u32 GetSoundCount() const; // 0x00740D08 (name is ours)
    u32 GetPlayerCount() const; // 0x00740DB0 (name after GetGroupCount)
    u32 GetWaveArchiveCount() const; // 0x00740E78 | nintendogs:callseq-callee [tier A]

    static const int FILENAME_MAX = 256;

    internal::SoundArchiveFileReader* m_pFileReader; // 0x004
    InfoHook* m_pInfoHook;                           // 0x008
    char m_ExtFileRoot[FILENAME_MAX];                // 0x00C
    u32 m_FileBlockOffset;                           // 0x10C
    nn::os::CriticalSection m_FileOpenLock;          // 0x110
};
ASSERT_SIZE(SoundArchive, 0x11C);
} // namespace snd
} // namespace nw
