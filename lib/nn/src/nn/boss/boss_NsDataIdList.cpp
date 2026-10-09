#include "nn/boss/boss_NsDataIdList.h"

namespace nn {
namespace boss {
// 0x0046ADAC (name is ours)
void nn::boss::NsDataIdList::Reset()
{
    m_StartIndex = 0;
    m_NextId = 0;
}

// 0x0046ADBC | nintendogs:bytes [tier A]
u32 nn::boss::NsDataIdList::GetNsDataId(unsigned short index)
{
    if (m_pIds != 0 && m_Capacity != 0 && m_Count > index) {
        return m_pIds[index];
    }
    return 0xFFFFFFFF;
}

// 0x0046ADE8 | nintendogs:bytes [tier A]
nn::boss::NsDataIdList::NsDataIdList(unsigned* pIds, unsigned short capacity)
    : m_StartIndex(0), m_NextId(0), m_pIds(pIds), m_Capacity(capacity)
{
    if (pIds == 0) {
        m_Capacity = 0;
    }
}

// 0x0046AE18
// 0x0046AE14 (deleting dtor)
nn::boss::NsDataIdList::~NsDataIdList()
{
}

} // namespace boss
} // namespace nn
