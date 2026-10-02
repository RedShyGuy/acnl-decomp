#pragma once

#include "decomp.h"
#include "Sound/dSoundSeqParser.h"
#include "sead/seadIDisposer.h"

// RTTI N14SoundSeqParser18SingletonDisposer_E @ 0x008CDA30
// vtable 0x008FB49C (vptr 0x008FB4A4), offset_to_top 0, 2 entries
class SoundSeqParser::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x00123604 (unverified)
    virtual ~SingletonDisposer_(); // 0x00278D60 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00278D1C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};
