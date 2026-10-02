#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 9BsShowMgr @ 0x008CD748
// vtable 0x008FA488 (vptr 0x008FA490), offset_to_top 0, 16 entries
class BsShowMgr : public ::Base
{
public:
    struct ShowPacket { u32 _unknown; }; // TODO: real type unknown (placeholder)
    BsShowMgr(); // ctor candidate(s) 0x006DB2EC (unverified)
    virtual ~BsShowMgr(); // 0x006DB36C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x006DB32C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x006DAFF4 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x006DB268 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x006DB260 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x006DAFEC slot 0x30 | slot vf_0x30 of oml::framework::Process
    void InitializeFromPacket(netgame::PlayerNo, BsShowMgr::ShowPacket const&); // 0x006DA6E0 | libgarden [tier A]
    void Show(SvFgName const&, nn::math::Vector<float, 3u> const&); // 0x006DA7FC | libgarden [tier A]
};
