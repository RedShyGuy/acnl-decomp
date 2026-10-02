#pragma once

#include "decomp.h"
#include "Npc/dNpcSimpleModel.h"

// RTTI 13NpcResetModel @ 0x008CBA20
// vtable 0x008EF370 (vptr 0x008EF378), offset_to_top 0, 20 entries
class NpcResetModel : public ::NpcSimpleModel
{
public:
    NpcResetModel(); // ctor address unknown
    virtual void vf_0x08(); // 0x00230310 slot 0x08 | virtual slot, introduced by HumanModel
    virtual void vf_0x0C(); // 0x002304F0 slot 0x0C | virtual slot, introduced by HumanModel
    virtual void vf_0x10(); // 0x0023052C slot 0x10 | virtual slot, introduced by HumanModel
    virtual void vf_0x14(); // 0x00230370 slot 0x14 | virtual slot, introduced by HumanModel
    virtual ~NpcResetModel(); // 0x002305D4 slot 0x1C | slot vf_0x1C of HumanModel
    // 0x002305A4 slot 0x20 | slot vf_0x20 of HumanModel (deleting dtor)
    virtual void vf_0x24(); // 0x002301FC slot 0x24 | virtual slot, introduced by HumanModel
    virtual void vf_0x30(); // 0x002303F0 slot 0x30 | virtual slot, introduced by NpcModelBase
    virtual void vf_0x34(); // 0x006AD00C slot 0x34 | virtual slot, introduced by NpcModelBase
};
