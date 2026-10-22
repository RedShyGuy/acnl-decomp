#include "nw/lyt/lyt_AnimResource.h"

#include "nw/lyt/lyt_Group.h"
#include "nw/lyt/lyt_Pane.h"

namespace nw {
namespace lyt {
// 0x004B510C | nintendogs:bytes [tier A]
void nw::lyt::AnimResource::Set(const void* pAnimResBuf)
{
    Init();

    const ut::BinaryFileHeader* pFileHeader = static_cast<const ut::BinaryFileHeader*>(pAnimResBuf);
    if (!ut::IsValidBinaryFile(pFileHeader, res::FILE_SIGNATURE_ANIMATION, res::FILE_VERSION, 1)) {
        return;
    }

    m_pFileHeader = pFileHeader;
    const ut::BinaryBlockHeader* pDataBlockHead = reinterpret_cast<const ut::BinaryBlockHeader*>(reinterpret_cast<uptr>(pFileHeader) + pFileHeader->headerSize);
    for (int i = 0; i < pFileHeader->dataBlocks; ++i) {
        switch (pDataBlockHead->kind) {
        case res::BLOCK_KIND_ANIM_SHARE:
            m_pShareBlock = reinterpret_cast<const res::AnimationShareBlock*>(pDataBlockHead);
            break;
        case res::BLOCK_KIND_ANIM_INFO:
            m_pResBlock = reinterpret_cast<const res::AnimationBlock*>(pDataBlockHead);
            break;
        case res::BLOCK_KIND_ANIM_TAG:
            m_pTagBlock = reinterpret_cast<const res::AnimationTagBlock*>(pDataBlockHead);
            break;
        }
        pDataBlockHead = reinterpret_cast<const ut::BinaryBlockHeader*>(reinterpret_cast<uptr>(pDataBlockHead) + pDataBlockHead->size);
    }
}

// 0x0073CAE4 (name is ours)
u16 nw::lyt::AnimResource::GetGroupNum() const
{
    if (m_pTagBlock == nullptr) {
        return 0;
    }
    return m_pTagBlock->groupNum;
}

// 0x0073CAF4 (name is ours)
const res::AnimationGroupRef* nw::lyt::AnimResource::GetGroupArray() const
{
    if (m_pTagBlock == nullptr) {
        return nullptr;
    }
    return reinterpret_cast<const res::AnimationGroupRef*>(reinterpret_cast<uptr>(m_pTagBlock) + m_pTagBlock->groupsOffset);
}

// 0x0073CBF8 (name is ours)
bool nw::lyt::AnimResource::IsDescendingBind() const
{
    if (m_pTagBlock == nullptr) {
        return false;
    }
    return (m_pTagBlock->flag & 1) != 0;
}

// 0x0073CC14 (name is ours)
u16 nw::lyt::AnimResource::GetAnimationShareInfoNum() const
{
    if (m_pShareBlock == nullptr) {
        return 0;
    }
    return m_pShareBlock->shareNum;
}

// 0x0073CC24 (name is ours)
const res::AnimationShareInfo* nw::lyt::AnimResource::GetAnimationShareInfoArray() const
{
    if (m_pShareBlock == nullptr) {
        return nullptr;
    }
    return reinterpret_cast<const res::AnimationShareInfo*>(reinterpret_cast<uptr>(m_pShareBlock) + m_pShareBlock->animShareInfoOffset);
}

// (inline)
inline u16 nw::lyt::AnimResource::CalcAnimationNum(Pane* pPane, bool bRecursive) const
{
    if (m_pResBlock == nullptr) {
        return 0;
    }

    u16 linkNum = 0;
    const u32* animContOffsets = reinterpret_cast<const u32*>(reinterpret_cast<uptr>(m_pResBlock) + m_pResBlock->animContOffsetsOffset);
    for (u16 i = 0; i < m_pResBlock->animContNum; ++i) {
        const res::AnimationContent* pAnimCont = reinterpret_cast<const res::AnimationContent*>(reinterpret_cast<uptr>(m_pResBlock) + animContOffsets[i]);
        if (pAnimCont->type == res::ANIMCONTENTTYPE_PANE) {
            if (pPane->FindPaneByName(pAnimCont->name, bRecursive) != nullptr) {
                ++linkNum;
            }
        } else {
            if (pPane->FindMaterialByName(pAnimCont->name, bRecursive) != nullptr) {
                ++linkNum;
            }
        }
    }
    return linkNum;
}

// 0x0073CB08 (name is ours)
u16 nw::lyt::AnimResource::CalcAnimationNum(Group* pGroup, bool bRecursive) const
{
    u16 linkNum = 0;
    Group::PaneLinkList& paneList = pGroup->GetPaneList();
    for (Group::PaneLinkList::iterator it = paneList.begin(); it != paneList.end(); ++it) {
        linkNum += CalcAnimationNum(Group::PaneLinkList::GetPointerFromNode(it.GetNode())->m_pTarget, bRecursive);
    }
    return linkNum;
}

} // namespace lyt
} // namespace nw
