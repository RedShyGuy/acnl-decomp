#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script10DetectCodeE @ 0x008D36DC
// vtable 0x009095CC (vptr 0x009095D4), offset_to_top 0, 8 entries
class DetectCode : public ::script::MsgEngine
{
public:
    DetectCode(); // ctor candidate(s) 0x005E89B0, 0x005E8A94, 0x005E8B7C, 0x005E8C64, 0x005E8CFC (unverified)
    virtual void vf_0x00(); // 0x005D5CEC slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005D5CDC slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x10(); // 0x005D5CC8 slot 0x10 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x005D5C98 slot 0x14 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x18(); // 0x005D5CC4 slot 0x18 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
