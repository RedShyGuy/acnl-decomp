#include "script/dRenderBase.h"
#include "script/dRenderRuby.h"

namespace script {
// TODO: default ctor added so derived stubs compile - may not exist
script::RenderRuby::RenderRuby()
{
}

// 0x005D7FC4 slot 0x00 | libgarden
script::RenderRuby::~RenderRuby()
{
}

// 0x005D7F84 slot 0x04 | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
void script::RenderRuby::vf_0x04()
{
}

// 0x005D7E68 slot 0x08 | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
void script::RenderRuby::vf_0x08()
{
}

// 0x005D7F24 slot 0x0C | virtual slot, introduced by nw::font::TagProcessorBase<wchar_t>
void script::RenderRuby::vf_0x0C()
{
}

// 0x005D748C | libgarden [tier A]
void script::RenderRuby::SetTextBoxes(nw::lyt::TextBox const*, nw::lyt::TextBox const*, nw::lyt::TextBox const*, nw::lyt::TextBox const*)
{
}

// 0x005D76CC | libgarden [tier A]
void script::RenderRuby::Clear()
{
}

// 0x005D77F8 | libgarden [tier A]
void script::RenderRuby::CommitText()
{
}

// 0x005D7A7C | libgarden [tier A]
script::RenderRuby::RenderRuby(unsigned int, unsigned int)
{
}

} // namespace script
