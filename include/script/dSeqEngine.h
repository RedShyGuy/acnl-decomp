#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script9SeqEngineE @ 0x008D3E94
// vtable 0x0090BBD8 (vptr 0x0090BBE0), offset_to_top 0, 8 entries
class SeqEngine : public ::script::MsgEngine
{
public:
    SeqEngine(); // ctor candidate(s) 0x00602690 (unverified)
    virtual void vf_0x00(); // 0x006026F8 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x006026E8 slot 0x04 | virtual slot, introduced by script::MsgEngine
};
} // namespace script
