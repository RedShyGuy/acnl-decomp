#pragma once

#include "decomp.h"
#include "script/dIMeasure.h"
#include "script/dSeqEngine.h"

namespace script {
// RTTI N6script11TalkMeasureE @ 0x008D3814
// vtable 0x0090A078 (vptr 0x0090A080), offset_to_top 0, 12 entries
// vtable 0x0090A0B0 (vptr 0x0090A0B8), offset_to_top -140, 6 entries
class TalkMeasure : public ::script::SeqEngine, public ::script::IMeasure
{
public:
    TalkMeasure(); // ctor address unknown
    virtual void vf_0x00(); // 0x005E23F4 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005E239C slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x08(); // 0x005E2350 slot 0x08 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x0C(); // 0x005E22AC slot 0x0C | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x10(); // 0x005E2388 slot 0x10 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x005E00E0 slot 0x14 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x18(); // 0x005E22B0 slot 0x18 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x20(); // 0x005E0D24 slot 0x20 | virtual slot, introduced by script::TalkMeasure
    virtual void vf_0x24(); // 0x005E1344 slot 0x24 | virtual slot, introduced by script::TalkMeasure
    virtual void vf_0x28(); // 0x005EAB60 slot 0x28 | virtual slot, introduced by script::TalkMeasure
    virtual void vf_0x2C(); // 0x0075BC9C slot 0x2C | virtual slot, introduced by script::TalkMeasure
};
} // namespace script
