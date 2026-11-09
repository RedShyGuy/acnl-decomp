#pragma once

#include "decomp.h"
#include "script/dIFlowRecept.h"
#include "script/dITalkRecept.h"

// RTTI 22MoveFromTalkReceptBase @ 0x008CCDF8
// vtable 0x008F6A74 (vptr 0x008F6A7C), offset_to_top 0, 64 entries
// vtable 0x008F6B7C (vptr 0x008F6B84), offset_to_top -124, 14 entries
class MoveFromTalkReceptBase : public ::script::ITalkRecept, public ::script::IFlowRecept
{
public:
    MoveFromTalkReceptBase(); // ctor address unknown
    virtual ~MoveFromTalkReceptBase(); // 0x00330CC0 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x00330C94 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void Slot94(); // 0x00330B38 slot 0x94
    virtual void OnMessageStart(script::TalkLabel* message); // 0x00330430 slot 0x9C
    virtual void OnEnd(); // 0x00330828 slot 0xAC
    virtual void vf_0xFC(); // 0x00330AA8 slot 0xFC | virtual slot, introduced by MoveFromTalkReceptBase
};
