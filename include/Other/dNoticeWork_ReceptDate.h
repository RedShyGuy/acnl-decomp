#pragma once

#include "decomp.h"
#include "Other/dNoticeWork.h"
#include "script/dIUiRecept.h"

// RTTI N10NoticeWork10ReceptDateE @ 0x008CD840
// vtable 0x008FA984 (vptr 0x008FA98C), offset_to_top 0, 39 entries
class NoticeWork::ReceptDate : public ::script::IUiRecept
{
public:
    ReceptDate(); // ctor address unknown
    virtual void vf_0x00(); // 0x00600F18 slot 0x00 | virtual slot, introduced by script::IUiRecept
    virtual void vf_0x04(); // 0x001AEC1C slot 0x04 | virtual slot, introduced by script::IUiRecept
    virtual void vf_0x08(); // 0x001AEB98 slot 0x08 | virtual slot, introduced by script::IUiRecept
};
