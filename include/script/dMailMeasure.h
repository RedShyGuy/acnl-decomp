#pragma once

#include "decomp.h"
#include "script/dIMeasure.h"
#include "script/dSeqEngine.h"

namespace script {
// RTTI N6script11MailMeasureE @ 0x008D37E8
// vtable 0x00909FFC (vptr 0x0090A004), offset_to_top 0, 11 entries
// vtable 0x0090A030 (vptr 0x0090A038), offset_to_top -140, 6 entries
class MailMeasure : public ::script::SeqEngine, public ::script::IMeasure
{
public:
    MailMeasure(); // ctor address unknown
    virtual void vf_0x00(); // 0x005DFAD0 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005DFA78 slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x08(); // 0x005DFA58 slot 0x08 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x0C(); // 0x005DF9B4 slot 0x0C | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x10(); // 0x005DFA64 slot 0x10 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x005DE1D0 slot 0x14 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x18(); // 0x005DF9B8 slot 0x18 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x20(); // 0x005DEC68 slot 0x20 | virtual slot, introduced by script::MailMeasure
    virtual void vf_0x24(); // 0x005DF084 slot 0x24 | virtual slot, introduced by script::MailMeasure
    virtual void vf_0x28(); // 0x0075BBF0 slot 0x28 | virtual slot, introduced by script::MailMeasure
};
} // namespace script
