#pragma once

#include <cstring>

#include "decomp.h"

namespace nw {
namespace ut {
// A dictionary of a resource file: a Patricia tree over the names. The object is a pointer to
// the dictionary data. Member and struct names are ours.
class ResDicPatricia
{
public:
    // a node of the tree; node 0 is the root, its left child the first real node
    struct Node
    {
        u32 ref;         // 0x0 the bit that decides (byte index << 3 | bit index)
        u16 idxLeft;     // 0x4
        u16 idxRight;    // 0x6
        s32 ofsName;     // 0x8 offset from this field to the name (0: none)
        s32 ofsData;     // 0xC offset from this field to the data (0: none)
    };
    // the dictionary data
    struct Data
    {
        u32 signature;   // 0x0
        u32 size;        // 0x4
        s32 numData;     // 0x8
        Node nodes[1];   // 0xC (numData + 1 nodes)
    };

    // the node of the name (NULL: not found)
    const Node* Get(const char* key, u32 length) const; // 0x007376D4 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
    // the data of the name (NULL: no dictionary, no name or not found; name is ours)
    void* GetData(const char* key) const; // 0x0073777C (name is ours)
    // the position of the name in the order of the data (-1: no dictionary, no name or not found;
    // inline, name is ours)
    int GetIndex(const char* key) const
    {
        if (m_pData != nullptr && key != nullptr) {
            const Node* pNode = Get(key, std::strlen(key));
            if (pNode != nullptr) {
                return pNode - &m_pData->nodes[1];
            }
        }
        return -1;
    }
    // the data at a position (NULL: no dictionary; inline, name is ours)
    void* GetDataByIndex(int index) const
    {
        if (m_pData == nullptr) {
            return nullptr;
        }
        const s32* pOffset = &m_pData->nodes[index + 1].ofsData;
        return *pOffset != 0 ? const_cast<char*>(reinterpret_cast<const char*>(pOffset) + *pOffset) : nullptr;
    }

    const Data* m_pData; // 0x0
};
ASSERT_SIZE(ResDicPatricia, 0x4);
} // namespace ut
} // namespace nw
