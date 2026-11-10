#pragma once

#include "decomp.h"
#include "gui/dTchOnButton.h"
#include "nn/math/math_Vector2.h"

namespace lyt {
class TextArea;
}
namespace nw {
namespace lyt {
class Bounding;
}
} // namespace nw

namespace gui {
// The text field of the keyboard (a lyt::TextArea): a touch puts the cursor there, dragging selects and above or below
// the field scrolls the lines (faster the farther and the longer). Only the class name is from the binary (RTTI);
// member and function names are ours.
// RTTI N3gui8EditAreaE @ 0x008D0ED8
// vtable 0x00904158 (vptr 0x00904160), offset_to_top 0, 17 entries
class EditArea : public ::gui::TchOnButton
{
public:
    // the margin of the scroll area inside the bounding pane
    static const s32 SCROLL_MARGIN = 5;

    EditArea(const Description& desc, lyt::TextArea* textArea); // 0x00500054
    ~EditArea() override; // 0x00500198 slot 0x00
    // 0x00500168 slot 0x04 (deleting dtor)
    u32 GetKind() const override { return KIND_TOUCH_ON_BUTTON; } // 0x0074756C slot 0x0C
    void UpdateIdle() override; // 0x004FFB18 slot 0x28
    void UpdatePressed() override; // 0x004FFCD4 slot 0x2C

    lyt::TextArea* m_pTextArea;     // 0x11C
    nw::lyt::Bounding* m_pBounding; // 0x120 the first pane of the group (G_textArea_N of a TextAreaOneLineComTitle)
    f32 m_AreaTop;                  // 0x124 the touch scrolls above and below this area
    f32 m_AreaBottom;               // 0x128
    f32 m_DragTop;                  // 0x12C the area with the touch of the press
    f32 m_DragBottom;               // 0x130
    s32 m_ScrollFrames;             // 0x134 the frames since the last scroll
    s32 m_OutsideFrames;            // 0x138 the frames above the area (the scrolling gets faster)
    nn::math::VEC2 m_LastScrollTouch; // 0x13C the touch of the last scroll
    bool m_IsFirstScroll;           // 0x144 the first scroll waits longer
};
ASSERT_OFFSET(EditArea, m_IsFirstScroll, 0x144);
ASSERT_SIZE(EditArea, 0x148);
} // namespace gui
