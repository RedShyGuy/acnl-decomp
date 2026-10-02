#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BankFile.h"

class nw::snd::internal::BankFile::KeyRegion
{
public:
    void GetVelocityRegion(unsigned) const; // 0x00742F4C | nintendogs:callseq-callee [tier A]
};
