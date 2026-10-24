#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_GroupFile.h"
#include "nw/ut/ut_Api.h"

class nw::snd::internal::GroupFile::FileHeader : public ::nw::ut::BinaryFileHeader
{
public:
    // the blocks (NULL: none)
    const void* GetFileBlock() const; // 0x0013FFF8 | nintendogs:callgraph [tier A]
    const void* GetInfoBlock() const; // 0x00140060 | nintendogs:bytes [tier A]
    const void* GetInfoExBlock() const; // 0x001400CC | nintendogs:callgraph [tier A]

    BinaryReferenceWithSize blocks[1]; // 0x14
};
