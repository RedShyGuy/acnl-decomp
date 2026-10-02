#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 17BsAudioLoadSignal @ 0x008CC45C
// vtable 0x008F33F0 (vptr 0x008F33F8), offset_to_top 0, 16 entries
class BsAudioLoadSignal : public ::Base
{
public:
    BsAudioLoadSignal(); // ctor candidate(s) 0x002C568C (unverified)
    virtual ~BsAudioLoadSignal(); // 0x002C56C0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002C56B0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002C55D4 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002C5640 slot 0x18 | slot vf_0x18 of oml::framework::Process
};
