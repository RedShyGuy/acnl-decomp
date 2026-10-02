#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script11CountReturnE @ 0x008D3774
// vtable 0x00909CF8 (vptr 0x00909D00), offset_to_top 0, 8 entries
class CountReturn : public ::script::MsgEngine
{
public:
    CountReturn(); // ctor candidate(s) 0x0075D2B0 (unverified)
    virtual void vf_0x00(); // 0x005D9198 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005D9188 slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x005D916C slot 0x14 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
