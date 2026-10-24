#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"
#include "nw/snd/internal/Util/snd_BitFlag.h"

class nw::snd::internal::SoundArchiveFile::WaveArchiveInfo
{
public:
    u32 GetWaveCount() const; // 0x00141E48 | nintendogs:bytes [tier A]

    u32 fileId;                 // 0x0
    bool isLoadIndividual;      // 0x4
    Util::BitFlag optionParams; // 0x8 bit 0 the string id, bit 1 the wave count
};
