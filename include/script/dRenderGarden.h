#pragma once

#include "decomp.h"
#include "script/dChip.h"
#include "nw/lyt/lyt_Types.h"
#include "script/dRenderMain.h"
#include "script/dRenderTrace.h"

namespace nw {
namespace lyt {
class TextBox;
}
} // namespace nw

namespace script {
class IWord;

// The renderer of a message in one or two text boxes without rubies (no virtual functions). Names after libgarden,
// members partly ours.
class RenderGarden
{
public:
    RenderGarden(u32 length); // 0x005E3BF8 | libgarden [tier A]
    ~RenderGarden(); // 0x005E3C64 | libgarden [tier A]

    void SetTextBoxes(nw::lyt::TextBox* pTextBox, nw::lyt::TextBox* pTextBox2); // 0x005E3768 | libgarden [tier A]
    void Clear(); // 0x005E38D4 | libgarden [tier A]
    // the message into the text buffer (name after libgarden)
    void SetText(const IWord* word); // 0x005E3920 (name after libgarden)
    // the text buffer into the text boxes
    void CommitText(); // 0x005E3A64 | libgarden [tier A]
    // (inline) the width of the written text in the text box (name is ours)
    DECOMP_EMIT_COPY f32 MeasureWidth() const // 0x0075BDAC
    {
        const char16* begin = m_Main.m_pText;
        const char16* end = m_Main.m_pBuf[m_Main.m_Count - 1].pText;
        return Chip::MeasureWidth(m_pTextBox, &begin, &end);
    }

    RenderMain m_Main;             // 0x00
    RenderTrace m_Trace;           // 0x44
    nw::lyt::TextBox* m_pTextBox;  // 0xB8
    nw::lyt::TextBox* m_pTextBox2; // 0xBC
    bool m_IsTextBoxSet;           // 0xC0
};
ASSERT_SIZE(RenderGarden, 0xC4);

// A RenderGarden whose text is scaled down to fit into the width of the text box (name is ours).
class RenderGardenFit : public RenderGarden
{
public:
    // the message, scaled to the width of the text box, into the text box (name is ours)
    void SetTextFit(const IWord* word); // 0x005E3984 (name is ours)

    nw::lyt::Size m_FontSize; // 0xC4 the font size of the text box without scaling (name is ours)
};
ASSERT_SIZE(RenderGardenFit, 0xCC);
} // namespace script
