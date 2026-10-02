#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveSoundFile.h"

class nw::snd::internal::WaveSoundFile::WaveSoundData
{
public:
    void GetNoteInfo(unsigned) const; // 0x0013D1FC | nintendogs:bytes [tier A]
};
