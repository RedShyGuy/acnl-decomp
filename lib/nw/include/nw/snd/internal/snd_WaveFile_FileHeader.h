#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveFile.h"

class nw::snd::internal::WaveFile::FileHeader
{
public:
    void GetInfoBlock() const; // 0x00743134 | nintendogs:bytes [tier A]
};
