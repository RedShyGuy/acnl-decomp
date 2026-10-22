#include <cstring>

#include "nw/ut/ut_ResDicPatricia.h"

namespace nw {
namespace ut {
namespace {
// the bit of the key at ref (0 beyond the end of the key; inline, name is ours)
inline bool GetBit(const char* key, u32 length, u32 ref)
{
    u32 index = ref >> 3;
    if (index < length) {
        return ((key[index] >> (ref & 7)) & 1) != 0;
    }
    return false;
}
} // namespace

// 0x007376D4 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
const nw::ut::ResDicPatricia::Node* nw::ut::ResDicPatricia::Get(const char* key, u32 length) const
{
    const Data* pData = m_pData;
    const Node* pPrev = &pData->nodes[0];
    const Node* pNode = &pData->nodes[pPrev->idxLeft];
    while (pPrev->ref > pNode->ref) {
        pPrev = pNode;
        if (GetBit(key, length, pNode->ref)) {
            pNode = &pData->nodes[pNode->idxRight];
        } else {
            pNode = &pData->nodes[pNode->idxLeft];
        }
    }
    if (pNode->ofsName != 0) {
        const char* pName = reinterpret_cast<const char*>(&pNode->ofsName) + pNode->ofsName;
        if (std::strcmp(key, pName) == 0) {
            return pNode;
        }
    }
    return nullptr;
}

} // namespace ut
} // namespace nw
