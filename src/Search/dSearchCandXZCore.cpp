#include "Search/dSearchCandXZCore.h"

SearchCandXZCore::SearchCandXZCore()
{
}

// 0x002BCEC0 slot 0x00
bool SearchCandXZCore::IsCandidate(s32 index)
{
    return IsCandidateXZ(index % m_Width, index / m_Width);
}

// 0x00306ED0
void SearchCandXZCore::RemoveEdges()
{
    for (s32 x = 0; x < m_Width; x++) {
        for (s32 z = 0; z < m_Height; z++) {
            if (x != 0 && z != 0 && m_Width - 1 != x && m_Height - 1 != z) {
                continue;
            }
            const s32 index = z * m_Width + x;
            const u32 bit = 1 << (index & 31);
            if (m_pBits[index >> 5] & bit) {
                m_pBits[index >> 5] &= ~bit;
                m_RestNum--;
            }
        }
    }
}
