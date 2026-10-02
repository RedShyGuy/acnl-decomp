#pragma once

#include "decomp.h"
#include "Other/dMailWork.h"
#include "script/dMsgEngine.h"

// RTTI N8MailWork16HeadUpdateEngineE @ 0x008D401C
// vtable 0x0090C0BC (vptr 0x0090C0C4), offset_to_top 0, 8 entries
class MailWork::HeadUpdateEngine : public ::script::MsgEngine
{
public:
    HeadUpdateEngine(); // ctor address unknown
    virtual void vf_0x00(); // 0x006AB160 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x006AB134 slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x08(); // 0x006AB06C slot 0x08 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x0C(); // 0x006AB02C slot 0x0C | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x006AAF6C slot 0x14 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x18(); // 0x006AB030 slot 0x18 | virtual slot, introduced by script::MsgEngine
};
