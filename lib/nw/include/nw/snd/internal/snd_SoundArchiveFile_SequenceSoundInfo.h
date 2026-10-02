#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"

class nw::snd::internal::SoundArchiveFile::SequenceSoundInfo
{
public:
    void GetBankIds(unsigned*) const; // 0x0013FD68 | nintendogs:bytes [tier A]
    void GetStartOffset() const; // 0x0013FDC8 | nintendogs:bytes [tier A]
    void GetChannelPriority() const; // 0x0013FDE8 | nintendogs:bytes [tier A]
    void IsReleasePriorityFix() const; // 0x0013FE10 | nintendogs:bytes [tier A]
};
