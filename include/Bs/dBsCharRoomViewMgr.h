#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 17BsCharRoomViewMgr @ 0x008CC468
// vtable 0x008F3438 (vptr 0x008F3440), offset_to_top 0, 22 entries
class BsCharRoomViewMgr : public ::UtlBase<Base>
{
public:
    BsCharRoomViewMgr(); // ctor candidate(s) 0x002C6FAC (unverified)
    virtual ~BsCharRoomViewMgr(); // 0x002C70D8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002C70C8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0082067C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00820718 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002C6A84 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002C6898 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x002C5C34 slot 0x40 | virtual slot, introduced by BsCharRoomViewMgr
    virtual void vf_0x44(); // 0x002C58EC slot 0x44 | virtual slot, introduced by BsCharRoomViewMgr
    virtual void vf_0x48(); // 0x002C56C4 slot 0x48 | virtual slot, introduced by BsCharRoomViewMgr
    virtual void vf_0x4C(); // 0x002C645C slot 0x4C | virtual slot, introduced by BsCharRoomViewMgr
    virtual void vf_0x50(); // 0x002C6288 slot 0x50 | virtual slot, introduced by BsCharRoomViewMgr
    virtual void vf_0x54(); // 0x002C60AC slot 0x54 | virtual slot, introduced by BsCharRoomViewMgr
};
