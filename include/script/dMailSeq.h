#pragma once

#include "decomp.h"
#include "script/dSeqEngine.h"

namespace script {
// RTTI N6script7MailSeqE @ 0x008D3AD8
// vtable 0x0090A928 (vptr 0x0090A930), offset_to_top 0, 8 entries
class MailSeq : public ::script::SeqEngine
{
public:
    MailSeq(); // ctor address unknown
    virtual void vf_0x00(); // 0x005F7604 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005F75F4 slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x08(); // 0x005F74EC slot 0x08 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x005F587C slot 0x14 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x18(); // 0x005F7338 slot 0x18 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
