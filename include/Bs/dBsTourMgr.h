#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 9BsTourMgr @ 0x008CD760
// vtable 0x008FA530 (vptr 0x008FA538), offset_to_top 0, 16 entries
class BsTourMgr : public ::Base
{
public:
    class TourMgrHioNode;
    struct State { u32 _unknown; }; // TODO: real type unknown (placeholder)
    BsTourMgr(); // ctor candidate(s) 0x006E16B8 (unverified)
    virtual ~BsTourMgr(); // 0x006E16E0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x006E16D0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x006E153C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x006E1664 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x006E1558 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x006E1534 slot 0x30 | slot vf_0x30 of oml::framework::Process
    void SetState(BsTourMgr::State); // 0x006E1404 | libgarden [tier A]
};
