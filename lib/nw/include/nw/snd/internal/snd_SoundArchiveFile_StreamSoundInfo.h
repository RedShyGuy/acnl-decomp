#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BinaryTypes.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"

// a stream sound of an archive of version 2.3.1.0 and later
// a track of a stream sound (name is ours)
struct nw::snd::internal::SoundArchiveFile::StreamTrackInfo
{
    u8 volume;                         // 0x00
    u8 pan;                            // 0x01
    u8 span;                           // 0x02
    u8 flags;                          // 0x03
    BinaryReference globalChannelIndexTable; // 0x04 BinaryTable<u8>
    BinaryReference sendValue;         // 0x0C bytes: main send, fx send A, fx send B (version 2.3.1.0 and later)
    u8 lpfFreq;                        // 0x14 (version 2.3.1.0 and later)
    u8 biquadType;                     // 0x15
    u8 biquadValue;                    // 0x16
};

// the player limits of the archive (name is ours)
struct nw::snd::internal::SoundArchiveFile::SoundArchivePlayerInfo
{
    u16 sequenceSoundMax;   // 0x0
    u16 sequenceTrackMax;   // 0x2
    u16 streamSoundMax;     // 0x4
    u16 streamTrackMax;     // 0x6
    u16 streamChannelMax;   // 0x8
    u16 waveSoundMax;       // 0xA
    u16 waveTrackMax;       // 0xC
    u8 streamBufferTimes;   // 0xE (version 2.3.2.0 and later)
};

class nw::snd::internal::SoundArchiveFile::StreamSoundInfo
{
public:
    // byte 0 the stream file type, byte 1 loop (1), then the loop start and end frames
    struct StreamSoundExtension
    {
        u32 streamTypeInfo; // 0x0
        u32 loopStartFrame; // 0x4
        u32 loopEndFrame;   // 0x8
    };

    const u32* GetSendValue() const; // 0x00740184 | nintendogs:bytes [tier C]
    const BinaryReferenceTable* GetTrackInfoTable() const; // 0x00740190 | nintendogs:bytes [tier C]
    const StreamSoundExtension* GetStreamSoundExtension() const; // 0x007401AC | nintendogs:bytes [tier B]

    u16 allocateTrackFlags;           // 0x00
    u16 allocateChannelCount;         // 0x02
    BinaryReference trackInfoTable;   // 0x04
    f32 pitch;                        // 0x0C
    BinaryReference sendValue;        // 0x10 a word: main send, fx send A, fx send B
    BinaryReference streamSoundExtension; // 0x18
    u32 prefetchFileId;               // 0x20 (version 2.3.2.0 and later)
};
