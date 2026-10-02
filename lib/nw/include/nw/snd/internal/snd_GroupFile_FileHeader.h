#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_GroupFile.h"

class nw::snd::internal::GroupFile::FileHeader
{
public:
    void GetFileBlock() const; // 0x0013FFF8 | nintendogs:callgraph [tier A]
    void GetInfoBlock() const; // 0x00140060 | nintendogs:bytes [tier A]
    void GetInfoExBlock() const; // 0x001400CC | nintendogs:callgraph [tier A]
};
