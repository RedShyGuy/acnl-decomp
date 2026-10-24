#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BinaryTypes.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"
#include "nw/snd/internal/Util/snd_BitFlag.h"

// a stream sound of an archive before version 2.3.1.0
class nw::snd::internal::SoundArchiveFile::StreamSoundInfoLegacy
{
public:
    u32 GetLoopEndFrame() const; // 0x007402E8 | fefates:bytes [tier B]
    u32 GetLoopStartFrame() const; // 0x00740308 | fefates:bytes [tier B]
    u8 GetStreamFileType() const; // 0x00740328 | fefates:bytes [tier B]
    const BinaryReferenceTable* GetTrackInfoTable() const; // 0x00740350 | nintendogs:bytes [tier C]
    bool IsLoop() const; // 0x0074036C | fefates:bytes [tier B]

    u16 allocateTrackFlags;         // 0x00
    u16 allocateChannelCount;       // 0x02
    BinaryReference trackInfoTable; // 0x04
    u8 m_Unknown0x0C[12];           // 0x0C
    Util::BitFlag optionParams;     // 0x18 bit 0: byte 0 the file type, byte 1 loop; bit 1 loop start, bit 2 loop end
};
