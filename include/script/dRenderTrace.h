#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
// RTTI N6script11RenderTraceE @ 0x008D3808
// vtable 0x0090A050 (vptr 0x0090A058), offset_to_top 0, 8 entries
class RenderTrace : public ::script::MsgEngine
{
public:
    RenderTrace(); // ctor candidate(s) 0x005E3BF8 (unverified)
    virtual void vf_0x00(); // 0x005DFE60 slot 0x00 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x04(); // 0x005DFE50 slot 0x04 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x08(); // 0x005DFE20 slot 0x08 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x0C(); // 0x005DFD58 slot 0x0C | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x10(); // 0x005DFE48 slot 0x10 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x14(); // 0x005DFB20 slot 0x14 | virtual slot, introduced by script::MsgEngine
    virtual void vf_0x18(); // 0x005DFD5C slot 0x18 | virtual slot, introduced by script::MsgEngine
    void SetText2(char16_t const* const*); // 0x0060231C | libgarden [tier A]
    void SetText(char16_t const* const*, char16_t const* const*); // 0x00602480 | libgarden [tier A]
};
} // namespace script
