#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script9CatEngineE @ 0x008D3E2C
// vtable 0x0090BA6C (vptr 0x0090BA74), offset_to_top 0, 8 entries
class CatEngine : public ::script::MsgEngine
{
public:
    CatEngine(); // ctor address unknown
    virtual void vf_0x00(); // 0x005FEB74 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005FEB64 slot 0x04 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
