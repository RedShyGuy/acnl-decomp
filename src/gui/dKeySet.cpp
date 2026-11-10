#include "gui/dKeySet.h"

#include "gui/dKey.h"

namespace gui {
// 0x00896CD0
extern const u16 g_KanaConvertChars[3] = {1, 2, 3};

// 0x004FE6FC slot 0x0C
void KeySet::Update()
{
    for (s32 i = 0; i < KEY_MAX; i++) {
        if (m_pKeys[i] != NULL) {
            m_pKeys[i]->Update();
        }
    }
}

// 0x004FE734 slot 0x1C
void KeySet::SetLabel(s32 label)
{
    m_Label = label;
    for (s32 i = 0; i < KEY_MAX; i++) {
        if (m_pKeys[i] != NULL) {
            m_pKeys[i]->m_Label = label;
            m_pKeys[i]->UpdateLabel();
        }
    }
}

// 0x004FE788 slot 0x00
// 0x004FE778 (deleting dtor)
KeySet::~KeySet()
{
    for (s32 i = 0; i < KEY_MAX; i++) {
        if (m_pKeys[i] != NULL) {
            delete m_pKeys[i];
            m_pKeys[i] = NULL;
        }
    }
}
} // namespace gui
