#pragma once

#include "decomp.h"

namespace nn {
namespace boss {
// RTTI N2nn4boss12NsDataIdListE @ 0x008D0364
// A buffer for the ids of the downloaded data (GetNsDataIdList fills it in parts; the member names
// are ours).
class NsDataIdList
{
public:
    NsDataIdList(unsigned* pIds, unsigned short capacity); // 0x0046ADE8 | nintendogs:bytes [tier A]
    virtual ~NsDataIdList();

    // the id at index, 0xFFFFFFFF past the end
    u32 GetNsDataId(unsigned short index); // 0x0046ADBC | nintendogs:bytes [tier A]
    // starts the listing from the beginning again
    void Reset(); // 0x0046ADAC (name is ours)

    u16 m_Count;      // 0x04, ids in the buffer
    u16 m_StartIndex; // 0x06, where the next GetNsDataIdList continues
    u32 m_NextId;     // 0x08, the last id of a full buffer
    u32* m_pIds;      // 0x0C
    u32 m_Capacity;   // 0x10
};
ASSERT_SIZE(NsDataIdList, 0x14);
} // namespace boss
} // namespace nn
