#pragma once

#include "decomp.h"
#include "script/dIMailRecept.h"

// RTTI 15NoticeLoadAdapt @ 0x008CC0D4
// vtable 0x008F1B2C (vptr 0x008F1B34), offset_to_top 0, 39 entries
class NoticeLoadAdapt : public ::script::IMailRecept
{
public:
    NoticeLoadAdapt(); // ctor address unknown
    virtual void vf_0x00(); // 0x003028E8 slot 0x00 | virtual slot, introduced by script::IMailRecept
    virtual void vf_0x04(); // 0x0029CED8 slot 0x04 | virtual slot, introduced by script::IMailRecept
    virtual void vf_0x8C(); // 0x0071C100 slot 0x8C | virtual slot, introduced by script::IMailRecept
    virtual void vf_0x90(); // 0x0071C108 slot 0x90 | virtual slot, introduced by script::IMailRecept
    virtual void vf_0x94(); // 0x0029CE84 slot 0x94 | virtual slot, introduced by script::IMailRecept
    virtual void vf_0x98(); // 0x0029CE94 slot 0x98 | virtual slot, introduced by script::IMailRecept
};
