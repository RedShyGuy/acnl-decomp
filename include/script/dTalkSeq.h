#pragma once

#include "decomp.h"
#include "script/dSeqEngine.h"

namespace script {
// RTTI N6script7TalkSeqE @ 0x008D3B04
// vtable 0x0090A974 (vptr 0x0090A97C), offset_to_top 0, 8 entries
class TalkSeq : public ::script::SeqEngine
{
public:
    TalkSeq(); // ctor address unknown
    virtual void vf_0x00(); // 0x005FCA44 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005FC9F8 slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x08(); // 0x005FC408 slot 0x08 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x0C(); // 0x005FC364 slot 0x0C | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x10(); // 0x005FC504 slot 0x10 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x005F8030 slot 0x14 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x18(); // 0x005FC370 slot 0x18 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
