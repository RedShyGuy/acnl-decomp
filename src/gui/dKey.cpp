#include "gui/dKey.h"

#include <cstring>
#include <cwchar>
#include <new>

#include "sead/seadHeap.h"
#include "sead/seadNew.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/lyt/lyt_Api.h"

namespace gui {
// 0x004FE548
Key::Key(const Description& desc) : m_Desc(desc), m_Label(0), m_TextBoxName(), m_ppLabels(NULL)
{
    if (m_Desc.textBoxName != NULL) {
        std::strncpy(m_TextBoxName, m_Desc.textBoxName, 16);
    }
    m_ppLabels = new (ssys::ma::lyt::GetAllocator()->m_pHeap, 4) wchar_t*[m_Desc.labelNum];
    for (s32 i = 0; i < m_Desc.labelNum; i++) {
        const u32 length = m_Desc.labels[i] != NULL ? std::wcslen(m_Desc.labels[i]) : 0;
        m_ppLabels[i] = new (ssys::ma::lyt::GetAllocator()->m_pHeap, 4) wchar_t[length + 1];
        std::wmemset(m_ppLabels[i], 0, length + 1);
        if (m_Desc.labels[i] != NULL) {
            std::wcsncpy(m_ppLabels[i], m_Desc.labels[i], length);
        }
    }
}

// 0x004FE65C slot 0x00
// 0x004FE648 (deleting dtor)
DECOMP_NOINLINE DECOMP_NOIPA Key::~Key()
{
    if (m_ppLabels == NULL) {
        return;
    }
    for (s32 i = 0; i < m_Desc.labelNum; i++) {
        if (m_ppLabels[i] != NULL) {
            ssys::ma::lyt::GetAllocator()->m_pHeap->free(m_ppLabels[i]);
            m_ppLabels[i] = NULL;
        }
    }
    ssys::ma::lyt::GetAllocator()->m_pHeap->free(m_ppLabels);
    m_ppLabels = NULL;
}
} // namespace gui
