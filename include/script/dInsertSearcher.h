#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script14InsertSearcherE @ 0x008D38D0
// vtable 0x0090A238 (vptr 0x0090A240), offset_to_top 0, 8 entries
class InsertSearcher : public ::script::MsgEngine
{
public:
    InsertSearcher(); // ctor candidate(s) 0x005F1F98, 0x005F2F58, 0x005F3368, 0x005F3598, 0x005F3B24 (unverified)
    virtual void vf_0x00(); // 0x005E6E6C slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005E6E5C slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x10(); // 0x005E6E48 slot 0x10 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x18(); // 0x005E6DA8 slot 0x18 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
