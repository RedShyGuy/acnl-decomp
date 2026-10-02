#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 15BsDemoCancelMgr @ 0x008CBED8
// vtable 0x008F1230 (vptr 0x008F1238), offset_to_top 0, 16 entries
class BsDemoCancelMgr : public ::Base
{
public:
    BsDemoCancelMgr(); // ctor candidate(s) 0x002850B0 (unverified)
    virtual ~BsDemoCancelMgr(); // 0x002850F0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002850E0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00284FAC slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00285098 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00284FC8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00284F90 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
