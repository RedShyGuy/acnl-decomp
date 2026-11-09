#include "Search/dSearchCandCore.h"
#include "stage/stage_Extern.h"
#include <cstring>

SearchCandCore::SearchCandCore()
{
}

// 0x00306FF8
SearchCandCore::SearchCandCore(s32 num, u32* bits)
    : m_Num(num), m_WordNum(num / 32 + ((num & 31) != 0 ? 1 : 0)), m_pBits(bits), m_RestNum(-1)
{
}

// 0x00276058 slot 0x00
bool SearchCandCore::IsCandidate(s32)
{
    return true;
}

// 0x00306EA8
void SearchCandCore::Reset()
{
    std::memset(m_pBits, 0xFF, m_WordNum * sizeof(u32));
    m_RestNum = m_Num;
}

// 0x003071A8
void SearchCandCore::Remove(s32 index)
{
    u32 bit = 1u << (index & 31);
    if ((m_pBits[index >> 5] & bit) != 0) {
        m_pBits[index >> 5] &= ~bit;
        m_RestNum--;
    }
}

// 0x00306F8C
s32 SearchCandCore::Pick()
{
    s32 rest = static_cast<s32>(GetRandomF01() * m_RestNum);
    for (s32 i = 0; i < m_Num; i++) {
        if ((m_pBits[i >> 5] & (1u << (i & 31))) != 0) {
            if (rest == 0) {
                return i;
            }
            rest--;
        }
    }
    return -1;
}
