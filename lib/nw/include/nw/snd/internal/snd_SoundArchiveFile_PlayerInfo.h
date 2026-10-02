#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"

class nw::snd::internal::SoundArchiveFile::PlayerInfo
{
public:
    void GetPlayerHeapSize() const; // 0x0073FE64 | nintendogs:bytes [tier A]
};
