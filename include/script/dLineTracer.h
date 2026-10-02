#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script10LineTracerE @ 0x008D36FC
// vtable 0x00909620 (vptr 0x00909628), offset_to_top 0, 8 entries
class LineTracer : public ::script::MsgEngine
{
public:
    LineTracer(); // ctor candidate(s) 0x005D62E0 (unverified)
    virtual void vf_0x00(); // 0x005D631C slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005D630C slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x0C(); // 0x005D6204 slot 0x0C | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x10(); // 0x005D6244 slot 0x10 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x005D61B4 slot 0x14 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x18(); // 0x005D6208 slot 0x18 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
