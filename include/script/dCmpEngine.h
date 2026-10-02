#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script9CmpEngineE @ 0x008D3E50
// vtable 0x0090BABC (vptr 0x0090BAC4), offset_to_top 0, 8 entries
class CmpEngine : public ::script::MsgEngine
{
public:
    CmpEngine(); // ctor candidate(s) 0x005DD894, 0x005F1E80, 0x0075C8CC (unverified)
    virtual void vf_0x00(); // 0x0060080C slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x006007FC slot 0x04 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
