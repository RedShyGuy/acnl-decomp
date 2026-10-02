#pragma once

#include "decomp.h"
#include "script/dRenderBase.h"

namespace script {
// RTTI N6script10RenderRubyE @ 0x008D3744
// vtable 0x00909990 (vptr 0x00909998), offset_to_top 0, 4 entries
class RenderRuby : public ::script::RenderBase
{
public:
    RenderRuby(); // TODO: default ctor added so derived stubs compile - may not exist
    virtual ~RenderRuby(); // 0x005D7FC4 slot 0x00 | libgarden
    virtual void vf_0x04(); // 0x005D7F84 slot 0x04 | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
    virtual void vf_0x08(); // 0x005D7E68 slot 0x08 | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
    virtual void vf_0x0C(); // 0x005D7F24 slot 0x0C | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
    void SetTextBoxes(nw::lyt::TextBox const*, nw::lyt::TextBox const*, nw::lyt::TextBox const*, nw::lyt::TextBox const*); // 0x005D748C | libgarden [tier A]
    void Clear(); // 0x005D76CC | libgarden [tier A]
    void CommitText(); // 0x005D77F8 | libgarden [tier A]
    RenderRuby(unsigned int, unsigned int); // 0x005D7A7C | libgarden [tier A]
};
} // namespace script
