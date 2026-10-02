#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script9CountCodeE @ 0x008D3E68
// vtable 0x0090BAE4 (vptr 0x0090BAEC), offset_to_top 0, 8 entries
class CountCode : public ::script::MsgEngine
{
public:
    CountCode(); // ctor candidate(s) 0x005D5B98 (unverified)
    virtual void vf_0x00(); // 0x00600854 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x00600844 slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x0060081C slot 0x14 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x18(); // 0x00600840 slot 0x18 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
