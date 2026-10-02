#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script17PeriodCountEngineE @ 0x008D3980
// vtable 0x0090A390 (vptr 0x0090A398), offset_to_top 0, 8 entries
class PeriodCountEngine : public ::script::MsgEngine
{
public:
    PeriodCountEngine(); // ctor candidate(s) 0x00605020 (unverified)
    virtual void vf_0x00(); // 0x005E87FC slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005E87EC slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x10(); // 0x005E87D8 slot 0x10 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x005E879C slot 0x14 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x18(); // 0x005E87BC slot 0x18 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
