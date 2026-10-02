#pragma once

#include "decomp.h"
#include "Other/dNoticeWork.h"
#include "script/dIUiRecept.h"

// RTTI N10NoticeWork10ReceptFootE @ 0x008CD84C
// vtable 0x008FAA28 (vptr 0x008FAA30), offset_to_top 0, 39 entries
class NoticeWork::ReceptFoot : public ::script::IUiRecept
{
public:
    ReceptFoot(); // ctor address unknown
    virtual void vf_0x00(); // 0x001AECF0 slot 0x00 | virtual slot, introduced by script::IUiRecept
    virtual void vf_0x04(); // 0x001AECE0 slot 0x04 | virtual slot, introduced by script::IUiRecept
    virtual void vf_0x50(); // 0x001AEC68 slot 0x50 | virtual slot, introduced by script::IUiRecept
    virtual void vf_0x54(); // 0x001AEC2C slot 0x54 | virtual slot, introduced by script::IUiRecept
    virtual void vf_0x58(); // 0x001AECA4 slot 0x58 | virtual slot, introduced by script::IUiRecept
};
