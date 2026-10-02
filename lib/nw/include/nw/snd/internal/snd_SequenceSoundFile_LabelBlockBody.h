#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SequenceSoundFile.h"

class nw::snd::internal::SequenceSoundFile::LabelBlockBody
{
public:
    void GetOffsetByLabel(const char*, unsigned*) const; // 0x007404E0 | nintendogs:bytes [tier A]
};
