#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveSoundFile.h"

class nw::snd::internal::WaveSoundFile::InfoBlockBody
{
public:
    void GetWaveIdTable() const; // 0x0013D1BC | nintendogs:callgraph [tier A]
    void GetWaveSoundData(unsigned) const; // 0x0013D1C8 | nintendogs:bytes [tier A]
};
