#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveSoundFile.h"

class nw::snd::internal::WaveSoundFile::InfoBlockBody
{
public:
    const BinaryTable<WaveId>* GetWaveIdTable() const; // 0x0013D1BC | nintendogs:callgraph [tier A]
    // the wave sound of an index (NULL: none)
    const WaveSoundData* GetWaveSoundData(u32 index) const; // 0x0013D1C8 | nintendogs:bytes [tier A]

    BinaryReference waveIdTable;        // 0x0 BinaryTable<WaveId>
    BinaryReference waveSoundDataTable; // 0x8 BinaryReferenceTable
};
