#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 8BsCamera @ 0x008CD454
// vtable 0x008F9874 (vptr 0x008F987C), offset_to_top 0, 16 entries
class BsCamera : public ::Base
{
public:
    BsCamera(); // ctor candidate(s) 0x006902E4 (unverified)
    virtual ~BsCamera(); // 0x0069037C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00690348 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0068FFC0 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00690288 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x006901A0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0068FF6C slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void Unk0(); // 0x00766EAC slot 0x3C | slot vf_0x3C of Base
};
