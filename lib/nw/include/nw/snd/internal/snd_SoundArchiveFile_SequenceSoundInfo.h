#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BinaryTypes.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"
#include "nw/snd/internal/Util/snd_BitFlag.h"

class nw::snd::internal::SoundArchiveFile::SequenceSoundInfo
{
public:
    // the first four banks (-1 for the others)
    void GetBankIds(u32* pBankIds) const; // 0x0013FD68 | nintendogs:bytes [tier A]
    u32 GetStartOffset() const; // 0x0013FDC8 | nintendogs:bytes [tier A]
    u8 GetChannelPriority() const; // 0x0013FDE8 | nintendogs:bytes [tier A]
    bool IsReleasePriorityFix() const; // 0x0013FE10 | nintendogs:bytes [tier A]

    BinaryReference bankIdTable;  // 0x00 BinaryTable<u32>
    u32 allocateTrackFlags;       // 0x08
    Util::BitFlag optionParams;   // 0x0C bit 0 the start offset; bit 1: byte 0 the channel priority, byte 1 release priority fix
};
