#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BankFile.h"

class nw::snd::internal::BankFile::InfoBlockBody
{
public:
    void GetWaveIdTable() const; // 0x0013D6C4 | nintendogs:callseq-callee [tier A]
    void GetInstrument(int) const; // 0x00742D38 | nintendogs:bytes [tier A]
    void GetInstrumentReferenceTable() const; // 0x00742D7C | nintendogs:callseq-callee [tier A]
};
