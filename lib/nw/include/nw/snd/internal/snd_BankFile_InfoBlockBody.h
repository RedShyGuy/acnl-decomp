#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BankFile.h"

class nw::snd::internal::BankFile::InfoBlockBody
{
public:
    const BinaryTable<WaveId>* GetWaveIdTable() const; // 0x0013D6C4 | nintendogs:bytes [tier A]
    // the instrument of a program number (NULL: none or a null instrument)
    const Instrument* GetInstrument(int programNo) const; // 0x00742D38 | nintendogs:bytes [tier A]
    const BinaryReferenceTable* GetInstrumentReferenceTable() const; // 0x00742D7C | nintendogs:bytes [tier A]

    BinaryReference waveIdTable;     // 0x0 BinaryTable<WaveId>
    BinaryReference instrumentTable; // 0x8
};
