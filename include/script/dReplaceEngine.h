#pragma once

#include "decomp.h"
#include "script/dMsgEngineModify.h"

namespace script {
// RTTI N6script13ReplaceEngineE @ 0x008D387C
// vtable 0x0090A180 (vptr 0x0090A188), offset_to_top 0, 7 entries
class ReplaceEngine : public ::script::MsgEngineModify
{
public:
    ReplaceEngine(); // ctor candidate(s) 0x005E45B4 (unverified)
    virtual void vf_0x00(); // 0x005E45E4 slot 0x00 | virtual slot, introduced by script::ReplaceEngine
    virtual void vf_0x04(); // 0x005E45E0 slot 0x04 | virtual slot, introduced by script::ReplaceEngine
    virtual void vf_0x08(); // 0x005E712C slot 0x08 | virtual slot, introduced by script::CapitalTopEngine
    virtual void vf_0x0C(); // 0x005E7124 slot 0x0C | virtual slot, introduced by script::CapitalTopEngine
    virtual void vf_0x10(); // 0x005E7130 slot 0x10 | virtual slot, introduced by script::ReplaceEngine
    virtual void vf_0x14(); // 0x005E44F4 slot 0x14 | virtual slot, introduced by script::ReplaceEngine
    virtual void vf_0x18(); // 0x005E7128 slot 0x18 | virtual slot, introduced by script::CapitalTopEngine
};
} // namespace script
