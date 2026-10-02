#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script16EpenthesisEngineE @ 0x008D392C
// vtable 0x0090A2D4 (vptr 0x0090A2DC), offset_to_top 0, 8 entries
class EpenthesisEngine : public ::script::MsgEngine
{
public:
    EpenthesisEngine(); // ctor candidate(s) 0x005F32A4, 0x005F3310 (unverified)
    virtual void vf_0x00(); // 0x005E729C slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005E728C slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x10(); // 0x005E7278 slot 0x10 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x005E71CC slot 0x14 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
