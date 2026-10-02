#pragma once

#include "decomp.h"
#include "script/dIMailRecept.h"

// RTTI 13MailLoadAdapt @ 0x008CBA08
// vtable 0x008EF2CC (vptr 0x008EF2D4), offset_to_top 0, 39 entries
class MailLoadAdapt : public ::script::IMailRecept
{
public:
    MailLoadAdapt(); // ctor candidate(s) 0x0022E648 (unverified)
    virtual void vf_0x00(); // 0x0022E6F4 slot 0x00 | virtual slot, introduced by script::IMailRecept
    virtual void vf_0x04(); // 0x0022E6C0 slot 0x04 | virtual slot, introduced by script::IMailRecept
    virtual void vf_0x88(); // 0x00632518 slot 0x88 | virtual slot, introduced by script::IMailRecept
    virtual void vf_0x8C(); // 0x00714330 slot 0x8C | virtual slot, introduced by script::IMailRecept
    virtual void vf_0x90(); // 0x00714404 slot 0x90 | virtual slot, introduced by script::IMailRecept
    virtual void vf_0x94(); // 0x0022DE58 slot 0x94 | virtual slot, introduced by script::IMailRecept
    virtual void vf_0x98(); // 0x0022E0B4 slot 0x98 | virtual slot, introduced by script::IMailRecept
};
