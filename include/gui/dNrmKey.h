#pragma once

#include "decomp.h"
#include "gui/dKey.h"

namespace gui {
// A key that puts a character (one per key set) with its button. Only the class name is from the binary (RTTI);
// member and function names are ours.
// RTTI N3gui6NrmKeyE @ 0x008D0EAC
// vtable 0x009040BC (vptr 0x009040C4), offset_to_top 0, 6 entries
class NrmKey : public ::gui::Key
{
public:
    struct Description : Key::Description
    {
        // (inline)
        Description() : pButton(NULL), chars() {}

        ButtonEx* pButton;              // 0x88
        u16 chars[LABEL_MAX];           // 0x8C the character of each key set
    };

    explicit NrmKey(const Description& desc); // 0x004FE914
    ~NrmKey() override; // 0x004FEA28 slot 0x00
    // 0x004FE9E4 slot 0x04 (deleting dtor)
    void Update() override; // 0x004FE910 slot 0x08
    u32 GetKind() const override { return 'NRMK'; } // 0x00747848 slot 0x0C
    ButtonEx* GetButton() override; // 0x004FE908 slot 0x10
    void UpdateLabel() override; // 0x004FE8BC slot 0x14

    // (inline) the character of the key set
    u16 GetChar() const { return m_pChars[m_Label]; }

    Description m_Desc2;  // 0xA8
    u16* m_pChars;        // 0x174
};
ASSERT_SIZE(NrmKey::Description, 0xCC);
ASSERT_SIZE(NrmKey, 0x178);
} // namespace gui
