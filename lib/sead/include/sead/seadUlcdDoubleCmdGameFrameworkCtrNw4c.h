#pragma once

#include "decomp.h"
#include "sead/seadDoubleCmdGameFrameworkCtrNw4c.h"

namespace sead {
// RTTI N4sead33UlcdDoubleCmdGameFrameworkCtrNw4cE @ 0x008D1FD0
// vtable 0x0090697C (vptr 0x00906984), offset_to_top 0, 38 entries
class UlcdDoubleCmdGameFrameworkCtrNw4c : public ::sead::DoubleCmdGameFrameworkCtrNw4c
{
public:
    UlcdDoubleCmdGameFrameworkCtrNw4c(); // ctor candidate(s) 0x0011EC54 (unverified)
    virtual void vf_0x00(); // 0x0074E614 slot 0x00 | virtual slot, introduced by sead::Framework
    virtual void vf_0x04(); // 0x0074E5B0 slot 0x04 | virtual slot, introduced by sead::Framework
    virtual ~UlcdDoubleCmdGameFrameworkCtrNw4c(); // 0x0054DF5C slot 0x08 | slot vf_0x08 of sead::Framework
    // 0x0054E1B4 slot 0x0C | slot vf_0x0C of sead::Framework (deleting dtor)
    virtual void vf_0x18(); // 0x0074E5FC slot 0x18 | virtual slot, introduced by sead::Framework
    virtual void createMethodTreeMgr_(sead::Heap*); // 0x00549000 slot 0x30 | slot vf_0x30 of sead::Framework
    virtual void procDraw_(); // 0x0054E098 slot 0x68 | slot vf_0x68 of sead::GameFrameworkCtrNw4c
    virtual void swapBuffer_(); // 0x0054DF94 slot 0x78 | nintendogs:callseq
    virtual void vf_0x88(); // 0x0054E094 slot 0x88 | virtual slot, introduced by sead::GameFrameworkCtrNw4c
    virtual void vf_0x90(); // 0x0054E050 slot 0x90 | virtual slot, introduced by sead::UlcdDoubleCmdGameFrameworkCtrNw4c
    virtual void vf_0x94(); // 0x0054E05C slot 0x94 | virtual slot, introduced by sead::UlcdDoubleCmdGameFrameworkCtrNw4c
    void setUlcdEnable(bool); // 0x0011EC38 | nintendogs:bytes [tier A]
};
} // namespace sead
