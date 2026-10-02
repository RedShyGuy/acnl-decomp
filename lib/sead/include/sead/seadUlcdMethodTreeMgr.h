#pragma once

#include "decomp.h"
#include "sead/seadDualScreenMethodTreeMgr.h"

namespace sead {
// RTTI N4sead17UlcdMethodTreeMgrE @ 0x008D1C20
// vtable 0x00906080 (vptr 0x00906088), offset_to_top 0, 8 entries
class UlcdMethodTreeMgr : public ::sead::DualScreenMethodTreeMgr
{
public:
    virtual void vf_0x00(); // 0x0074C9BC slot 0x00 | virtual slot, introduced by sead::MethodTreeMgr
    virtual void vf_0x04(); // 0x0074C970 slot 0x04 | virtual slot, introduced by sead::MethodTreeMgr
    virtual ~UlcdMethodTreeMgr(); // 0x005492A0 slot 0x08 | slot vf_0x08 of sead::MethodTreeMgr
    // 0x0054921C slot 0x0C | slot vf_0x0C of sead::MethodTreeMgr (deleting dtor)
    virtual void attachMethod(int, sead::MethodTreeNode*); // 0x0054CB44 slot 0x10 | nintendogs:callseq
    virtual void vf_0x14(); // 0x0054CD3C slot 0x14 | virtual slot, introduced by sead::MethodTreeMgr
    virtual void pauseAll(bool); // 0x00548F8C slot 0x18 | nintendogs:bytes
    UlcdMethodTreeMgr(); // 0x00549018 | nintendogs:bytes [tier A]
};
} // namespace sead
