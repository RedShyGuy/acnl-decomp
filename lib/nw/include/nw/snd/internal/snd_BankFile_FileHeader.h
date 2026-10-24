#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BankFile.h"

class nw::snd::internal::BankFile::FileHeader : public ::nw::ut::BinaryFileHeader
{
public:
    // the info block (NULL: none)
    const void* GetInfoBlock() const; // 0x0013D658 | nintendogs:bytes [tier A]

    BinaryReferenceWithSize blocks[1]; // 0x14
};
