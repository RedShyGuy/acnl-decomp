#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"
#include "nw/snd/internal/Util/snd_BitFlag.h"

class nw::snd::internal::SoundArchiveFile::WaveSoundInfo
{
public:
    u8 GetChannelPriority() const; // 0x0013FD14 | nintendogs:bytes [tier A]
    u8 GetIsReleasePriorityFix() const; // 0x0013FD3C | nintendogs:bytes [tier A]

    u32 index;                  // 0x0
    u32 allocateTrackCount;     // 0x4
    Util::BitFlag optionParams; // 0x8 bit 0: byte 0 the channel priority, byte 1 release priority fix
};
