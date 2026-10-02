#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script10CopyEngineE @ 0x008D36C4
// vtable 0x009095A4 (vptr 0x009095AC), offset_to_top 0, 8 entries
class CopyEngine : public ::script::MsgEngine
{
public:
    CopyEngine(); // ctor candidate(s) 0x005FEA74 (unverified)
    virtual void vf_0x00(); // 0x005D5B88 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005D5B78 slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x10(); // 0x005D5B58 slot 0x10 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
