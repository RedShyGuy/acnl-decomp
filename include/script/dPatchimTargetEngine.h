#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script19PatchimTargetEngineE @ 0x008D3A1C
// vtable 0x0090A4C4 (vptr 0x0090A4CC), offset_to_top 0, 8 entries
class PatchimTargetEngine : public ::script::MsgEngine
{
public:
    PatchimTargetEngine(); // ctor address unknown
    virtual void vf_0x00(); // 0x005E9DC0 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005E9DB0 slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x005E9D84 slot 0x14 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
