#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BankFile.h"

class nw::snd::internal::BankFile::Instrument
{
public:
    void GetKeyRegion(unsigned) const; // 0x00742C84 | nintendogs:callseq-callee [tier A]
};
