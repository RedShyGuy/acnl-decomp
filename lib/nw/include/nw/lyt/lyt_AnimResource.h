#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_ResourceFormat.h"
#include "nw/ut/ut_Api.h"

namespace nw {
namespace lyt {
class Group;
class Pane;

// The blocks of an animation file (BCLAN): the animations, the tag (the groups it binds to)
// and the shared animations. The class name is from the reference symbols; the names of the
// inline functions and members are ours.
class AnimResource
{
public:
    // (inline)
    AnimResource() { Init(); }
    explicit AnimResource(const void* pAnimResBuf) { Set(pAnimResBuf); }

    // the blocks of the file; all NULL if it is no animation file
    void Set(const void* pAnimResBuf); // 0x004B510C | nintendogs:bytes [tier A]

    // (inline)
    DECOMP_EMIT_COPY void Init() // 0x004B51B4 (name is ours)
    {
        m_pFileHeader = nullptr;
        m_pResBlock = nullptr;
        m_pTagBlock = nullptr;
        m_pShareBlock = nullptr;
    }
    const ut::BinaryFileHeader* GetFileHeader() const { return m_pFileHeader; }
    const res::AnimationBlock* GetResourceBlock() const { return m_pResBlock; }
    const res::AnimationTagBlock* GetTagBlock() const { return m_pTagBlock; }
    const res::AnimationShareBlock* GetShareBlock() const { return m_pShareBlock; }

    // the groups of the tag (none without a tag)
    u16 GetGroupNum() const; // 0x0073CAE4 (name is ours)
    const res::AnimationGroupRef* GetGroupArray() const; // 0x0073CAF4 (name is ours)
    // the groups bind with their descendants
    bool IsDescendingBind() const; // 0x0073CBF8 (name is ours)
    // the shared animations
    u16 GetAnimationShareInfoNum() const; // 0x0073CC14 (name is ours)
    const res::AnimationShareInfo* GetAnimationShareInfoArray() const; // 0x0073CC24 (name is ours)

    // the number of animation links the panes of the group (and their descendants) need
    u16 CalcAnimationNum(Group* pGroup, bool bRecursive) const; // 0x0073CB08 (name is ours)
    // (inline) the number of animation contents that find their pane / material in the pane
    u16 CalcAnimationNum(Pane* pPane, bool bRecursive) const;

    const ut::BinaryFileHeader* m_pFileHeader;      // 0x0
    const res::AnimationBlock* m_pResBlock;         // 0x4 'pai1'
    const res::AnimationTagBlock* m_pTagBlock;      // 0x8 'pat1'
    const res::AnimationShareBlock* m_pShareBlock;  // 0xC 'pah1'
};
ASSERT_SIZE(AnimResource, 0x10);
} // namespace lyt
} // namespace nw
