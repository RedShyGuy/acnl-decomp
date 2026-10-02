#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script13ElisionEngineE @ 0x008D384C
// vtable 0x0090A130 (vptr 0x0090A138), offset_to_top 0, 8 entries
class ElisionEngine : public ::script::MsgEngine
{
public:
    ElisionEngine(); // ctor address unknown
    virtual void vf_0x00(); // 0x005E4174 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005E4164 slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x10(); // 0x005E4150 slot 0x10 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x005E40EC slot 0x14 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
