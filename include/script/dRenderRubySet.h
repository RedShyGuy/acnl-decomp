#pragma once

#include "decomp.h"
#include "script/dRenderMain.h"
#include "script/dRenderRuby.h"
#include "script/dRenderTrace.h"
#include "script/dChip.h"

namespace nw {
namespace lyt {
class TextBox;
}
} // namespace nw

namespace script {
class IWord;

// The renderer of a message with rubies in up to two pairs of text boxes (main text, ruby text), no virtual functions.
// libgarden calls it RenderRuby, but the RTTI gives that name to the renderer of the rubies, so the name is ours; the
// function names are libgarden's.
class RenderRubySet
{
public:
    // length: the characters of both texts; storeNum: the rubies of the main text
    RenderRubySet(u32 length, u32 storeNum); // 0x005D7A7C | libgarden [tier A]
    ~RenderRubySet(); // 0x005D7B0C (name after libgarden)

    void SetTextBoxes(nw::lyt::TextBox* pText, nw::lyt::TextBox* pTextRuby, nw::lyt::TextBox* pText2,
                      nw::lyt::TextBox* pText2Ruby); // 0x005D748C | libgarden [tier A]
    void Clear(); // 0x005D76CC | libgarden [tier A]
    // the default colors from the text boxes again (name is ours)
    void UpdateColors(); // 0x005D7750 (name is ours)
    // the message into the text buffers (name after libgarden)
    void SetText(const IWord* word); // 0x005D7790 (name after libgarden)
    // (inline) the width of the written main text in its text box (name is ours)
    f32 MeasureWidth() const
    {
        const char16* begin = m_Main.m_pText;
        const char16* end = m_Main.m_pBuf[m_Main.m_Count - 1].pText;
        return Chip::MeasureWidth(m_pText, &begin, &end);
    }
    // the text buffers into the text boxes
    void CommitText(); // 0x005D77F8 | libgarden [tier A]

    RenderMain m_Main;                  // 0x00
    RenderRuby m_Ruby;                  // 0x44
    RenderTrace m_Trace;                // 0x78
    nw::lyt::TextBox* m_pText;          // 0xEC
    nw::lyt::TextBox* m_pTextRuby;      // 0xF0
    nw::lyt::TextBox* m_pText2;         // 0xF4
    nw::lyt::TextBox* m_pText2Ruby;     // 0xF8
    bool m_IsTextBoxSet;                // 0xFC
};
ASSERT_SIZE(RenderRubySet, 0x100);
} // namespace script
