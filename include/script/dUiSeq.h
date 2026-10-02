#pragma once

#include "decomp.h"
#include "script/dSeqEngine.h"

namespace script {
// RTTI N6script5UiSeqE @ 0x008D3A84
// vtable 0x0090A594 (vptr 0x0090A59C), offset_to_top 0, 8 entries
class UiSeq : public ::script::SeqEngine
{
public:
    UiSeq(); // ctor address unknown
    virtual void vf_0x00(); // 0x005ED450 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005ED440 slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x08(); // 0x005ED434 slot 0x08 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x005EB490 slot 0x14 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x18(); // 0x005ED394 slot 0x18 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
