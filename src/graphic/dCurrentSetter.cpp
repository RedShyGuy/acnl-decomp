#include "graphic/dCurrentSetter.h"
#include "graphic/dMemoryMgr.h"

namespace graphic {
// 0x00134080 (name is ours)
graphic::CurrentSetter::CurrentSetter(ssys::ma::VramHeap* heap0, ssys::ma::VramHeap* heap1, ssys::ma::VramHeap* heap2)
{
    m_pPrevHeaps[0] = NULL;
    m_pPrevHeaps[1] = NULL;
    m_pPrevHeaps[2] = NULL;
    if (heap0 != NULL) {
        m_pPrevHeaps[0] = MemoryMgr::s_pInstance->m_pCurrentHeaps[0];
        MemoryMgr::s_pInstance->m_pCurrentHeaps[0] = heap0;
    }
    if (heap1 != NULL) {
        m_pPrevHeaps[1] = MemoryMgr::s_pInstance->m_pCurrentHeaps[1];
        MemoryMgr::s_pInstance->m_pCurrentHeaps[1] = heap1;
    }
    if (heap2 != NULL) {
        m_pPrevHeaps[2] = MemoryMgr::s_pInstance->m_pCurrentHeaps[2];
        MemoryMgr::s_pInstance->m_pCurrentHeaps[2] = heap2;
    }
}

// 0x001340F8 slot 0x00 (name is ours)
// 0x006179D0 (deleting dtor)
graphic::CurrentSetter::~CurrentSetter()
{
    if (m_pPrevHeaps[0] != NULL) {
        MemoryMgr::s_pInstance->m_pCurrentHeaps[0] = m_pPrevHeaps[0];
    }
    if (m_pPrevHeaps[1] != NULL) {
        MemoryMgr::s_pInstance->m_pCurrentHeaps[1] = m_pPrevHeaps[1];
    }
    if (m_pPrevHeaps[2] != NULL) {
        MemoryMgr::s_pInstance->m_pCurrentHeaps[2] = m_pPrevHeaps[2];
    }
}

} // namespace graphic
