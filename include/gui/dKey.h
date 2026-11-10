#pragma once

#include <cstring>

#include "decomp.h"
#include "gui/dButtonEx.h"

namespace gui {

// The label of a key of a keyboard: a text per key set (KeySet::m_Label) shown in a text box of the button's layout.
// Only the class name is from the binary (RTTI); member and function names are ours.
// RTTI N3gui3KeyE @ 0x008D0E90
// vtable 0x00904050 (vptr 0x00904058), offset_to_top 0, 6 entries
class Key
{
public:
    static const s32 LABEL_MAX = 32;

    struct Description
    {
        // (inline)
        Description() : textBoxName(NULL) { std::memset(labels, 0, sizeof(labels)); }

        s32 labelNum;                    // 0x00
        const char* textBoxName;         // 0x04 (NULL: no text box)
        const wchar_t* labels[LABEL_MAX]; // 0x08 (NULL: an empty label)
    };

    explicit Key(const Description& desc); // 0x004FE548
    virtual ~Key(); // 0x004FE65C slot 0x00
    // 0x004FE648 slot 0x04 (deleting dtor)
    virtual void Update() = 0; // slot 0x08
    virtual u32 GetKind() const = 0; // slot 0x0C
    virtual ButtonEx* GetButton() = 0; // slot 0x10
    // the text box gets the label of m_Label
    virtual void UpdateLabel() {} // 0x004FE544 slot 0x14

protected:
    // (inline) the label is put into the text box of the button
    void SetLabelText(ButtonEx* button);

public:
    Description m_Desc;       // 0x04
    s32 m_Label;              // 0x8C
    char m_TextBoxName[17];   // 0x90
    wchar_t** m_ppLabels;     // 0xA4 copies of the labels
};
ASSERT_SIZE(Key, 0xA8);

inline void Key::SetLabelText(ButtonEx* button)
{
    if (m_TextBoxName[0] != '\0') {
        button->m_Desc.pLayout->SetText(m_ppLabels[m_Label], m_TextBoxName, false, 0, 0);
    }
}
} // namespace gui
