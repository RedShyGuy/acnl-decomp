#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script18CmpPartFuzzyEngineE @ 0x008D39EC
// vtable 0x0090A474 (vptr 0x0090A47C), offset_to_top 0, 8 entries
class CmpPartFuzzyEngine : public ::script::MsgEngine
{
public:
    class EngineA;
    class EngineB;
    CmpPartFuzzyEngine(); // ctor candidate(s) 0x0075C5B8 (unverified)
    virtual void vf_0x00(); // 0x005E9B30 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005E9B20 slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x10(); // 0x005E9948 slot 0x10 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
