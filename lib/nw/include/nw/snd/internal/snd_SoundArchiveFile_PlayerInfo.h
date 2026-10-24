#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"
#include "nw/snd/internal/Util/snd_BitFlag.h"

class nw::snd::internal::SoundArchiveFile::PlayerInfo
{
public:
    u32 GetPlayerHeapSize() const; // 0x0073FE64 | nintendogs:bytes [tier A]

    u32 playableSoundMax;       // 0x0
    Util::BitFlag optionParams; // 0x4 bit 0 the string id, bit 1 the heap size
};
