#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"

class nw::snd::internal::SoundArchiveFile::WaveSoundInfo
{
public:
    void GetChannelPriority() const; // 0x0013FD14 | nintendogs:bytes [tier A]
    void GetIsReleasePriorityFix() const; // 0x0013FD3C | nintendogs:bytes [tier A]
};
