#pragma once

#include "decomp.h"
#include "Other/dMailWork.h"
#include "script/dMsgEngine.h"

// RTTI N8MailWork14BodyLineEngineE @ 0x008D4010
// vtable 0x0090C094 (vptr 0x0090C09C), offset_to_top 0, 8 entries
class MailWork::BodyLineEngine : public ::script::MsgEngine
{
public:
    BodyLineEngine(); // ctor candidate(s) 0x006AA7DC, 0x006AAEB8 (unverified)
    virtual void vf_0x00(); // 0x006AAF44 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x006AAF18 slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x08(); // 0x006AAE7C slot 0x08 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x10(); // 0x006AAEA4 slot 0x10 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x006AAD70 slot 0x14 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x18(); // 0x006AAE54 slot 0x18 | virtual slot, introduced by script::MsgEngine
};
