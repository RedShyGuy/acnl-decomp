#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SequenceSoundFile.h"

// a BinaryReferenceTable of LabelInfo
class nw::snd::internal::SequenceSoundFile::LabelBlockBody
{
public:
    // the offset of the first label whose name starts with pLabel; false if there is none
    bool GetOffsetByLabel(const char* pLabel, u32* pOffset) const; // 0x007404E0 | nintendogs:bytes [tier A]

    BinaryReferenceTable labelInfoTable; // 0x0
};
