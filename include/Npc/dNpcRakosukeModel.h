#pragma once

#include "decomp.h"
#include "Npc/dNpcSimpleModel.h"

// RTTI 16NpcRakosukeModel @ 0x008CC374
// vtable 0x008F2B44 (vptr 0x008F2B4C), offset_to_top 0, 21 entries
class NpcRakosukeModel : public ::NpcSimpleModel
{
public:
    NpcRakosukeModel(); // ctor address unknown
    virtual void vf_0x0C(); // 0x002BB6F0 slot 0x0C | virtual slot, introduced by HumanModel
    virtual void vf_0x10(); // 0x002BB8B0 slot 0x10 | virtual slot, introduced by HumanModel
    virtual void vf_0x14(); // 0x002BB5DC slot 0x14 | virtual slot, introduced by HumanModel
    virtual ~NpcRakosukeModel(); // 0x002BB9FC slot 0x1C | slot vf_0x1C of HumanModel
    // 0x002BB9D4 slot 0x20 | slot vf_0x20 of HumanModel (deleting dtor)
    virtual void vf_0x30(); // 0x002BB65C slot 0x30 | virtual slot, introduced by NpcModelBase
    virtual void vf_0x34(); // 0x002BB87C slot 0x34 | virtual slot, introduced by NpcModelBase
    virtual void vf_0x50(); // 0x002BB5D0 slot 0x50 | virtual slot, introduced by NpcRakosukeModel
};
