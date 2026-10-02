#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"

class nw::snd::internal::SoundArchiveFile::FileHeader
{
public:
    void GetInfoBlockSize() const; // 0x0073FD34 | nintendogs:bytes [tier A]
    void GetFileBlockOffset() const; // 0x0073FD74 | nintendogs:callseq-callee [tier A]
    void GetStringBlockSize() const; // 0x0073FDF4 | nintendogs:bytes [tier A]
    void GetStringBlockOffset() const; // 0x0073FE2C | nintendogs:bytes [tier A]
};
