#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_StreamSoundLoader.h"
#include "nw/snd/internal/snd_BasicSound.h"
#include "nw/snd/snd_SoundArchive.h"

namespace nw {
namespace snd {
class SoundHandle;

// What can start sounds: the sound archive player (the class and function names are from the reference
// symbols, the result codes and the members of StartInfo are ours).
// RTTI N2nw3snd14SoundStartableE @ 0x008D08F4
class SoundStartable
{
public:
    // the results of starting a sound (names are ours, after the code that returns them)
    enum StartResult : u8
    {
        START_SUCCESS = 0,
        START_ERR_LOW_PRIORITY = 1,           // the players play sounds of a higher priority
        START_ERR_INVALID_LABEL = 2,          // no sound of the label
        START_ERR_INVALID_SOUND = 3,          // no information of the sound
        START_ERR_SEQ_NOT_LOADED = 5,         // the sequence is not loaded and the player has no heap
        START_ERR_WSD_NOT_LOADED = 7,         // the wave sound file is not loaded and the player has no heap
        START_ERR_WARC_NOT_LOADED = 8,        // the wave archive is not loaded and the player has no heap
        START_ERR_CANNOT_OPEN_STREAM = 10,
        START_ERR_NOT_AVAILABLE = 11,         // no sound archive
        START_ERR_NO_SOUND_INSTANCE = 13,     // all sounds of the kind play sounds of a higher priority
        START_ERR_INVALID_PLAYER = 14,
        START_ERR_INVALID_START_LABEL = 15,   // no start label of the sequence
        START_ERR_INVALID_WARC = 18,          // the wave archive of a bank (Util::GetWaveArchiveOfBank)
        START_ERR_UNKNOWN = 255,
    };

    // how a sound starts (names are ours)
    struct StartInfo
    {
        // the parameters that are set (enableFlag)
        enum EnableFlag
        {
            ENABLE_START_OFFSET = 1 << 0,
            ENABLE_PLAYER_ID = 1 << 1,
            ENABLE_PLAYER_PRIORITY = 1 << 2,
            ENABLE_ACTOR_PLAYER_ID = 1 << 3,
            ENABLE_SEQ_SOUND_INFO = 1 << 4,
            ENABLE_STRM_SOUND_INFO = 1 << 5,
            ENABLE_WAVE_SOUND_INFO = 1 << 6,
            ENABLE_HOLD_FADE_FRAMES = 1 << 7,
            ENABLE_STOP_CALLBACK = 1 << 8,
            ENABLE_STRM_META_INFO = 1 << 9,
            ENABLE_STRM_META_INFO2 = 1 << 10,
        };
        // the units of startOffset
        enum StartOffsetType : u8
        {
            START_OFFSET_TYPE_MILLISEC = 0,
            START_OFFSET_TYPE_TICK = 1,   // sequences
            START_OFFSET_TYPE_SAMPLE = 2, // wave sounds and streams
        };
        // another sequence data, start label and banks
        struct SeqSoundInfo
        {
            const void* pSeqFile;     // 0x00 (NULL: of the archive)
            const char* pStartLabel;  // 0x04 (NULL: the start of the sound)
            u32 bankIds[4];           // 0x08 (-1: of the archive)
        };
        // where the stream comes from
        struct StreamSoundInfo
        {
            const char* pExtFilePath;                       // 0x00
            const void* pData;                              // 0x04 the stream in memory
            u32 dataSize;                                   // 0x08
            internal::driver::StreamRegionCallback regionCallback; // 0x0C
            void* regionCallbackArg;                        // 0x10
            const void* pPrefetchData;                      // 0x14
        };
        // another wave of the wave sound
        struct WaveSoundInfo
        {
            const void* pWaveFile; // 0x0
            s8 waveType;           // 0x4
        };

        u32 enableFlag;                               // 0x00
        StartOffsetType startOffsetType;              // 0x04
        int startOffset;                              // 0x08
        u32 playerId;                                 // 0x0C
        int playerPriority;                           // 0x10
        u32 actorPlayerId;                            // 0x14
        SeqSoundInfo seqSoundInfo;                    // 0x18
        StreamSoundInfo streamSoundInfo;              // 0x30
        SoundArchive::StreamSoundInfo streamSoundMetaInfo;   // 0x48
        SoundArchive::StreamSoundInfo2 streamSoundMetaInfo2; // 0x8C
        WaveSoundInfo waveSoundInfo;                  // 0x98
        int holdFadeFrames;                           // 0xA0 (ENABLE_HOLD_FADE_FRAMES; HoldSound stops the sound with it)
        internal::BasicSound::StopCallback stopCallback; // 0xA4
    };

    virtual ~SoundStartable() {} // slot 0x00

    // the sound of the id is prepared for the handle
    virtual StartResult detail_SetupSound(SoundHandle* pHandle, u32 soundId, bool isHold, const StartInfo* pStartInfo) = 0; // slot 0x08
    virtual u32 detail_GetItemId(const char* pLabel) = 0; // slot 0x0C

    StartResult StartSound(SoundHandle* pHandle, u32 soundId, const StartInfo* pStartInfo); // 0x004C03F8 | nintendogs:bytes [tier A]
    StartResult StartSound(SoundHandle* pHandle, const char* pLabel, const StartInfo* pStartInfo); // 0x004C0384 | fefates:bytes [tier B]
    StartResult PrepareSound(SoundHandle* pHandle, u32 soundId, const StartInfo* pStartInfo); // 0x004C0438 | nintendogs:bytes [tier A]
    // starts the sound or keeps it playing for one more frame
    StartResult HoldSound(SoundHandle* pHandle, u32 soundId, const StartInfo* pStartInfo); // 0x004C04A4 (name is ours)
    StartResult HoldSound(SoundHandle* pHandle, const char* pLabel, const StartInfo* pStartInfo); // 0x004C0458 (name is ours)
};
} // namespace snd
} // namespace nw
