#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script16ContainTagEngineE @ 0x008D3914
// vtable 0x0090A2AC (vptr 0x0090A2B4), offset_to_top 0, 8 entries
class ContainTagEngine : public ::script::MsgEngine
{
public:
    ContainTagEngine(); // ctor candidate(s) 0x005FD688, 0x005FD8D0, 0x0075C82C (unverified)
    virtual void vf_0x00(); // 0x005E71BC slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005E71AC slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x10(); // 0x005E7190 slot 0x10 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x18(); // 0x005E7184 slot 0x18 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x1C(); // 0x005E7178 slot 0x1C | virtual slot, introduced by script::MsgEngine
};
} // namespace script
