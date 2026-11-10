#pragma once

#include "decomp.h"
#include "gui/dKey.h"
#include "sead/seadNew.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/lyt/lyt_Api.h"
#include "sys/dTextManager.h"

namespace gui {
// A multi-tap key of the cellphone keyboard: per key set the characters that its presses cycle through
// (sys::TextManager::CellPhoneInput). Only the class name is from the binary (RTTI); member and function names are ours.
// RTTI N3gui6MulKeyE @ 0x008D0EA0
// vtable 0x0090409C (vptr 0x009040A4), offset_to_top 0, 6 entries
class MulKey : public ::gui::Key
{
public:
    struct Description : Key::Description
    {
        // (inline)
        Description() : pButton(NULL) { std::memset(inputs, 0, sizeof(inputs)); }

        ButtonEx* pButton;                                       // 0x88
        const sys::TextManager::CellPhoneInput* inputs[LABEL_MAX]; // 0x8C the characters of each key set
    };

    // (inline) the inputs are copied
    explicit MulKey(const Description& desc) : Key(desc), m_Desc2(desc), m_pInputs(NULL)
    {
        m_pInputs = new (ssys::ma::lyt::GetAllocator()->m_pHeap, 4) sys::TextManager::CellPhoneInput[m_Desc2.labelNum];
        for (s32 i = 0; i < m_Desc2.labelNum; i++) {
            if (m_Desc2.inputs[i] != NULL) {
                m_pInputs[i] = *m_Desc2.inputs[i];
            }
        }
    }
    ~MulKey() override; // 0x004FE87C slot 0x00
    // 0x004FE830 slot 0x04 (deleting dtor)
    void Update() override; // 0x004FE82C slot 0x08
    u32 GetKind() const override { return 'MULK'; } // 0x0074783C slot 0x0C
    ButtonEx* GetButton() override; // 0x004FE824 slot 0x10
    void UpdateLabel() override; // 0x004FE7D8 slot 0x14

    // (inline) the input of the key set
    sys::TextManager::CellPhoneInput* GetInput() { return &m_pInputs[m_Label]; }

    Description m_Desc2;                         // 0xA8
    sys::TextManager::CellPhoneInput* m_pInputs; // 0x1B4
};
ASSERT_SIZE(MulKey::Description, 0x10C);
ASSERT_SIZE(MulKey, 0x1B8);
} // namespace gui
