#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_SequenceSoundPlayer.h"
#include "nw/snd/internal/driver/snd_StreamSoundLoader.h"
#include "nw/snd/internal/driver/snd_StreamSoundPlayer.h"
#include "nw/snd/internal/driver/snd_WaveSoundLoader.h"
#include "nw/snd/internal/driver/snd_WaveSoundPlayer.h"
#include "nw/snd/internal/snd_DriverCommand.h"

namespace nw {
namespace snd {
namespace internal {
// The commands for the players of the kinds of sounds (all names are ours; the ids are in DriverCommand).

// a wave sound player with prepared data (34)
struct DriverCommandWaveSoundPrepare : DriverCommand
{
    driver::WaveSoundPlayer* player;               // 0x10
    driver::WaveSoundPlayer::StartInfo startInfo;  // 0x14
    driver::WaveSoundPlayer::PrepareArg arg;       // 0x20
};
ASSERT_SIZE(DriverCommandWaveSoundPrepare, 0x2C);

// a wave sound player that loads its data (35)
struct DriverCommandWaveSoundLoad : DriverCommand
{
    driver::WaveSoundPlayer* player;               // 0x10
    driver::WaveSoundPlayer::StartInfo startInfo;  // 0x14
    driver::WaveSoundLoader::Arg arg;              // 0x20
};
ASSERT_SIZE(DriverCommandWaveSoundLoad, 0x38);

// the channel parameters of a wave sound player (37)
struct DriverCommandWaveSoundChannelParam : DriverCommand
{
    driver::WaveSoundPlayer* player; // 0x10
    u8 priority;                     // 0x14
    bool isReleasePriorityFix;       // 0x15
    u8 padding[2];                   // 0x16
};
ASSERT_SIZE(DriverCommandWaveSoundChannelParam, 0x18);

// the tracks and callbacks of a sequence sound player (13)
struct DriverCommandSequenceSoundSetup : DriverCommand
{
    driver::SequenceSoundPlayer* player;            // 0x10
    driver::SequenceSoundPlayer::SetupArg arg;      // 0x14
    u8 channelPriority;                             // 0x20
    bool isReleasePriorityFix;                      // 0x21
    u8 padding[2];                                  // 0x22
    SequenceUserprocCallback userproc;              // 0x24
    void* userprocArg;                              // 0x28
};
ASSERT_SIZE(DriverCommandSequenceSoundSetup, 0x2C);

// a sequence sound player that loads its data (14)
struct DriverCommandSequenceSoundLoad : DriverCommand
{
    driver::SequenceSoundPlayer* player;               // 0x10
    driver::SequenceSoundPlayer::StartInfo startInfo;  // 0x14
    driver::SequenceSoundLoader::Arg arg;              // 0x20
};
ASSERT_SIZE(DriverCommandSequenceSoundLoad, 0x54);

// a sequence sound player with prepared data (15)
struct DriverCommandSequenceSoundPrepare : DriverCommand
{
    driver::SequenceSoundPlayer* player;               // 0x10
    driver::SequenceSoundPlayer::PrepareArg arg;       // 0x14
};
ASSERT_SIZE(DriverCommandSequenceSoundPrepare, 0x40);

// a skip of a sequence sound player (16)
struct DriverCommandSequenceSoundSkip : DriverCommand
{
    driver::SequenceSoundPlayer* player; // 0x10
    int offsetType;                      // 0x14 SequenceSoundPlayer::OffsetType
    int offset;                          // 0x18
};
ASSERT_SIZE(DriverCommandSequenceSoundSkip, 0x1C);

// the tempo ratio of a sequence sound player (17)
struct DriverCommandSequenceSoundTempoRatio : DriverCommand
{
    driver::SequenceSoundPlayer* player; // 0x10
    f32 tempoRatio;                      // 0x14
};
ASSERT_SIZE(DriverCommandSequenceSoundTempoRatio, 0x18);

// the channel priority of a sequence sound player (18)
struct DriverCommandSequenceSoundChannelPrio : DriverCommand
{
    driver::SequenceSoundPlayer* player; // 0x10
    u8 priority;                         // 0x14
    u8 padding[3];                       // 0x15
};
ASSERT_SIZE(DriverCommandSequenceSoundChannelPrio, 0x18);

// a variable of a sequence sound player, a track or the global ones (19, 20, 21)
struct DriverCommandSequenceSoundSetVar : DriverCommand
{
    driver::SequenceSoundPlayer* player; // 0x10 (not for the global ones)
    int trackNo;                         // 0x14 (only for a track)
    int varNo;                           // 0x18
    int value;                           // 0x1C
};
ASSERT_SIZE(DriverCommandSequenceSoundSetVar, 0x20);

// the mute of the tracks of a sequence sound player (22)
struct DriverCommandSequenceTrackMute : DriverCommand
{
    driver::SequenceSoundPlayer* player; // 0x10
    u32 trackBitFlag;                    // 0x14
    int mute;                            // 0x18 SeqMute
};
ASSERT_SIZE(DriverCommandSequenceTrackMute, 0x1C);

// the silence of the tracks of a sequence sound player (23)
struct DriverCommandSequenceTrackSilence : DriverCommand
{
    driver::SequenceSoundPlayer* player; // 0x10
    u32 trackBitFlag;                    // 0x14
    bool isSilence;                      // 0x18
    u8 padding[3];                       // 0x19
    int fadeTimes;                       // 0x1C
};
ASSERT_SIZE(DriverCommandSequenceTrackSilence, 0x20);

// a parameter of the tracks of a sequence sound player (24..)
struct DriverCommandSequenceTrackParam : DriverCommand
{
    driver::SequenceSoundPlayer* player; // 0x10
    u32 trackBitFlag;                    // 0x14
    f32 value;                           // 0x18
};
ASSERT_SIZE(DriverCommandSequenceTrackParam, 0x1C);

// the biquad filter of the tracks of a sequence sound player (30)
struct DriverCommandSequenceTrackBiquad : DriverCommand
{
    driver::SequenceSoundPlayer* player; // 0x10
    u32 trackBitFlag;                    // 0x14
    int type;                            // 0x18
    f32 value;                           // 0x1C
};
ASSERT_SIZE(DriverCommandSequenceTrackBiquad, 0x20);

// an integer parameter of the tracks of a sequence sound player (31 bank index, 32 transpose, 33 velocity range)
struct DriverCommandSequenceTrackValue : DriverCommand
{
    driver::SequenceSoundPlayer* player; // 0x10
    u32 trackBitFlag;                    // 0x14
    union
    {
        int bankIndex;     // 31
        s8 transpose;      // 32
        u8 velocityRange;  // 33
    };                                   // 0x18
};
ASSERT_SIZE(DriverCommandSequenceTrackValue, 0x1C);

// the initial volume of the tracks of a stream sound player (46)
struct DriverCommandStreamSoundTrackInitialVolume : DriverCommand
{
    driver::StreamSoundPlayer* player;   // 0x10
    u32 trackBitFlag;                    // 0x14
    u32 volume;                          // 0x18
};
ASSERT_SIZE(DriverCommandStreamSoundTrackInitialVolume, 0x1C);
// the tracks and the buffer pool of a stream sound player (39)
struct DriverCommandStreamSoundSetup : DriverCommand
{
    driver::StreamSoundPlayer* player;           // 0x10
    driver::StreamSoundPlayer::SetupArg arg;     // 0x14
};
ASSERT_SIZE(DriverCommandStreamSoundSetup, 0x64);

// a stream sound player that loads its stream (40)
struct DriverCommandStreamSoundPrepare : DriverCommand
{
    driver::StreamSoundPlayer* player;           // 0x10
    u32 padding0x14;                             // 0x14
    driver::StreamSoundPlayer::PrepareArg arg;   // 0x18
};
ASSERT_SIZE(DriverCommandStreamSoundPrepare, 0x2C);

// a stream sound player that starts with the prefetch data (41)
struct DriverCommandStreamSoundPreparePrefetch : DriverCommand
{
    driver::StreamSoundPlayer* player;                  // 0x10
    driver::StreamSoundPlayer::PreparePrefetchArg arg;  // 0x14
};
ASSERT_SIZE(DriverCommandStreamSoundPreparePrefetch, 0x28);

// the header the task thread loaded (42)
struct DriverCommandStreamSoundLoadHeader : DriverCommand
{
    driver::StreamSoundPlayer* player;                  // 0x10
    bool result;                                        // 0x14
    u8 padding0x15[3];                                  // 0x15
    nn::snd::CTR::AdpcmParam* adpcmParams[driver::StreamSoundLoader::CHANNEL_MAX]; // 0x18 (NULL: not ADPCM)
    u16 requestId;                                      // 0x38
    u8 padding0x3A[2];                                  // 0x3A
};
ASSERT_SIZE(DriverCommandStreamSoundLoadHeader, 0x3C);

// a block the task thread loaded (43)
struct DriverCommandStreamSoundLoadData : DriverCommand
{
    driver::StreamSoundPlayer* player;   // 0x10
    bool result;                         // 0x14
    u8 padding0x15[3];                   // 0x15
    LoadDataParam param;                 // 0x18
};
ASSERT_SIZE(DriverCommandStreamSoundLoadData, 0x64);

// a parameter of the tracks of a stream sound player (45, 47, 48)
struct DriverCommandStreamSoundTrackParam : DriverCommand
{
    driver::StreamSoundPlayer* player;   // 0x10
    u32 trackBitFlag;                    // 0x14
    f32 value;                           // 0x18
};
ASSERT_SIZE(DriverCommandStreamSoundTrackParam, 0x1C);

} // namespace internal
} // namespace snd
} // namespace nw
