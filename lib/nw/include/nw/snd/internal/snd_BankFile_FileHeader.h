#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BankFile.h"

class nw::snd::internal::BankFile::FileHeader
{
public:
    void GetInfoBlock() const; // 0x0013D658 | nintendogs:bytes [tier A]
};
