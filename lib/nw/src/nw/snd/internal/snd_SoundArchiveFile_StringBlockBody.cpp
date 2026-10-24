#include "nw/snd/internal/snd_SoundArchiveFile_StringBlockBody.h"
#include "nw/snd/snd_SoundArchive.h"
#include <cstring>

namespace nw {
namespace snd {
namespace internal {
// 0x007401D4 | fefates:bytes [tier B]
u32 nw::snd::internal::SoundArchiveFile::StringBlockBody::GetItemIdImpl(Sections section, const char* pName) const
{
    const PatriciaTree* pTree = section <= SECTIONS_PATRICIA_TREE ? static_cast<const PatriciaTree*>(GetReferedPointer(this, sections[section])) : nullptr;
    if (pTree->rootIdx >= pTree->nodeCount) {
        return SoundArchive::INVALID_ID;
    }
    // down to the leaf by the bits of the name
    const PatriciaTreeNode* pNode = &pTree->nodes[pTree->rootIdx];
    const int length = std::strlen(pName);
    while (!(pNode->flags & PatriciaTreeNode::FLAG_LEAF)) {
        const int byte = pNode->bit >> 3;
        const int bit = pNode->bit & 7;
        u32 nodeIdx;
        if (byte < length && (pName[byte] & (1 << (7 - bit)))) {
            nodeIdx = pNode->rightIdx;
        } else {
            nodeIdx = pNode->leftIdx;
        }
        pNode = &pTree->nodes[nodeIdx];
    }
    const char* pString = GetString(pNode->strIdx);
    if (std::strcmp(pName, pString) == 0) {
        return pNode->itemId;
    }
    return SoundArchive::INVALID_ID;
}

// 0x007402BC | nintendogs:bytes [tier A]
const char* nw::snd::internal::SoundArchiveFile::StringBlockBody::GetString(u32 strId) const
{
    if (strId == SoundArchive::INVALID_ID) {
        return nullptr;
    }
    const StringTable* pTable = static_cast<const StringTable*>(GetReferedPointer(this, sections[SECTIONS_STRING_TABLE]));
    if (pTable == nullptr) {
        return nullptr;
    }
    return reinterpret_cast<const char*>(pTable) + pTable->items[strId].offset;
}

} // namespace internal
} // namespace snd
} // namespace nw
