#pragma once

#include "decomp.h"
#include "script/dSeqEngine.h"

namespace script {
// RTTI N6script9ChoiceSeqE @ 0x008D3E44
// vtable 0x0090BA94 (vptr 0x0090BA9C), offset_to_top 0, 8 entries
class ChoiceSeq : public ::script::SeqEngine
{
public:
    ChoiceSeq(); // ctor candidate(s) 0x005D5634 (unverified)
    virtual void vf_0x00(); // 0x006006D4 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x006006C4 slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x08(); // 0x006006A4 slot 0x08 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x0C(); // 0x00600608 slot 0x0C | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x10(); // 0x006006B0 slot 0x10 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x005FEB84 slot 0x14 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x18(); // 0x0060060C slot 0x18 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
