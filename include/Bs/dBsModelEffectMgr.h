#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 16BsModelEffectMgr @ 0x008CC2B4
// vtable 0x008F289C (vptr 0x008F28A4), offset_to_top 0, 16 entries
class BsModelEffectMgr : public ::Base
{
public:
    BsModelEffectMgr(); // ctor candidate(s) 0x002B6A8C (unverified)
    virtual ~BsModelEffectMgr(); // 0x002B6AC0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002B6AB0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002B6958 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002B69F0 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002B69AC slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002B6914 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
