#pragma once

#include "decomp.h"
#include "Npc/dNpcSimpleModel.h"

// RTTI 16NpcKameyamaModel @ 0x008CC350
// vtable 0x008F2A88 (vptr 0x008F2A90), offset_to_top 0, 20 entries
class NpcKameyamaModel : public ::NpcSimpleModel
{
public:
    NpcKameyamaModel(); // ctor address unknown
    bool PlayAnim(u16 id, u32 playMode, u32 flags, f32 frame, f32 speed, f32 duration, bool* changed, u8 morphType) override; // 0x002BB3E0 slot 0x00
    virtual ~NpcKameyamaModel(); // 0x002BB4C0 slot 0x1C | slot vf_0x1C of HumanModel
    // 0x002BB4B0 slot 0x20 | slot vf_0x20 of HumanModel (deleting dtor)
};
