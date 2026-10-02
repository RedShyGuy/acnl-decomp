#pragma once

#include "decomp.h"
#include "Npc/dNpcModelBase.h"

// RTTI 8NpcModel @ 0x008CD51C
// vtable 0x008F9CC4 (vptr 0x008F9CCC), offset_to_top 0, 20 entries
class NpcModel : public ::NpcModelBase
{
public:
    NpcModel(); // ctor address unknown
    virtual void vf_0x00(); // 0x006ACEFC slot 0x00 | virtual slot, introduced by HumanModel
    virtual void vf_0x04(); // 0x006ACD48 slot 0x04 | virtual slot, introduced by HumanModel
    virtual void vf_0x08(); // 0x006ACD70 slot 0x08 | virtual slot, introduced by HumanModel
    virtual void vf_0x0C(); // 0x006ACFE4 slot 0x0C | virtual slot, introduced by HumanModel
    virtual void vf_0x10(); // 0x006AD1D8 slot 0x10 | virtual slot, introduced by HumanModel
    virtual void vf_0x14(); // 0x006ACDD0 slot 0x14 | virtual slot, introduced by HumanModel
    virtual ~NpcModel(); // 0x006AD30C slot 0x1C | slot vf_0x1C of HumanModel
    // 0x006AD2FC slot 0x20 | slot vf_0x20 of HumanModel (deleting dtor)
    virtual void vf_0x30(); // 0x006ACE34 slot 0x30 | virtual slot, introduced by NpcModelBase
    virtual void vf_0x34(); // 0x006AD048 slot 0x34 | virtual slot, introduced by NpcModelBase
    virtual void vf_0x38(); // 0x006ABFD0 slot 0x38 | virtual slot, introduced by NpcModelBase
    virtual void vf_0x3C(); // 0x006ACBE4 slot 0x3C | virtual slot, introduced by NpcModelBase
    virtual void vf_0x40(); // 0x006ACB8C slot 0x40 | virtual slot, introduced by NpcModelBase
    virtual void vf_0x44(); // 0x006ACCEC slot 0x44 | virtual slot, introduced by NpcModelBase
    virtual void vf_0x48(); // 0x00213968 slot 0x48 | virtual slot, introduced by NpcModelBase
    virtual void vf_0x4C(); // 0x006AC450 slot 0x4C | virtual slot, introduced by NpcModelBase
};
