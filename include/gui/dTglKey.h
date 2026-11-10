#pragma once

#include "decomp.h"
#include "gui/dKey.h"

namespace gui {
// The key that switches between two key sets (shift): its label follows the key set. Only the class name is from the
// binary (RTTI); member and function names are ours.
// RTTI N3gui6TglKeyE @ 0x008D0EB8
// vtable 0x009040DC (vptr 0x009040E4), offset_to_top 0, 6 entries
class TglKey : public ::gui::Key
{
public:
    struct Description : Key::Description
    {
        // (inline)
        Description() : pButton(NULL) {}

        ButtonEx* pButton;  // 0x88
    };

    // (inline)
    explicit TglKey(const Description& desc) : Key(desc), m_Desc2(desc) {}
    ~TglKey() override; // 0x004FE658 slot 0x00 (a nop that falls into ~Key)
    // 0x004FEAB8 slot 0x04 (deleting dtor)
    void Update() override; // 0x004FEAB4 slot 0x08
    u32 GetKind() const override { return 'TGLK'; } // 0x00747854 slot 0x0C
    ButtonEx* GetButton() override; // 0x004FEAAC slot 0x10
    void UpdateLabel() override; // 0x004FEA60 slot 0x14

    Description m_Desc2;  // 0xA8
};
ASSERT_SIZE(TglKey, 0x134);
} // namespace gui
