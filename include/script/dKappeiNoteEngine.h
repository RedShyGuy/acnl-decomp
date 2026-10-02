#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script16KappeiNoteEngineE @ 0x008D3944
// vtable 0x0090A2FC (vptr 0x0090A304), offset_to_top 0, 8 entries
class KappeiNoteEngine : public ::script::MsgEngine
{
public:
    KappeiNoteEngine(); // ctor candidate(s) 0x00602890 (unverified)
    virtual void vf_0x00(); // 0x005E7344 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005E7334 slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x10(); // 0x005E7320 slot 0x10 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x005E72AC slot 0x14 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x18(); // 0x005E72E4 slot 0x18 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
