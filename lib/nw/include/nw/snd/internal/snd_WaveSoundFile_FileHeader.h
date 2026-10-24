#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveSoundFile.h"
#include "nw/ut/ut_Api.h"

class nw::snd::internal::WaveSoundFile::FileHeader : public ::nw::ut::BinaryFileHeader
{
public:
    // the info block (NULL: none)
    const void* GetInfoBlock() const; // 0x0013D150 | nintendogs:bytes [tier A]

    BinaryReferenceWithSize blocks[1]; // 0x14
};
