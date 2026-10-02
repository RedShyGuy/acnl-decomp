#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveArchiveFile.h"

class nw::snd::internal::WaveArchiveFile::FileHeader
{
public:
    void GetFileBlock() const; // 0x0013FAC4 | nintendogs:bytes [tier A]
    void GetInfoBlock() const; // 0x0013FAF8 | nintendogs:bytes [tier A]
    void GetFileBlockOffset() const; // 0x00141D28 | nintendogs:bytes [tier A]
    void GetInfoBlockOffset() const; // 0x00143A90 | nintendogs:bytes [tier A]
};
