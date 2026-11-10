#pragma once

#include "decomp.h"
#include "gui/dButtonEx.h"

namespace gui {
// A ButtonEx whose press plays a second animation with it (ANIM_EX_5 with ANIM_PRESS, ANIM_EX_7 with ANIM_DECIDE)
// and follows the touch: the pressed look while the touch is on the button, the idle look while it is off. It takes
// its layout and sounds from a second copy of the description that keeps the sounds as given. Only the class name is
// from the binary (RTTI); member and function names are ours.
// RTTI N3gui9ButtonEx2E @ 0x008D0EEC
// vtable 0x009041C0 (vptr 0x009041C8), offset_to_top 0, 15 entries
class ButtonEx2 : public ::gui::ButtonEx
{
public:
    // the description the game makes a ButtonEx2 with (the same as that of ButtonEx; name is ours)
    struct Description : ButtonEx::Description
    {
        Description(); // 0x004FF3B0 (falls into ButtonEx::Description())
    };

    explicit ButtonEx2(const ButtonEx::Description& desc); // 0x00500C94
    ~ButtonEx2() override; // 0x00500D0C slot 0x00
    // 0x00500CDC slot 0x04 (deleting dtor)
    u32 GetKind() const override { return KIND_BUTTON_EX2; } // 0x0074786C slot 0x0C
    void SetEnabled(bool enabled, bool playAnimation, bool keepAnimation) override; // 0x00500B44 slot 0x10
    // (isRelease: the press is played)
    void Reset(bool isRelease) override; // 0x00500ABC slot 0x1C
    void UpdateIdle() override; // 0x00500864 slot 0x28
    void UpdatePressed() override; // 0x00500950 slot 0x2C
    void UpdateDecided() override; // 0x0050091C slot 0x30

    ButtonEx::Description m_Desc2;   // 0x11C (m_pDesc; its sounds are not defaulted)
    bool m_IsReset2;       // 0x204
    bool m_IsSlidOut;      // 0x205 the touch is off the button
};
ASSERT_OFFSET(ButtonEx2, m_Desc2, 0x11C);
ASSERT_SIZE(ButtonEx2, 0x208);
} // namespace gui
