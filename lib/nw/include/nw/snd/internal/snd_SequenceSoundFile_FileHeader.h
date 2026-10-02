#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SequenceSoundFile.h"

class nw::snd::internal::SequenceSoundFile::FileHeader
{
public:
    void GetDataBlock() const; // 0x00740404 | nintendogs:bytes [tier A]
};
