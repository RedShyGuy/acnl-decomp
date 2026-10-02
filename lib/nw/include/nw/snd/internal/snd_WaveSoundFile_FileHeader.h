#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveSoundFile.h"

class nw::snd::internal::WaveSoundFile::FileHeader
{
public:
    void GetInfoBlock() const; // 0x0013D150 | nintendogs:bytes [tier A]
};
