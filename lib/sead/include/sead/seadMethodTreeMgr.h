#pragma once

#include "decomp.h"

namespace sead {
// RTTI N4sead13MethodTreeMgrE @ 0x008D1674
// vtable 0x009055E0 (vptr 0x009055E8), offset_to_top 0, 8 entries
class MethodTreeMgr
{
public:
    MethodTreeMgr(); // ctor candidate(s) 0x005431C4 (unverified)
    virtual void vf_0x00(); // 0x0074B99C slot 0x00 | virtual slot, introduced by sead::MethodTreeMgr
    virtual void vf_0x04(); // 0x0074B950 slot 0x04 | virtual slot, introduced by sead::MethodTreeMgr
    virtual ~MethodTreeMgr(); // 0x00543200 slot 0x08 | slot vf_0x08 of sead::MethodTreeMgr
    // 0x005431E0 slot 0x0C | slot vf_0x0C of sead::MethodTreeMgr (deleting dtor)
    virtual void attachMethod(int, sead::MethodTreeNode*); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void pauseAll(bool); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
};
} // namespace sead
