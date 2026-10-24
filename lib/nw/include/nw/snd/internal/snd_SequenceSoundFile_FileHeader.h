#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SequenceSoundFile.h"
#include "nw/ut/ut_Api.h"

class nw::snd::internal::SequenceSoundFile::FileHeader : public ::nw::ut::BinaryFileHeader
{
public:
    // the blocks (NULL: none)
    const void* GetDataBlock() const; // 0x00740404 | nintendogs:bytes [tier A]
    const void* GetLabelBlock() const; // 0x00740470 | nintendogs:callgraph [tier C]

    BinaryReferenceWithSize blocks[1]; // 0x14
};
