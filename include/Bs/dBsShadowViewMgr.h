#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 15BsShadowViewMgr @ 0x008CBFD4
// vtable 0x008F174C (vptr 0x008F1754), offset_to_top 0, 16 entries
class BsShadowViewMgr : public ::Base
{
public:
    BsShadowViewMgr(); // ctor candidate(s) 0x00290428 (unverified)
    virtual ~BsShadowViewMgr(); // 0x00290494 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00290468 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0028FF28 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00290344 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Draw(); // 0x0028FF10 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
