#pragma once

#include "decomp.h"
#include "gui/dTchOnButton.h"

namespace lyt {
class TextArea;
}
namespace nw {
namespace lyt {
class Bounding;
}
} // namespace nw

namespace gui {
// The touch area of a text area on the upper (B_text_T) or lower (B_text_B) part of a layout: a touch puts the cursor
// there and decides at once. Only the class name is from the binary (RTTI); member and function names are ours.
// RTTI N3gui13TchOnButtonSpE @ 0x008D0E48
// vtable 0x00903EE8 (vptr 0x00903EF0), offset_to_top 0, 17 entries
class TchOnButtonSp : public ::gui::TchOnButton
{
public:
    TchOnButtonSp(bool isBottom, const Description& desc, lyt::TextArea* textArea); // 0x004FB0E4
    ~TchOnButtonSp() override; // 0x004FB214 slot 0x00
    // 0x004FB1E4 slot 0x04 (deleting dtor)
    u32 GetKind() const override { return KIND_TOUCH_ON_BUTTON; } // 0x0074756C slot 0x0C
    void UpdateIdle() override; // 0x004FB000 slot 0x28
    void UpdatePressed() override; // 0x004F7FAC slot 0x2C

    nw::lyt::Bounding* m_pBounding;  // 0x11C
    lyt::TextArea* m_pTextArea;      // 0x120
    Description m_Desc2;             // 0x124 (only its sounds are used)
};
ASSERT_SIZE(TchOnButtonSp, 0x20C);
} // namespace gui
