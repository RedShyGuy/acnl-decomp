#pragma once

#include "decomp.h"
#include "nw/io/io_FileStream.h"
#include "nw/snd/internal/driver/snd_StreamSoundLoader.h"
#include "nw/snd/internal/driver/snd_StreamSoundPlayer.h"
#include "nw/snd/internal/snd_BasicSound.h"
#include "nw/snd/internal/snd_MoveValue.h"
#include "nw/snd/internal/snd_SoundInstanceManager.h"

namespace nw {
namespace snd {
class StreamSoundHandle;

namespace internal {
// A stream sound: the BasicSound with its StreamSoundPlayer and the loader of the stream (the class and
// function names are from the reference symbols, the members and the names of the unnamed functions are
// ours).
// RTTI N2nw3snd8internal11StreamSoundE @ 0x008D0990
// vtable 0x00903054 (vptr 0x0090305C), offset_to_top 0, 13 entries
class StreamSound : public ::nw::snd::internal::BasicSound
{
public:
    typedef SoundInstanceManager<StreamSound> StreamSoundInstanceManager;

    static const int TRACK_MAX = driver::StreamSoundPlayer::TRACK_MAX;
    // the buffer of the file stream of the sound archive
    static const u32 FILE_STREAM_BUFFER_SIZE = 0x200; // (name is ours)

    static const TypeInfo s_TypeInfo;

    explicit StreamSound(StreamSoundInstanceManager& manager); // 0x004C675C | fefates:bytes [tier B]
    virtual const TypeInfo* GetRuntimeTypeInfo() const; // 0x0073F380 slot 0x00 (name after nw::gfx)
    virtual ~StreamSound(); // 0x004C6818 slot 0x04 | fefates:bytes

    virtual void Initialize(); // 0x004C6168 slot 0x0C | nintendogs:callseq
    virtual void Finalize(); // 0x004C66EC slot 0x10 | fefates:bytes
    virtual bool IsPrepared() const; // 0x0073F344 slot 0x14 | fefates:bytes
    virtual bool IsAttachedTempSpecialHandle(); // 0x004C660C slot 0x18 | fefates:bytes
    virtual void DetachTempSpecialHandle(); // 0x004C08B0 slot 0x1C
    virtual driver::BasicSoundPlayer* GetBasicSoundPlayerHandle(); // 0x004C6604 slot 0x20
    virtual void OnUpdatePlayerPriority(); // 0x004C655C slot 0x24 | fefates:bytes
    virtual void UpdateMoveValue(); // 0x004C644C slot 0x28 | fefates:bytes
    virtual void UpdateParam(); // 0x004C6278 slot 0x30 | mk7dlp:bytes-fuzzy

    void Setup(const driver::StreamSoundPlayer::SetupArg& arg); // 0x004C6620 | fefates:bytes [tier B]
    void Prepare(io::FileStream* pFileStream, const driver::StreamSoundPlayer::PrepareBaseArg& arg); // 0x004C6688 | fefates:bytes [tier B]
    void PreparePrefetch(const void* pPrefetchData, const driver::StreamSoundPlayer::PrepareBaseArg& arg); // 0x004C63F4 | fefates:bytes [tier B]

    // the tracks of a bit field
    void SetTrackVolume(u32 trackBitFlag, f32 volume, int frames); // 0x004C6340 | nintendogs:bytes-fuzzy [tier A]
    void SetTrackPan(u32 trackBitFlag, f32 pan); // 0x004C6220 (name after the player)
    void SetTrackSurroundPan(u32 trackBitFlag, f32 surroundPan); // 0x004C6504 (name after the player)

    // (for StreamSoundHandle; -1: not playing)
    long GetPlayLoopCount() const; // 0x0073F35C (name is ours)
    bool ReadStreamDataInfo(StreamDataInfo* pInfo) const; // 0x0073F38C (name after the player)
    long GetPlaySamplePosition(bool isOriginal) const; // 0x0073F3A8 (name after the player)

    driver::StreamSoundPlayer m_StreamSoundPlayer;          // 0x0118
    driver::StreamSoundLoader m_StreamSoundLoader;          // 0x2314
    StreamSoundHandle* m_pTempSpecialHandle;                // 0x3040
    StreamSoundInstanceManager* m_pManager;                 // 0x3044
    MoveValue<f32, int> m_TrackVolumes[TRACK_MAX];          // 0x3048
    u32 m_Unknown0x3088;                                    // 0x3088
    u8 m_FileStreamBuffer[FILE_STREAM_BUFFER_SIZE];         // 0x308C
    u16 m_AllocTrackFlag;                                   // 0x328C
    bool m_InitializeFlag;                                  // 0x328E
    u8 padding0x328F;                                       // 0x328F
    void* m_pCacheBuffer;                                   // 0x3290 of the file stream
    u32 m_CacheSize;                                        // 0x3294
};
ASSERT_SIZE(StreamSound, 0x3298);
} // namespace internal
} // namespace snd
} // namespace nw
