#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"

class nw::snd::internal::SoundArchiveFile::WaveArchiveInfo
{
public:
    void GetWaveCount() const; // 0x00141E48 | nintendogs:bytes [tier A]
};
