#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script12InsertEngineE @ 0x008D3834
// vtable 0x0090A0D0 (vptr 0x0090A0D8), offset_to_top 0, 8 entries
class InsertEngine : public ::script::MsgEngine
{
public:
    InsertEngine(); // ctor candidate(s) 0x005D5564, 0x005E5120, 0x005ED20C, 0x005F73D8, 0x005F74F8, 0x00602A14 (unverified)
    virtual void vf_0x00(); // 0x005E3750 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005E3740 slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x005E27B4 slot 0x14 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x18(); // 0x005E36EC slot 0x18 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
